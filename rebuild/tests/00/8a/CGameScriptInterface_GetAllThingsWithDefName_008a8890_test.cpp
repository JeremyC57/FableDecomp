// Behaviour test for CGameScriptInterface::GetAllThingsWithDefName (0x008A8890): collect the script things of every thing
// whose def index matches the named def (looked up by name), appending to the caller's vector; 0 and an untouched vector
// when nothing matches, otherwise the vector's new size (things already in it count).
#include <cstdio>
#include <cstring>
#include <cstdlib>

class CScriptThing { public:
    long UID;
    CScriptThing() : UID(0) {}
    CScriptThing(const CScriptThing& o) : UID(o.UID) {}
};
struct CScriptThingVector {                 // stand-in for vector<CScriptThing>: fixed capacity + growth by copy
    CScriptThing* Start;
    unsigned int  Size;
    unsigned int  Capacity;
    CScriptThingVector() : Start(0), Size(0), Capacity(0) {}
    ~CScriptThingVector() { delete[] Start; }
    unsigned int size() const { return Size; }
    void reserve(unsigned int n)
    {
        if (n <= Capacity) return;
        CScriptThing* p = new CScriptThing[n];
        for (unsigned int i = 0; i < Size; ++i) p[i] = Start[i];
        delete[] Start;
        Start = p;
        Capacity = n;
    }
    void push_back(const CScriptThing& x)
    {
        if (Size == Capacity) reserve(Capacity ? Capacity * 2 : 1);
        Start[Size++] = x;
    }
};
class CThing { public:
    long DefIndex;
    long UID;
    CScriptThing GetScriptThing() const { CScriptThing s; s.UID = UID; return s; }
};
class CThingFilter_HasDefGlobalIndex { public:
    unsigned long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(unsigned long index) : DefGlobalIndex(index) {}
    bool operator()(const CThing& t) const { return (unsigned long)t.DefIndex == DefGlobalIndex; }
};
struct CThingPtrVector {
    CThing** Start;
    CThing** Finish;
    CThingPtrVector() : Start(0), Finish(0) {}
    ~CThingPtrVector() { if (Start) free(Start); }
    bool empty() const { return Start == Finish; }
    unsigned int size() const { return Finish - Start; }
};
class CWorld { public:
    CThing* Things;
    int     Count;
    void GetAllThings(const CThingFilter_HasDefGlobalIndex& filter, CThingPtrVector& out) const
    {
        out.Start = out.Finish = (CThing**)malloc(sizeof(CThing*) * (Count + 1));
        for (int i = 0; i < Count; ++i)
            if (filter(Things[i])) *out.Finish++ = &Things[i];
    }
};
static const char* g_defNames[] = { "CREATURE_BANDIT", "OBJECT_CHEST", "CREATURE_HOBBE" };
static long GetDefGlobalIndexFromName(const char* name)
{
    for (int i = 0; i < 3; ++i)
        if (!std::strcmp(name, g_defNames[i])) return 100 + i;
    return -1;
}

// the row's logic
static long GetAllThingsWithDefName(const CWorld& world, const char* defName, CScriptThingVector& things)
{
    CThingPtrVector found;
    world.GetAllThings(CThingFilter_HasDefGlobalIndex(GetDefGlobalIndexFromName(defName)), found);
    if (found.empty())
        return 0;
    things.reserve(found.size());
    for (CThing** it = found.Start; it != found.Finish; ++it) {
        CThing* thing = *it;
        things.push_back(thing->GetScriptThing());
    }
    return things.size();
}

int main()
{
    CThing things[5] = { {100, 1}, {102, 2}, {100, 3}, {101, 4}, {100, 5} };
    CWorld world = { things, 5 };
    CScriptThingVector out;
    if (GetAllThingsWithDefName(world, "CREATURE_BANDIT", out) != 3 || out.size() != 3 ||
        out.Start[0].UID != 1 || out.Start[1].UID != 3 || out.Start[2].UID != 5) {
        std::printf("008a8890_TEST FAIL\n");
        return 1;
    }
    // appends: the result is the vector's new size, not the number found
    if (GetAllThingsWithDefName(world, "CREATURE_HOBBE", out) != 4 || out.Start[3].UID != 2) {
        std::printf("008a8890_TEST FAIL\n");
        return 1;
    }
    // no match: 0, vector untouched
    if (GetAllThingsWithDefName(world, "NOT_A_DEF", out) != 0 || out.size() != 4) {
        std::printf("008a8890_TEST FAIL\n");
        return 1;
    }
    std::printf("008a8890_TEST PASS\n");
    return 0;
}