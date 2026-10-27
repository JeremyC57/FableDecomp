// CCreatureActionBase::GetRandomAnimFromName(const CThingCreatureBase&, const CCharString&) -- a random loaded animation of the given name for the creature
// Leaf: ?GetRandomAnimFromName@CCreatureActionBase@@KIABVCAnimationEntry@@ABVCThingCreatureBase@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x006924B0, 9 bytes: `push 1 ; push edx ; call 0x00662FA0 ; ret` (`__fastcall`: creature in ecx, name in
// edx, nothing on the stack).
// Identity (proven): Ego_r.pdb / FableWin.pdb name the protected static __fastcall
// ?GetRandomAnimFromName@CCreatureActionBase@@KIABVCAnimationEntry@@ABVCThingCreatureBase@@ABVCCharString@@@Z; the body
// forwards to CThingCreatureBase::GetRandomLoadedAnimFromName(name, true) 0x00662FA0 (PDB const member) with the creature
// still in ecx; the landed caller CCreatureAction_YellOverHere ctor 0x008421E0 binds this exact symbol (83 call sites, all
// creature-action ctors). The manifest label "CMainGameComponent::BeginInputLoading" is propagated.
// Shape: the reference result is the pointer the member returns; `call ; ret` (the callee pops 8 bytes, this one none).
class CAnimationEntry;
class CCharString;
class CThingCreatureBase { public:
    const CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool loadedOnly) const;   // 0x00662FA0
};
class CCreatureActionBase { public:
protected:
    static const CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature, const CCharString& name);
};

const CAnimationEntry& __fastcall CCreatureActionBase::GetRandomAnimFromName(const CThingCreatureBase& creature,
                                                                             const CCharString& name)
{
    return *creature.GetRandomLoadedAnimFromName(name, true);
}