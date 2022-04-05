#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <deque>
using namespace std;

// Function declarations //
void add_node (unordered_map < string, unordered_map < string, int > > &dag, string line);
void print_nodes ( deque<string> *list);
deque<string> order_nodes (unordered_map < string, unordered_map < string, int > > &dag);
void trans (unordered_map < string, unordered_map < string, int > > &node, 
			unordered_map < string, unordered_map < string, int > > &dag);

//Global variables //
string last_file;
deque<string> path;
unordered_map < string, int > listed;
unordered_map < string, unordered_map < string, int > > file_DAG;

// Adds a node to a given DAG 
void add_node (unordered_map < string, unordered_map < string, int > > &dag, string line){
	if ( line.substr(0,9) == "#include "){
		dag[last_file][line.substr(10, line.size() - 11)];
		try { dag.at(line.substr(10, line.size() - 11)); }
		catch (out_of_range) { dag[line.substr(10, line.size() - 11)]; }
	}
	else{
		try { dag.at(line); }
		catch (out_of_range){  
			dag[line];
		}
		last_file = line;
	}
}

void trans (unordered_map < string, int > ::iterator node, 
			unordered_map < string, unordered_map < string, int > > &dag){
	
	for ( auto it = dag[node->first].begin(); it != dag[node->first].end(); ++it ){
		try { listed.at(it->first); }
		catch (out_of_range){  
			trans(it, dag);
		}	
	}
	try { listed.at(node->first); }
	catch (out_of_range){  
		listed[node->first];
		path.push_back(node->first);
	}	
}

// Takes the nodes in a DAG and orders them into a deque in sequential order
deque<string> order_nodes (unordered_map < string, unordered_map < string, int > > &dag){
	
	for ( auto it = dag.begin(); it != dag.end(); ++it ){
		for ( auto it2 = it->second.begin(); it2 != it->second.end(); ++it2 ){
			try { listed.at(it2->first); }
			catch (out_of_range){  
				trans(it2, dag);
			}	
		}
		try { listed.at(it->first); }
		catch (out_of_range){  
			listed[it->first];
			path.push_back(it->first);
		}	
	}

	return path;
}

// Prints a given deque to the terminal
void print_nodes ( deque<string> &list){
	deque<string>::iterator itr {list.begin()};
	while (itr != list.end()){
    	cout << *itr++ << endl;
	}
}

int main(int argc, char** argv)
{
	//first input argument is always the program name...
	char* fileName = argv[1];
	
	//open the file
	std::ifstream fin(fileName);
	
	if(!fin)
	{
		std::cout<< "File not found" << std::endl;
		return EXIT_FAILURE;
	}

	std::string line;
	while(std::getline(fin,line))
	{
		//this is just to get you going.  You'll need to do much more here...
		add_node(file_DAG, line);
	}
	
	order_nodes(file_DAG);

	/*for (auto& x: file_DAG) {
		std::cout << x.first << endl;
		for (auto& y: x.second) {
			std::cout << "\t" << y.first << std::endl;
		}
	}
	cout << endl << endl;*/

	print_nodes(path);
	
	return EXIT_SUCCESS;
}
