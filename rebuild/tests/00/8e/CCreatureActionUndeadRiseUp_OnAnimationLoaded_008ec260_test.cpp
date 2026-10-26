// Behaviour test for 008ec260: src/008ec260.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CCreatureAction_UndeadRiseUp::OnAnimationLoaded()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_UndeadRiseUp::OnAnimationLoaded() -- once the rise animation is loaded, spawn the creature definition's "RISE" special effect at the undead's position (when its special-effects sub-def maps that name), then the combat-action OnAnimationLoaded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC260, length 192 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, twice), CDefClassBase::GetSubDef<
// CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) 0x00669580 on the thing's definition (+0x70), CCharString(const char*,
// long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30
// (landed as CCRC::CalcAndLookup), CThing::GetPosition 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool)
// 0x006E0880, CCombatActionBase::OnAnimationLoaded 0x00859EF0 (direct base
// call, after the effect).
// Shape: the TeleportIn/TeleportOut lookup (0x008B9F20) without the CreateEffects gate: the def from the plain out-slot is
// copied into a named pointer (ebp) and released by hand at the very end; the creature is a named local; the `found && ...`
// condition with the implicitly converted string temporary keeps its construction flag in ebx (zeroed at entry) and the
// condition in a frame byte. GetEffectFromString is a same-TU noinline stand-in (retry B's lever, FIXES.md) whose body
// only writes the effect type. VC7.1 then knows the effect-type word's address does not escape the call, which gives
// retail's registers (`this` in edi, &PCreature in esi) and keeps `push 0` ahead of the GetPItem/GetPosition calls.
class C3DVector { public: float X, Y, Z; };

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;
};
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name);   // 0x00669D30 body
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const   // 0x00669D30 stand-in
{
    long found = Find_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> class CDefPointer { public:      // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};

struct CThingAnimatable {
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

class CThing : public CThingAnimatable { public:
    unsigned char  _pad0[0x70];
    CDefClassBase* m_pDef;                      // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCombatActionBase { public:
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    void OnAnimationLoaded();                   // 0x00859EF0
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

class CCreatureAction_UndeadRiseUp : public CCombatActionBase { public:
    void OnAnimationLoaded();
};

void CCreatureAction_UndeadRiseUp::OnAnimationLoaded()
{
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    CThing* creature = PCreature.GetPItem();
    CDefClassBase* def = creature->m_pDef;
    bool found = def->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (found && effects->GetEffectFromString("RISE", effectType))
        CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
    CCombatActionBase::OnAnimationLoaded();
    ReleaseDef(effects);
}

// ---- stubs and driver ----
static const char* g_lastText = 0;
static int g_strings = 0, g_stringDtors = 0, g_destroys = 0, g_effects = 0, g_baseCalls = 0, g_finds = 0;
static long g_effectType = 0; static const C3DVector* g_effectPos = 0; static bool g_effectFlag = true;
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; g_lastText = text; ++g_strings; CHECK(length == -1); }
CCharString::~CCharString() { ++g_stringDtors; }
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { ++g_destroys; }
static CSpecialEffectsDef* g_subDef = 0;
static bool g_subDefResult = false;
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<T>& out) { out.m_p = (T*)g_subDef; return g_subDefResult; }
static long g_mapped = -1;
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name)
{
    ++g_finds; CHECK(def == g_subDef && strcmp((const char*)name.m_pData, "RISE") == 0); return g_mapped;
}
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { return g_pos; }
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{
    ++g_effects; g_effectType = type; g_effectPos = position; g_effectFlag = flag; return 0;
}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CCombatActionBase::v00() { }
void CCombatActionBase::Initialise() { }
void CCombatActionBase::OnAnimationLoaded() { ++g_baseCalls; }
int main()
{
    static unsigned char thingRaw[sizeof(CThing)];
    CThing* thing = (CThing*)thingRaw;
    static unsigned char defRaw[sizeof(CDefClassBase)];
    thing->m_pDef = (CDefClassBase*)defRaw;
    CSpecialEffectsDef effects; effects.m_refCount = 2;
    static unsigned char actionRaw[sizeof(CCreatureAction_UndeadRiseUp)];
    CCreatureAction_UndeadRiseUp* action = (CCreatureAction_UndeadRiseUp*)actionRaw;
    action->PCreature.m_pItem = thing;
    // no special-effects sub-def: base call only, no string
    g_subDef = 0; g_subDefResult = false;
    action->OnAnimationLoaded();
    CHECK(g_baseCalls == 1 && g_strings == 0 && g_effects == 0 && g_destroys == 0);
    // sub-def present, "RISE" mapped to 17: the effect at the creature's position, then one release
    g_subDef = &effects; g_subDefResult = true; g_mapped = 17;
    action->OnAnimationLoaded();
    CHECK(g_strings == 1 && g_stringDtors == 1 && g_finds == 1 && g_effects == 1 && g_effectType == 17 && g_effectPos == &g_pos && !g_effectFlag);
    CHECK(g_baseCalls == 2 && effects.m_refCount == 1 && g_destroys == 0);
    // "RISE" not mapped: no effect; the last reference is released and the def destroyed
    g_mapped = -1;
    action->OnAnimationLoaded();
    CHECK(g_strings == 2 && g_stringDtors == 2 && g_effects == 1 && g_baseCalls == 3 && effects.m_refCount == 0 && g_destroys == 1);
    if (fails == 0) printf("008ec260_TEST PASS\n");
    return fails;
}
