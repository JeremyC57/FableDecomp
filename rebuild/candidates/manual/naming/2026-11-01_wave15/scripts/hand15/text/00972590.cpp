// CAIStateGroup_JackOfBladesStage2::Cleanup() -- run the shared Jack of Blades cleanup, then transition every environment
// channel back to its default theme
// Leaf: ?Cleanup@CAIStateGroup_JackOfBladesStage2@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00972590; vtable slot 9 of CAIStateGroup_JackOfBladesStage2 (Ego_r.pdb +
// FableWin.pdb ?Cleanup@CAIStateGroup_JackOfBladesStage2@@UAEXXZ).
// Callees: CAIStateGroup_JackOfBladesBase::Cleanup @ 0x00972110 (qualified base call), CAIStateGroupBase::GetWorld @ 0x008FCFC0
// (Ego_r ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ, protected const), CEnvironment::TransitionToDefaultThemeAllChannels
// @ 0x006B3950 (Ego_r ?TransitionToDefaultThemeAllChannels@CEnvironment@@QAEXJM@Z, bound by 0x0078F6E0) on the world's
// PEnvironment (scoped_ptr<CEnvironment>, CWorld +0x18), arguments 3 and 2.0f (`push 40000000h`).
// Re-modelled as the member in naming wave 15 (was the free __fastcall stand-in `Cleanup(CAIStateGroup_JackOfBladesStage2*)`
// calling Sub_972110 / Sub_8FCFC0 / CObjC::Sub_6B3950). The engine header flattens the state-group base chain (no vptr model),
// so the chain is modelled locally, as in the landed sibling 0x00972110. Shape: the world is a named local (the unnamed
// `GetWorld().PEnvironment->...` hoists both pushes above the GetWorld call).
class CEnvironment { public:
    void TransitionToDefaultThemeAllChannels(long channels, float time);   // 0x006B3950
};
class CWorld { public:
    unsigned char _pad[0x18];
    CEnvironment* PEnvironment;                 // +0x18 scoped_ptr<CEnvironment> (its pointer; operator-> leaves a dead `add eax,18h`)
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9 (+0x24)
protected:
    CWorld& GetWorld() const;                   // 0x008FCFC0
};
class CAIStateGroup_JackOfBladesBase : public CAIStateGroupBase { public:
    virtual void Cleanup();                     // 0x00972110
};
class CAIStateGroup_JackOfBladesStage2 : public CAIStateGroup_JackOfBladesBase { public:
    virtual void Cleanup();
};

void CAIStateGroup_JackOfBladesStage2::Cleanup()
{
    CAIStateGroup_JackOfBladesBase::Cleanup();
    CWorld& world = GetWorld();
    world.PEnvironment->TransitionToDefaultThemeAllChannels(3, 2.0f);
}
