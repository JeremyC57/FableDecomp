// Behaviour test for 0x0069A1A0 CGameCameraManager::InitSpecialCameraList (model copy): the old list is released and
// cleared, each speciality id that resolves to a def is appended (ref-counted), unknown ids are skipped, and the index
// points at the first entry.
#include <cstdio>
#include <vector>

struct Def { long RefCount; int destroyed; void AddRef() { ++RefCount; } void Release() { if (--RefCount == 0) ++destroyed; } };

struct DefPtr {
    Def* Object;
    DefPtr() : Object(0) { }
    DefPtr(const DefPtr& o) : Object(o.Object) { if (Object) Object->AddRef(); }
    DefPtr& operator=(const DefPtr& o) { if (o.Object) o.Object->AddRef(); if (Object) Object->Release(); Object = o.Object; return *this; }
    ~DefPtr() { if (Object) Object->Release(); }
};

static Def g_defs[4];
static bool GetDef(long id, DefPtr& out)
{
    if (id < 0 || id >= 4) return false;
    DefPtr found; found.Object = &g_defs[id]; g_defs[id].AddRef();
    out = found;
    return true;
}

struct Manager {
    std::vector<DefPtr> SpecialCameraList;
    int SpecialCameraListIndex;
    std::vector<long> SpecialityCameraList;
    void InitSpecialCameraList()
    {
        SpecialCameraList.clear();
        for (std::vector<long>::const_iterator it = SpecialityCameraList.begin(); it != SpecialityCameraList.end(); ++it) {
            long globalIndex = *it;
            DefPtr def;
            if (GetDef(globalIndex, def))
                SpecialCameraList.push_back(def);
        }
        SpecialCameraListIndex = 0;
    }
};

int main()
{
    for (int i = 0; i < 4; ++i) { g_defs[i].RefCount = 1; g_defs[i].destroyed = 0; }
    Manager m;
    m.SpecialCameraListIndex = 7;
    m.SpecialityCameraList.push_back(2);
    m.SpecialityCameraList.push_back(9);                         // unknown: skipped
    m.SpecialityCameraList.push_back(0);
    m.SpecialityCameraList.push_back(2);
    m.InitSpecialCameraList();
    int ok = m.SpecialCameraList.size() == 3 && m.SpecialCameraList[0].Object == &g_defs[2] &&
             m.SpecialCameraList[1].Object == &g_defs[0] && m.SpecialCameraList[2].Object == &g_defs[2] &&
             m.SpecialCameraListIndex == 0 && g_defs[2].RefCount == 3 && g_defs[0].RefCount == 2;
    m.SpecialityCameraList.clear();
    m.SpecialityCameraList.push_back(1);
    m.InitSpecialCameraList();                                    // the old entries are released
    ok = ok && m.SpecialCameraList.size() == 1 && g_defs[2].RefCount == 1 && g_defs[0].RefCount == 1 &&
         g_defs[1].RefCount == 2 && g_defs[2].destroyed == 0;
    if (!ok) { std::printf("0069a1a0_TEST FAIL\n"); return 1; }
    std::printf("0069a1a0_TEST PASS\n");
    return 0;
}