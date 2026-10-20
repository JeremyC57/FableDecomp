// Behaviour test for 006a1490: src/006a1490.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCCarrying::IsCarriedThingInPrimarySlot(CThing* thing)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCarrying::IsCarriedThingInPrimarySlot(CThing* thing) -- find the carry slot holding the thing and report whether that slot's def is a primary slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A1490, 173 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::SetPItem @ 0x00A01B90, ::GetPItem @ 0x00A01B50,
// ::~CBaseIntelligentPointer @ 0x00A01C10 (key vtable 0x01237F1C), CDefinitionManager::Get @ 0x0044C6B0 (static),
// CDefinitionManager::GetDef<CCarrySlotDef>(long, CDefPointer*) @ 0x006A3060 (manifest "GetDef<CThingPhysicalSwitchDef>",
// byte twin), the def's virtual slot 1 on the last release.
// Layout: CarrySlots at +0x0c (12-byte vector of {long slot id, CIntelligentPointer<CThing>}), as UpdateCarriedObjects
// 0x006A1D60; CCarrySlotDef (Ego_r.pdb) PrimarySlot +0x30.
// Shape: the thing is wrapped in a CIntelligentPointer key temporary; an inline find walks the slots comparing GetPItem() results
// (the end re-read every pass, `return m_pLast` on a miss); the def out pointer is homed in the dead `thing` slot and released
// by a CDefPointer dtor calling the def's own inline Release() with no null test (`mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax
// ; jne`, GOTCHAS chunk 172); the flag survives the release in bl and both results share the earlier `return false` tail.
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct SCarrySlotEntry {                                // pair<long, CIntelligentPointer<CThing>>, 12 bytes
    long                    first;
    CBaseIntelligentPointer second;
};
struct CCarrySlotMap {                                  // retail CVectorMap<long, CIntelligentPointer<CThing>>
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    __forceinline SCarrySlotEntry* FindValue(const CBaseIntelligentPointer& value)
    {
        for (SCarrySlotEntry* it = m_pFirst; it != m_pLast; ++it)
            if (it->second.GetPItem() == value.GetPItem())
                return it;
        return m_pLast;
    }
};
struct CCarrySlotDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
    unsigned char _pad08[0x30 - 0x08];
    bool PrimarySlot;                                   // +0x30
    __forceinline void Release() { if (DecRefToZero(m_refCount)) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
struct CDefinitionManager {
    static CDefinitionManager& Get();                   // 0x0044C6B0
    bool GetDef(long index, CDefPointer<CCarrySlotDef>* out);   // 0x006A3060
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CCarrySlotDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CTCCarrying {
    virtual void v00();
    unsigned char _pad04[0x0c - 0x04];                  // CTCBase state
    CCarrySlotMap CarrySlots;                           // +0x0c
    bool IsCarriedThingInPrimarySlot(CThing* thing);
};

bool CTCCarrying::IsCarriedThingInPrimarySlot(CThing* thing)
{
    SCarrySlotEntry* slot = CarrySlots.FindValue(CIntelligentPointer(thing));
    if (slot != CarrySlots.m_pLast) {
        bool primary;
        {
            long index = slot->first;
            CDefPointer<CCarrySlotDef> def;
            CDefinitionManager::Get().GetDef(index, &def);
            primary = def->PrimarySlot;
        }
        if (primary)
            return true;
    }
    return false;
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0; static int g_ctor = 0, g_dtor = 0;
void* FindDef_CDefinitionManager_GetDef_0() { return g_def; }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_ctor; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtor; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CCarrySlotDef::v00() {}
void CCarrySlotDef::Destroy() { ++g_destroyed; }
static CDefinitionManager g_mgr;
CDefinitionManager& CDefinitionManager::Get() { return g_mgr; }
void CTCCarrying::v00() {}
int main()
{
    CThing* a = (CThing*)0x1000; CThing* b = (CThing*)0x2000; CThing* c = (CThing*)0x3000;
    unsigned char raw[3 * sizeof(SCarrySlotEntry)]; memset(raw, 0, sizeof(raw));
    SCarrySlotEntry* slots = (SCarrySlotEntry*)raw;
    slots[0].first = 1; slots[0].second.m_pItem = a; slots[1].first = 2; slots[1].second.m_pItem = b;
    CCarrySlotDef def; def.m_refCount = 5; def.PrimarySlot = true; g_def = &def;
    CTCCarrying carry; memset(carry._pad04, 0, sizeof(carry._pad04)); carry.CarrySlots.m_pFirst = slots; carry.CarrySlots.m_pLast = slots + 2; carry.CarrySlots.m_pEnd = slots + 2;
    CHECK(carry.IsCarriedThingInPrimarySlot(b)); CHECK(def.m_refCount == 4); CHECK(g_ctor == 1 && g_dtor == 1);
    def.PrimarySlot = false; CHECK(!carry.IsCarriedThingInPrimarySlot(a)); CHECK(def.m_refCount == 3);
    CHECK(!carry.IsCarriedThingInPrimarySlot(c)); CHECK(def.m_refCount == 3); CHECK(g_ctor == 3 && g_dtor == 3);
    def.m_refCount = 1; def.PrimarySlot = true; CHECK(carry.IsCarriedThingInPrimarySlot(a)); CHECK(g_destroyed == 1);
    if (fails == 0) printf("006a1490_TEST PASS\n"); else printf("006a1490_TEST FAIL\n");
    return fails;
}