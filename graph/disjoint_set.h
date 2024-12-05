#ifndef _DISJOINT_SET_H_
#define _DISJOINT_SET_H_

#include <iostream>
#include <vector>

class DisjointSet
{
public:
    DisjointSet(int n)
    {
        sets.resize(n, -1);
        numberOfSets = n;
    }

    /** find the representive of the set that has target, and return it. */
    int find(int target)
    {
        if (sets[target] < 0)
            return target;
        else
        {
            int rep = find(sets[target]);
            sets[target] = rep;
            return rep;
        }
    }
    /** union two disjoint sets */
    int unionSet(int set1, int set2)
    {
        int s1 = find(set1);
        int s2 = find(set2);
        if (s1 == s2)
        {
            return numberOfSets;
        }
        if (sets[s1] < sets[s2])
        {
            sets[s1] += sets[s2];
            sets[s2] = s1;
        }
        else
        {
            sets[s2] += sets[s1];
            sets[s1] = s2;
        }
        numberOfSets -= 1;
        return numberOfSets;
    }

    int getNumberOfSets() const { return numberOfSets; }

private:
    std::vector<int> sets;
    int numberOfSets;
};

#endif//_DISJOINT_SET_H_