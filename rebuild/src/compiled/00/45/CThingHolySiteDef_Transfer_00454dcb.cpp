// CThingHolySiteDef::Transfer(CPersistContext& context) -- persist the CThingBaseDef part, then the graphic and the player-start flag
// Leaf: ?Transfer@CThingHolySiteDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00454DCB, virtual (Ego_r.pdb ?Transfer@CThingHolySiteDef@@UAEXAAVCPersistContext@@@Z), `ret 4`.
// Callees: CThingBaseDef::Transfer 0x00676BE0; Transfer<CEngineGraphic> 0x00451E11, Transfer<bool> 0x0043314A.
// Layout: Ego_r.pdb CThingHolySiteDef (0x84) less 4: Graphic +0x70 (14 bytes), PlayerStart +0x7E.
// namepass DEF1: the stand-in archive class and base were CPersistContext / CThingBaseDef (one name per callee).
class CEngineGraphic { public: unsigned char _body[0x0e]; };   // retail 14 bytes
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CThingBaseDef { public:
    virtual void Transfer(CPersistContext& context);   // 0x00676BE0
    unsigned char _body04[0x70 - 4];                   // retail def base (Ego_r.pdb CThingBaseDef 0x74 less 4)
};
class CThingHolySiteDef : public CThingBaseDef { public:
    CEngineGraphic Graphic;         // +0x70
    bool PlayerStart;               // +0x7e
    virtual void Transfer(CPersistContext& context);
};

void CThingHolySiteDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.Transfer(Graphic);
    context.Transfer(PlayerStart);
}
