//
//  main.cpp
//  Labyrinth
//
//  Created by Zhou Ye on 9/25/14.
//  Copyright (c) 2014 ZhouYe. All rights reserved.
//

#include <iostream>
#include <queue>
#include <stack>
using namespace std;

class Node {
    
public:
    int x; //x axis
    int y; //y axis
    int z; //z axis
    Node *pred; //parent node
    /*Constructor*/
    Node(int x, int y, int z, Node *pred) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->pred = pred;
    }
    
};

ostream& operator<<(ostream &strm, const Node n) {
    return strm << "(" << n.x << "," << n.y << "," << n.z << ")";
}

stack<Node> shortestPath(Node sink) {
    stack<Node> s; //stack for backtracking
    s.push(sink);
    Node n = sink;
    while (n.pred!=NULL) {
        s.push(*n.pred); //parent node
        n = *n.pred;
    }
    return s;
}

void bfs(char ***grid, int d1, int d2, int d3) {
    /*initialization*/
    Node source = Node(0, 0, 0, NULL); //prince
    bool*** check = new bool**[d1]; //record if the node has been checked
    for (int i=0; i<d1; i++) {
        check[i] = new bool*[d2];
        for (int j=0; j<d2; j++) {
            check[i][j] = new bool[d3];
            for (int k=0; k<d3; k++) {
                check[i][j][k] = false; //mark all nodes as unchecked
            }
        }
    }
    cout << "Finish Initialization" << endl;
    /*queue*/
    queue<Node> q; //queue structure to perform BFS (you can also store the pointers)
    q.push(source);
    Node *sink = NULL; //princess
    while(!q.empty()) {
        Node temp = q.front();
        if (grid[temp.x][temp.y][temp.z]==L'2') {
            sink = &temp;
            break; //we find the princess
        }
        check[temp.x][temp.y][temp.z] = true;
        /*
         we have five directions; for each direction, we check three things:
         (1) if the node is outside the labyrinth
         (2) if the node is checked
         (3) if the node is a wall
         */
        /*up*/
        if (temp.y-1>=0 && !check[temp.x][temp.y-1][temp.z] && grid[temp.x][temp.y-1][temp.z]!=L'o') {
            q.push(*new Node(temp.x, temp.y-1, temp.z, &q.front()));
        }
        /*down*/
        if (temp.y+1<d2 && !check[temp.x][temp.y+1][temp.z] && grid[temp.x][temp.y+1][temp.z]!=L'o') {
            q.push(*new Node(temp.x, temp.y+1, temp.z, &q.front()));
        }
        /*left*/
        if (temp.z-1>=0 && !check[temp.x][temp.y][temp.z-1] && grid[temp.x][temp.y][temp.z-1]!=L'o') {
            q.push(*new Node(temp.x, temp.y, temp.z-1, &q.front()));
        }
        /*right*/
        if (temp.z+1<d3 && !check[temp.x][temp.y][temp.z+1] && grid[temp.x][temp.y][temp.z+1]!=L'o') {
            q.push(*new Node(temp.x, temp.y, temp.z+1, &q.front()));
        }
        /*below*/
        if (temp.x+1<d1 && !check[temp.x+1][temp.y][temp.z] && grid[temp.x+1][temp.y][temp.z]!=L'o') {
            q.push(*new Node(temp.x+1, temp.y, temp.z, &q.front()));
        }
        q.pop();
    }
    cout << "Finish BFS!" << endl;
    if (sink!=NULL) {
        /*print shortest path*/
        stack<Node> sp = shortestPath(*sink); //find the shortest path
        int dist = 0;
        while (!sp.empty()) {
            cout << sp.top() << endl;
            sp.pop();
            dist++;
        }
        cout << "Shortest Path From Prince To Princess = " << dist << endl;
    }
    else {
        cout << "No Path From Prince To Princess" << endl;
    }
};

int main() {
    char simple_grid[3][3][3] =
	{
		{
			{ L'1', L'.', L'.' },
			{ L'o', L'o', L'.' },
			{ L'.', L'.', L'.' }
		},
		{
			{ L'o', L'o', L'o' },
			{ L'.', L'.', L'o' },
			{ L'.', L'o', L'o' }
		},
		{
			{ L'o', L'o', L'o' },
			{ L'o', L'.', L'.' },
			{ L'o', L'.', L'2' }
		}
	};
    int d1 = sizeof(simple_grid)/sizeof(simple_grid[0]);
    int d2 = sizeof(simple_grid[0])/sizeof(simple_grid[0][0]);
    int d3 = sizeof(simple_grid[0][0]);
    char ***grid = new char**[d1];
    for (int i=0; i<d1; i++) {
        grid[i] = new char*[d2];
        for (int j=0; j<d2; j++) {
            grid[i][j] = new char[d3];
            for (int k=0; k<d3; k++) {
                cout << simple_grid[i][j][k];
                grid[i][j][k] = simple_grid[i][j][k];
            }
            cout << endl;
        }
        cout << endl;
    }
    bfs(grid, d1, d2, d3);
    return 0;
}

