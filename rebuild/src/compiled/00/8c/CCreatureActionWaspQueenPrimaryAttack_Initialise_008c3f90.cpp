// CCreatureAction_WaspQueenPrimaryAttack::Initialise() -- name the action's animation "STRIKE_MEDIUM_FRONT", run the base Initialise, remove the creature's hit-on-collision component (interface type 200) and add a fresh "CTCHitOnCollision", arming its object-hit byte
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C3F90, length 131 bytes (byte-identical to the other wasp-queen attack up to the relocations).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 and ~CCharString @ 0x0099EAE0 (fable_string.h), CCombatActionBase::
// SetCombatAnimation(const CCharString&) @ 0x008574D0 (landed naming of the 63-body family), CCombatActionBase::Initialise @ 0x00857520
// (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice), CThing::RemoveTC(long) @ 0x004C9840
// (landed naming; manifest "CBaseObject::OnPropertyChanged" is propagated), CThing::AddTC(const CCharString&, bool, CTCParamsBase*)
// @ 0x004C9D60.
// Shape: both strings are scoped named locals (re-addressed with `lea` for the argument); once `this` is no longer needed esi
// advances to &PCreature; the new component rides in esi across the string destructor and is null-checked before the byte store
// (the landed WaspQueenDiveAttack 0x008C4250 shape without the presence test).
#include "fable_string.h"
class CTCParamsBase;
class CTCHitOnCollision { public:                  // the component AddTC returns
    unsigned char _pad[0x19];
    unsigned char m_hitsObjects;            // +0x19
};
class CThing { public:
    void RemoveTC(long interfaceType);      // 0x004C9840
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

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +8
    unsigned char _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_WaspQueenPrimaryAttack : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_WaspQueenPrimaryAttack::Initialise()
{
    {
        CCharString name("STRIKE_MEDIUM_FRONT", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    PCreature.GetPItem()->RemoveTC(200);
    CTCHitOnCollision* pHit;
    {
        CCharString name("CTCHitOnCollision", -1);
        pHit = PCreature.GetPItem()->AddTC(name, false, 0);
    }
    if (pHit)
        pHit->m_hitsObjects = 1;
}