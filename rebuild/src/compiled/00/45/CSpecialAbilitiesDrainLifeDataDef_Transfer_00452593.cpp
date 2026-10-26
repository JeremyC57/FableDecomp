// CSpecialAbilitiesDrainLifeDataDef::Transfer(CPersistContext& context) -- persist Damage
// Leaf: ?Transfer@CSpecialAbilitiesDrainLifeDataDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00452593, virtual `ret 4` (RTTI .?AVCSpecialAbilitiesDrainLifeDataDef@@ : CSubDefClassBase : CDefClassBase).
// Callees: CPersistContext::Transfer<float> 0x00431061.
// Layout: Ego_r.pdb CSpecialAbilitiesDrainLifeDataDef (0x2c): Damage +0x28 (float).
// namepass DEF1: the stand-in archive class was CPersistContext (one name per callee).
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CSpecialAbilitiesDrainLifeDataDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base04[0x24];       // CSubDefClassBase body
    float Damage;           // +0x28
};

void CSpecialAbilitiesDrainLifeDataDef::Transfer(CPersistContext& context)
{
    context.Transfer(Damage);
}
