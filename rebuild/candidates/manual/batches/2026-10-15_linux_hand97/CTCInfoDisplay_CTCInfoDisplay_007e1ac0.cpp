// CTCInfoDisplay::CTCInfoDisplay(CThing*) -- CTCBase(owner), vtable, both direction flags false, two empty text tags, zero radius and time
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1AC0, length 57 bytes (catalogued as the propagated label
// "CTCOwnedEntity::CTCOwnedEntity"; the vtable it stores, 0x01270044, is CTCInfoDisplay's in rtti-vtables.tsv, and its
// only caller @ 0x004D5E86 is the factory).
// Leaf: ??0CTCInfoDisplay
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CCharString::CCharString() @ 0x0099E4B0 (twice).
// PDB CTCInfoDisplay (0x20 bytes): Forward +0x0c, Backward +0x0d, TextTag +0x10, TextTagBack +0x14, Radius +0x18,
// DisplayTime +0x1c.
// Shape: one zero register (ebx) for the two bytes before the string constructors and the two words after them.
struct CThing;

struct CCharString {
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
};

struct CTCBase_007e1ac0 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
    CTCBase_007e1ac0(CThing* owner);                // 0x00686800
};

struct CTCInfoDisplay : CTCBase_007e1ac0 {
    bool        Forward;                            // +0x0c
    bool        Backward;                           // +0x0d
    CCharString TextTag;                            // +0x10
    CCharString TextTagBack;                        // +0x14
    float       Radius;                             // +0x18
    float       DisplayTime;                        // +0x1c
    CTCInfoDisplay(CThing* owner);
    virtual void v00();
};

CTCInfoDisplay::CTCInfoDisplay(CThing* owner)
    : CTCBase_007e1ac0(owner), Forward(false), Backward(false), Radius(0.0f), DisplayTime(0.0f)
{
}
