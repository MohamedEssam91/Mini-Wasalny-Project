#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <fstream>
using namespace std;
class edge
{
public:
	map<pair<int, int>, int> edges;
};
class vertex
{
public:
	edge e;
	map<string, int> ID;
};
class Graph
{

public:
	vertex x;
	vector<vector<pair<int, int>>> adj;
	vector<string> name;
	vector<int> dis, p;

	int n;

	void initializeGraph(int numOfnodes)
	{ // Create n nodes numbered from 1 to n
		n = numOfnodes;
		adj.resize(n + 1);
		name.resize(n + 1);
	}

	void deleteGraph()
	{
		x.ID.clear();
		adj.clear();
		name.clear();
		x.e.edges.clear();
		n = 0;
	}
	void addEdge(string u_name, int u_id, string v_name, int v_id, int w, bool is_bi_edge)
	{ // add edge between node u and node v with weight w
		if (name[u_id] == ""	)
			name[u_id] = u_name, x.ID[u_name] = u_id;
		if (name[v_id] == "")
			name
			[v_id] = v_name,
			x.ID[v_name] = v_id;
		x.e.edges[{u_id, v_id}] = adj[u_id].size();
		adj[u_id].push_back({ v_id, w });
		if (is_bi_edge)
		{
			x.e.edges[{v_id, u_id}] = adj[v_id].size();
			adj[v_id].push_back({ u_id, w });

		}
	}
	void update_name_of_town(int townId, string newName) // updating name 
	{
		if (name[townId] != "") // checks that town really exists 
			x.ID.erase({ name[townId], static_cast<unsigned long long>(townId ) }); // deletes old name for that id  ,  convert from int to unsigned long long to use in the algoritm 
		name[townId] = newName; //Set new name for entered ID .
		x.ID[newName] = townId; //connects id with the town
	}
	void update_weight_of_edge(string firstTown, string secTown, int newDistance) // update distance on edges 
	{
		int u_id = x.ID[firstTown], v_id = x.ID[secTown];   // initialize IDs
		if (x.e.edges.find({ u_id, v_id }) != x.e.edges.end()) //checks that they exist
			adj[u_id][x.e.edges[{u_id, v_id}]].second = newDistance;
		if (x.e.edges.find({ v_id, u_id }) != x.e.edges.end())
			adj[v_id][x.e.edges[{v_id, u_id}]].second = newDistance;
	}
	void display_graph()
	{ // print the graph data
		if (x.ID.size() == 0) {
			cout << "There is no towns in the system yet.\n You can start by adding a town right now!!" << endl;
		}
		else {
			for (int i = 1; i <= n; i++)
			{
				for (int j = 0; j < adj[i].size(); j++)
				{
					int u = adj[i][j].first, w = adj[i][j].second;
					cout << name[i] << " (" << i << "): " << name[u] << " (" << u << ") , weight: " << w << "\n";
				}
			}
		}
	}
	void DisplayTowns() {
		if (x.ID.size() == 0) {
			cout << "There is no towns in the system yet.\n You can start by adding a town right now!!" << endl;
		}
		else {
			cout << "The available towns right now and their IDs are :\n";
			for (int i = 1; i <= n; i++) {
				if (x.ID[name[i]] == 0)
					continue;
				else
					cout << "Name of town:" << name[i] << ", ID:" << x.ID[name[i]] << endl;
			}
		}
	}
	void dijkstra(int source)
	{
		dis.assign(n + 1, INT_MAX);
		p.assign(n + 1, -1);
		dis[source] = 0;
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
		q.push({ 0, source });
		while (!q.empty())
		{
			int v = q.top().second, c = q.top().first;
			q.pop();
			if (c > dis[v])
				continue;
			for (auto& [u, w] : adj[v])
			{
				if (dis[v] + w < dis[u])
				{
					dis[u] = dis[v] + w;
					p[u] = v;
					q.push({ dis[u], u });
				}
			}
		}
	}
	void get_path(int v, vector<string>& path)
	{
		if (p[v] != -1)
			get_path(p[v], path);
		path.push_back(name[v]);
	}
	int shortest_path_total_distance(string source, string destination)
	{
		// return the shortest total distance from source to destination and if there is no path return INT_MAX
		dijkstra(x.ID[source]);
		return dis[x.ID[destination]];
	}
	vector<string> shortest_path_nodes(string source, string destination)
	{
		// return a vector of nodes in the shortest path from source to destination and if there is no path return {-1}
		if (shortest_path_total_distance(source, destination) == INT_MAX)
			return { "-1" };
		vector<string> path;
		get_path(x.ID[destination], path);
		return path;
	}
};
// Save data to a file the vertex and graph to act as database for the graph to be used for persistancy
void save_data(Graph g)
{
	ofstream file;
	file.open("Graph.txt");
	if (g.n == 0) {
		file << g.n;
	}
	else {
		file << g.n << "\n";
		for (int i = 1; i <= g.n; i++)
		{
			file << g.name[i] << " " << i << "\n";
		}

		for (int i = 1; i <= g.n; i++)
		{
			for (int j = 0; j < g.adj[i].size(); j++)
			{
				file << "\n" << i << " " << g.adj[i][j].first << " " << g.adj[i][j].second;
			}
		}
	}
	file.close();
}

// read data from file
Graph read_data()
{
	Graph g;
	ifstream file;
	file.open("Graph.txt");
	if (file.peek() == -1){
		g.n = 0;
		cout << "Please add towns as there is no saved towns yet." << endl;
	}
	else {
		file >> g.n;
		g.name.resize(g.n + 1);
		g.adj.resize(g.n + 1);
		if (g.n == 0) {
			cout << "Please add towns as there is no saved towns yet." << endl;
		}
		else {
			for (int i = 1; i <= g.n; i++)
			{
				file >> g.name[i];
				int id;
				file >> id;
				g.x.ID[g.name[i]] = id;
			}
			while (!file.eof() && file.peek() != -1) {
				int u, v, w;
				file >> u >> v >> w;
				g.addEdge(g.name[u], u, g.name[v], v, w, false);
			}
		}
	}
	file.close();
	return g;
}

int main()
{
	Graph g;
	int numOfVertices, id1, id2, weight, townUpdateID;
	string first, second, townUpdateName;
	bool bi;
	int option, updateOption;
	// read data from file
	g = read_data();
	do
	{
		cout << "--------------------------------------------------------------------------------------------------------" << endl;
		cout << "Welcome to Mini Wassalni !" << endl;
		cout << "                                                           " << endl;
		cout << "Select Your Operation Please , Or Select 0 To exit." << endl;
		cout << "                                                           " << endl;
		cout << "If it's The First Time To Use Mini Wassalni , Please Enter Number Of Towns First." << endl;
		cout << "                                                           " << endl;

		cout << "   1. Enter Number Of Towns" << endl;
		cout << "   2. Add Towns" << endl;
		cout << "   3. Update Towns" << endl;
		cout << "   4. Delete Towns" << endl;
		cout << "   5. Show Towns" << endl;
		cout << "   6. Show Shortest Path Distance Between 2 Towns" << endl;
		cout << "   7. Show Shortest Path Towns Between 2 Towns" << endl;
		cout << "   0. Exit Program" << endl;
		cout << "--------------------------------------------------------------------------------------------------------" << endl;

		cin >> option;
		switch (option)
		{

		case 0:
			cout << "Nice To See You , Please Come Again !!" << endl;
			// save data to file
			save_data(g);
			break;
		case 1:
			cout << "                                                           " << endl;
			cout << "1. Enter Number Of Towns: " << endl;
			cout << "Enter Number Of Towns You Want To Add Please: ";
			cin >> numOfVertices;
			g.initializeGraph(numOfVertices);
			cout << "Added Succefully, Now You Can Add " << numOfVertices << " Towns In The Graph." << endl;

			break;
		case 2:
			g.DisplayTowns();
			cout << endl;
			
				cout << "Enter first town name: ";
				cin >> first;
				
					cout << "Enter first town ID: ";
					cin >> id1;
					
					
				
				cout << "Enter second town name: ";
				cin >> second;
					cout << "Enter second town ID: ";
					cin >> id2;
			
				
				cout << "Enter distance between towns: ";
				cin >> weight;

				cout << "Enter 1 for undirected graph , or 0 for directed graph ";
				cin >> bi;
				g.addEdge(first, id1, second, id2, weight, bi);
				cout << "Town Added Succefully !" << endl;
				cout << "                                                           " << endl;
			
			break;
		case 3:
			if (g.x.ID.size() == 0) {
				cout << "There is no towns in the system yet.\n You can start by adding a town right now!!" << endl;
			}
			else {
				g.DisplayTowns();
				cout << "                                                           " << endl;
				cout << "3. Update Towns" << endl;
				cout << "Choose What You Want To Update: " << endl;
				cout << "1. Update Town's Name." << endl;
				cout << "2. Update Distance Between Two Towns." << endl;
				cin >> updateOption;
				if (updateOption == 1)
				{
					cout << "Enter Town's ID: ";
					cin >> townUpdateID;
					cout << "Enter New Town's name: ";
					cin >> townUpdateName;
					g.update_name_of_town(townUpdateID, townUpdateName);
					cout << "Town's Name Updated Successfully ." << endl;
				}
				else if (updateOption == 2)
				{
					cout << "Enter First Town Name: ";
					cin >> first;
					cout << "Enter Second Town Name: ";
					cin >> second;
					cout << "Enter New Distance Between the towns: ";
					cin >> weight;
					g.update_weight_of_edge(first, second, weight);
					cout << "Distance Updated Successfully." << endl;
					cout << "                                                           " << endl;
				}
				else
				{
					cout << ""
						"Invalid Option."
						<< endl;
				}

				cout << "                                                           " << endl;
			}
			break;
		case 4:
			cout << "                                                           " << endl;

			cout << "4. Delete Towns" << endl;
			g.deleteGraph();
			cout << "Towns Deleted Successfully. " << endl;
			cout << "                                                           " << endl;

			break;
		case 5:
			cout << "5. Show Towns" << endl;
			g.display_graph();
			cout << endl;

			break;
		case 6:
			if (g.x.ID.size() == 0) {
				cout << "There is no towns in the system yet.\n You can start by adding a town right now!!" << endl;
			}
			else {
				g.DisplayTowns();
				cout << "                                                           " << endl;
				cout << "6. Show Shortest Path Distance Between 2 Towns" << endl;
				cout << "Enter First Town Name: ";
				cin >> first;
				cout << "Enter Second Town Name: ";
				cin >> second;
				cout << "Shortest Distance Between " << first << " And " << second << " Is: " << g.shortest_path_total_distance(first, second) << endl;
				cout << "                                                           " << endl;
			}
			break;
		case 7:
			if (g.x.ID.size() == 0) {
				cout << "There is no towns in the system yet.\n You can start by adding a town right now!!" << endl;
			}
			else {
				g.DisplayTowns();
				cout << "                                                           " << endl;
				cout << "7. Show Shortest Path Towns Between 2 Towns" << endl;
				cout << "Enter First Town Name: ";
				cin >> first;
				cout << "Enter Second Town Name: ";
				cin >> second;
				for (auto i : g.shortest_path_nodes(first, second))
				{
					cout << i << " ";
				}

				cout << "                                                           " << endl;
			}
			break;
		}
	} while (option != 0);
}
