// Behaviour test for CTCSkeletalMorph::ForceSkeletalMorph (0x007864C0): Changed cleared, the morph cleared, one new parameter on
// the target added (count block at 1), set to 1.0 before the apply; the local reference is dropped at the end.
#include <cstdio>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static void* g_destroyed = 0; static int g_destroyCalls = 0;
void __fastcall Destroy_00786a30(void* object) { g_destroyed = object; ++g_destroyCalls; }
class CCPPointerInfo { public:
    long References;                                    // +0
    void (__fastcall* PfnDestroy)(void*);               // +4
    void* Object;                                       // +8
    CCPPointerInfo(void* object) : References(1), PfnDestroy(Destroy_00786a30), Object(object) { }
};
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
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
class C3DSkeletalMorphTarget;
class C3DSkeletalMorphParameter { public:
    C3DSkeletalMorphTarget* Target; float Value;
    C3DSkeletalMorphParameter(C3DSkeletalMorphTarget* target) : Target(target), Value(-1.0f) { }
    void SetCurrentValue(float value) { Value = value; }
};
class C3DSkeletalMorph { public:
    int cleared, applied; bool keep; C3DSkeletalMorphParameter* added; CCPPointerInfo* addedInfo; float valueAtApply;
    void ClearParameters() { ++cleared; }
    void AddParameter(const CCountedPointer<C3DSkeletalMorphParameter>& parameter)
    {
        added = parameter.Data; addedInfo = parameter.Info;
        if (keep) ++parameter.Info->References;
    }
    void ApplyParameters() { ++applied; valueAtApply = added->Value; }
};
class CTCSkeletalMorph { public:
    virtual void v00() { }
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

int main()
{
    int bad = 0;
    C3DSkeletalMorph morph; morph.cleared = 0; morph.applied = 0; morph.keep = true; morph.added = 0;
    CTCSkeletalMorph tc; tc.Changed = true; tc.PSkeletalMorph.Data = &morph; tc.PSkeletalMorph.Info = 0;
    C3DSkeletalMorphTarget* target = (C3DSkeletalMorphTarget*)0x1234;
    tc.ForceSkeletalMorph(target);
    if (tc.Changed || morph.cleared != 1 || morph.applied != 1 || morph.valueAtApply != 1.0f) bad |= 1;
    if (!morph.added || morph.added->Target != target || morph.addedInfo->References != 1 || morph.addedInfo->Object != morph.added) bad |= 2;
    if (g_destroyCalls != 0) bad |= 4;                         // the morph still holds it
    morph.keep = false;
    tc.ForceSkeletalMorph(target);
    if (g_destroyCalls != 1 || g_destroyed != morph.added) bad |= 8;   // last reference dropped: the deleter runs
    tc.PSkeletalMorph.Data = 0;
    if (bad) { std::printf("007864c0_TEST FAIL\n"); return 1; }
    std::printf("007864c0_TEST PASS\n");
    return 0;
}