// CTCSkeletalMorph::ForceSkeletalMorph(C3DSkeletalMorphTarget* target) -- reset the skeletal morph to a single parameter driving the given target at full weight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007864C0, 182 bytes (`ret 4`).
// Callees: C3DSkeletalMorph members on PSkeletalMorph @ 0x00A45410 (clear the parameters), @ 0x00A45470 (add a parameter, by
// const CCountedPointer&) and @ 0x00A45330 (apply) -- unnamed in the manifest (propagated labels), named descriptively;
// operator new @ 0x00BFEA1A (the 0x14-byte parameter and the 12-byte count block), C3DSkeletalMorphParameter::
// C3DSkeletalMorphParameter(C3DSkeletalMorphTarget*) @ 0x00A455A0, C3DSkeletalMorphParameter::SetCurrentValue(float)
// @ 0x00A45370, operator delete @ 0x00BFE9BC; the count block holds the per-type deleter 0x00786A30.
// Layout (retail; Ego_r.pdb with the 12-byte CArray): Changed +0x0c, Morphs +0x10, PSkeletalMorph (CCountedPointer<
// C3DSkeletalMorph>) +0x1c.
// Shape: a CCountedPointer local built from `new` through the raw-pointer constructor (the block's object re-read from the
// pointer); its destructor decrements the block through the pointer and re-reads it before the destroy and the delete.
#include <stddef.h>
void* operator new(size_t size);                        // 0x00BFEA1A
void operator delete(void* p);                          // 0x00BFE9BC
void __fastcall Destroy_00786a30(void* object);         // 0x00786A30: the per-type deleter stored in the count block
struct CCPPointerInfo {
    long References;                                    // +0
    void (__fastcall* PfnDestroy)(void*);               // +4
    void* Object;                                       // +8
    CCPPointerInfo(void* object) : References(1), PfnDestroy(Destroy_00786a30), Object(object) { }
};
template <class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* p) : Data(p) { if (Data) Info = new CCPPointerInfo(Data); else Info = 0; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->PfnDestroy(Info->Object);
                delete Info;
            }
        }
    }
    T* operator->() const { return Data; }
};
struct C3DSkeletalMorphTarget;
struct C3DSkeletalMorphParameter {
    unsigned char _data[0x14];
    C3DSkeletalMorphParameter(C3DSkeletalMorphTarget* target);   // 0x00A455A0
    void SetCurrentValue(float value);                  // 0x00A45370
};
struct C3DSkeletalMorph {
    void ClearParameters();                             // 0x00A45410
    void AddParameter(const CCountedPointer<C3DSkeletalMorphParameter>& parameter);   // 0x00A45470
    void ApplyParameters();                             // 0x00A45330
};
struct CTCSkeletalMorph {
    virtual void v00();
    void* ParentThing;                                  // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    bool Changed;                                       // +0x0c
    unsigned char _pad0d[0x10 - 0x0d];
    unsigned char Morphs[0xc];                          // +0x10 CArray<CTCSkeletalMorph::CSkeletalMorph>
    CCountedPointer<C3DSkeletalMorph> PSkeletalMorph;   // +0x1c
    void ForceSkeletalMorph(C3DSkeletalMorphTarget* target);
};

void CTCSkeletalMorph::ForceSkeletalMorph(C3DSkeletalMorphTarget* target)
{
    Changed = false;
    PSkeletalMorph->ClearParameters();
    CCountedPointer<C3DSkeletalMorphParameter> parameter(new C3DSkeletalMorphParameter(target));
    PSkeletalMorph->AddParameter(parameter);
    parameter->SetCurrentValue(1.0f);
    PSkeletalMorph->ApplyParameters();
}
