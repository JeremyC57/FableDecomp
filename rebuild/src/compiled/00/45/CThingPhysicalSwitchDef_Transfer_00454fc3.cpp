// CThingPhysicalSwitchDef::Transfer(CPersistContext& context) -- persist the CThingBaseDef part, the trigger type, both graphics and the trigger sound
// Leaf: ?Transfer@CThingPhysicalSwitchDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00454FC3, virtual (Ego_r.pdb ?Transfer@CThingPhysicalSwitchDef@@UAEXAAVCPersistContext@@@Z), `ret 4`.
// Callees: CThingBaseDef::Transfer 0x00676BE0; Transfer<EPSwitchTriggerType> 0x00455008, Transfer<CEngineGraphic> 0x00451E11 (x2), Transfer<long> 0x00431102.
// Layout: Ego_r.pdb CThingPhysicalSwitchDef (0x98) less 4: TriggerType +0x70, UntriggeredGraphic +0x74, TriggeredGraphic +0x82, TriggerSound +0x90.
// namepass DEF1: the stand-in archive class and base were CPersistContext / CThingBaseDef (one name per callee).
#pragma optimize("s",on)
enum EPSwitchTriggerType { };
class CEngineGraphic { public: unsigned char _body[0x0e]; };   // retail 14 bytes
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CThingBaseDef { public:
    virtual void Transfer(CPersistContext& context);   // 0x00676BE0
    unsigned char _body04[0x70 - 4];                   // retail def base (Ego_r.pdb CThingBaseDef 0x74 less 4)
};
class CThingPhysicalSwitchDef : public CThingBaseDef { public:
    EPSwitchTriggerType TriggerType; // +0x70
    CEngineGraphic UntriggeredGraphic; // +0x74
    CEngineGraphic TriggeredGraphic; // +0x82
    long TriggerSound;              // +0x90
    virtual void Transfer(CPersistContext& context);
};

void CThingPhysicalSwitchDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.Transfer(TriggerType);
    context.Transfer(UntriggeredGraphic);
    context.Transfer(TriggeredGraphic);
    context.Transfer(TriggerSound);
}
