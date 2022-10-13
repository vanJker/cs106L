/*
 * TODO: complete this file comment.
 * File: main.cpp
 *
 * Impletation of force-directed graph layout algorithm
 */
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SimpleGraph.h"

using namespace std;
const double kPi = 3.14159265358979323;
const double krepel = 0.001;
const double kattract = 0.001;

void Welcome();

/* Custom useful functions and structs */
struct Delta
{
    vector<double> dx;
    vector<double> dy;
};
int getInteger(const string& prompt = "Time to run: ",
              const string& reprompt = "Invalid integer.");
string PromptUserForFile(ifstream& ifs,
                         const string& prompt = "Enter a filename: ",
                         const string& reprompt = "Invalid filename.");
int ReadGraph(ifstream& stream, SimpleGraph& graph);
void InitPosition(SimpleGraph& graph);
void InitDelta(SimpleGraph& graph, Delta& delta);
void RepulsiveForce(SimpleGraph& graph, Delta& delta);
void AttractiveForce(SimpleGraph& graph, Delta& delta);
void MovePosition(SimpleGraph& graph, Delta& delta);


// Main method
int main() {
    Welcome();
    /* TODO: your implementation here */
    ifstream ifs;
    SimpleGraph graph;
    Delta delta;
    InitGraphVisualizer(graph);

    // 1. Implement code to read a graph from disk.
    PromptUserForFile(ifs);
    ReadGraph(ifs, graph);

    // 2. Implement the logic to initially position the nodes.
    InitPosition(graph);
    DrawGraph(graph);

    // 3. Implement the force-directed layout algorithm.
    int elapsedTime = getInteger();
    time_t startTime = time(NULL);
    while (difftime(time(NULL), startTime) < elapsedTime) {
        InitDelta(graph, delta);
        RepulsiveForce(graph, delta);
        AttractiveForce(graph, delta);
        MovePosition(graph, delta);
        // InitGraphVisualizer(graph);
        DrawGraph(graph);
    }

    // 4. Display last graph.
    cout << "Finish" << endl;
    DrawGraph(graph);

    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

/* Get an integer from user type. */
int getInteger(const string& prompt, const string& reprompt) {
    while (true) {
        cout << prompt;
        string line; int result; char trash;
        if (!getline(cin, line))
            throw domain_error("[shortened");
        istringstream iss(line);
        if (iss >> result && !(iss >> trash))
            return result;
        cout << reprompt << endl;
    }
}

/* Prompts the user for a filename, opens the iftream to
 * the file, reprompt if the filename is not valid and
 * then return the filename. */
string PromptUserForFile(ifstream& ifs,
                         const string& prompt,
                         const string& reprompt) {
    while (true) {
        cout << prompt;
        string line; string result; char trash;
        if (!getline(cin, line))
            throw domain_error("[shortened]");
        istringstream iss(line);
        if (iss >> result && !(iss >> trash))
            ifs.open(result);
        if (ifs.is_open()) return result;
        cout << reprompt << endl;
    }
}

/* Read a graph from given file stream, and then return
 * the number of nodes in the graph. */
int ReadGraph(ifstream& ifs, SimpleGraph& graph) {
    size_t n;
    size_t v, u;
    // get number of nodes
    ifs >> n;
    // set nodes
    for (size_t i = 0; i < n; ++i)
        graph.nodes.push_back(Node{0.0, 0.0});
    // set edges
    while (ifs >> v >> u)
        graph.edges.push_back(Edge{v, u});
    return n;
}

/* Initially position all of the nodes in the graph in a
 * circle. */
void InitPosition(SimpleGraph& graph) {
    size_t n = graph.nodes.size();
    for (size_t i = 0; i < n; ++i) {
        graph.nodes[i].x = cos(2 * kPi * i / n);
        graph.nodes[i].y = sin(2 * kPi * i / n);
    }
}

/* Initally set all dx and dy to be 0. */
void InitDelta(SimpleGraph& graph, Delta& delta) {
    size_t n = graph.nodes.size();
    delta.dx.resize(n, 0.0);
    delta.dy.resize(n, 0.0);
}

/* Compute repulsive forces. */
void RepulsiveForce(SimpleGraph& graph, Delta& delta) {
    double x0, y0;
    double x1, y1;
    double Frepel;
    double theta;

    size_t n = graph.nodes.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            x0 = graph.nodes[i].x;
            y0 = graph.nodes[i].y;
            x1 = graph.nodes[j].x;
            y1 = graph.nodes[j].y;

            Frepel = krepel / sqrt((y1-y0)*(y1-y0) + (x1-x0)*(x1-x0));
            theta = atan2(y1 - y0, x1 - x0);
            delta.dx[i] -= Frepel * cos(theta);
            delta.dy[i] -= Frepel * sin(theta);
            delta.dx[j] += Frepel * cos(theta);
            delta.dy[j] += Frepel * sin(theta);
        }
    }
}

/* Compute attractive forces. */
void AttractiveForce(SimpleGraph& graph, Delta& delta) {
    Node node0, node1;
    double x0, y0;
    double x1, y1;
    double Fattract;
    double theta;

    for (Edge edge : graph.edges) {
        node0 = graph.nodes[edge.start];
        node1 = graph.nodes[edge.end];

        x0 = node0.x; y0 = node0.y;
        x1 = node1.x; y1 = node1.y;

        Fattract = kattract * ((y1-y0)*(y1-y0) + (x1-x0)*(x1-x0));
        theta = atan2(y1 - y0, x1 - x0);
        delta.dx[edge.start] += Fattract * cos(theta);
        delta.dy[edge.start] += Fattract * sin(theta);
        delta.dx[edge.end] -= Fattract * cos(theta);
        delta.dy[edge.end] -= Fattract * sin(theta);
    }
}

/* Move position of nodes by delta. */
void MovePosition(SimpleGraph& graph, Delta& delta) {
    size_t n = graph.nodes.size();
    for (size_t i = 0; i < n; ++i) {
        graph.nodes[i].x += delta.dx[i];
        graph.nodes[i].y += delta.dy[i];
    }
}


