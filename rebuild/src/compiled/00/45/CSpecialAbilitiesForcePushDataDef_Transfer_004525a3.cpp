// CSpecialAbilitiesForcePushDataDef::Transfer(CPersistContext& context) -- persist Damage
// Leaf: ?Transfer@CSpecialAbilitiesForcePushDataDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004525A3, virtual `ret 4` (RTTI .?AVCSpecialAbilitiesForcePushDataDef@@ : CSubDefClassBase : CDefClassBase).
// Callees: CPersistContext::Transfer<float> 0x00431061.
// Layout: Ego_r.pdb CSpecialAbilitiesForcePushDataDef (0x2c): Damage +0x28 (float).
// namepass DEF1: the stand-in archive class was CPersistContext (one name per callee).
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CSpecialAbilitiesForcePushDataDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base04[0x24];       // CSubDefClassBase body
    float Damage;           // +0x28
};

void CSpecialAbilitiesForcePushDataDef::Transfer(CPersistContext& context)
{
    context.Transfer(Damage);
}
