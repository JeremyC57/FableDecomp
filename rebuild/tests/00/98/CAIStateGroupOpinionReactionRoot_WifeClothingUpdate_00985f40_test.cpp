// Behaviour test for 00985f40: src/00985f40.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAIStateGroup_OpinionReactionRoot::WifeClothingUpdate(EOpinionAttitudeType attitude)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_OpinionReactionRoot::WifeClothingUpdate(EOpinionAttitudeType attitude) -- when the wife has noticed an appearance change, say the
// Leaf: WifeClothingUpdate
// matching "<category prefix><category suffix>" line to the hero; on success clear the noticed flag.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00985F40, length 201 bytes (`ret 4`). PDB: ?WifeClothingUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NW4EOpinionAttitudeType@@@Z
// -- the attitude argument selects the line suffix (retail reads the argument slot `mov edx,[esp+14h]` for the second
// table index); the category out value is declared inside the interface test, so it shares the lookup key's frame word.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xD7 = CTCWife, bit 23 of the dword at
// +0x38), CTCWife::GetAppearanceChangedNoticed @ 0x006CED50, CCharString copy ctor @ 0x0099EC30 from the category
// string table @ 0x013BAB40, CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (result dropped), CCharString::operator+= @
// 0x0099F0A0 with the second table @ 0x013BABD0, SayToThingReactingTo @ 0x0095EE70, CTCWife::
// ClearAppearanceChangedNoticed @ 0x006CECD0, ~CCharString @ 0x0099EAE0.
// Shape: the category out value shares the lookup key's frame word; a NAMED CCharString line destroyed on both returns.
// Engine header names for the state group; the header flattens the base, so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
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

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    CCharString& operator+=(const CCharString& other);   // 0x0099F0A0
    ~CCharString();                             // 0x0099EAE0
};

extern CCharString g_WifeAppearanceLinePrefix[];    // 0x013BAB40 (indexed by EWifeAppearanceCategories)
extern CCharString g_WifeAppearanceLineSuffix[];    // 0x013BABD0

struct CTCWife {
    bool GetAppearanceChangedNoticed(long* category);   // 0x006CED50
    void ClearAppearanceChangedNoticed(const long* category);   // 0x006CECD0
};

enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE };

struct CAIStateGroup_OpinionReactionRoot : CAIStateGroupBase {
    CWorld* GetPWorld();                        // 0x008FCFC0 (CAIStateGroupBase)
    long SayToThingReactingTo(const CCharString& line);  // 0x0095EE70
    bool WifeClothingUpdate(EOpinionAttitudeType attitude);
};

bool CAIStateGroup_OpinionReactionRoot::WifeClothingUpdate(EOpinionAttitudeType attitude)
{
    CTCWife* wife;
    if (PBrain->PCreature->GetInterface(&wife, 0xd7)) {
        long category;
        if (wife->GetAppearanceChangedNoticed(&category)) {
            CCharString line(g_WifeAppearanceLinePrefix[category]);
            GetPWorld();
            line += g_WifeAppearanceLineSuffix[attitude];
            if (SayToThingReactingTo(line) == 2) {
                wife->ClearAppearanceChangedNoticed(&category);
                return true;
            }
            return false;
        }
    }
    return false;
}

// ---- stubs and driver ----
#include <stdlib.h>
static int g_live = 0;
CCharString::CCharString(const CCharString& other) { const char* s = (const char*)other.PStringData; char* p = (char*)malloc(strlen(s) + 1); strcpy(p, s); PStringData = p; ++g_live; }
CCharString& CCharString::operator+=(const CCharString& other)
{
    const char* a = (const char*)PStringData; const char* b = (const char*)other.PStringData;
    char* p = (char*)malloc(strlen(a) + strlen(b) + 1); strcpy(p, a); strcat(p, b); free(PStringData); PStringData = p; return *this;
}
CCharString::~CCharString() { free(PStringData); --g_live; }
struct SRaw { const char* p; };
static SRaw g_rawPrefix[3] = { { "P0_" }, { "P1_" }, { "P2_" } };
static SRaw g_rawSuffix[3] = { { "S0" }, { "S1" }, { "S2" } };
CCharString g_WifeAppearanceLinePrefix[3] = { *(CCharString*)&g_rawPrefix[0], *(CCharString*)&g_rawPrefix[1], *(CCharString*)&g_rawPrefix[2] };
CCharString g_WifeAppearanceLineSuffix[3] = { *(CCharString*)&g_rawSuffix[0], *(CCharString*)&g_rawSuffix[1], *(CCharString*)&g_rawSuffix[2] };
static bool g_noticed = false; static long g_category = 0; static int g_cleared = 0; static long g_clearedCategory = -1;
bool CTCWife::GetAppearanceChangedNoticed(long* category) { if (!g_noticed) return false; *category = g_category; return true; }
void CTCWife::ClearAppearanceChangedNoticed(const long* category) { ++g_cleared; g_clearedCategory = *category; }
static int g_worldCalls = 0;
CWorld* CAIStateGroup_OpinionReactionRoot::GetPWorld() { ++g_worldCalls; return 0; }
static long g_sayRet = 2; static char g_said[64]; static int g_says = 0;
long CAIStateGroup_OpinionReactionRoot::SayToThingReactingTo(const CCharString& line) { ++g_says; strcpy(g_said, (const char*)line.PStringData); return g_sayRet; }
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type == 0xd7 ? &g_entries[0] : last; }
int main()
{
    static CTCWife wife;
    g_entries[0].type = 0xd7; g_entries[0].component = (CTCBase*)&wife;
    static CThing creature; memset(&creature, 0, sizeof(creature));
    creature.m_interfaces.first = g_entries; creature.m_interfaces.last = g_entries + 1;
    static CAIBrain brain; brain.PCreature = &creature;
    static double buf[8]; CAIStateGroup_OpinionReactionRoot& g = *(CAIStateGroup_OpinionReactionRoot*)buf; g.PBrain = &brain;
    int base = g_live;
    // no wife interface
    CHECK(!g.WifeClothingUpdate((EOpinionAttitudeType)1)); CHECK(g_says == 0);
    creature.m_presentBits[0xd7 >> 5] = 1UL << (0xd7 & 31);
    // wife present but nothing noticed
    CHECK(!g.WifeClothingUpdate((EOpinionAttitudeType)1)); CHECK(g_says == 0);
    // noticed: prefix by category, suffix by attitude; said successfully -> cleared
    g_noticed = true; g_category = 2;
    CHECK(g.WifeClothingUpdate((EOpinionAttitudeType)1)); CHECK(g_says == 1); CHECK(strcmp(g_said, "P2_S1") == 0);
    CHECK(g_cleared == 1); CHECK(g_clearedCategory == 2); CHECK(g_worldCalls == 1); CHECK(g_live == base);
    // the line could not be said: not cleared
    g_sayRet = 1; g_category = 0;
    CHECK(!g.WifeClothingUpdate((EOpinionAttitudeType)2)); CHECK(strcmp(g_said, "P0_S2") == 0); CHECK(g_cleared == 1); CHECK(g_live == base);
    if (fails == 0) printf("00985f40_TEST PASS\n"); else printf("00985f40_TEST FAIL\n");
    return fails;
}
