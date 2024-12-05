#ifndef _MST_H_
#define _MST_H_

#include <cstdlib>
#include <future>

#include "disjoint_set.h"
#include "graph.h"

/** Minimum Spanning Tree */
class MST
{
public:
    static std::vector<shared_ptr<Edge>> Kruskal(Graph& g)
    {
        // to give each vertex an index
        g.assignIndex();
        // Store edges in the MST
        std::vector<shared_ptr<Edge>> mstEdges;
        // set a disjoint set up
        DisjointSet ds(g.getVertexList().size());
        // build heap
        std::priority_queue<shared_ptr<Edge>, std::vector<shared_ptr<Edge>>, decltype(&Edge::compareWeight)> q(&Edge::compareWeight);
        for (auto& e : g.getEdgeList())
            q.push(e);
        
        int numSet = ds.getNumberOfSets();
        while (!q.empty())
        {
            shared_ptr<Edge> e = q.top();
            q.pop();

            int updatedNumSet = ds.unionSet(e->v1->index, e->v2->index);
            if (numSet > updatedNumSet)
            {
                numSet = updatedNumSet;
                mstEdges.emplace_back(e);
            }
            if (numSet == 1)
                break;
        }

        if (ds.getNumberOfSets() > 1)
        {
            std::cout << "Graph is disconnected. Number of components: " << ds.getNumberOfSets() << std::endl;
            mstEdges.clear();
            return mstEdges;
        }
        else
        {
            std::cout << "Minimum Spanning Tree (MST):" << std::endl;
            for (shared_ptr<Edge> e : mstEdges)
                g.printEdge(e);
            std::cout << calculateMSTWeight(mstEdges) << std::endl;
            mstDotFileCreate(mstEdges, "Kruskal");
            generatePngAsync("Kruskal");
            return mstEdges;
        }
    }

    static std::vector<shared_ptr<Edge>> Prim(Graph& g, const std::string& start)
    {
        // start Vertex
        shared_ptr<Vertex> v = g.getVertex(start);
        // assign an index to each vertex;
        g.assignIndex();
        // store edges in mst
        std::vector<shared_ptr<Edge>> mstEdges;
        // priory_queue to store edges and sort them by weight
        std::priority_queue<shared_ptr<Edge>, std::vector<shared_ptr<Edge>>, decltype(&Edge::compareWeight)> q(&Edge::compareWeight);
        // partition
        std::unordered_set<int> partition;

        // start vertex partition
        partition.insert(v->index);
        // push all edges of the start vertex
        for (auto& e : *g.getVertexList()[v])
            q.push(e);

        while (!q.empty())
        {
            shared_ptr<Edge> edge = q.top();
            q.pop();
            int vertexIndex = edge->v2->index;
            if (!partition.count(vertexIndex)) // check if visited
            {
                mstEdges.emplace_back(edge);
                partition.emplace(vertexIndex);
            
                for (auto& e : *g.getVertexList()[edge->v2])
                {
                    if (!partition.count(e->v2->index))
                        q.push(e);
                }
            }
        }
        if (partition.size() < g.getVertexList().size())
        {
            std::cout << "Graph is disconnected. Remained vertices: " << g.getVertexList().size() - partition.size() << std::endl;
            mstEdges.clear();
            return mstEdges;
        }
        else
        {
            std::cout << "Minimum Spanning Tree (MST):" << std::endl;
            for (shared_ptr<Edge> e : mstEdges)
                g.printEdge(e);
            std::cout << calculateMSTWeight(mstEdges) << std::endl;
            mstDotFileCreate(mstEdges, "Prim");
            generatePngAsync("Prim");
            return mstEdges;
        }
    }

private:
    static void mstDotFileCreate(const std::vector<shared_ptr<Edge>>& mstEdges, const std::string& str)
    {
        std::string dot = str + ".dot";
        std::ofstream dotFile(dot);
        dotFile << "graph MST {\n";
        // configure graph style
        dotFile << "    layout=neato;\n";
        dotFile << "    splines=false;\n";
        dotFile << "    node [shape=ellipse, style=filled, fillcolor=lightgreen];\n";
        auto normalizeLength = [](int weight) { return std::max(1.0, std::min(2.0, weight / 4.0)); };
        // print edges
        for (shared_ptr<Edge> e : mstEdges)
        {
            double edgeLength = normalizeLength(e->weight);
            dotFile << "    \"" << e->v1->key << "\" -- \"" << e->v2->key << "\" [label=" << e->weight << ", len=" << edgeLength << "];\n";
        }
        dotFile << "}\n";
        dotFile.close();
    }

    static void generatePngAsync(const std::string& dotFileName)
    {
        std::string command = "neato -Tpng " + dotFileName + ".dot -o " + dotFileName + ".png";

        // 비동기 실행 및 결과 확인
        auto future = std::async(std::launch::async, [&]() { return std::system(command.c_str()); });

        int result = future.get(); // check the result of the command
        if (result == 0)
            std::cout << "Successfully generated " << dotFileName << ".png\n";
        else
            std::cerr << "Failed to generate " << dotFileName << ".png\n";
    }

    static int calculateMSTWeight(const std::vector<shared_ptr<Edge>>& mstEdges)
    {
        int totalWeight = 0;
        for (const auto& edge : mstEdges)
        {
            totalWeight += edge->weight;
        }
        return totalWeight;
    }
};

#endif//_MST_H_