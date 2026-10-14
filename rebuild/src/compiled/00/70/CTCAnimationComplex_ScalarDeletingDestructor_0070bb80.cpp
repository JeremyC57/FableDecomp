// CTCAnimationComplex::`scalar deleting destructor'(unsigned int) -- delete the owned animate-thing, ~CTCBase, optional delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070BB80, length 63 bytes.
// Callees: CAnimateThing::~CAnimateThing @ 0x0070BA90 (a direct call: the owned object's class is
// NOT polymorphic, so `delete` is destructor + operator delete), operator delete @ 0x00BFE9BC
// (twice: the owned object, then this when bit 0 of the flag is set), CTCBase::~CTCBase
// @ 0x00686830. Vtable 0x012650A4 CTCAnimationComplex (rtti-vtables.tsv); the manifest's
// "CAInputProcess" label is propagated.
// Shape: the pointer at +0xc is loaded into edi before this class's vtable store (scheduling),
// `if (p) delete p;` keeps the explicit null test, then the base and the deleting tail. The
// destructor is `__forceinline` so its body lands inside the `??_G` as retail has it (otherwise
// VC7.1 emits the `??1` out of line and the deleting destructor merely calls it).
void __cdecl operator delete(void* block);     // 0x00BFE9BC

struct CAnimateThing {
    unsigned char _body[4];
    ~CAnimateThing();                           // 0x0070BA90
};

struct CTCBase {
    virtual void v00();
    unsigned char _pad[0x8];                    // +0x04..+0x0b
    ~CTCBase();                                 // 0x00686830
};

struct CTCAnimationComplex : CTCBase {
    CAnimateThing* m_pAnimateThing;             // +0x0c
    __forceinline virtual ~CTCAnimationComplex()
    {
        if (m_pAnimateThing != 0)
            delete m_pAnimateThing;
    }
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new CTCAnimationComplex;
}