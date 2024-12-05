#include "graph.h"
#include "mst.h"
#include "path.h"

void printOptions();
void processOptions();
void processPrim(Graph& g);
void processKruskal(Graph& g);
void processDijkstra(Graph& g);

int main()
{
    Graph g;
    g.readFile("walking_times_between_zone1_london_underground_stations.csv");

    while (1)
    {
        printOptions();
        std::cout << "Enter a key: ";
        char key;
        std::cin >> key;
        std::cin.ignore();
        switch (key)
        {
        case 'p': processPrim(g); break;
        case 'k': processKruskal(g); break;
        case 'd': processDijkstra(g); break;
        case 'q': std::cout << "Good-bye" << std::endl; return 0;
        default: std::cout << "Invalid. Choose 'p', 'k', 'd', or 'q'." << std::endl;
        }
    }
}

void printOptions()
{
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "|Algorithm-------------------------------------|key|" << std::endl;
    std::cout << "|Prim(MST)                 .png file generate -|'p'|" << std::endl;
    std::cout << "|Kruskal(MST)              .png file generate -|'k'|" << std::endl;
    std::cout << "|Dijkstra(shortest path)   .png file generate -|'d'|" << std::endl;
    std::cout << "|Exit                                         -|'q'|" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
}

void processPrim(Graph& g)
{
    g.display();
    std::cout << "Please Enter a station number or name where you start: ";
    std::string start;
    std::getline(std::cin, start);
    try
    {
        shared_ptr<Vertex> v = g.getVertex(start);
        if (v == nullptr)
        {
            int num = std::stoi(start);
            start = g.numKeySet[num];
        }
        MST::Prim(g, start);
    }
    catch (const std::exception& e)
    {
        std::cout << "Your input is wrong. Try again." << std::endl;
    }
}

void processKruskal(Graph& g)
{
    MST::Kruskal(g);
}

void processDijkstra(Graph& g)
{
    g.display();
    std::cout << "Please Enter a station number or name where you start: ";
    std::string start;
    std::getline(std::cin, start);
    std::cout << "Please Enter a station number or name where you end: ";
    std::string end;
    std::getline(std::cin, end);
    try
    {
        shared_ptr<Vertex> v1 = g.getVertex(start);
        shared_ptr<Vertex> v2 = g.getVertex(end);
        if (v1 == nullptr)
        {
            int numStart = std::stoi(start);
            start = g.numKeySet[numStart];
        }
        if (v2 == nullptr)
        {
            int numEnd = std::stoi(end);
            end = g.numKeySet[numEnd];
        }
        Path::Dijkstra(g, start, end);
    }
    catch (const std::exception& e)
    {
        std::cout << "Your input is wrong. Try again." << std::endl;
    }
}