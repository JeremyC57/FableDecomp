// Behaviour test for 008cc8e0: src/008cc8e0.cpp copied verbatim; Initialise sets the lunge animation, clears the physics
// lunge state, and (unless the creature's 0x100 flag is set) adds a hit-on-collision component that hits creatures only,
// with the sub-def's damage when the creature has one (the def released); then marks the lunge frame and runs the base.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_BalverineLungeAttack::Initialise() -- animation "STRIKE_MEDIUM_LUNGE"; clear the creature's +0x60 object's lunge state; unless the creature is flagged (+0x38 bit 8) add a "CTCHitOnCollision" (hits creatures, not objects) whose damage comes from the creature's sub-definition; mark the lunge frame; base Initialise
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CC8E0, 214 bytes (224 with padding), virtual Initialise.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (fable_string.h),
//   CCombatActionBase::SetAnimationName 0x008574D0, CBaseIntelligentPointer::GetPItem 0x00A01B50, the +0x60 object's virtual
//   slot 84 (+0x150, 0), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60, CDefClassBase::GetSubDef<T>
//   0x008CD0D0 on the creature's def (+0x70), SetLungedThisFrame 0x009054F0 (__fastcall, no argument),
//   CCombatActionBase::Initialise 0x00857520; the def is released through its vtable slot 1.
// Layout: retail CCombatActionBase (PCreature +8, landed 0x008C4040); CTCHitOnCollision bytes +0x19/+0x1a and its damage
//   +0x1c; the creature's flag dword +0x38 and def +0x70; the sub-def's value +0x40.
// Shape: both strings are full-expression temporaries sharing one 4-byte slot with the def pointer (`push ecx` frame);
//   the +0x60 object is read through an inline accessor (loaded straight into ecx); the def pointer is a zeroed local released through the
//   inline Release () with the register-held pointer.
// GetSubDef<T> gets a same-TU `auto_inline(off)` stand-in body that writes only the out slot: VC7.1 then knows the def
//   pointer does not escape, keeps the def in ecx for the value and the release, and lets the strings share its slot;
//   the creature's def class is a named local so GetPItem is called before the out pointer is pushed.
#include "fable_string.h"

class CTCParamsBase;

class CTCHitOnCollision { public:                      // the component AddTC returns
    unsigned char _pad[0x19];
    bool m_hitsCreatures;                       // +0x19
    bool m_hitsObjects;                         // +0x1a
    unsigned char _pad1b;
    long m_damage;                              // +0x1c
};

struct CThingPhysics {                          // the creature's +0x60 object
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void SetLunging(bool lunging);       // slot 84 (+0x150)
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

struct CLungeDef : CDefBase {
    unsigned char _pad08[0x38];
    long m_damage;                              // +0x40
};

template<class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    T* operator->() const { return m_p; }
};

class CDefClassBase { public:
    template<class T> bool GetSubDef(CDefPointer<T>& out);     // 0x008CD0D0 (this instantiation)
};
void* FindSubDef_CDefClassBase_GetSubDef(CDefClassBase* def);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad00[0x38];
    unsigned long m_flags38;                    // +0x38
    unsigned char _pad3c[0x60 - 0x3c];
    CThingPhysics* m_pPhysics;                  // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                      // +0x70
    CThingPhysics* GetPhysics() const { return m_pPhysics; }
    CTCHitOnCollision* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};

void __fastcall SetLungedThisFrame();           // 0x009054F0

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;          // +8
    unsigned char _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_BalverineLungeAttack : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_BalverineLungeAttack::Initialise()
{
    {
        CCharString name("STRIKE_MEDIUM_LUNGE", -1);
        SetAnimationName(name);
    }
    PCreature.GetPItem()->GetPhysics()->SetLunging(false);
    if (!(PCreature.GetPItem()->m_flags38 & 0x100)) {
        CTCHitOnCollision* pHit;
        {
            CCharString name("CTCHitOnCollision", -1);
            pHit = PCreature.GetPItem()->AddTC(name, false, 0);
        }
        pHit->m_hitsCreatures = true;
        pHit->m_hitsObjects = false;
        CDefPointer<CLungeDef> def;
        CDefClassBase* defClass = PCreature.GetPItem()->m_pDef;
        if (defClass->GetSubDef(def))
            pHit->m_damage = def->m_damage;
    }
    SetLungedThisFrame();
    CCombatActionBase::Initialise();
}

// ---- harness ----
fable_i32 g_CWideStringInstanceCount_013BCA20, g_CCharStringInstanceCount_013BD800;
static int g_strings = 0, g_stringDtors = 0, g_lunging = -1, g_baseInit = 0, g_lungedFrame = 0, g_destroyed = 0, g_addTC = 0;
static char g_lastAnim[64], g_lastTC[64];
CCharString::CCharString(const char* text, long) : storage_((CCharStringData*)text) { ++g_strings; }
CCharString::~CCharString() { ++g_stringDtors; }
CCharString::operator const char*() const { return (const char*)storage_; }
void CThingPhysics::v00() { }
void CThingPhysics::v01() { }
void CThingPhysics::v02() { }
void CThingPhysics::v03() { }
void CThingPhysics::v04() { }
void CThingPhysics::v05() { }
void CThingPhysics::v06() { }
void CThingPhysics::v07() { }
void CThingPhysics::v08() { }
void CThingPhysics::v09() { }
void CThingPhysics::v10() { }
void CThingPhysics::v11() { }
void CThingPhysics::v12() { }
void CThingPhysics::v13() { }
void CThingPhysics::v14() { }
void CThingPhysics::v15() { }
void CThingPhysics::v16() { }
void CThingPhysics::v17() { }
void CThingPhysics::v18() { }
void CThingPhysics::v19() { }
void CThingPhysics::v20() { }
void CThingPhysics::v21() { }
void CThingPhysics::v22() { }
void CThingPhysics::v23() { }
void CThingPhysics::v24() { }
void CThingPhysics::v25() { }
void CThingPhysics::v26() { }
void CThingPhysics::v27() { }
void CThingPhysics::v28() { }
void CThingPhysics::v29() { }
void CThingPhysics::v30() { }
void CThingPhysics::v31() { }
void CThingPhysics::v32() { }
void CThingPhysics::v33() { }
void CThingPhysics::v34() { }
void CThingPhysics::v35() { }
void CThingPhysics::v36() { }
void CThingPhysics::v37() { }
void CThingPhysics::v38() { }
void CThingPhysics::v39() { }
void CThingPhysics::v40() { }
void CThingPhysics::v41() { }
void CThingPhysics::v42() { }
void CThingPhysics::v43() { }
void CThingPhysics::v44() { }
void CThingPhysics::v45() { }
void CThingPhysics::v46() { }
void CThingPhysics::v47() { }
void CThingPhysics::v48() { }
void CThingPhysics::v49() { }
void CThingPhysics::v50() { }
void CThingPhysics::v51() { }
void CThingPhysics::v52() { }
void CThingPhysics::v53() { }
void CThingPhysics::v54() { }
void CThingPhysics::v55() { }
void CThingPhysics::v56() { }
void CThingPhysics::v57() { }
void CThingPhysics::v58() { }
void CThingPhysics::v59() { }
void CThingPhysics::v60() { }
void CThingPhysics::v61() { }
void CThingPhysics::v62() { }
void CThingPhysics::v63() { }
void CThingPhysics::v64() { }
void CThingPhysics::v65() { }
void CThingPhysics::v66() { }
void CThingPhysics::v67() { }
void CThingPhysics::v68() { }
void CThingPhysics::v69() { }
void CThingPhysics::v70() { }
void CThingPhysics::v71() { }
void CThingPhysics::v72() { }
void CThingPhysics::v73() { }
void CThingPhysics::v74() { }
void CThingPhysics::v75() { }
void CThingPhysics::v76() { }
void CThingPhysics::v77() { }
void CThingPhysics::v78() { }
void CThingPhysics::v79() { }
void CThingPhysics::v80() { }
void CThingPhysics::v81() { }
void CThingPhysics::v82() { }
void CThingPhysics::v83() { }
void CThingPhysics::SetLunging(bool lunging) { g_lunging = lunging ? 1 : 0; }
void CDefBase::v00() { }
void CDefBase::Destroy() { ++g_destroyed; }
static CLungeDef g_lunge;
static bool g_haveLunge = true;
void* FindSubDef_CDefClassBase_GetSubDef(CDefClassBase*) { if (!g_haveLunge) return 0; ++g_lunge.m_count; return &g_lunge; }
static CTCHitOnCollision g_hit;
CTCHitOnCollision* CThing::AddTC(const CCharString& name, bool activate, CTCParamsBase* params)
{
    ++g_addTC; strcpy(g_lastTC, (const char*)name); CHECK(!activate && params == 0); return &g_hit;
}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::v00() { }
void __fastcall SetLungedThisFrame() { ++g_lungedFrame; }
void CCombatActionBase::v00() { }
void CCombatActionBase::Initialise() { ++g_baseInit; }
void CCombatActionBase::SetAnimationName(const CCharString& name) { strcpy(g_lastAnim, (const char*)name); }
int main()
{
    CThingPhysics phys;
    CDefClassBase defClass;
    CThing creature;
    memset(&creature, 0, sizeof creature);
    creature.m_pPhysics = &phys; creature.m_pDef = &defClass;
    CCreatureAction_BalverineLungeAttack a;
    a.PCreature.m_pItem = &creature;
    g_lunge.m_count = 1; g_lunge.m_damage = 42;
    memset(&g_hit, 0xAB, sizeof g_hit);
    a.Initialise();
    CHECK(strcmp(g_lastAnim, "STRIKE_MEDIUM_LUNGE") == 0 && g_lunging == 0);
    CHECK(g_addTC == 1 && strcmp(g_lastTC, "CTCHitOnCollision") == 0);
    CHECK(g_hit.m_hitsCreatures == true && g_hit.m_hitsObjects == false && g_hit.m_damage == 42);
    CHECK(g_lunge.m_count == 1 && g_destroyed == 0);
    CHECK(g_lungedFrame == 1 && g_baseInit == 1 && g_strings == 2 && g_stringDtors == 2);
    g_haveLunge = false; g_hit.m_damage = 7;            // no sub-def: damage untouched
    a.Initialise();
    CHECK(g_addTC == 2 && g_hit.m_damage == 7 && g_lungedFrame == 2 && g_baseInit == 2);
    creature.m_flags38 = 0x100;                         // flagged: no component
    a.Initialise();
    CHECK(g_addTC == 2 && g_lungedFrame == 3 && g_baseInit == 3 && g_strings == 5 && g_stringDtors == 5);
    creature.m_flags38 = 0; g_haveLunge = true; g_lunge.m_count = 0; g_lunge.m_damage = 9;   // holder owns the last ref
    a.Initialise();
    CHECK(g_hit.m_damage == 9 && g_destroyed == 1 && g_lunge.m_count == 0);
    if (fails == 0) printf("008cc8e0_TEST PASS\n");
    return fails;
}