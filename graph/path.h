#ifndef _PATH_H_
#define _PATH_H_

#include "graph.h"

#include <limits>
#include <future>

class Path
{
public:
    static std::vector<shared_ptr<Edge>> Dijkstra(Graph& g, const std::string& start, const std::string& end = "")
    {
        shared_ptr<Vertex> v = g.getVertex(start);
        g.assignIndex();
        int n = g.getVertexList().size();

        // vector for distance
        std::vector<int> distance(n, std::numeric_limits<int>::max());
        // vector for visit
        std::vector<bool> visit(n, false);
        // vector for previous node
        std::vector<int> predecessor(n, -1); // -1 means phi

        // make a pair with distance and vertex index to store it in queue
        using Pair = std::pair<int, int>;
        // create a vector to store paths
        std::vector<shared_ptr<Edge>> paths;
        // priory_queue to store Pair(distance & vertex index)
        std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> q;

        distance[v->index] = 0;
        visit[v->index] = true;
        for (auto& e : *g.getVertexList().at(v))
        {
            q.push({e->weight, e->v2->index});
            distance[e->v2->index] = e->weight;
            predecessor[e->v2->index] = v->index;
        }

        while (!q.empty())
        {
            int currentVertex = q.top().second;
            int currentDistance = q.top().first;
            q.pop();

            if (visit[currentVertex])
                continue;
            visit[currentVertex] = true;
            for (auto& e : *g.getVertexList()[g.getVertex(currentVertex)])
            {
                if (distance[e->v2->index] > e->weight + currentDistance)
                {
                    distance[e->v2->index] = e->weight + currentDistance;
                    predecessor[e->v2->index] = currentVertex;
                    q.push({distance[e->v2->index], e->v2->index});
                }
            }
        }

        // find the shortest path
        shared_ptr<Vertex> target = end != "" ? g.getVertex(end) : nullptr;
        if (target != nullptr)
        {
            int check = target->index;
            while (predecessor[check] != -1)
            {
                paths.emplace_back(g.getEdge(g.getVertex(predecessor[check]), g.getVertex(check))); // from predecessor to vertex
                check = predecessor[check];
            }
        }
        else
        {
            for (int i = 0; i < distance.size(); i++)
            {
                if (predecessor[i] != -1)
                    paths.emplace_back(g.getEdge(g.getVertex(predecessor[i]), g.getVertex(i))); // from predecessor to vertex

#ifdef DEBUG
                std::cout << g.getVertex(i)->key << " " << distance[i] << " "
                          << (predecessor[i] != -1 ? g.getVertex(predecessor[i])->key : v->key) << std::endl;
#endif
            }
        }

        mstDotFileCreate(paths, "Dijkstra");
        generatePngAsync("Dijkstra");
        return paths;
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
};

#endif//_PATH_H_