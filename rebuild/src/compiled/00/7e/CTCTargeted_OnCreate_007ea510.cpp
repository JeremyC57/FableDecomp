// CTCTargeted::OnCreate() -- loads the targeting sub-def from the parent's def; copies its glow flag (default on) and its targeting flags (default none)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EA510, length 62 bytes. Vtable 0x0123F024 (CTCTargeted) slot 4.
// Callee: CDefClassBase::GetSubDef<CTargetingDef>(CDefPointer<const CTargetingDef>*) @ 0x007EA960 (bool; manifest names a
// byte-identical instantiation "GetSubDef<CAICreatureWillPowerIndicatorDef>"), called on the def object the parent thing
// holds at +0x70.
// Layout: engine/CTCTargeted.h (Ego_r.pdb): ParentThing +0x04 (header `obj`), TargetingFlags +0x0c, UseTargetGlow +0x11,
// PTargetingDef +0x14. CTargetingDef: TargetingFlags +0x28, UseTargetGlow +0x2c (descriptive, from the two reads).
// The engine header is a flattened POD and the method needs the def-pointer member typed, so the class is a local model.
// Shape: &PTargetingDef is formed once in edi and re-used to read the object (`mov edi,[edi]`) for the null-tested
// flags copy; the two arms of that last `if` end in separate returns (no join).
struct CTargetingDef {
    unsigned char _pad_0x00[0x28];
    unsigned long TargetingFlags;            // +0x28
    bool          UseTargetGlow;             // +0x2c
};

template<class T> struct CDefPointer {
    T* Object;
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer<const CTargetingDef>* out);   // 0x007EA960
};

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                     // +0x70
};

struct CTCTargeted {
    void*         __vftable;                 // +0x00
    CThing*       ParentThing;               // +0x04
    unsigned char _pad_0x08[0x4];            // +0x08
    unsigned long TargetingFlags;            // +0x0c
    bool          Targetable;                // +0x10
    bool          UseTargetGlow;             // +0x11
    bool          ValidTargetWithoutLOS;     // +0x12
    unsigned char _pad_0x13[0x1];
    CDefPointer<const CTargetingDef> PTargetingDef;   // +0x14
    void OnCreate();
};

void CTCTargeted::OnCreate()
{
    if (ParentThing->PDef->GetSubDef(&PTargetingDef))
        UseTargetGlow = PTargetingDef.Object->UseTargetGlow;
    else
        UseTargetGlow = true;
    if (PTargetingDef.Object)
        TargetingFlags = PTargetingDef.Object->TargetingFlags;
    else
        TargetingFlags = 0;
}