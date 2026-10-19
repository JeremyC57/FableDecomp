// CActionEventExplosion::Dispatch(const CCreatureActionBase&) -- resolve an effect point, create its thing, and add the source as an explosion exclusion
// Retail address 0x008717a0, 262 bytes. Captures SourceThing's appearance before the action helper-name callback; TC41 is CTCDExplosion and AddExcludedThing appends to ImmuneThings. PDB CThing uses UsingComponents/Components/PTCGraphicAppearance at +0x20/+0x48/+0x74; the local retail view adjusts Components to +0x44 and PTCGraphicAppearance to +0x64. CTCDExplosion::ImmuneThings is donor +0x68 and retail +0x60 after native vector widths differ.
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"
#define CThing CThing_DonorPDB
#include "engine/_quarantine/CThing.h"
#undef CThing

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector First; C3DVector Second; };
struct CThing;
struct CInterfaceMap {
    struct Entry { long Type; void* Component; };
    Entry* First;
    Entry* Last;
    Entry* LowerBound(const long* type); // 0x0040F020
};
struct CCreatureActionBaseVirtuals {
    virtual ~CCreatureActionBaseVirtuals();
    virtual void Validate() const;
    virtual void EditUpdate(float elapsed);
    virtual void PerformAction() = 0;
    virtual void FinishAction();
    virtual bool IsPlayingAnimation() const;
    virtual long GetAnimDelayTime() const;
    virtual long GetAnimationIndex() const;
    virtual long GetAnimationTransitionInTime() const;
    virtual long GetAnimationTransitionOutTime() const;
    virtual unsigned long GetAnimExtraFlags() const;
    virtual CCreatureActionBase* Clone() const = 0;
    virtual void Initialise();
    virtual bool IsStrikeEvent(long event) const;
    virtual bool IsStrikeEventActive() const;
    virtual bool IsStrikeAction() const;
    virtual bool IsPartOfComboChain() const;
    virtual bool IsStrikeEventFinishedBeingActive() const;
    virtual bool IsVitalStrike() const;
    virtual bool IsKnockdownStrike() const;
    virtual bool IsBlockCounter() const;
    virtual bool IsMeleeFlourish() const;
    virtual bool IsActionWithStrikeEventHelperName() const;
    virtual CCharString GetStrikeEventHelperName() const; // slot 23, verified CCreatureActionBase const method
};
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, float scale); // 0x004C0560
    bool GetDummyObject(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, CRightHandedSet* orientation, float scale); // 0x004C08C0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    unsigned char _pad4c[0x18];
    CTCGraphicAppearance* PTCGraphicAppearance; // +0x64, native adjusted view from PDB +0x74
    C3DVector* GetPosition(); // 0x004C73D0
};
struct CTCDExplosion {
    void AddExcludedThing(CThing* thing); // 0x0080CBF0
};
struct CActionEventExplosion {
    unsigned char _pad00[0x0c];
    CThing* SourceThing;                  // +0x0c
    unsigned char _pad10[4];
    CCharString AttachmentObject;         // +0x14
    long ExplosionDefIndex;               // +0x18
    unsigned char Dummy;                  // +0x1c
    unsigned char UseActionHelper;        // +0x1d
    void Dispatch(const CCreatureActionBase& action);
};
CThing* __fastcall CreateThing(long id, C3DVector* position); // 0x0080C460

void CActionEventExplosion::Dispatch(const CCreatureActionBase& action)
{
    C3DVector sourcePosition = *SourceThing->GetPosition();
    CTCGraphicAppearance* appearance = SourceThing->PTCGraphicAppearance;
    if (UseActionHelper) {
        CCharString helperName = ((const CCreatureActionBaseVirtuals&)action).GetStrikeEventHelperName();
        appearance->GetHelperPoint(helperName, 0, 0, &sourcePosition, 1.0f);
    } else if (Dummy) {
        CRightHandedSet orientation;
        appearance->GetDummyObject(AttachmentObject, 0, 0, &sourcePosition, &orientation, 1.0f);
    } else {
        appearance->GetHelperPoint(AttachmentObject, 0, 0, &sourcePosition, 1.0f);
    }
    CThing* created = CreateThing(ExplosionDefIndex, &sourcePosition);
    if (created) {
        CTCDExplosion* explosion;
        if ((created->UsingComponents[2] & 2) != 0) {
            CInterfaceMap* components = &created->Components;
            long key = 0x41;
            CInterfaceMap::Entry* found = components->LowerBound(&key);
            if (found == components->Last || found->Type > 0x41)
                found = components->Last;
            explosion = (CTCDExplosion*)found->Component;
        }
        explosion->AddExcludedThing(SourceThing);
    }
}