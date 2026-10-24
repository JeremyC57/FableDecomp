// Behaviour test for 0x004FE5B0 CWorldMap::DeregisterScriptedMapBrush(brush): a registered brush name's node is
// unlinked, its key destroyed, the node freed and the count dropped by one; an unknown name leaves the table alone.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_keys_destroyed = 0;

struct NodeModel {
    NodeModel* Next;
    char Key[32];
    long MapNumber;
};

struct TableModel {
    NodeModel* Head;
    unsigned int NodeCount;
    NodeModel* find(const char* name)
    {
        for (NodeModel* n = Head; n; n = n->Next)
            if (std::strcmp(n->Key, name) == 0) return n;
        return 0;                                   // end()
    }
    void erase(NodeModel* position)
    {
        NodeModel** link = &Head;                   // the rebalance step: unlink the node
        while (*link != position) link = &(*link)->Next;
        *link = position->Next;
        ++g_keys_destroyed;                         // _Destroy(&value): the key's destructor
        if (position != 0) std::free(position);
        --NodeCount;
    }
    void insert(const char* name, long map)
    {
        NodeModel* n = (NodeModel*)std::malloc(sizeof(NodeModel));
        std::strcpy(n->Key, name); n->MapNumber = map; n->Next = Head; Head = n; ++NodeCount;
    }
};

static void deregister(TableModel& t, const char* name)
{
    NodeModel* it = t.find(name);
    if (it != 0)
        t.erase(it);
}

int main()
{
    TableModel t = { 0, 0 };
    t.insert("BRIDGE_UP", 2); t.insert("GATE_OPEN", 0); t.insert("WALL_DOWN", 1);
    deregister(t, "GATE_OPEN");
    deregister(t, "NOT_THERE");
    if (t.NodeCount != 2 || g_keys_destroyed != 1 || t.find("GATE_OPEN") != 0 || t.find("BRIDGE_UP") == 0 || t.find("WALL_DOWN")->MapNumber != 1) {
        std::printf("004fe5b0_TEST FAIL\n");
        return 1;
    }
    deregister(t, "BRIDGE_UP"); deregister(t, "WALL_DOWN");
    if (t.NodeCount != 0 || t.Head != 0 || g_keys_destroyed != 3) { std::printf("004fe5b0_TEST FAIL\n"); return 1; }
    std::printf("004fe5b0_TEST PASS\n");
    return 0;
}