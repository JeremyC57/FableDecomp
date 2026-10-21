// Behaviour test for 008b74d0: src/008b74d0.cpp copied verbatim; GetSubDef, CalcAndLookup, the animation/base
// Initialise, GetPItem, GetPosition and the special-effect Create are stubs that record their calls.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_OnGenerate::Initialise() -- the GENERATE_DROP animation, base Initialise, then spawn the creature's GENERATE special effect at its position when its special-effects def maps one
// Leaf: Initialise
// Grade: RELOCATION_MATCH -- GetSubDef<CSpecialEffectsDef> is an extern call (no same-TU body) whose plain out word is read once, right after the call, into a by-value local (ebp) through an inline helper, inside a block holding the out word, so the release uses the cached pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B74D0, 262 bytes (272 with padding), virtual Initialise.
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("GENERATE_DROP" 0x0127A5AC / "GENERATE" 0x0127A5A0), CCombatActionBase::SetAnimationName
//   0x008574D0, CCombatActionBase::Initialise 0x00857520, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CCRC::CalcAndLookup 0x00669D30, CThing::GetPosition
//   0x004C73D0, the special-effect Create 0x006E0880 (__fastcall).
// Shape: the JackDeathInto 0x008C0EA0 model with the lookup in one `&&` expression: the thing's def is a by-value
//   CDefPointer temporary and the name an implicit conversion, both destroyed at the end of the full expression under
//   VC's construction flags (ebx bits 1 and 2); the found flag lives in a frame byte. The sub-def out word is a plain
//   struct zeroed at the top of a block (it shares the GENERATE_DROP string's word); an inline helper calls GetSubDef and
//   copies the out word into `effects`, and the effects def is released by hand from that copy at the end.
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

void __fastcall CreateSpecialEffect(long effect, const C3DVector* position, bool attached);   // 0x006E0880

struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
};
struct SEffectsOut { CSpecialEffectsDef* Object; };
__forceinline bool GetSubDefRaw(CDefClassBase* def, SEffectsOut* out, CSpecialEffectsDef*& result)
{
    bool found = def->GetSubDef((CDefPointer<CSpecialEffectsDef>*)out);
    result = out->Object;
    return found;
}
struct CCreatureAction_OnGenerate : CCombatActionBase {
    void Initialise();
};

void CCreatureAction_OnGenerate::Initialise()
{
    {
        CCharString name("GENERATE_DROP");
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    CSpecialEffectsDef* effects;
    long effect;
    bool found;
    {
        SEffectsOut effectsOut;
        effectsOut.Object = 0;
        effect = 0;
        found = GetSubDefRaw(PCreature.GetPItem()->GetDef().operator->(), &effectsOut, effects) && effects->CalcAndLookup("GENERATE", &effect);
    }
    CSpecialEffectsDef* const effectsCopy = effects;
    if (found)
        CreateSpecialEffect(effect, &PCreature.GetPItem()->GetPosition(), false);
    if (effectsCopy && DecRefToZero(effectsCopy->RefCount))
        effectsCopy->Destroy();
}

// ---- stubs ----
static char lastStr[64]; static int strCtors, strDtors, setAnimCalls, baseInitCalls, createCalls, effectsDestroys, defDestroys;
static char animName[64], lookupName[64];
static bool subDefResult, lookupResult; static CSpecialEffectsDef* subDefObject; static long lookupValue, createdEffect;
static const C3DVector* createdPos; static bool createdFlag; static int subDefCalls, lookupCalls; static long defRefAtSubDef;
static CThing* theThing; static C3DVector thePos;
CCharString::CCharString(const char* text, long length) { ++strCtors; PStringData = (void*)text; CHECK(length == -1); }
CCharString::~CCharString() { ++strDtors; }
void CSpecialEffectsDef::v00() { }
void CSpecialEffectsDef::Destroy() { ++effectsDestroys; }
bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{
    ++lookupCalls; strcpy(lookupName, (const char*)name.PStringData);
    CHECK(*value == 0);
    if (lookupResult) *value = lookupValue;
    return lookupResult;
}
CDefPointer<CSpecialEffectsDef> CDefinitionManager::GetPDefFromGlobalIndex(long) { return CDefPointer<CSpecialEffectsDef>(); }
void CDefClassBase::v00() { } void CDefClassBase::Destroy() { ++defDestroys; } void CDefClassBase::v08() { }
void CDefClassBase::v0c() { } void CDefClassBase::v10() { } void CDefClassBase::v14() { } void CDefClassBase::v18() { }
void CDefClassBase::v1c() { } void CDefClassBase::v20() { } void CDefClassBase::v24() { } void CDefClassBase::v28() { }
void CDefClassBase::v2c() { } void CDefClassBase::v30() { } void CDefClassBase::v34() { }
long* CDefClassBase::GetSubDefIndex(const CCharString&) { return 0; }
bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    ++subDefCalls; defRefAtSubDef = RefCount;
    CHECK(out->Object == 0);
    out->Object = subDefObject;
    if (subDefObject) ++subDefObject->RefCount;
    return subDefResult;
}
const C3DVector& CThing::GetPosition() const { return thePos; }
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void __fastcall CreateSpecialEffect(long effect, const C3DVector* position, bool attached)
{ ++createCalls; createdEffect = effect; createdPos = position; createdFlag = attached; }
void CCombatActionBase::v00() { }
void CCombatActionBase::SetAnimationName(const CCharString& name) { ++setAnimCalls; strcpy(animName, (const char*)name.PStringData); }
void CCombatActionBase::Initialise() { ++baseInitCalls; CHECK(setAnimCalls == 1); }
static void reset() { strCtors = strDtors = setAnimCalls = baseInitCalls = createCalls = effectsDestroys = defDestroys = 0;
    subDefCalls = lookupCalls = 0; lookupName[0] = animName[0] = 0; createdFlag = true; createdEffect = -1; }

int main()
{
    static CThing thing; memset(&thing, 0, sizeof(thing));
    static CDefClassBase def; def.RefCount = 2; thing.m_pDef = &def;
    static CSpecialEffectsDef fx; fx.RefCount = 0;
    thePos.x = 1; thePos.y = 2; thePos.z = 3;
    CCreatureAction_OnGenerate action; memset(((char*)&action) + 4, 0, sizeof(action) - 4);
    action.PCreature.m_pItem = &thing;

    // 1: sub-def found and maps GENERATE -> effect created at the creature's position; refs balanced, last ref destroys
    reset(); subDefResult = true; subDefObject = &fx; lookupResult = true; lookupValue = 42;
    action.Initialise();
    CHECK(strcmp(animName, "GENERATE_DROP") == 0 && baseInitCalls == 1);
    CHECK(subDefCalls == 1 && defRefAtSubDef == 3 && def.RefCount == 2 && defDestroys == 0);
    CHECK(lookupCalls == 1 && strcmp(lookupName, "GENERATE") == 0);
    CHECK(createCalls == 1 && createdEffect == 42 && createdPos == &thePos && createdFlag == false);
    CHECK(fx.RefCount == 0 && effectsDestroys == 1);
    CHECK(strCtors == strDtors && strCtors == 2);

    // 2: lookup misses -> no effect; def still released (not destroyed while others hold it)
    reset(); fx.RefCount = 5; lookupResult = false;
    action.Initialise();
    CHECK(lookupCalls == 1 && createCalls == 0 && fx.RefCount == 5 && effectsDestroys == 0);
    CHECK(strCtors == strDtors && strCtors == 2);

    // 3: no sub-def -> no lookup (the name string is never built), nothing to release
    reset(); subDefResult = false; subDefObject = 0;
    action.Initialise();
    CHECK(subDefCalls == 1 && lookupCalls == 0 && createCalls == 0 && effectsDestroys == 0);
    CHECK(strCtors == 1 && strDtors == 1);

    // 4: GetSubDef reports false but filled the pointer -> no lookup, the pointer is still released
    reset(); subDefResult = false; subDefObject = &fx; fx.RefCount = 0;
    action.Initialise();
    CHECK(lookupCalls == 0 && createCalls == 0 && effectsDestroys == 1 && fx.RefCount == 0);

    // 5: the thing's def at refcount 0 -> the by-value def temporary's release destroys it
    reset(); def.RefCount = 0; subDefResult = true; subDefObject = 0; lookupResult = false;
    action.Initialise();
    CHECK(defDestroys == 1 && def.RefCount == 0);

    if (fails == 0) printf("008b74d0_TEST PASS\n");
    return fails;
}