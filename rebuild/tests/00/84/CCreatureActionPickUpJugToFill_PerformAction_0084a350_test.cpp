// Behaviour test for 0084a350: src/0084a350.cpp copied verbatim (its same-TU GetSubDef and LowerBound bodies run for
// real); the def manager, the components and the thing calls stubbed so the jug goes into the carry slot named by its
// carryable def, the creature gets mode 0x2b, the jug is marked unfilled, the def is released, and a dead jug is ignored.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_PickUpJugToFill::PerformAction() -- when the jug is alive: put it in the creature's carry slot named by the jug's carryable def (+0x30), add creature mode 0x2b and clear the "filled" byte of the jug's interface-0xBB component
// Retry 10b (getsw sweep, FIXES B-15): the fill-carry-slot flag is read through an inline getter `GetFillCarrySlot()`; the plain member read gives the parked register/scheduling difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084A350, length 321 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PJug +0xa8, PCreature +0x08), CVectorMap::LowerBound 0x0040F020 (inlined
//   untested GetInterface: 0x46 CTCCarrying, bit 6 of +0x28; 0x31 CTCCreatureModeManager, bit 17 of +0x24; 0xBB the jug component,
//   bit 27 of +0x34), CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E, CTCCarrying::PutThingInCarrySlot 0x006A1F80,
//   CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10.
// Layout: retail CCreatureActionBase is 0xa8 bytes: the jug pointer is this class's first member (+0xa8); CThing +0x70 the def,
//   +0x91 dead bit; CCarryableDef +0x30 the carry slot used here; the jug component's byte +0x10.
// Shape: the landed CCreatureAction_PickUpGoods 0x00848730 model: IS_THING_ALIVE on PJug, every lookup unchecked (the miss path
//   reads the out word), the carryable def a CDefPointer local whose release has its null test folded (the def was dereferenced),
//   the GetSubDef and LowerBound same-TU stand-ins carry retail's LTCG knowledge (def in ebx, key and def share one frame word).
struct CThing;
struct CThingObject;
struct C3DVector { float x, y, z; };
struct CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CCarryableDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x28 - 0x08];
    unsigned char _pad28[0x30 - 0x28];
    long FillCarrySlot;                               // +0x30
    long GetFillCarrySlot() const { return FillCarrySlot; }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
struct CDefinitionManager {
    CDefPointer<CCarryableDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CCarryableDef>* out); // 0x005CC10E
};
#pragma auto_inline(off)
// Same-TU stand-ins (retail's LTCG knowledge): GetSubDef<CCarryableDef> 0x005CC10E (the landed GetSubDef body,
// 0x00485815's shape) and CVectorMap::LowerBound 0x0040F020 -- VC7.1 then knows neither keeps the address of the def
// pointer / lookup key, so the def stays in ebp and the frame words are shared like retail's.
bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCarryableDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCarryableDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries): compiled above the caller so VC7.1 knows the key's address does not escape, which lets
// the key share its frame word with the AddTC name string and the by-value target copy.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x90 - 0x74];
    signed char m_player;                             // +0x90
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;             // 0x004C73D0
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
struct CThingObject : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};
struct CTCCarrying {
    void PutThingInCarrySlot(CThingObject* thing, long slot, bool immediately);   // 0x006A1F80
};
struct CTCCreatureModeManager {
    bool AddMode(unsigned long mode);                 // 0x0073AE10
};
struct CTCJugComponent {                              // interface type 0xBB
    unsigned char _pad[0x10];
    unsigned char Filled;                             // +0x10
};
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
struct CCreatureAction_PickUpJugToFill : CCreatureActionBase {
    CBaseIntelligentPointer PJug;                     // +0xa8
    void PerformAction();
};

void CCreatureAction_PickUpJugToFill::PerformAction()
{
    if (PJug.GetPItem() && !(PJug.GetPItem()->m_flags91 & 1)) {
        CTCCarrying* carrying;
        PCreature.GetPItem()->GetInterface(&carrying, 0x46);
        CDefPointer<CCarryableDef> carryDef;
        CThing* jug = PJug.GetPItem();                    // named: the GetPItem call runs before the out-pointer push
        jug->m_pDef->GetSubDef(&carryDef);
        carrying->PutThingInCarrySlot((CThingObject*)PJug.GetPItem(), carryDef->GetFillCarrySlot(), true);
        CTCCreatureModeManager* modes;
        PCreature.GetPItem()->GetInterface(&modes, 0x31);
        modes->AddMode(0x2b);
        CTCJugComponent* jugComponent;
        PJug.GetPItem()->GetInterface(&jugComponent, 0xbb);
        jugComponent->Filled = 0;
    }
}
// ---- end source ----
void CCarryableDef::v00() {}
void CDefClassBase::v00() {} void CDefClassBase::v04() {} void CDefClassBase::v08() {} void CDefClassBase::v0c() {} void CDefClassBase::v10() {} void CDefClassBase::v14() {} void CDefClassBase::v18() {} void CDefClassBase::v1c() {} void CDefClassBase::v20() {} void CDefClassBase::v24() {} void CDefClassBase::v28() {} void CDefClassBase::v2c() {} void CDefClassBase::v30() {} void CDefClassBase::v34() {}
void CBaseIntelligentPointer::v00() {}
void CCreatureActionBase::v00() {}
CCharString::CCharString(const char* t, long) : PStringData((void*)t) {}
CCharString::~CCharString() {}
static int g_destroys = 0;
void CCarryableDef::Destroy() { ++g_destroys; }
static CCarryableDef g_carry;
CDefPointer<CCarryableDef> CDefinitionManager::GetPDefFromGlobalIndex(long)
{ static void* s_holder; s_holder = &g_carry; ++g_carry.RefCount; return *(CDefPointer<CCarryableDef>*)&s_holder; }
static long g_subIndex = 3;
long* CDefClassBase::GetSubDefIndex(const CCharString& n) { return strcmp((const char*)n.PStringData, "CCarryableDef") == 0 ? &g_subIndex : 0; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static int g_puts = 0; static CThingObject* g_put = 0; static long g_slot = -1;
void CTCCarrying::PutThingInCarrySlot(CThingObject* t, long slot, bool imm) { if (imm) { ++g_puts; g_put = t; g_slot = slot; } }
static unsigned long g_mode = 0;
bool CTCCreatureModeManager::AddMode(unsigned long m) { g_mode = m; return true; }
int main()
{
    g_carry.RefCount = 1; g_carry.FillCarrySlot = 4;
    static CTCCarrying carrying; static CTCCreatureModeManager modes; static CTCJugComponent jugComp; jugComp.Filled = 1;
    static CInterfaceMap::SEntry cEntries[2] = { { 0x31, (CTCBase*)&modes }, { 0x46, (CTCBase*)&carrying } };
    static CInterfaceMap::SEntry jEntries[1] = { { 0xbb, (CTCBase*)&jugComp } };
    static CThing creature, jug; memset(&creature, 0, sizeof(creature)); memset(&jug, 0, sizeof(jug));
    creature.m_interfaces.first = cEntries; creature.m_interfaces.last = cEntries + 2;
    creature.m_presentBits[0x31 >> 5] |= 1UL << (0x31 & 31); creature.m_presentBits[0x46 >> 5] |= 1UL << (0x46 & 31);
    jug.m_interfaces.first = jEntries; jug.m_interfaces.last = jEntries + 1; jug.m_presentBits[0xbb >> 5] |= 1UL << (0xbb & 31);
    static CDefinitionManager dm; static CDefClassBase jugDef; jugDef.m_pManager = &dm; jug.m_pDef = &jugDef;
    static unsigned char raw[sizeof(CCreatureAction_PickUpJugToFill)]; memset(raw, 0, sizeof(raw));
    CCreatureAction_PickUpJugToFill* a = (CCreatureAction_PickUpJugToFill*)raw;
    a->PCreature.m_pItem = &creature; a->PJug.m_pItem = &jug;
    a->PerformAction();
    CHECK(g_puts == 1 && g_put == (CThingObject*)&jug && g_slot == 4 && g_mode == 0x2b && jugComp.Filled == 0);
    CHECK(g_carry.RefCount == 1 && g_destroys == 0);
    jug.m_flags91 = 1; a->PerformAction(); CHECK(g_puts == 1);
    a->PJug.m_pItem = 0; a->PerformAction(); CHECK(g_puts == 1);
    if (fails == 0) printf("0084a350_TEST PASS\n"); else printf("0084a350_TEST FAIL\n");
    return fails;
}