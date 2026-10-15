// C3DSkeletalMorphParameter::C3DSkeletalMorphParameter(const C3DSkeletalMorphParameter&) -- CBase(), vtable, an empty shared-data pointer assigned from the source's, the source's current value, no owning morph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44D80, length 104 bytes.
// Leaf: ??0C3DSkeletalMorphParameter
// Callees: 0x0099A2F0 CBase::CBase, the counted block's destroy thunk (`call [info+4]`) and 0x00BFE9BC
// operator delete (the assignment's release path, unreachable after the zeroing but still emitted).
// Vtable 0x0129D490 = C3DSkeletalMorphParameter (rtti-vtables.tsv).
// Layout (PDB, 20 bytes): CurrentValue +4, SkeletalMorph +8, SharedData +0xc (CCountedPointer<CShared>:
// data +0xc, count block +0x10).
// Shape: SharedData is default-constructed (both words nulled from ecx) and then ASSIGNED in the body through
// the inlined CCountedPointer::operator= (the source's block and object read before the block compare; VC7.1
// keeps the release test on the known-null block); CurrentValue and SkeletalMorph are body assignments after it.
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

struct CCPPointerInfo {                                  // PDB: RefCount +0, DeleteFunc +4, Data +8
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template<class T> struct CCountedPointer {
    T*              Data;                                // +0
    CCPPointerInfo* Info;                                // +4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info != 0) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info != 0)
                ++info->RefCount;
        }
        return *this;
    }
};

struct CBase {
    virtual void v00();
    CBase();                                             // 0x0099A2F0
};

struct C3DSkeletalMorph;
struct C3DSkeletalMorphParameter : CBase {
    struct CShared;
    float                     CurrentValue;              // +0x04
    C3DSkeletalMorph*         SkeletalMorph;             // +0x08
    CCountedPointer<CShared>  SharedData;                // +0x0c   sizeof == 0x14
    virtual void v00();                                  // own vtable 0x0129D490
    C3DSkeletalMorphParameter(const C3DSkeletalMorphParameter& other);
};

C3DSkeletalMorphParameter::C3DSkeletalMorphParameter(const C3DSkeletalMorphParameter& other)
    : CBase()
{
    SharedData = other.SharedData;
    CurrentValue = other.CurrentValue;
    SkeletalMorph = 0;
}
