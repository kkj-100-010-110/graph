#include "graph.h"

// Vertex class
Vertex::Vertex(const std::string& key) : key(key) {}
Vertex::Vertex(const std::string& key, int index) : key(key), index(index) {}
// Equality operator for Vertex based on key
bool operator==(const shared_ptr<Vertex> lhs, const shared_ptr<Vertex> rhs) { return lhs->key == rhs->key; }

// Edge class
Edge::Edge(const std::string& key, int weight) : key(key), v1(nullptr), v2(nullptr), weight(weight) {}
Edge::Edge(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2, const std::string& key, int weight) : key(key), v1(v1), v2(v2), weight(weight) {}
Edge::Edge(const std::string& s1, const std::string& s2, const std::string& key, int weight) : key(key), weight(weight)
{
    auto vertex1 = make_shared<Vertex>(s1);
    auto vertex2 = make_shared<Vertex>(s2);
    v1 = vertex1;
    v2 = vertex2;
}
// Equality operator for Edge based on key
bool operator==(const shared_ptr<Edge> lhs, const shared_ptr<Edge> rhs) { return lhs->key == rhs->key; }

// Graph class

Graph::Graph() {}

void Graph::readFile(const std::string& csvFilename)
{
    std::ifstream csvFile{csvFilename};
    std::string csvLine;
    if (csvFile.is_open())
    {
        std::getline(csvFile, csvLine); // skip the first line
        while (std::getline(csvFile, csvLine))
        {
            try
            {
                std::vector<std::string> tokens;
                std::string token;
                int start, end;
                start = csvLine.find_first_not_of(',', 0);
                do
                {
                    end = csvLine.find_first_of(',', start);
                    if (start == csvLine.length() || start == end)
                        break;
                    if (end >= 0)
                        token = csvLine.substr(start, end - start);
                    else
                        token = csvLine.substr(start, csvLine.length() - start);
                    tokens.push_back(token);
                    start = end + 1;
                } while (end > 0);
                insertVertex(tokens[0]);
                insertVertex(tokens[1]);
                insertEdge(tokens[0], tokens[1], tokens[4], std::stoi(tokens[2]));
            }
            catch (const std::exception& e)
            {
                std::cout << "Graph::readFile(): " << e.what() << '\n';
            }
        }
    }
}

void Graph::insertVertex(const std::string& key)
{
    auto v = make_shared<Vertex>(key);
    if (vertexList.find(v) == vertexList.end())
        vertexList[v] = make_shared<std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash>>();
}

void Graph::insertEdge(const std::string& key, int weight)
{
    auto e = make_shared<Edge>(key, weight);
    edgeList.emplace(e);
}

/** to use for csv file read */
void Graph::insertEdge(const std::string& vertex1key, const std::string& vertex2key, const std::string& key, int weight)
{
    auto v1 = getVertex(vertex1key);
    auto v2 = getVertex(vertex2key);
    auto e = make_shared<Edge>(v1, v2, key, weight);
    v1->degree += 1;
    vertexList[v1]->emplace(e);
    edgeList.emplace(e);
}

shared_ptr<Vertex> Graph::getVertex(const std::string& key)
{
    auto it = std::find_if(vertexList.begin(), vertexList.end(), [&key](const auto& pair) { return pair.first->key == key; });
    if (it == vertexList.end())
    {
        return nullptr;
// #ifndef DEBUG
//         throw std::runtime_error("Vertex not found: " + key);
// #endif
    }
    return it->first;
}

shared_ptr<Vertex> Graph::getVertex(int index)
{
    auto it = std::find_if(vertexList.begin(), vertexList.end(), [index](const auto& pair) { return pair.first->index == index; });
    if (it == vertexList.end())
    {
#ifndef DEBUG
        throw std::runtime_error("Vertex with the index not found: " + std::to_string(index));
#endif
        return nullptr;
    }
    return it->first;
}

shared_ptr<Edge> Graph::getEdge(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2)
{
    for (auto& e : edgeList)
    {
        if (e->v1->key == v1->key && e->v2->key == v2->key)
            return e;
    }
    return nullptr;
}

shared_ptr<std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash>> Graph::incidentEdges(shared_ptr<Vertex> v) const { return vertexList.at(v); }

shared_ptr<std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash>> Graph::incidentEdgesByMatrix(shared_ptr<Vertex> v) const
{
    shared_ptr<std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash>> res = make_shared<std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash>>();
    for (int i = 0; i < vertexList.size(); i++)
    {
        if (adjacencyMatrix[v->index][i])
            res->emplace(adjacencyMatrix[v->index][i]);
    }
    return res;
}

std::unordered_map<shared_ptr<Vertex>, shared_ptr<std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash>>, Vertex::VertexHash>
Graph::getVertexList() const
{
    return vertexList;
}

std::unordered_set<shared_ptr<Edge>, Edge::EdgeHash> Graph::getEdgeList() const { return edgeList; }

bool Graph::areAdjacent(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2)
{
    auto from = vertexList.find(v1);
    auto to = vertexList.find(v2);
    if (from == vertexList.end() || to == vertexList.end())
        throw std::runtime_error("Vertex not found");
    auto& it = v1->degree > v2->degree ? to->second : from->second;
    std::string key = v1->degree > v2->degree ? v1->key : v2->key;
    for (auto& e : *it)
    {
        if (e->v1->key == key || e->v2->key == key)
            return true;
    }
    return false;
}

bool Graph::areAdjacentByMatrix(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2)
{
    return adjacencyMatrix[v1->index][v2->index] == nullptr ? false : true;
}

void Graph::assignIndex()
{
    int n = vertexList.size();
    int index = 0;
    for (auto& e : vertexList)
        e.first->index = index++;
}

void Graph::buildAdjacencyMatrix()
{
    assignIndex();
    int n = vertexList.size();

    adjacencyMatrix.resize(n, std::vector<shared_ptr<Edge>>(n, nullptr)); // Resize for a square matrix

    for (auto& edge : edgeList)
    {
        int x = edge->v1->index;
        int y = edge->v2->index;
        adjacencyMatrix[x][y] = edge;
    }
}

// Display
void Graph::display()
{
    std::vector<std::string> stations;
    for (auto& e : vertexList)
        stations.emplace_back(e.first->key);
    std::sort(stations.begin(), stations.end());

    size_t size = 0;
    for (auto& e : stations)
        size = std::max(size, e.length());
    size += 5;

    std::cout << std::endl;
    // std::cout << std::setw(size * 5 / 2) << "<<STATIONS>>" << std::endl;
    std::cout
    << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< STATION LIST >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
    << std::endl;
    std::cout << std::endl;
    int i = 1;
    for (auto& e : stations)
    {
        numKeySet.emplace(i, e);
        std::string s = "(" + std::to_string(i) + ")" + e;
        std::cout << std::setw(size) << std::left << s;
        if (i % 5 == 0)
            std::cout << std::endl;
        i++;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

// Print
void Graph::printVertex(shared_ptr<Vertex> v)
{
    auto& edges = vertexList[v];
    std::cout << "Vertex key: " << v->key << ", degree: " << v->degree << ", index: " << v->index << std::endl;
    std::cout << "  Incidient edges" << std::endl;
    for (auto& e : *edges)
        std::cout << "   Edge key: " << e->key << " to " << e->v2->key << ", weight: " << e->weight << std::endl;
}

void Graph::printEdge(const std::string& key1, const std::string& key2)
{
    printEdge(getEdge(getVertex(key1), getVertex(key2)));
}

void Graph::printEdge(shared_ptr<Edge> e)
{
    std::cout << "Key: " << e->key << " from " << e->v1->key << " to " << e->v2->key << " minutes: " << e->weight << std::endl;
}

void Graph::printIncidentEdges(shared_ptr<Vertex> v)
{
    for (auto& e : *vertexList[v])
        printEdge(e);
}

void Graph::printVertexList()
{
    for (auto& v : vertexList)
    {
        std::cout << "Vertex key: " << v.first->key << ", degree: " << v.first->degree << ", index: " << v.first->index << std::endl;
        std::cout << "  Incidient edges" << std::endl;
        for (auto& e : *v.second)
            std::cout << "   Edge key: " << e->key << " to " << e->v2->key << ", weight: " << e->weight << std::endl;
    }
    std::cout << "Size: " << vertexList.size() << std::endl;
}

void Graph::printEdgeList()
{
    for (auto& e : edgeList)
        std::cout << "Key: " << e->key << " from " << e->v1->key << " to " << e->v2->key << " minutes: " << e->weight << std::endl;
    std::cout << "Size: " << edgeList.size() << std::endl;
}


// Traversal
void Graph::initState()
{
    if (visit.size() == vertexList.size())
        std::fill(visit.begin(), visit.end(), false);
    else
        visit.resize(vertexList.size(), false);
}

void Graph::BFS(const std::string& start)
{
    assignIndex();
    initState();

    shared_ptr<Vertex> v = getVertex(start); // starting vertex
    int i = 1;
    std::cout << "G" << i++ << std::endl;
    _BFS(v);
    for (auto& e : vertexList)
    {
        shared_ptr<Vertex> vertex = e.first;
        if (!visit[vertex->index])
        {
            std::cout << "G" << i++ << std::endl;
            _BFS(vertex);
        }
    }
}

void Graph::_BFS(shared_ptr<Vertex> v)
{
    std::queue<shared_ptr<Vertex>> q;
    visit[v->index] = true;
    q.push(v);
    while (!q.empty())
    {
        shared_ptr<Vertex> tmp = q.front();
        q.pop();
        std::cout << tmp->key << std::endl;
        auto edges = incidentEdges(tmp);
        for (auto& e : *edges)
        {
            shared_ptr<Vertex> neighbor = e->v2;
            if (!visit[neighbor->index])
            {
                visit[neighbor->index] = true;
                q.push(neighbor);
            }
        }
    }
}

/** explore all vertices */
void Graph::DFS(const std::string& start)
{
    DFSCommon(start, [this](shared_ptr<Vertex> v) { _DFS(v); });
}

void Graph::DFSByStack(const std::string& start)
{
    DFSCommon(start, [this](shared_ptr<Vertex> v) { _DFSByStack(v); });
}

void Graph::DFSCommon(const std::string& start, std::function<void(shared_ptr<Vertex>)> traversalFunc)
{
    assignIndex();
    initState();

    shared_ptr<Vertex> v = getVertex(start);
    int i = 1;
    std::cout << "G" << i++ << std::endl;
    traversalFunc(v);
    for (auto& e : vertexList)
    {
        shared_ptr<Vertex> vertex = e.first;
        if (!visit[vertex->index])
        {
            std::cout << "G" << i++ << std::endl;
            traversalFunc(vertex);
        }
    }
}

// recursion
void Graph::_DFS(shared_ptr<Vertex> v)
{
    if (visit[v->index])
        return;
    else
    {
        visit[v->index] = true;
        std::cout << v->key << std::endl;
        for (auto& e : *vertexList[v])
            _DFS(e->v2);
    }
}

void Graph::_DFSByStack(shared_ptr<Vertex> v)
{
    std::stack<shared_ptr<Vertex>> s;
    visit[v->index] = true;
    s.push(v);
    while (!s.empty())
    {
        shared_ptr<Vertex> current = s.top();
        std::cout << current->key << std::endl;
        s.pop();
        auto edges = incidentEdges(current);
        for (auto& e : *edges)
        {
            shared_ptr<Vertex> neighbor = e->v2;
            if (!visit[neighbor->index])
            {
                visit[neighbor->index] = true;
                s.push(neighbor);
            }
        }
    }
}