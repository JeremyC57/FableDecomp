// CTCBed::CTCBed(CThing* parent) -- CTCBase(parent), own vtable, Empty/Animating = true, GettingIn/GettingOut = false, BedSleepAnim = -1, null def
// Leaf: CTCBed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00746820, length 51 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800.
// Vtable 0x01268964 = CTCBed (rtti-vtables.tsv): the chunk label "CTCEditor" is propagated.
// PDB CTCBed (0x18 bytes): Empty +0x0c, GettingIn +0x0d, GettingOut +0x0e, Animating +0x0f,
// BedSleepAnim +0x10, PDef (CDefPointer<const CBedDef>) +0x14.
// Shape: speed TU; the false/true values live in eax/cl, and the compiler interleaves the init-list
// stores with the derived vtable store (the two zero bytes before it, the rest after).
struct CThing;
struct CBedDef;

struct CTCBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                          // 0x00686800
};

template<class T> struct CDefPointer {
    T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

struct CTCBed : CTCBase_Calls {
    bool                       Empty;                       // +0x0c
    bool                       GettingIn;                   // +0x0d
    bool                       GettingOut;                  // +0x0e
    bool                       Animating;                   // +0x0f
    long                       BedSleepAnim;                // +0x10
    CDefPointer<const CBedDef> PDef;                        // +0x14
    virtual void v00();                                     // own vtable 0x01268964
    CTCBed(CThing* parent);
};

CTCBed::CTCBed(CThing* parent)
    : CTCBase_Calls(parent)
    , Empty(true)
    , GettingIn(false)
    , GettingOut(false)
    , Animating(true)
    , BedSleepAnim(-1)
    , PDef()
{
}
