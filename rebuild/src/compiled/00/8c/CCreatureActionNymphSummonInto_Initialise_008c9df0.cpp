// CCreatureAction_NymphSummonInto::Initialise() -- names the animation "SUMMON_CREATURES_INTO", tells the creature's body object (+0x60, virtual slot 36) to start with flag 1, then runs the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C9DF0, length 72 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), the body's virtual slot 36 (`call [edx+90h]`, one argument), CCombatActionBase::Initialise @ 0x00857520 (non-virtual call).
// Shape: the 63-body "scoped name + base Initialise" family (landed 0x008CC760) with one virtual call in between; the body pointer is read through an inline accessor so that it is fetched into ecx before the argument push (a bare member read lands in eax and is moved).
#include "fable_string.h"
struct CCreatureBody {                      // the object at +0x60 of the creature: slot 36 takes one flag
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetSummoning(long flag);   // slot 36
};
struct CThingCreature {
    unsigned char _pad[0x60];
    CCreatureBody* m_pBody;                 // +0x60
    __forceinline CCreatureBody* GetBody() { return m_pBody; }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreature* m_pItem;                // +4
    CThingCreature* GetPItem() const;       // 0x00A01B50
};

class CCombatActionBase { public:
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;      // +8
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_NymphSummonInto : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_NymphSummonInto::Initialise()
{
    {
        CCharString name("SUMMON_CREATURES_INTO", -1);
        SetAnimationName(name);
    }
    PCreature.GetPItem()->GetBody()->SetSummoning(1);
    CCombatActionBase::Initialise();
}