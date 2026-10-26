// CCreatureAction_WhisperSommersaultForward::Initialise() -- name the animation "MOVEMENT_LONG_FORWARD", run the base Initialise, clear Landed, then give the creature a "CTCSolidToThing" component that is told not to be solid to the combat target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C0820, length 130 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 and ~CCharString @ 0x0099EAE0 (fable_string.h),
// CCombatActionBase::SetAnimationName(const CCharString&) @ 0x008574D0 (landed naming), CCombatActionBase::Initialise @ 0x00857520
// (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, the combat target at +0xa8),
// CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, CTCSolidToThing::AddThingNotSolidTo(CThing*) @ 0x00770FA0.
// Shape: two scoped named strings; the `Landed = false` store (+0x114, Ego_r.pdb +0x12c) is scheduled into the second
// string's argument pushes; the new component rides in edi across the string destructor and is used without a null test.
// The intelligent pointer at +0xa8 is the first CCombatActionBase member (retail CCreatureActionBase is 0xa8 bytes).
#include "fable_string.h"
class CTCParamsBase;
class CThing;
class CTCSolidToThing { public:
    void AddThingNotSolidTo(CThing* thing); // 0x00770FA0
};
class CThing { public:
    CTCSolidToThing* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
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

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;        // +0xa8 CIntelligentPointer<CThing> (the combat target)
    unsigned char _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_WhisperSommersaultForward : public CCombatActionBase { public:
    bool Landed;                            // +0x114
    virtual void Initialise();
};

void CCreatureAction_WhisperSommersaultForward::Initialise()
{
    {
        CCharString name("MOVEMENT_LONG_FORWARD", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    Landed = false;
    CTCSolidToThing* solid;
    {
        CCharString name("CTCSolidToThing", -1);
        solid = PCreature.GetPItem()->AddTC(name, false, 0);
    }
    solid->AddThingNotSolidTo(PTarget.GetPItem());
}