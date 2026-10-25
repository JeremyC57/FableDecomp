// Behaviour test for 00c372c0: CResourceMap constructor -- stores owner and data, zeroes the counters, builds the container over its embedded node, zeroes the range, sets the enabled flag and registers itself with the owner's registry
#include <cstdio>

struct CResourceMap;
struct CRegistry {
    CResourceMap* m_last;
    int m_adds;
    void Add(CResourceMap* map) { m_last = map; ++m_adds; }
};
struct COwner {
    long m_pad[2];
    CRegistry m_registry;
};

struct CNode {
    long m_kind;
    CNode(long kind) : m_kind(kind) { }
};
struct CRoot {
    CNode* m_pNode;
    CRoot(CNode* node) : m_pNode(node) { }
};
struct CContainer : CRoot {
    CNode m_node;
    CContainer() : CRoot(&m_node), m_node(0) { }
};
struct CRange {
    void* m_first;
    void* m_last;
    void* m_end;
    CRange() : m_first(0), m_last(0), m_end(0) { }
};

struct CResourceMap {
    COwner* m_pOwner;
    void* m_pData;
    long m_8;
    void* m_pResource;
    CContainer m_entries;
    CRange m_range;
    bool m_enabled;
    CResourceMap(COwner* owner, void* data)
        : m_pOwner(owner), m_pData(data), m_8(0), m_pResource(0), m_enabled(true)
    {
        m_pOwner->m_registry.Add(this);
    }
};

int main()
{
    int data;
    COwner owner;
    owner.m_registry.m_last = 0; owner.m_registry.m_adds = 0;
    CResourceMap map(&owner, &data);
    if (map.m_pOwner != &owner || map.m_pData != &data || map.m_8 != 0 || map.m_pResource != 0
        || map.m_entries.m_pNode != &map.m_entries.m_node || map.m_entries.m_node.m_kind != 0
        || map.m_range.m_first || map.m_range.m_last || map.m_range.m_end || !map.m_enabled
        || owner.m_registry.m_last != &map || owner.m_registry.m_adds != 1) { std::printf("00c372c0_TEST FAIL\n"); return 1; }
    std::printf("00c372c0_TEST PASS\n");
    return 0;
}