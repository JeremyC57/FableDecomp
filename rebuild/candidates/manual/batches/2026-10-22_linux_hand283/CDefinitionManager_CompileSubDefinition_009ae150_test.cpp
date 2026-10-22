// Behaviour test for CDefinitionManager::CompileSubDefinition (0x009AE150): an existing sub def of the same parent is
// reused (not new), one owned by another parent is removed from the parent, re-instantiated and handed the old parent,
// a missing one is instantiated (new); the def is then persisted from the text and flagged |= 5; reference counts balance.
#include <cstdio>

struct Def {
    long ref;
    unsigned char flags;
    int removed, persisted;
    bool persistNew;
    Def* parentSet;
    int destroyed;
    long subIndex, subParent;
    bool hasSub;
    Def() : ref(1), flags(0), removed(0), persisted(0), persistNew(false), parentSet(0), destroyed(0), subIndex(0), subParent(0), hasSub(false) { }
    void Release() { if (--ref == 0) ++destroyed; }
};
struct Ptr {
    Def* p;
    Ptr() : p(0) { }
    Ptr(Def* d) : p(d) { if (p) ++p->ref; }
    Ptr(const Ptr& o) : p(o.p) { if (p) ++p->ref; }
    ~Ptr() { if (p) p->Release(); }
    Ptr& operator=(const Ptr& o)
    {
        if (p != o.p) { if (p) { p->Release(); p = 0; } p = o.p; if (p) ++p->ref; }
        return *this;
    }
};

static Def g_defs[8];
static Def g_fresh;
static int g_loads;
static Ptr GetPDef(long i) { ++g_loads; return Ptr(&g_defs[i]); }
static Ptr InstantiateSubDef(long) { return Ptr(&g_fresh); }

static void CompileSubDefinition(long parentIndex)
{
    Ptr parent = GetPDef(parentIndex);
    Def* parentDef = parent.p;
    Ptr subDef;
    bool isNew;
    if (parentDef->hasSub) {
        if (parentDef->subParent != parentIndex) {
            Ptr oldParent = GetPDef(parentDef->subIndex);
            ++parentDef->removed;
            subDef = InstantiateSubDef(parentIndex);
            subDef.p->parentSet = oldParent.p;
            isNew = false;
        } else {
            subDef = GetPDef(parentDef->subIndex);
            isNew = false;
        }
    } else {
        subDef = InstantiateSubDef(parentIndex);
        isNew = true;
    }
    if (subDef.p) {
        ++subDef.p->persisted;
        subDef.p->persistNew = isNew;
        subDef.p->flags |= 5;
    }
}

int main()
{
    int ok = 1;
    // missing sub def -> new
    CompileSubDefinition(1);
    if (!g_fresh.persisted || !g_fresh.persistNew || g_fresh.flags != 5 || g_fresh.ref != 1 || g_defs[1].ref != 1) ok = 0;
    // same parent -> reuse def 3
    g_defs[2].hasSub = true; g_defs[2].subParent = 2; g_defs[2].subIndex = 3;
    CompileSubDefinition(2);
    if (g_defs[3].persisted != 1 || g_defs[3].persistNew || g_defs[3].flags != 5 || g_defs[3].ref != 1) ok = 0;
    // other parent -> moved
    g_defs[4].hasSub = true; g_defs[4].subParent = 6; g_defs[4].subIndex = 6;
    CompileSubDefinition(4);
    if (g_defs[4].removed != 1 || g_fresh.parentSet != &g_defs[6] || g_fresh.persisted != 2 || g_fresh.persistNew) ok = 0;
    for (int i = 0; i < 8; ++i)
        if (g_defs[i].ref != 1 || g_defs[i].destroyed) ok = 0;
    if (!ok) { std::printf("009ae150_TEST FAIL\n"); return 1; }
    std::printf("009ae150_TEST PASS\n");
    return 0;
}
