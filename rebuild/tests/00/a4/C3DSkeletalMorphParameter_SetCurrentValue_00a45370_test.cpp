// Behaviour test for 0x00A45370 C3DSkeletalMorphParameter::SetCurrentValue
#include <cstdio>
#include <cstdlib>
class CCPPointerInfo { public: long RefCount; void (*DeleteFunc)(void*); void* Data; };
static int g_deleted = 0;
static void DeleteData(void*) { ++g_deleted; }
template<class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    void Clear()
    {
        if (Info != 0) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); std::free(Info); } }
        Info = 0; Data = 0;
    }
};
class C3DSkeletalMorph { public: CCountedPointer<int> CombinedMorph; };
struct CShared { bool SignedFlag; };
class C3DSkeletalMorphParameter { public:
    float CurrentValue; C3DSkeletalMorph* SkeletalMorph; CCountedPointer<CShared> SharedData;
    float GetMinimumValue() const { return SharedData.Data->SignedFlag ? -1.0f : 0.0f; }
    void SetCurrentValue(float value)
    {
        CurrentValue = value;
        if (value < GetMinimumValue()) CurrentValue = GetMinimumValue();
        if (CurrentValue > 1.0f) CurrentValue = 1.0f;
        if (SkeletalMorph) SkeletalMorph->CombinedMorph.Clear();
    }
};
int main()
{
    CShared unsignedShared = { false }, signedShared = { true };
    int target = 0;
    CCPPointerInfo* info = (CCPPointerInfo*)std::malloc(sizeof(CCPPointerInfo)); info->RefCount = 1; info->DeleteFunc = DeleteData; info->Data = &target;
    C3DSkeletalMorph morph; morph.CombinedMorph.Data = &target; morph.CombinedMorph.Info = info;
    C3DSkeletalMorphParameter p; p.SkeletalMorph = &morph; p.SharedData.Data = &unsignedShared; p.SharedData.Info = 0;
    p.SetCurrentValue(-0.5f);
    bool ok = p.CurrentValue == 0.0f && g_deleted == 1 && morph.CombinedMorph.Data == 0 && morph.CombinedMorph.Info == 0;
    p.SetCurrentValue(2.0f); ok = ok && p.CurrentValue == 1.0f;
    p.SetCurrentValue(0.5f); ok = ok && p.CurrentValue == 0.5f;
    p.SharedData.Data = &signedShared; p.SkeletalMorph = 0;
    p.SetCurrentValue(-0.5f); ok = ok && p.CurrentValue == -0.5f;
    p.SetCurrentValue(-3.0f); ok = ok && p.CurrentValue == -1.0f && g_deleted == 1;
    if (!ok) { std::printf("00a45370_TEST FAIL\n"); return 1; }
    std::printf("00a45370_TEST PASS\n");
    return 0;
}