// CThingSwitchDef::Transfer(CPersistContext& context) -- persist the CThingBaseDef part, then the trigger type
// Leaf: ?Transfer@CThingSwitchDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00454F15, virtual (Ego_r.pdb ?Transfer@CThingSwitchDef@@UAEXAAVCPersistContext@@@Z), `ret 4`.
// Callees: CThingBaseDef::Transfer 0x00676BE0; Transfer<ESwitchTriggerType> 0x00454E5B.
// Layout: Ego_r.pdb CThingSwitchDef (0x78) less 4: TriggerType +0x70.
// namepass DEF1: the stand-in archive class and base were CPersistContext / CThingBaseDef (one name per callee).
#pragma optimize("s",on)
enum ESwitchTriggerType { };
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CThingBaseDef { public:
    virtual void Transfer(CPersistContext& context);   // 0x00676BE0
    unsigned char _body04[0x70 - 4];                   // retail def base (Ego_r.pdb CThingBaseDef 0x74 less 4)
};
class CThingSwitchDef : public CThingBaseDef { public:
    ESwitchTriggerType TriggerType; // +0x70
    virtual void Transfer(CPersistContext& context);
};

void CThingSwitchDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.Transfer(TriggerType);
}
