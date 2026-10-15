// CTCSkeletalMorph::`scalar deleting destructor'(unsigned int) -- stores the CTCSkeletalMorph vtable, destroys the counted skeletal-morph pointer (+0x1c) and the morph array (+0x10), runs CTCBase::~CTCBase and frees on bit 0
// Leaf: ??_GCTCSkeletalMorph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007874C0, length 54 bytes (vtable 0x0126B6F4 slot 0).
// Callees: CCountedPointer<C3DSkeletalMorph>::~CCountedPointer @ 0x005BA4F1 (manifest "_Dest_val<...>",
// propagated), CArray<CTCSkeletalMorph::CSkeletalMorph>::~CArray @ 0x00787340 (manifest
// "CCombatWheel::ResetRings", propagated), CTCBase::~CTCBase @ 0x00686830 (manifest "~CChunkedFileChunk",
// propagated), operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CTCSkeletalMorph vtable 0x0126B6F4.
// Layout: PDB CTCSkeletalMorph (40 bytes) puts Morphs at +0x10 and PSkeletalMorph at +0x20; the retail
// CArray is 0x0c bytes (wave 23 rule), so the counted pointer sits at +0x1c (the `lea ecx,[esi+1Ch]`).
// Shape: own vtable store, member destructors in reverse declaration order, then the base destructor;
// a `new` factory pulls the vtable (and so the `??_G`) into the TU, the virtual destructor is inline.
struct CThing;

struct CTCBase {                                   // engine/CTCBase.h offsets, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing& owner);                        // 0x00686800
    ~CTCBase();                                    // 0x00686830
};

struct CSkeletalMorphArray {                       // CArray<CTCSkeletalMorph::CSkeletalMorph>, 0x0c bytes
    void* m_pData;
    long  m_size;
    long  m_capacity;
    CSkeletalMorphArray();
    ~CSkeletalMorphArray();                        // 0x00787340
};

struct CCountedPointerSkeletalMorph {              // CCountedPointer<C3DSkeletalMorph>
    void* m_pObject;
    void* m_pCount;
    CCountedPointerSkeletalMorph();
    ~CCountedPointerSkeletalMorph();               // 0x005BA4F1
};

struct CTCSkeletalMorph : CTCBase {                // vtable 0x0126B6F4
    bool                         Changed;          // +0x0c
    CSkeletalMorphArray          Morphs;           // +0x10
    CCountedPointerSkeletalMorph PSkeletalMorph;   // +0x1c
    CTCSkeletalMorph(CThing& owner) : CTCBase(owner) { }
    __forceinline virtual ~CTCSkeletalMorph() { }
};

CTCSkeletalMorph* Create_CTCSkeletalMorph(CThing& owner)
{
    return new CTCSkeletalMorph(owner);
}
