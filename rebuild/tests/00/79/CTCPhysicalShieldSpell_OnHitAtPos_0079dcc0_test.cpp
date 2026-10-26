// Behaviour test for 0079dcc0: src/0079dcc0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCPhysicalShieldSpell::OnHitAtPos(const C3DVector& position)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCPhysicalShieldSpell::OnHitAtPos(const C3DVector& position) -- a hit on the shield spawns one of the two PHYSICAL_HIT special effects (a coin toss on the world's random seed) at the hit position, when the shield def's special-effects sub-def maps that name
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079DCC0, 178 bytes (`ret 4`; PDB ?OnHitAtPos@CTCPhysicalShieldSpell@@UAEXABVC3DVector@@@Z).
// Callees: CCharString() @ 0x0099E4B0, CTCBase::GetWorld() @ 0x00686A70, CWorld::GetPRandomSeed() @ 0x0049D810, GFFloatRandom
//   @ 0x004978A0 (`__fastcall`, the float on the stack, the seed in ecx), CCharString::operator=(const char*) @ 0x0099EFE0
//   ("PHYSICAL_HIT1" @ 0x0126C524 / "PHYSICAL_HIT2" @ 0x0126C514), CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString @ 0x00669D30, CTCSpecialAbilities::CreateParticleEffect(long, const C3DVector&)
//   @ 0x0072AB70 (static `__fastcall`, PDB SIPAVCThing@@JABVC3DVector@@@Z), ~CCharString @ 0x0099EAE0. 0.5f @ 0x0122F59C.
// Shape: if/else of the two operator= calls tail-merges into `push A / jmp / push B / call`; the out-slot def and the zero
//   effect type follow the landed 0x008EC320. Tried: `&&` condition, nested if, the call through the slot member, a holder
//   whose destructor releases. Next: lane gap: register choice for the out-slot def copy (chunk 161 0x69A8B0 family).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
class C3DVector { public: float X, Y, Z; };
class CThing;
class CCharString { public:
    void* m_pData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWorld { public:
    unsigned long* GetPRandomSeed();                    // 0x0049D810
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> class CDefPointer { public:                 // the out-slot GetSubDef fills (counted reference handed over)
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
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CSpecialAbilitiesPhysicalShieldDef : public CDefClassBase { public: };
class CTCSpecialAbilities { public:
    static CThing* __fastcall CreateParticleEffect(long type, const C3DVector& position);   // 0x0072AB70
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    unsigned char _pad0a[2];
    CWorld* GetWorld();                                 // 0x00686A70
};
class CTCPhysicalShieldSpell : public CTCBase { public:
    CSpecialAbilitiesPhysicalShieldDef* PDef;           // +0x0c CDefPointer<const CSpecialAbilitiesPhysicalShieldDef>::Object
    virtual void OnHitAtPos(const C3DVector& position);
};

void CTCPhysicalShieldSpell::OnHitAtPos(const C3DVector& position)
{
    CCharString effectName;
    if (GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed()) < 0.5f)
        effectName = "PHYSICAL_HIT1";
    else
        effectName = "PHYSICAL_HIT2";
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    bool found = PDef->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (found) {
        if (effects->GetEffectFromString(effectName, effectType))
            CTCSpecialAbilities::CreateParticleEffect(effectType, position);
    }
    ReleaseDef(effects);
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_find = 0, g_release = 0, g_destroy = 0, g_create = 0, g_lookup = 0, g_assign = 0;
static float g_random = 0; static const char* g_lastText = 0; static long g_type = -1; static const C3DVector* g_pos = 0; static long g_mapType = 0; static bool g_mapFound = false;
static unsigned long g_seed = 7; static CWorld g_world;
void* FindDef_s7r_0() { ++g_find; return g_def; }
void ReleaseDef_s7(void*) { ++g_release; }
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() { ++g_destroy; }
void CTCBase::v00() {}
CCharString::CCharString() { m_pData = 0; }
CCharString::~CCharString() {}
CCharString& CCharString::operator=(const char* text) { ++g_assign; m_pData = (void*)text; g_lastText = text; return *this; }
unsigned long* CWorld::GetPRandomSeed() { return &g_seed; }
CWorld* CTCBase::GetWorld() { return &g_world; }
float __fastcall GFFloatRandom(float range, unsigned long* seed) { CHECK(range == 1.0f && seed == &g_seed); return g_random; }
bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const { ++g_lookup; CHECK(name.m_pData == (void*)g_lastText); CHECK(type == 0); type = g_mapType; return g_mapFound; }
CThing* __fastcall CTCSpecialAbilities::CreateParticleEffect(long type, const C3DVector& position) { ++g_create; g_type = type; g_pos = &position; return 0; }
int main()
{
    static CSpecialAbilitiesPhysicalShieldDef shield; static CSpecialEffectsDef effects; effects.m_refCount = 5;
    CTCPhysicalShieldSpell s; s.PDef = &shield;
    C3DVector pos = { 1, 2, 3 };
    g_def = 0; g_random = 0.25f; s.OnHitAtPos(pos); CHECK(g_find == 1 && g_lookup == 0 && g_create == 0 && strcmp(g_lastText, "PHYSICAL_HIT1") == 0);
    g_def = &effects; g_random = 0.75f; g_mapFound = true; g_mapType = 9; s.OnHitAtPos(pos);
    CHECK(strcmp(g_lastText, "PHYSICAL_HIT2") == 0 && g_lookup == 1 && g_create == 1 && g_type == 9 && g_pos == &pos && effects.m_refCount == 4);
    g_mapFound = false; s.OnHitAtPos(pos); CHECK(g_lookup == 2 && g_create == 1 && effects.m_refCount == 3);
    effects.m_refCount = 1; g_random = 0.5f; s.OnHitAtPos(pos); CHECK(strcmp(g_lastText, "PHYSICAL_HIT2") == 0 && effects.m_refCount == 0 && g_destroy == 1);
    CHECK(g_release == 0);
    if (fails == 0) printf("0079dcc0_TEST PASS\n"); else printf("0079dcc0_TEST FAIL\n");
    return fails;
}