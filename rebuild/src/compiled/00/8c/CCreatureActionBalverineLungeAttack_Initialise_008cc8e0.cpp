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

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

void __fastcall SetLungedThisFrame();           // 0x009054F0

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +8
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