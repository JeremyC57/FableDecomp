// CThingPhysicalSwitchDef::CThingPhysicalSwitchDef() -- CThingBaseDef part, this class's vtable, then both switch graphics default-constructed (zero bank, steps, flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00454F7C, length 65 bytes.
// Leaf: ??0CThingPhysicalSwitchDef
// Callees: CThingBaseDef::CThingBaseDef @ 0x006772D0. Vtable 0x0123339C = CThingPhysicalSwitchDef (rtti-vtables.tsv;
// slots 18/20 are the landed Transfer 0x00454FC3 and GetSizeofClass 0x00454FBD).
// Label correction: the manifest's "CParticleUpdateParameters<unsigned long,unsigned long>" constructor is
// propagated. Layout: Ego_r.pdb CThingPhysicalSwitchDef (0x98) minus 4 (the retail def base is 4 bytes smaller --
// the rule of the landed CThingCreatureDef constructor 0x00670F70, whose model this reuses): TriggerType +0x70,
// UntriggeredGraphic +0x74, TriggeredGraphic +0x82 (14-byte CEngineGraphic, pack(2)), TriggerSound +0x90.
// Shape: size-optimised def module (`#pragma optimize("s", on)`; under /O2 the float zeros fold into integer
// stores through esi, as in the /O2 CThingCreatureDef). Only the two graphics have constructors, so TriggerType and TriggerSound are left alone; each inline
// CEngineGraphic constructor runs on a `lea eax` member pointer with the zero in ecx and the float zeros as
// `fldz ; fstp`, the first fldz hoisted above the vtable store.
#pragma optimize("s", on)
class CPersistContext;

class CThingBaseDef { public:
    CThingBaseDef();                                    // 0x006772D0
    virtual void Transfer(CPersistContext& context);
    unsigned char _body[0x70 - 4];
};

#pragma pack(push, 2)
class CEngineGraphic { public:
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0c
    unsigned char Type;                                 // +0x0d
    CEngineGraphic() : BankIndex(0), AnimStep(0), RenderSizeX(0), AdditiveAlpha(false), Type(0) {}
};
#pragma pack(pop)

#pragma pack(push, 2)
class CThingPhysicalSwitchDef : public CThingBaseDef { public:        // vtable 0x0123339C
    long           TriggerType;                         // +0x70 EPSwitchTriggerType
    CEngineGraphic UntriggeredGraphic;                  // +0x74
    CEngineGraphic TriggeredGraphic;                    // +0x82
    long           TriggerSound;                        // +0x90
    virtual void Transfer(CPersistContext& context);    // 0x00454FC3
    CThingPhysicalSwitchDef();
};
#pragma pack(pop)

CThingPhysicalSwitchDef::CThingPhysicalSwitchDef()
{
}