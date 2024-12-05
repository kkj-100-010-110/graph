#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <memory>
#include <fstream>
#include <exception>

using std::make_shared;
using std::shared_ptr;

class Vertex
{
public:
    explicit Vertex(const std::string& key);
    Vertex(const std::string& key, int index);

    std::string key;
    // std::string data;
    int index;
    size_t degree = 0;

    friend bool operator==(const shared_ptr<Vertex> lhs, const shared_ptr<Vertex> rhs);

    struct VertexHash
    {
        size_t operator()(const shared_ptr<Vertex> v) const { return std::hash<std::string>()(v->key); }
    };
};

class Edge
{
public:
    Edge(const std::string& key, int weight = 0);
    Edge(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2, const std::string& key, int weight = 0);
    Edge(const std::string& s1, const std::string& s2, const std::string& key, int weight = 0);

    std::string key;
    shared_ptr<Vertex> v1; // consider this as from
    shared_ptr<Vertex> v2; // consider this as to
    int weight;

    friend bool operator==(const shared_ptr<Edge> lhs, const shared_ptr<Edge> rhs);

    struct EdgeHash
    {
        size_t operator()(shared_ptr<Edge> e) const { return std::hash<std::string>()(e->key); }
    };

    static bool compareWeight(const shared_ptr<Edge>& e1, const shared_ptr<Edge>& e2) { return e1->weight > e2->weight; }
};

class Graph
{
public:
    std::unordered_map<int, std::string> numKeySet;

    Graph();
    void readFile(const std::string& csvFilename);
    void insertVertex(const std::string& key);
    void insertEdge(const std::string& key, int weight = 0);
    /** to use for csv file read */
    void insertEdge(const std::string& vertex1key, const std::string& vertex2key, const std::string& key, int weight = 0);
    shared_ptr<Vertex> getVertex(const std::string& key);
    shared_ptr<Vertex> getVertex(int index);
    shared_ptr<Edge> getEdge(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2);
    shared_ptr<std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash>> incidentEdges(shared_ptr<Vertex> v) const;
    shared_ptr<std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash>> incidentEdgesByMatrix(shared_ptr<Vertex> v) const;
    bool areAdjacent(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2);
    bool areAdjacentByMatrix(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2);
    void assignIndex();
    void buildAdjacencyMatrix();
    std::unordered_map<shared_ptr<Vertex>, shared_ptr<std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash>>, Vertex::VertexHash> getVertexList() const;
    std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash> getEdgeList() const;
    
    // Traversal
    /** set up visit */
    void initState();
    /** explore all vertices */
    void BFS(const std::string& start);
    void DFS(const std::string& start);
    void DFSByStack(const std::string& start);

    // Display
    void display();

    // Print
    void printVertex(shared_ptr<Vertex> v);
    void printEdge(const std::string& key1, const std::string& key2);
    void printEdge(shared_ptr<Edge> e);
    void printIncidentEdges(shared_ptr<Vertex> v);
    void printVertexList();
    void printEdgeList();

private:
    std::unordered_map<shared_ptr<Vertex>, shared_ptr<std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash>>, Vertex::VertexHash> vertexList;
    std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash> edgeList;
    std::vector<std::vector<shared_ptr<Edge>>> adjacencyMatrix;

    std::vector<bool> visit;

    // Traversal utility functions
    void _BFS(shared_ptr<Vertex> v);
    void DFSCommon(const std::string& start, std::function<void(shared_ptr<Vertex>)> traversalFunc);
    void _DFS(shared_ptr<Vertex> v);
    void _DFSByStack(shared_ptr<Vertex> v);
};

#endif//_GRAPH_HPP_