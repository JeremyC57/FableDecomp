// C3DAnimationInfo::Reset() -- private virtual (slot 3): drops the animation data (CIVCountedPointer at +0xc) and zeroes the cached durations and movement vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24D40, length 48 bytes.
// Callees: CIVCountedPointeeBase slot 1 (Delete, `call [eax+4]`) when the count reaches zero.
// Layout from engine/C3DAnimationInfo.h: Data_Object +0xc, Duration +0x10, NonLoopingDuration
// +0x14, MovementVector X/Y/Z +0x18/+0x1c/+0x20, Rotation +0x24 (not reset). `Data = NULL`
// through the raw-pointer assignment: the old object is released and nulled inside the
// `if (Object)`, then the pointer is stored again unconditionally (the `if (p)` addref folds
// away for a null constant), which is why retail writes +0xc twice. The vector is zeroed
// with a chained assignment (`X = Y = Z = 0`), hence the Z, Y, X store order.
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1
    unsigned long IVRefCount;              // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer& operator=(CIVCountedPointeeBase* p)
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
        Object = p;
        if (p)
            ++p->IVRefCount;
        return *this;
    }
};

class C3DVector { public:
    float X, Y, Z;
};

class C3DAnimationInfo { public:                  // engine/C3DAnimationInfo.h offsets
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void Reset();                  // slot 3 @ 0x00A24D40
    void* Bank;                            // +4 (CMeshDataBankEntry)
    unsigned long Index;                   // +8
    CIVCountedPointer Data;                // +0xc
    float Duration;                        // +0x10
    float NonLoopingDuration;              // +0x14
    C3DVector MovementVector;              // +0x18
    float Rotation;                        // +0x24
};

void C3DAnimationInfo::Reset()
{
    Data = 0;
    Duration = 0.0f;
    NonLoopingDuration = 0.0f;
    MovementVector.X = MovementVector.Y = MovementVector.Z = 0.0f;   // chained: Z, Y, X stores
}