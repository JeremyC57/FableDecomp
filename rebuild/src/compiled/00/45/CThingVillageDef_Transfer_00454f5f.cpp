// CThingVillageDef::Transfer(CPersistContext& context) -- persist the CThingBaseDef part, then the graphic
// Leaf: ?Transfer@CThingVillageDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00454F5F, virtual (Ego_r.pdb ?Transfer@CThingVillageDef@@UAEXAAVCPersistContext@@@Z), `ret 4`.
// Callees: CThingBaseDef::Transfer 0x00676BE0; Transfer<CEngineGraphic> 0x00451E11.
// Layout: Ego_r.pdb CThingVillageDef (0x84) less 4: Graphic +0x70.
// namepass DEF1: the stand-in archive class and base were CPersistContext / CThingBaseDef (one name per callee).
#pragma optimize("s",on)
class CEngineGraphic { public: unsigned char _body[0x0e]; };   // retail 14 bytes
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CThingBaseDef { public:
    virtual void Transfer(CPersistContext& context);   // 0x00676BE0
    unsigned char _body04[0x70 - 4];                   // retail def base (Ego_r.pdb CThingBaseDef 0x74 less 4)
};
class CThingVillageDef : public CThingBaseDef { public:
    CEngineGraphic Graphic;         // +0x70
    virtual void Transfer(CPersistContext& context);
};

void CThingVillageDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.Transfer(Graphic);
}
