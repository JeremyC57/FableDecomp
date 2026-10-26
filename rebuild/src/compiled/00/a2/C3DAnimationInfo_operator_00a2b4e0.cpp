// C3DAnimationInfo::operator=(const C3DAnimationInfo& other) -- memberwise assignment of an animation info record: the CMeshDataBankEntry bank/index, the counted data pointer, both durations, the movement vector and the rotation (the vptr is left alone)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2B4E0, 109 bytes, __fastcall, ret 4, returns `this`.
// Leaf: ??4C3DAnimationInfo
// The catalog label (CAnimationEntry::operator=) is propagated: the only caller is the SGI copy loop
// 0x00A2BC70 over 0x28-byte elements (`imul 66666667h ; sar 4` = divide by 40), and the touched layout is
// C3DAnimationInfo's (engine/C3DAnimationInfo.h, 0x28 bytes; cf. the landed copy constructor 0x00A2A4F0).
// Callee: the old data block's virtual slot 1 when its intrusive count drops to zero.
// Shape: the base CMeshDataBankEntry assignment copies Bank (+4) and Index (+8) but not the vptr; the
// CIVCountedPointer assignment is the inline compare / release-through-a-local / null-store / add-ref
// form; the durations copy through registers; C3DVector is a plain POD whose implicit (block) assignment
// VC addresses through the two `lea` pointers (+0x18) -- a user memberwise operator= folds the offsets
// into `[edi+18h]`/`[esi+18h]` moves instead; Rotation last.
class CMeshDataBank;

class CMeshDataBankEntry { public:                // vtable 0x0129CD28
    virtual void v00();
    CMeshDataBank* Bank;                   // +4
    unsigned long Index;                   // +8
};

class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);
    virtual void Delete();                 // slot 1
    unsigned long IVRefCount;              // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                CIVCountedPointeeBase* old = Object;
                if (--old->IVRefCount == 0)
                    old->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

class C3DVector { public:
    float X, Y, Z;
};

class C3DAnimationInfo : public CMeshDataBankEntry { public:   // vtable 0x0129CD64
    CIVCountedPointer Data;                // +0xc
    float Duration;                        // +0x10
    float NonLoopingDuration;              // +0x14
    C3DVector MovementVector;              // +0x18
    float Rotation;                        // +0x24
    C3DAnimationInfo& operator=(const C3DAnimationInfo& other);
};

C3DAnimationInfo& C3DAnimationInfo::operator=(const C3DAnimationInfo& other)
{
    Bank = other.Bank;
    Index = other.Index;
    Data = other.Data;
    Duration = other.Duration;
    NonLoopingDuration = other.NonLoopingDuration;
    MovementVector = other.MovementVector;
    Rotation = other.Rotation;
    return *this;
}