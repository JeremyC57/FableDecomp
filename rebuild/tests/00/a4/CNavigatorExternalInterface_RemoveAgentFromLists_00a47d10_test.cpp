// Behaviour test for 0x00A47D10: CNavigatorExternalInterface::RemoveAgentFromLists (every copy of the agent leaves every
// registered agent vector; other agents keep their order)
#include <cstdio>
#include <cstring>
#include <list>

class CSteeringAgent { public: int Id; };

// Host model of the STLport vector<const CSteeringAgent*> the row walks: raw-pointer iterators, erase(position) is STLport's
// `if (position + 1 != end()) memmove(...); --finish;`. The storage keeps one spare word in front, so the row's `--it` before
// begin() stays inside the array.
struct AgentVector {
    typedef const CSteeringAgent** iterator;
    const CSteeringAgent* Storage[16];
    iterator Start, Finish;
    AgentVector() : Start(Storage + 1), Finish(Storage + 1) { }
    void push_back(const CSteeringAgent* a) { *Finish++ = a; }
    iterator begin() { return Start; }
    iterator end() { return Finish; }
    unsigned size() const { return (unsigned)(Finish - Start); }
    iterator erase(iterator position) {
        if (position + 1 != end()) memmove(position, position + 1, (char*)Finish - (char*)(position + 1));
        --Finish;
        return position;
    }
};

class CNavigatorExternalInterface {
public:
    static std::list<AgentVector*> AgentLists;
    static void RemoveAgentFromLists(const CSteeringAgent* agent);
};
std::list<AgentVector*> CNavigatorExternalInterface::AgentLists;

// the row's body
void CNavigatorExternalInterface::RemoveAgentFromLists(const CSteeringAgent* agent)
{
    for (std::list<AgentVector*>::iterator list = AgentLists.begin(); list != AgentLists.end(); ++list) {
        for (AgentVector::iterator it = (*list)->begin(); it != (*list)->end(); ++it) {
            if (*it == agent) {
                AgentVector::iterator position = it;
                --it;
                (*list)->erase(position);
            }
        }
    }
}

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }

int main()
{
    CSteeringAgent a, b, c;
    a.Id = 1; b.Id = 2; c.Id = 3;
    AgentVector v1, v2, v3;
    v1.push_back(&a); v1.push_back(&b); v1.push_back(&a); v1.push_back(&a); v1.push_back(&c);
    v2.push_back(&b); v2.push_back(&c);
    v3.push_back(&a);
    CNavigatorExternalInterface::AgentLists.push_back(&v1);
    CNavigatorExternalInterface::AgentLists.push_back(&v2);
    CNavigatorExternalInterface::AgentLists.push_back(&v3);
    CNavigatorExternalInterface::RemoveAgentFromLists(&a);
    Check(v1.size() == 2 && v1.Start[0] == &b && v1.Start[1] == &c);
    Check(v2.size() == 2 && v2.Start[0] == &b && v2.Start[1] == &c);
    Check(v3.size() == 0);
    CNavigatorExternalInterface::RemoveAgentFromLists(&c);
    Check(v1.size() == 1 && v1.Start[0] == &b);
    Check(v2.size() == 1 && v2.Start[0] == &b);
    Check(CNavigatorExternalInterface::AgentLists.size() == 3);
    if (g_fail) { std::printf("00a47d10_TEST FAIL\n"); return 1; }
    std::printf("00a47d10_TEST PASS\n");
    return 0;
}