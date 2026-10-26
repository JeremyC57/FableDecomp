// CTCFish::CTCFish(CThing& thing) -- constructor: CTCBase, the fish def taken from the thing's base def, then a random length, weight and speed (each the def's minimum plus a random whole amount below its maximum)
// Leaf: ??0CTCFish
//
// Retail 0x007DFB20, 304 bytes incl. padding (`ret 4`); vtable 0x0126FD2C. Ego_r.pdb CTCFish (0x1c).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CDefClassBase::GetSubDef<CFishDef>(CDefPointer<const CFishDef>*) const
// @ 0x007B2600 (label "GetSubDef<CAICreatureWillPowerIndicatorDef>" is a byte-twin instantiation), CTCBase::GetPRandomSeed
// @ 0x00686DA0 (three times), _ftol2 @ 0x00BFEA70, GFROR13 @ 0x00497890 (__fastcall). Float 0x0122DCB4 = 2^32 (the unsigned
// conversion fix-up).
// Layout = Ego_r.pdb CTCFish: PDef +0x0c, Length +0x10, Weight +0x14, Speed +0x18; CFishDef MinLength +0x28, MaxLength +0x2c,
// MinWeight +0x30, MaxWeight +0x34, MinSpeed +0x38, MaxSpeed +0x3c (PDB offsets); retail CThing base def +0x70.
// Shape: PDef is an init-list CDefPointer (one immediate zero store after the vtable) filled through the base def; each
// field is `Min + (float)GFRandom((unsigned long)Max, GetPRandomSeed())` with the engine's inline LCG random (seed advanced in
// place, GFROR13, `range ? seed % range : 0`; arguments evaluated right to left, so the seed accessor runs before the
// float-to-integer conversion); the def pointer is re-read from the member before each field.
unsigned long __fastcall GFROR13(unsigned long value);      // 0x00497890

__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CThing;
class CFishDef { public:
    virtual void v00();
    long RefCount;                                          // +0x04
    unsigned char _pad08[0x28 - 0x08];
    float MinLength;                                        // +0x28
    float MaxLength;                                        // +0x2c
    float MinWeight;                                        // +0x30
    float MaxWeight;                                        // +0x34
    float MinSpeed;                                         // +0x38
    float MaxSpeed;                                         // +0x3c
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<const CFishDef>* out) const; // 0x007B2600
};

class CThing {
public:
    unsigned char _pad00[0x70];
    const CDefClassBase* PBaseDef;                          // +0x70 CDefPointer<CThingBaseDef const>::Object
};

class CTCBase {
public:
    CTCBase(CThing* parent);                                // 0x00686800
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x4];                              // ToBeRemoved, IsInGlobalUpdateTCs
    unsigned long* GetPRandomSeed();                        // 0x00686DA0
};

class CTCFish : public CTCBase {                            // vtable 0x0126FD2C
public:
    CDefPointer<const CFishDef> PDef;                       // +0x0c
    float Length;                                           // +0x10
    float Weight;                                           // +0x14
    float Speed;                                            // +0x18
    CTCFish(CThing& thing);
    virtual void v00();
};

CTCFish::CTCFish(CThing& thing)
    : CTCBase(&thing), PDef()
{
    ParentThing->PBaseDef->GetSubDef(&PDef);
    Length = PDef->MinLength + (float)GFRandom((unsigned long)PDef->MaxLength, GetPRandomSeed());
    Weight = PDef->MinWeight + (float)GFRandom((unsigned long)PDef->MaxWeight, GetPRandomSeed());
    Speed = PDef->MinSpeed + (float)GFRandom((unsigned long)PDef->MaxSpeed, GetPRandomSeed());
}