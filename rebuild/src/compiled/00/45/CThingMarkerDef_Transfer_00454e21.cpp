// CThingMarkerDef::Transfer(CPersistContext& context) -- persist the CThingBaseDef part, the graphic, the trigger type and the properties
// Leaf: ?Transfer@CThingMarkerDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00454E21, virtual (Ego_r.pdb ?Transfer@CThingMarkerDef@@UAEXAAVCPersistContext@@@Z), `ret 4`.
// Callees: CThingBaseDef::Transfer 0x00676BE0; Transfer<CEngineGraphic> 0x00451E11, Transfer<ESwitchTriggerType> 0x00454E5B, Transfer<unsigned long> 0x00431020.
// Layout: Ego_r.pdb CThingMarkerDef (0x8c) less 4: Graphic +0x70, TriggerType +0x80, Properties +0x84.
// namepass DEF1: the stand-in archive class and base were CPersistContext / CThingBaseDef (one name per callee).
#pragma optimize("s",on)
enum ESwitchTriggerType { };
class CEngineGraphic { public: unsigned char _body[0x0e]; };   // retail 14 bytes
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CThingBaseDef { public:
    virtual void Transfer(CPersistContext& context);   // 0x00676BE0
    unsigned char _body04[0x70 - 4];                   // retail def base (Ego_r.pdb CThingBaseDef 0x74 less 4)
};
class CThingMarkerDef : public CThingBaseDef { public:
    CEngineGraphic Graphic;         // +0x70
    ESwitchTriggerType TriggerType; // +0x80
    unsigned long Properties;       // +0x84
    virtual void Transfer(CPersistContext& context);
};

void CThingMarkerDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.Transfer(Graphic);
    context.Transfer(TriggerType);
    context.Transfer(Properties);
}
