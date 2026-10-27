// CTCCreatureNavigation::OnSetInLimbo(bool) -- interrupt the current navigator when the creature goes in or out of limbo
// Leaf: ?OnSetInLimbo@CTCCreatureNavigation@@UAEX_N@Z
// Retail: Fable.exe @ 0x006CB920, 11 bytes: `mov ecx,[ecx+0Ch] ; mov eax,[ecx] ; call [eax+48h] ; ret 4`.
// Identity (proven): CTCCreatureNavigation vtable 0x0125EC24 slot 16 (rtti-vtables.tsv); slot 16 is CTCBase::OnSetInLimbo(bool)
// (landed CTCDParticleEmitter 0x006E0FC0, CTCWeaponTrail 0x0076E260, CTCObjectAugmentations 0x00768310 sit in slot 16) and
// Ego_r.pdb names ?OnSetInLimbo@CTCCreatureNavigation@@UAEX_N@Z. The flag is not read.
// Callee: CNavigatorBase::Interrupt(), virtual slot 18 (+0x48), as in the landed SetNavigatorToType 0x006CBA80.
// Shape: `call ; ret 4` -- the callee pops nothing, so VC7.1 cannot turn the call into a tail jump.
class CNavigatorBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Interrupt();                       // slot 18
};
class CThing;
class CTCBase { public:                             // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};
class CTCCreatureNavigation : public CTCBase { public:
    CNavigatorBase* PNavigator;                     // +0x0C
    virtual void OnSetInLimbo(bool inLimbo);        // slot 16
};

void CTCCreatureNavigation::OnSetInLimbo(bool)
{
    PNavigator->Interrupt();
}