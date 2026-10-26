// Behaviour test for 00a45410 (C3DSkeletalMorph::Reset): every morph parameter is released (erase(begin(), end())), the array keeps its storage with last == first, and the combined morph is released and nulled.
#include <cstdio>
static int g_deletedData = 0;
static int g_deletedInfos = 0;
class CCPPointerInfo { public:
    long RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
    static void operator delete(void* p) { ++g_deletedInfos; ::operator delete(p); }
};
static void DeleteData(void*) { ++g_deletedData; }
template<class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    void Clear()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
struct SMoveTag { SMoveTag() { } };
struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };
class C3DSkeletalMorphParameter { public: int id; };
class C3DSkeletalMorphTarget { public: int id; };
typedef CCountedPointer<C3DSkeletalMorphParameter> PMorphParameter;
PMorphParameter* _Move(PMorphParameter* first, PMorphParameter* last, PMorphParameter* dest, const SMoveTag&, bool)   // stands in for 0x00A46460
{
    for (; first != last; ++first, ++dest)
        *dest = *first;
    return dest;
}
void DestroyRange(PMorphParameter* first, PMorphParameter* last, const SNonscalarPtrIteratorTag&)   // stands in for 0x00A464E0
{
    for (; first != last; ++first)
        first->Clear();
}
template<class T> struct CRetailArray {
    T* first; T* last; T* end_of_storage;
    void clear()
    {
        T* l = last;
        T* f = first;
        T* kept = _Move(l, l, f, SMoveTag(), false);
        DestroyRange(kept, last, SNonscalarPtrIteratorTag());
        last = kept;
    }
};
struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable() { }
};
class C3DSkeletalMorph : public CBaseClassNonCopyable { public:
    CRetailArray<PMorphParameter>           MorphParameters;
    CCountedPointer<C3DSkeletalMorphTarget> CombinedMorph;
    void Reset();
};
void C3DSkeletalMorph::Reset()
{
    MorphParameters.clear();
    CombinedMorph.Clear();
}
static CCPPointerInfo* NewInfo(long refs)
{
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = refs; info->DeleteFunc = DeleteData; info->Data = 0;
    return info;
}
int main()
{
    PMorphParameter params[3];
    CCPPointerInfo shared;
    shared.RefCount = 5; shared.DeleteFunc = DeleteData; shared.Data = 0;
    params[0].Data = 0; params[0].Info = NewInfo(1);
    params[1].Data = 0; params[1].Info = &shared;
    params[2].Data = 0; params[2].Info = NewInfo(1);
    C3DSkeletalMorph morph;
    morph.MorphParameters.first = params;
    morph.MorphParameters.last = params + 3;
    morph.MorphParameters.end_of_storage = params + 3;
    morph.CombinedMorph.Data = 0;
    morph.CombinedMorph.Info = NewInfo(1);
    morph.Reset();
    if (morph.MorphParameters.first != params || morph.MorphParameters.last != params || morph.MorphParameters.end_of_storage != params + 3) { std::printf("00a45410_TEST FAIL\n"); return 1; }
    if (g_deletedData != 3 || g_deletedInfos != 3 || shared.RefCount != 4) { std::printf("00a45410_TEST FAIL\n"); return 1; }
    if (morph.CombinedMorph.Info != 0 || morph.CombinedMorph.Data != 0 || params[1].Info != 0) { std::printf("00a45410_TEST FAIL\n"); return 1; }
    std::printf("00a45410_TEST PASS\n");
    return 0;
}