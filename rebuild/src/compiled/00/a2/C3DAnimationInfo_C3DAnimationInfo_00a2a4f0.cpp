// C3DAnimationInfo::C3DAnimationInfo(const C3DAnimationInfo&) -- copy constructor: the CMeshDataBankEntry base (vtable 0x0129CD28, Bank, Index), then the own vtable (0x0129CD64), the counted data pointer (addref), durations, movement vector and rotation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2A4F0, length 82 bytes. No callees.
// Layout from engine/C3DAnimationInfo.h (0x28 bytes) over engine/CMeshDataBankEntry.h (Bank +4,
// Index +8): Data_Object +0xc, Duration +0x10, NonLoopingDuration +0x14, MovementVector +0x18,
// Rotation +0x24. The base copy is the compiler-generated memberwise copy (its vtable store
// survives because the base is polymorphic and inlined first); the CIVCountedPointer copy
// constructor stores and addrefs; the C3DVector is copied memberwise through one register.
struct CMeshDataBank;

struct CMeshDataBankEntry {                // vtable 0x0129CD28
    virtual void v00();
    CMeshDataBank* Bank;                   // +4
    unsigned long Index;                   // +8
};

struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);
    virtual void Delete();
    unsigned long IVRefCount;              // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct C3DAnimationInfo : CMeshDataBankEntry {   // vtable 0x0129CD64
    CIVCountedPointer Data;                // +0xc
    float Duration;                        // +0x10
    float NonLoopingDuration;              // +0x14
    C3DVector MovementVector;              // +0x18
    float Rotation;                        // +0x24
    C3DAnimationInfo(const C3DAnimationInfo& other);
};

C3DAnimationInfo::C3DAnimationInfo(const C3DAnimationInfo& other)
    : CMeshDataBankEntry(other),
      Data(other.Data),
      Duration(other.Duration),
      NonLoopingDuration(other.NonLoopingDuration),
      MovementVector(other.MovementVector),
      Rotation(other.Rotation)
{
}