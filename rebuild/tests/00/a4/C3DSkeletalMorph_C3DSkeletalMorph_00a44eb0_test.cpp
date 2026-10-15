// Behaviour test for 0x00A44EB0 C3DSkeletalMorph::~C3DSkeletalMorph
#include <cstdio>
#include <cstdlib>
struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void*); void* Data; };
static int g_deleted = 0, g_destroyedRange = 0, g_freed = 0, g_baseDtor = 0;
static void DeleteData(void*) { ++g_deleted; }
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info != 0) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); std::free(Info); } }
        Data = 0; Info = 0;
    }
    T* operator->() const { return Data; }
};
struct C3DSkeletalMorph;
struct C3DSkeletalMorphParameter { float CurrentValue; C3DSkeletalMorph* SkeletalMorph; };
template<class T> struct CRetailArray {
    T* first; T* last; T* end_of_storage;
    unsigned long Size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long i) { return first[i]; }
    ~CRetailArray() { g_destroyedRange += (int)(last - first); if (first != 0) { ++g_freed; std::free(first); } }
};
struct CBase { ~CBase() { ++g_baseDtor; } };
struct C3DSkeletalMorph : CBase {
    CRetailArray<CCountedPointer<C3DSkeletalMorphParameter> > MorphParameters;
    CCountedPointer<int> CombinedMorph;
    ~C3DSkeletalMorph() { for (unsigned long i = 0; i < MorphParameters.Size(); ++i) MorphParameters[i]->SkeletalMorph = 0; }
};
int main()
{
    C3DSkeletalMorphParameter p0, p1;
    {
        C3DSkeletalMorph* m = new C3DSkeletalMorph;
        p0.SkeletalMorph = m; p1.SkeletalMorph = m;
        typedef CCountedPointer<C3DSkeletalMorphParameter> P;
        P* arr = (P*)std::malloc(2 * sizeof(P));
        arr[0].Data = &p0; arr[0].Info = 0; arr[1].Data = &p1; arr[1].Info = 0;
        m->MorphParameters.first = arr; m->MorphParameters.last = arr + 2; m->MorphParameters.end_of_storage = arr + 2;
        int target = 0;
        CCPPointerInfo* info = (CCPPointerInfo*)std::malloc(sizeof(CCPPointerInfo)); info->RefCount = 1; info->DeleteFunc = DeleteData; info->Data = &target;
        m->CombinedMorph.Data = &target; m->CombinedMorph.Info = info;
        delete m;
    }
    bool ok = p0.SkeletalMorph == 0 && p1.SkeletalMorph == 0 && g_deleted == 1 && g_destroyedRange == 2 && g_freed == 1 && g_baseDtor == 1;
    if (!ok) { std::printf("00a44eb0_TEST FAIL\n"); return 1; }
    std::printf("00a44eb0_TEST PASS\n");
    return 0;
}