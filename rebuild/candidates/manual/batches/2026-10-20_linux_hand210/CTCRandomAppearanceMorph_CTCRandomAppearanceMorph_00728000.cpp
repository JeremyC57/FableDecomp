// CTCRandomAppearanceMorph::CTCRandomAppearanceMorph(CThing& thing) -- thing-component and appearance-component bases, an empty mesh-morph set, the appearance seed taken from (and advancing) the world seed, then registration with the owner's graphic appearance
// Leaf: ??0CTCRandomAppearanceMorph
// Retail: Fable.exe (TLC, VC7.1) @ 0x00728000, 188 bytes (`ret 4`; PDB ??0CTCRandomAppearanceMorph@@QAE@AAVCThing@@@Z).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, operator new @ 0x00BFEA1A (0x18), CCountedPointer<CMeshMorphSet>::Reset(T*)
//   @ 0x007222F0 (manifest type argument propagated), CThing::PeekComponent() const @ 0x004C7A70 (PDB
//   ?PeekComponent@CThing@@QBEABVCMainGameComponent@@XZ; landed callers name it "GetDisplayEngine"), GFROR13 @ 0x00497890,
//   CTCGraphicAppearance::RegisterComponent @ 0x004BD390.
// Vtables: CAGraphicAppearanceComponent 0x0125A170, CTCRandomAppearanceMorph 0x0126653C (primary) / 0x01266520 (component base).
// Layout: Ego_r.pdb CTCRandomAppearanceMorph: PMeshMorphSet +0x10 (CCountedPointer), Seed +0x18, PrimHandles +0x1c (12-byte
//   retail STLport vector, PDB 16), PTCSkeletalMorph +0x28 (PDB +0x2c); CMainGameComponent WorldSeed +0x1613c; CThing
//   PTCGraphicAppearance +0x64 (as the landed ReplaceableMesh ctor 0x007A66A0, whose two-base model this follows).
// Shape: the counted pointer, the vector and the init-list pointer all zero after the vtable stores (one `xor ebp,ebp`); the
//   morph set's two vectors zero inside the null-checked new; GetPWorldSeed() is the inline `&PeekComponent().WorldSeed`
//   (called twice: read, then the inline seed step `*s = *s * 0x24A1 + 0x24DF ; *s = GFROR13(*s)`).
struct CThing;
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
template <class T> struct vector {                      // retail STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CMainGameComponent {
    unsigned char _pad00[0x1613c];
    unsigned long WorldSeed;                            // +0x1613c (PDB = retail)
};
namespace NGraphicAppearance {
    struct CPrimitive;
    struct CAGraphicAppearanceComponent {               // vtable 0x0125A170
        virtual void v00();
        CAGraphicAppearanceComponent() { }
    };
}
struct CTCGraphicAppearance {
    void RegisterComponent(NGraphicAppearance::CAGraphicAppearanceComponent* component);   // 0x004BD390
};
struct CThing {
    unsigned char         _pad_0x00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64 (retail)
    const CMainGameComponent& PeekComponent() const;    // 0x004C7A70
    unsigned long* GetPWorldSeed() const { return const_cast<unsigned long*>(&PeekComponent().WorldSeed); }
};
inline unsigned long GFNextRandomSeed(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
struct CTCBase {                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* owner);                             // 0x00686800
};
struct CTCSkeletalMorph;
struct CTCAppearanceMorphBase {
    struct CMeshMorphSet {                              // 0x18 bytes
        vector<long> MeshMorphs;
        vector<long> TextureMorphs;
    };
};
template <class T> struct CCountedPointer {             // {object, count block}
    T*    Object;
    void* Info;
    CCountedPointer() : Object(0), Info(0) { }
    void Reset(T* object);                              // 0x007222F0
};
struct CTCRandomAppearanceMorph : CTCBase, NGraphicAppearance::CAGraphicAppearanceComponent {
    CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> PMeshMorphSet;   // +0x10
    unsigned long     Seed;                             // +0x18
    vector<NGraphicAppearance::CPrimitive*> PrimHandles; // +0x1c
    CTCSkeletalMorph* PTCSkeletalMorph;                 // +0x28 (PDB +0x2c)
    virtual void v00();
    CTCRandomAppearanceMorph(CThing& thing);
};

CTCRandomAppearanceMorph::CTCRandomAppearanceMorph(CThing& thing)
    : CTCBase(&thing), CAGraphicAppearanceComponent(), PTCSkeletalMorph(0)
{
    PMeshMorphSet.Reset(new CTCAppearanceMorphBase::CMeshMorphSet);
    Seed = *thing.GetPWorldSeed();
    GFNextRandomSeed(thing.GetPWorldSeed());
    ParentThing->PTCGraphicAppearance->RegisterComponent(this);
}
