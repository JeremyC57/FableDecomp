// Behaviour test for 0x00A455A0 C3DSkeletalMorphParameter::C3DSkeletalMorphParameter(const C3DSkeletalMorphTarget&)
#include <cstdio>
#include <cstring>
class CCharString { public: char t[16]; CCharString() { t[0] = 0; } CCharString& operator=(const CCharString& o) { std::strcpy(t, o.t); return *this; } };
template<class T> class CArray { public:
    T* b; T* e; T* c;
    CArray() : b(0), e(0), c(0) {}
    CArray& operator=(const CArray& o) { b = o.b; e = o.e; c = o.c; return *this; }
};
class C3DSkeletalMorphTarget { public:
    CCharString Name; CArray<int> BoneMorphs;
    C3DSkeletalMorphTarget& operator=(const C3DSkeletalMorphTarget& o) { Name = o.Name; BoneMorphs = o.BoneMorphs; return *this; }
};
template<class T> class CCountedPointer { public:
    T* Data; void* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer() { delete Data; }
    void Reset(T* o) { delete Data; Data = o; }
    T* operator->() const { return Data; }
};
static int g_base = 0;
struct CBase { CBase() { ++g_base; } virtual ~CBase() {} };
class C3DSkeletalMorphParameter : public CBase { public:
    class CShared { public: C3DSkeletalMorphTarget PositiveTarget, NegativeTarget; bool SignedFlag; };
    float CurrentValue; void* SkeletalMorph; CCountedPointer<CShared> SharedData;
    C3DSkeletalMorphParameter(const C3DSkeletalMorphTarget& target) : CBase()
    {
        SharedData.Reset(new CShared);
        SharedData->PositiveTarget = target;
        SharedData->SignedFlag = false;
        CurrentValue = 0.0f;
        SkeletalMorph = 0;
    }
};
int main()
{
    int bones[2];
    C3DSkeletalMorphTarget t; std::strcpy(t.Name.t, "SMILE"); t.BoneMorphs.b = bones; t.BoneMorphs.e = bones + 2; t.BoneMorphs.c = bones + 2;
    C3DSkeletalMorphParameter p(t);
    bool ok = g_base == 1 && p.SharedData.Data != 0 && std::strcmp(p.SharedData->PositiveTarget.Name.t, "SMILE") == 0
        && p.SharedData->PositiveTarget.BoneMorphs.e == bones + 2 && p.SharedData->NegativeTarget.Name.t[0] == 0
        && p.SharedData->NegativeTarget.BoneMorphs.b == 0 && !p.SharedData->SignedFlag && p.CurrentValue == 0.0f && p.SkeletalMorph == 0;
    if (!ok) { std::printf("00a455a0_TEST FAIL\n"); return 1; }
    std::printf("00a455a0_TEST PASS\n");
    return 0;
}