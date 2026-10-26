// CDragonActionHoverDef::Transfer(CPersistContext& context) -- persist WindSpeed
// Leaf: ?Transfer@CDragonActionHoverDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00452CCD, virtual `ret 4` (RTTI .?AVCDragonActionHoverDef@@ : CSubDefClassBase : CDefClassBase).
// Callees: CPersistContext::Transfer<float> 0x00431061.
// Layout: Ego_r.pdb CDragonActionHoverDef (0x2c): WindSpeed +0x28 (float).
// namepass DEF1: the stand-in archive class was CPersistContext (one name per callee).
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CDragonActionHoverDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base04[0x24];       // CSubDefClassBase body
    float WindSpeed;        // +0x28
};

void CDragonActionHoverDef::Transfer(CPersistContext& context)
{
    context.Transfer(WindSpeed);
}
