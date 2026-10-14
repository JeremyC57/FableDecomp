// C3DAnimationBlendState::SetBaseAnimation(const CIVCountedPointer<C3DAnimationStateBase>&) -- unshares the data block, then replaces its BaseAnimation (+8)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3C2D0, length 64 bytes.
// Callees: C3DAnimationBlendState::MakeOwnCopyOfData @ 0x00A3C1A0; CIVCountedPointeeBase slot 1 (Delete) at zero count.
// C3DAnimationBlendState is {vptr, CIVCountedPointer<CSharedData> Data} (PDB layout, 8 bytes);
// CSharedData has BaseAnimation at +8 (PDB), CombinationBlends CArray at +0xc and SkeletalMorph at
// +0x18 in retail (the retail CArray is 12 bytes, the Ego PDB's is 16). After MakeOwnCopyOfData
// the inlined CIVCountedPointer::operator= runs on Data->BaseAnimation: VC forms its address once
// (`add esi,8`) and works through it.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1
    unsigned long IVRefCount;              // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CIVCountedPointeeBase* old = Object;   // held in a register across the release (retail `dec [ecx+4]`)
        if (old != other.Object) {
            if (old) {
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;             // re-read: the argument is a reference
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

struct CSharedData : CIVCountedPointeeBase {
    CIVCountedPointer BaseAnimation;       // +8
    unsigned char CombinationBlends[0xc];  // +0xc CArray<C3DAnimationStateAlpha> (12 bytes in retail)
    void* SkeletalMorph[2];                // +0x18 CCountedPointer<const C3DSkeletalMorph>
};

struct C3DAnimationBlendState {
    virtual void v00();
    CSharedData* Data;                     // +4 CIVCountedPointer<CSharedData>::Object
    void MakeOwnCopyOfData();              // 0x00A3C1A0
    void SetBaseAnimation(const CIVCountedPointer& animation);
};

void C3DAnimationBlendState::SetBaseAnimation(const CIVCountedPointer& animation)
{
    MakeOwnCopyOfData();
    Data->BaseAnimation = animation;
}
