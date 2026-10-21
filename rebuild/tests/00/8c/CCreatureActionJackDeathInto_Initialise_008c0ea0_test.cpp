// Behaviour test for 008c0ea0: src/008c0ea0.cpp copied verbatim; Initialise runs the base Initialise, swaps creature
// mode 0x34 for 0x35 (removing 0x34 only when active), and when the creature def's CSpecialEffectsDef sub-def maps
// "DEATH" spawns that effect at the creature's position; every def reference is dropped.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// CCreatureAction_JackDeathInto::Initialise() -- base Initialise; swap creature mode 0x34 for 0x35; spawn the creature's DEATH special effect at its position when its special-effects def maps one
// Leaf: ?Initialise@CCreatureAction_JackDeathInto
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C0EA0, 268 bytes (272 with padding), virtual Initialise.
// Callees: CCombatActionBase::Initialise 0x00857520 (direct, first), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::
//   LowerBound 0x0040F020 (inlined lookup of 0x31 = CTCCreatureModeManager, bit 17 of +0x24), CTCCreatureModeManager::
//   IsModeActive 0x0073A8A0 / RemoveMode 0x0073ABA0 / AddMode 0x0073AE10, CDefClassBase::GetSubDef<CSpecialEffectsDef>
//   0x00669580, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("DEATH" 0x01277C9C), CCRC::CalcAndLookup(const
//   CCharString&, long*) 0x00669D30 (on the def), CThing::GetPosition 0x004C73D0, the special-effect Create(long, const
//   C3DVector*, bool) 0x006E0880 (__fastcall).
// Shape: the mode manager is a zero-initialised out variable of the landed out-parameter GetInterface (one GetPItem, the
//   `xor esi,esi` preload; the result is not tested); the thing's def is
//   a by-value CDefPointer copy (addref, GetSubDef, release in the full expression); the sub-def and the looked-up
//   effect id are zero-initialised locals of their own `{}` block after the mode code (so the inlined lookup key shares
//   the effect id's frame word, not the sub-def's); the name string is a named local in its own block and shares
//   the sub-def's frame word (the def stays in esi) -- which needs the same-TU GetSubDef stand-in below (retail's LTCG
//   knowledge that the def pointer's address does not escape).
struct C3DVector { float x, y, z; };
struct CTCBase;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CSpecialEffectsDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30 (CCRC, at the def)
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
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
    CDefPointer<CSpecialEffectsDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void Destroy(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x1c - 8];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CSpecialEffectsDef> 0x00669580 (its landed body): VC7.1 then knows the sub-def pointer's
// address does not escape, so the def is held in esi and the name string reuses its frame word, as in retail.
bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    long* pIndex;
    {
        CCharString name("CSpecialEffectsDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CSpecialEffectsDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
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
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    const C3DVector& GetPosition() const;             // 0x004C73D0
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
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
    CDefPointer<CDefClassBase> GetDef() const { return CDefPointer<CDefClassBase>(m_pDef); }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CTCCreatureModeManager {
    bool IsModeActive(unsigned long mode);            // 0x0073A8A0
    void RemoveMode(unsigned long mode);              // 0x0073ABA0
    bool AddMode(unsigned long mode);                 // 0x0073AE10
};
void __fastcall CreateSpecialEffect(long effect, const C3DVector* position, bool attached);   // 0x006E0880

struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void Initialise();                                // 0x00857520
};
struct CCreatureAction_JackDeathInto : CCombatActionBase {
    void Initialise();
};

void CCreatureAction_JackDeathInto::Initialise()
{
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modes = 0;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    if (modes->IsModeActive(0x34))
        modes->RemoveMode(0x34);
    modes->AddMode(0x35);
{
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    PCreature.GetPItem()->GetDef()->GetSubDef(&effects);
    if (effects.Object) {
        bool found;
        {
            CCharString name("DEATH");
            found = effects->CalcAndLookup(name, &effect);
        }
        if (found)
            CreateSpecialEffect(effect, &PCreature.GetPItem()->GetPosition(), false);
    }
    }
}
// ---- harness ----
static int g_baseInit = 0, g_removed = 0, g_added = 0, g_spawned = 0, g_destroyedFx = 0, g_destroyedDef = 0;
static bool g_active = false; static long g_spawnEffect = -1; static const C3DVector* g_spawnPos = 0;
static char g_lookupName[32]; static bool g_hasDeath = true; static long g_subIndex = 5; static bool g_hasSub = true;
static C3DVector g_pos = { 1, 2, 3 };
static CSpecialEffectsDef g_fx; static CThing g_thing; static CDefinitionManager g_manager;
CCharString::CCharString(const char* text, long length) { (void)length; PStringData = (void*)text; }
CCharString::~CCharString() { }
void CSpecialEffectsDef::v00() { }
void CSpecialEffectsDef::Destroy() { ++g_destroyedFx; }
bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{ strcpy(g_lookupName, (const char*)name.PStringData); if (!g_hasDeath) return false; *value = 0x2a; return true; }
CDefPointer<CSpecialEffectsDef> CDefinitionManager::GetPDefFromGlobalIndex(long index)
{ CHECK(index == g_subIndex); return CDefPointer<CSpecialEffectsDef>(&g_fx); }
void CDefClassBase::v00() { } void CDefClassBase::Destroy() { ++g_destroyedDef; } void CDefClassBase::v08() { }
void CDefClassBase::v0c() { } void CDefClassBase::v10() { } void CDefClassBase::v14() { } void CDefClassBase::v18() { }
void CDefClassBase::v1c() { } void CDefClassBase::v20() { } void CDefClassBase::v24() { } void CDefClassBase::v28() { }
void CDefClassBase::v2c() { } void CDefClassBase::v30() { } void CDefClassBase::v34() { }
long* CDefClassBase::GetSubDefIndex(const CCharString& name)
{ return (g_hasSub && strcmp((const char*)name.PStringData, "CSpecialEffectsDef") == 0) ? &g_subIndex : 0; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
const C3DVector& CThing::GetPosition() const { return g_pos; }
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
bool CTCCreatureModeManager::IsModeActive(unsigned long mode) { CHECK(mode == 0x34); return g_active; }
void CTCCreatureModeManager::RemoveMode(unsigned long mode) { CHECK(mode == 0x34); ++g_removed; }
bool CTCCreatureModeManager::AddMode(unsigned long mode) { CHECK(mode == 0x35); ++g_added; return true; }
void __fastcall CreateSpecialEffect(long effect, const C3DVector* position, bool attached)
{ CHECK(!attached); ++g_spawned; g_spawnEffect = effect; g_spawnPos = position; }
void CCombatActionBase::v00() { }
void CCombatActionBase::Initialise() { ++g_baseInit; }
int main()
{
    static CDefClassBase def; def.RefCount = 1; def.m_pManager = &g_manager;
    static unsigned char modesBuf[4]; CTCCreatureModeManager* modes = (CTCCreatureModeManager*)modesBuf;
    static CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x31, (CTCBase*)modesBuf } };
    g_thing.m_presentBits[1] = 1UL << (0x31 & 31); g_thing.m_interfaces.first = entries; g_thing.m_interfaces.last = entries + 2;
    g_thing.m_pDef = &def; g_fx.RefCount = 1;
    static CCreatureAction_JackDeathInto action; action.PCreature.m_pItem = &g_thing;
    (void)modes;

    // mode 0x34 inactive; DEATH found: effect 0x2a spawned at the creature
    action.Initialise();
    CHECK(g_baseInit == 1 && g_removed == 0 && g_added == 1);
    CHECK(g_spawned == 1 && g_spawnEffect == 0x2a && g_spawnPos == &g_pos && strcmp(g_lookupName, "DEATH") == 0);
    CHECK(def.RefCount == 1 && g_fx.RefCount == 1 && g_destroyedDef == 0 && g_destroyedFx == 0);

    // mode active: removed first
    g_active = true;
    action.Initialise();
    CHECK(g_removed == 1 && g_added == 2 && g_spawned == 2);

    // DEATH not mapped: nothing spawned
    g_hasDeath = false;
    action.Initialise();
    CHECK(g_spawned == 2 && g_fx.RefCount == 1);

    // no special-effects sub-def: no lookup
    g_hasSub = false; g_lookupName[0] = 0;
    action.Initialise();
    CHECK(g_spawned == 2 && g_lookupName[0] == 0 && def.RefCount == 1 && g_destroyedDef == 0);

    if (fails == 0) printf("008c0ea0_TEST PASS\n");
    return fails;
}