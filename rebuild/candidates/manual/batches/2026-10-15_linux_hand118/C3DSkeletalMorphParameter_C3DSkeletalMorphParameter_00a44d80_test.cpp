// Behaviour test for 0x00A44D80 C3DSkeletalMorphParameter copy constructor
#include <cstdio>
#include <cstdlib>
struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void*); void* Data; };
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* data = o.Data; CCPPointerInfo* info = o.Info;
        if (Info != info) {
            if (Info != 0) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); std::free(Info); } }
            Data = data; Info = info;
            if (info != 0) ++info->RefCount;
        }
        return *this;
    }
};
static int g_base = 0;
struct CBase { CBase() { ++g_base; } virtual ~CBase() {} };
struct C3DSkeletalMorphParameter : CBase {
    float CurrentValue; void* SkeletalMorph; CCountedPointer<int> SharedData;
    C3DSkeletalMorphParameter() {}
    C3DSkeletalMorphParameter(const C3DSkeletalMorphParameter& o) : CBase() { SharedData = o.SharedData; CurrentValue = o.CurrentValue; SkeletalMorph = 0; }
};
int main()
{
    int shared = 5; CCPPointerInfo info = { 1, 0, &shared };
    C3DSkeletalMorphParameter a; a.CurrentValue = 0.25f; a.SkeletalMorph = &a; a.SharedData.Data = &shared; a.SharedData.Info = &info;
    C3DSkeletalMorphParameter b(a);
    bool ok = g_base == 2 && b.CurrentValue == 0.25f && b.SkeletalMorph == 0 && b.SharedData.Data == &shared && b.SharedData.Info == &info && info.RefCount == 2;
    C3DSkeletalMorphParameter e; e.CurrentValue = 1.0f; e.SkeletalMorph = 0;
    C3DSkeletalMorphParameter f(e);
    ok = ok && f.SharedData.Info == 0 && f.SharedData.Data == 0 && f.CurrentValue == 1.0f;
    if (!ok) { std::printf("00a44d80_TEST FAIL\n"); return 1; }
    std::printf("00a44d80_TEST PASS\n");
    return 0;
}
