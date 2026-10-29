// Behaviour test for 007ee5a0: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCVillageMember::OnInitialActivate() -- re-attaches the owner to the village recorded by UID when it is not already attached, and (with the village-member check enabled) looks for a village when an owner that needs one has none recorded
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
class CTCBase;
class CTCVillage;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingVillage : public CThing { public: };

class CTCVillage { public:
    void AddThingToVillage(CThing& thing);      // 0x0065B854
};
class CThingManager { public:
    CThing* GetPThingWithUID(unsigned __int64 uid);   // 0x0051ED80
};
class CThingFilter_HasTC { public:
    long Type;                                  // ETCInterfaceType
    CThingFilter_HasTC(long type) : Type(type) {}
};
class CThingSearchTools { public:
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out) const;   // 0x007EE660
};
extern bool g_bCheckVillageMembers_013b86d8;    // 0x013B86D8

class CVillageMemberDef { public:
    unsigned char _pad_0x00[0x25];
    bool          NeedsVillageToFunctionCorrectly;   // +0x25
};

class CTCVillageMember { public:
    void*              __vftable;               // +0x00
    CThing*            ParentThing;             // +0x04 (CTCBase)
    unsigned char      _pad_0x08[0x4];
    CVillageMemberDef* PDef;                    // +0x0c CDefPointer<const CVillageMemberDef>::Object
    unsigned char      PVillage[0x8];           // +0x10 CIntelligentPointer<CThing>
    unsigned __int64   VillageUID;              // +0x18
    CThingManager* GetThingManager();           // 0x00686AE0 (CTCBase)
    CThingSearchTools* GetThingSearchTools();   // 0x00686D80 (CTCBase)
    CTCVillage* GetPVillage() const;            // 0x007EE540
    void OnInitialActivate();
};

void CTCVillageMember::OnInitialActivate()
{
    if (VillageUID != 0) {
        CThing* villageThing = GetThingManager()->GetPThingWithUID(VillageUID);
        if (villageThing) {
            CTCVillage* village;
            villageThing->GetInterface(&village, 0x22);
            if (village != GetPVillage())
                village->AddThingToVillage(*ParentThing);
        }
    }
    if (g_bCheckVillageMembers_013b86d8 && VillageUID == 0 && PDef && PDef->NeedsVillageToFunctionCorrectly) {
        CThingVillage* village;
        GetThingSearchTools()->GetPFirstThing(CThingFilter_HasTC(0x22), &village);
    }
}

// ---- stubs and driver ----
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static CThing* g_added; static int g_adds;
static CTCVillage g_village;
void CTCVillage::AddThingToVillage(CThing& thing) { if (this == &g_village) ++g_adds; g_added = &thing; }
static CThing g_villageThing; static unsigned __int64 g_askedUID;
CThing* CThingManager::GetPThingWithUID(unsigned __int64 uid) { g_askedUID = uid; return uid == 0x100000002ULL ? &g_villageThing : 0; }
static int g_searches; static long g_searchType;
template <class TFilter, class TThing> bool CThingSearchTools::GetPFirstThing(const TFilter& filter, TThing** out) const
{ ++g_searches; g_searchType = filter.Type; *out = 0; return false; }
bool g_bCheckVillageMembers_013b86d8 = false;
static CThingManager g_manager; static CThingSearchTools g_tools; static CTCVillage* g_current;
CThingManager* CTCVillageMember::GetThingManager() { return &g_manager; }
CThingSearchTools* CTCVillageMember::GetThingSearchTools() { return &g_tools; }
CTCVillage* CTCVillageMember::GetPVillage() const { return g_current; }

int main()
{
    static CThing owner;
    static CVillageMemberDef def;
    static CTCVillageMember member;
    member.ParentThing = &owner; member.PDef = &def;
    g_villageThing.m_presentBits[0x22 >> 5] = 1UL << (0x22 & 31);
    g_entries[0].type = 0x22; g_entries[0].component = (CTCBase*)&g_village;
    g_villageThing.m_interfaces.first = g_entries; g_villageThing.m_interfaces.last = g_entries + 1;
    member.VillageUID = 0x100000002ULL;
    member.OnInitialActivate();                                    // not attached yet: attach
    if (g_askedUID != 0x100000002ULL || g_adds != 1 || g_added != &owner || g_searches) { std::printf("007ee5a0_TEST FAIL (attach)\n"); return 1; }
    g_current = &g_village;
    member.OnInitialActivate();                                    // already attached
    if (g_adds != 1) { std::printf("007ee5a0_TEST FAIL (attached)\n"); return 1; }
    member.VillageUID = 7;                                         // no such thing
    member.OnInitialActivate();
    if (g_adds != 1 || g_searches) { std::printf("007ee5a0_TEST FAIL (missing)\n"); return 1; }
    member.VillageUID = 0; g_bCheckVillageMembers_013b86d8 = true;
    member.OnInitialActivate();                                    // def does not need a village
    if (g_searches) { std::printf("007ee5a0_TEST FAIL (no need)\n"); return 1; }
    def.NeedsVillageToFunctionCorrectly = true;
    member.OnInitialActivate();
    if (g_searches != 1 || g_searchType != 0x22) { std::printf("007ee5a0_TEST FAIL (search)\n"); return 1; }
    std::printf("007ee5a0_TEST PASS\n");
    return 0;
}