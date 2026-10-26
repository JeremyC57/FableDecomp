// behaviour test for 0x00910100 CAIStateGroup_MinionListenToStory::IsRunnable (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
// Grade: RELOCATION_MATCH -- one `return a && b && c && d;` with a long-returning (BOOL) GetInterface gives retail's int-width `mov eax,1` / `xor eax,eax`; the search runs in an inline FindMarker(filter, index) whose block-local out thing shares the key word while the function-scope filter keeps its own word, its index stored right before the call; same-TU LowerBound body
// CAIStateGroup_MinionListenToStory::IsRunnable() -- runnable during the village's cook time when a STORY_PLACE_MARKER
// thing exists in the world.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00910100, length 180 bytes (+ int3 padding).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("STORY_PLACE_MARKER" @ 0x0128F5F0),
// CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the bitset dword at
// +0x24), GFGetPVillageInterface @ 0x007EE540 (twice), CTCVillage::IsCookTime @ 0x00658A80,
// CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPFirstThing<CThingFilter_HasDefGlobalIndex>
// @ 0x00910740 (filter {def index} and the out thing in the 8-byte frame).
// Shape: the def index is looked up first through a CCharString temporary; the out-thing slot shares the key word.
// The engine header flattens the base, so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)


class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline long GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return 1;
        }
        return 0;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
};

struct CTCVillager;
class CTCVillage { public:
    bool IsCookTime();                          // 0x00658A80
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

class CThingFilter_HasDefGlobalIndex { public:
    long DefGlobalIndex;
};

class CThingSearchTools { public:
    bool GetPFirstThing(const CThingFilter_HasDefGlobalIndex& filter, CThing** ppThing);   // 0x00910740
};

struct CAIStateGroup_MinionListenToStory : CAIStateGroupBase {
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000 (CAIStateGroupBase)
    bool FindMarker(CThingFilter_HasDefGlobalIndex& filter, long markerIndex) { CThing* marker; filter.DefGlobalIndex = markerIndex; return PeekThingSearchTools()->GetPFirstThing(filter, &marker); }
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionListenToStory::IsRunnable()
{
    long markerIndex;
    {
        CCharString name("STORY_PLACE_MARKER");
        markerIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThingFilter_HasDefGlobalIndex filter;
    CTCVillager* villager;
    return PBrain->PCreature->GetInterface(&villager, 0x23) && GFGetPVillageInterface(villager) != 0
        && GFGetPVillageInterface(villager)->IsCookTime() && FindMarker(filter, markerIndex);
}


static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static int strCtor, strDtor, idxCalls, villCalls, cookCalls, peekCalls, firstCalls;
static const char* strText; static long strLen;
static long markerIdx, filterIdx;
static bool cookTime, found;
static CTCVillage village; static CTCVillage* villageRet;
static CTCVillager* villArg;
static CThingSearchTools tools;
static CThing** firstOut;
CCharString::CCharString(const char* text, long length) { ++strCtor; strText = text; strLen = length; PStringData = 0; }
CCharString::~CCharString() { ++strDtor; }
static CGameDefinitionManager gdm;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return gdm; }
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) { ++idxCalls; return markerIdx; }
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* v) { ++villCalls; villArg = v; return villageRet; }
bool CTCVillage::IsCookTime() { ++cookCalls; return cookTime; }
CThingSearchTools* CAIStateGroup_MinionListenToStory::PeekThingSearchTools() { ++peekCalls; return &tools; }
bool CThingSearchTools::GetPFirstThing(const CThingFilter_HasDefGlobalIndex& f, CThing** pp) { ++firstCalls; filterIdx = f.DefGlobalIndex; firstOut = pp; return found; }
CAIStateGroupBase::~CAIStateGroupBase() { }
void CAIStateGroupBase::v01() { } void CAIStateGroupBase::v02() { } void CAIStateGroupBase::v03() { } void CAIStateGroupBase::v04() { }
bool CAIStateGroupBase::IsStillValid() { return false; }
void CAIStateGroupBase::v06() { } void CAIStateGroupBase::v07() { } void CAIStateGroupBase::v08() { }
void CAIStateGroupBase::Cleanup() { } void CAIStateGroupBase::OnReplace() { } bool CAIStateGroupBase::IsRunnable() { return false; }
void CAIStateGroupBase::Update() { } void CAIStateGroupBase::InitStateGroup() { }

struct CTCVillager { int x; };
static CTCVillager villagerComp;
static CThing creature;
static CInterfaceMap::SEntry entries[3];
static CAIBrain brain;

static void Reset(bool hasVillager)
{
    strCtor = strDtor = idxCalls = villCalls = cookCalls = peekCalls = firstCalls = 0;
    memset(&creature, 0, sizeof creature);
    entries[0].type = 5;    entries[0].component = 0;
    entries[1].type = 0x23; entries[1].component = (CTCBase*)&villagerComp;
    entries[2].type = 0x40; entries[2].component = 0;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 3;
    if (hasVillager) creature.m_presentBits[1] |= 1UL << 3;
    brain.PCreature = &creature;
    markerIdx = 77; filterIdx = -1; cookTime = true; found = true; villageRet = &village; villArg = 0; firstOut = 0;
}

int main()
{
    CAIStateGroup_MinionListenToStory g; g.PBrain = &brain;
    Reset(true); CHECK(g.IsRunnable() == true);
    CHECK(strCtor == 1 && strDtor == 1 && strLen == -1 && strcmp(strText, "STORY_PLACE_MARKER") == 0 && idxCalls == 1);
    CHECK(villCalls == 2 && villArg == &villagerComp && cookCalls == 1 && peekCalls == 1 && firstCalls == 1 && filterIdx == 77 && firstOut != 0);
    Reset(false); CHECK(g.IsRunnable() == false); CHECK(strDtor == 1 && villCalls == 0 && firstCalls == 0);
    Reset(true); villageRet = 0; CHECK(g.IsRunnable() == false); CHECK(villCalls == 1 && cookCalls == 0);
    Reset(true); cookTime = false; CHECK(g.IsRunnable() == false); CHECK(cookCalls == 1 && firstCalls == 0);
    Reset(true); found = false; CHECK(g.IsRunnable() == false); CHECK(firstCalls == 1);
    if (fails == 0) printf("00910100_TEST PASS\n");
    return fails;
}