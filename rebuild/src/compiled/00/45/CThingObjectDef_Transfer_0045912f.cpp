// CThingObjectDef::Transfer(CPersistContext& context) -- persist the object definition: CThingBaseDef part, a zero Health promoted to 1 outside the load/save modes, graphic, rotation time, materials, death object, property, die-off timer, then the five named flag bits
// Leaf: ?Transfer@CThingObjectDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045912F, length 445 bytes (virtual, `ret 4`; unaligned entry, size-optimised
// def module with a frame pointer).
// Identity: vtable 0x01234E94 (CThingObjectDef, RTTI) slot 18 is this body.
// Callees: CThingBaseDef::Transfer 0x00676BE0; CPersistContext typed transfers Transfer(CEngineGraphic&) 0x00451E11,
//   Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(unsigned long&) 0x00431020,
//   Transfer<bool>(name, value, default) 0x004045C0.
// Layout: Ego_r.pdb CThingObjectDef (0xA0) minus 4 (retail def base is 4 bytes smaller): base Health +0x50, Graphic +0x70
//   (16-byte CEngineGraphic), RotationTime +0x80, Material +0x84, FloorMaterial +0x88, OnDeathObject +0x8C, Property +0x90,
//   DieOffTimer +0x94, flag bits +0x98. CPersistContext::Mode +0x18 (landed SerialiseGameState rows). No engine header.
// Shape: the CThingBuildingDef::Transfer 0x00454B8E model: the five flags share ONE bool local homed in the dead
//   context-argument slot ([ebp+0Bh]), the `false` default is a one-byte temporary at [ebp-1], and each bit is merged back
//   with xor/and/xor; the constant 1 lives in ebx (bit masks and the mode test).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CEngineGraphic { public:
    unsigned char _body[0x10];
};

class CPersistContext { public:
    unsigned char _pad00[0x18];
    long          Mode;                                                      // +0x18
    template <class T> void Transfer(T& value);                                              // 0x00431102
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: bool 0x004045C0
};

class CThingBaseDef { public:
    virtual void Transfer(CPersistContext& context);                         // 0x00676BE0
    unsigned char _pad04[0x50 - 4];
    float         Health;                                                    // +0x50
    unsigned char _pad54[0x70 - 0x54];
};

class CThingObjectDef : public CThingBaseDef { public:            // vtable 0x01234E94
    CEngineGraphic Graphic;                         // +0x70
    float          RotationTime;                    // +0x80
    long           Material;                        // +0x84
    long           FloorMaterial;                   // +0x88
    long           OnDeathObject;                   // +0x8c
    unsigned long  Property;                        // +0x90
    long           DieOffTimer;                     // +0x94
    bool           IsFragile : 1;                   // +0x98 bit 0
    bool           IsClothing : 1;                  // +0x98 bit 1
    bool           IsSelfIlluminating : 1;          // +0x98 bit 2
    bool           UseHighDetailQuadTree : 1;       // +0x98 bit 3
    bool           Electrocutable : 1;              // +0x98 bit 4
    virtual void Transfer(CPersistContext& context);
};

void CThingObjectDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    if (context.Mode != 2 && context.Mode != 3 && context.Mode != 1)
    {
        if (Health == 0.0f)
            Health = 1.0f;
    }
    context.Transfer(Graphic);
    context.Transfer(RotationTime);
    context.Transfer(Material);
    context.Transfer(FloorMaterial);
    context.Transfer(OnDeathObject);
    context.Transfer(Property);
    context.Transfer(DieOffTimer);
    bool flag = IsFragile;
    context.Transfer("IsFragile", flag, false);
    IsFragile = flag;
    flag = IsClothing;
    context.Transfer("IsClothing", flag, false);
    IsClothing = flag;
    flag = IsSelfIlluminating;
    context.Transfer("IsSelfIlluminating", flag, false);
    IsSelfIlluminating = flag;
    flag = UseHighDetailQuadTree;
    context.Transfer("UseHighDetailQuadTree", flag, false);
    UseHighDetailQuadTree = flag;
    flag = Electrocutable;
    context.Transfer("Electrocutable", flag, false);
    Electrocutable = flag;
}