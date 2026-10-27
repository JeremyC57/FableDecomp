// C3DAnimation2::CanRelocateData(long) -- the animation's data may be moved unless it is locked against memory relocation
// Leaf: ?CanRelocateData@C3DAnimation2@@UBE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A553F0, 13 bytes, `ret 4`.
// Identity: slot 2 of C3DAnimation2's CMovableResource vtable 0x0129D534 (slot 0 the ??_G thunk 0x00A55400, slot 1
//   RelocateData 0x00A4E960); Ego_r.pdb `?CanRelocateData@C3DAnimation2@@UBE_NJ@Z` (public virtual const, bool (long)).
// Layout: RTTI (ghidra_out/rtti_bases.tsv) puts CMovableResource at +0x2C behind C3DAnimationStats, so the override runs
//   with `this` at that base and LockedFromMemMemoryRelocation (Ego_r C3DAnimation2 +0x78 = CMovableResource+0x48) reads
//   as [ecx+48h]. engine/C3DAnimation2.h flattens both vptrs into data members, so the class is a local model here.
class C3DAnimationStats {                                  // primary base (vptr +0x00; C3DAnimationStats/CBaseObject/CBaseClass)
public:
    virtual ~C3DAnimationStats();
    unsigned char Stats[0x28];                             // +0x04 .. +0x2B
};

class CMovableResource {                                   // secondary base, vptr at C3DAnimation2+0x2C
public:
    virtual ~CMovableResource();
    virtual void RelocateData(long delta, void* oldBase, void* newBase, long size) = 0;   // slot 1
    virtual bool CanRelocateData(long delta) const = 0;                                     // slot 2
};

class C3DAnimation2 : public C3DAnimationStats, public CMovableResource {
public:
    unsigned char Data[0x44];                              // +0x30 .. +0x73: Allocator .. RigidBonePositions
    bool LockedFromMemMemoryRelocation;                    // +0x74
    virtual void RelocateData(long delta, void* oldBase, void* newBase, long size);
    virtual bool CanRelocateData(long delta) const;
};

bool C3DAnimation2::CanRelocateData(long) const
{
    return !LockedFromMemMemoryRelocation;
}