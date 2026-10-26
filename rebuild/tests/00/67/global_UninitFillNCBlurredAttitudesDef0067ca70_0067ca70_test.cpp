#include <cstdio>
#include <new>
#pragma pack(push, 1)
static int g_baseCopies = 0, g_mapCopies = 0, g_attCopies = 0;
class CDefClassBase { public: virtual void v00() { } unsigned char _body[0x21]; CDefClassBase() { } CDefClassBase(const CDefClassBase& o) { ++g_baseCopies; } };
struct CSubDefInfoVector { void* m_begin; void* m_end; void* m_capacity; CSubDefInfoVector() : m_begin(0), m_end(0), m_capacity(0) { } CSubDefInfoVector(const CSubDefInfoVector& o) : m_begin(o.m_begin), m_end(o.m_end), m_capacity(o.m_capacity) { ++g_mapCopies; } };
struct CKeyPairCompareLess { };
struct CSubDefInfoMap { CSubDefInfoVector m_entries; CKeyPairCompareLess m_compare; unsigned char m_sorted; CSubDefInfoMap() : m_sorted(0) { } CSubDefInfoMap(const CSubDefInfoMap& o) : m_entries(o.m_entries), m_compare(o.m_compare), m_sorted(o.m_sorted) { } };
class CParentDefClassBase : public CDefClassBase { public: virtual void v00() { } long InstantiationName; CSubDefInfoMap SubDefInfoMap; CParentDefClassBase() { } CParentDefClassBase(const CParentDefClassBase& o) : CDefClassBase(o), InstantiationName(o.InstantiationName), SubDefInfoMap(o.SubDefInfoMap) { } };
struct CAttitudeVector { void* m_begin; void* m_end; void* m_capacity; CAttitudeVector() : m_begin(0), m_end(0), m_capacity(0) { } CAttitudeVector(const CAttitudeVector& o) : m_begin(o.m_begin), m_end(o.m_end), m_capacity(o.m_capacity) { ++g_attCopies; } };
class CBlurredAttitudesDef : public CParentDefClassBase { public: virtual void v00() { } unsigned char _pad[0x3c - 0x37]; CAttitudeVector Attitudes; CBlurredAttitudesDef() { } CBlurredAttitudesDef(const CBlurredAttitudesDef& o) : CParentDefClassBase(o), Attitudes(o.Attitudes) { } };
#pragma pack(pop)
struct CRetailAllocator { };
CBlurredAttitudesDef* __fastcall UninitFillN(CBlurredAttitudesDef* first, unsigned long count, const CBlurredAttitudesDef& value, CRetailAllocator)
{ for (; 0 < count; --count, ++first) new (first) CBlurredAttitudesDef(value); return first; }
int main()
{
    CBlurredAttitudesDef value; value.InstantiationName = 0x55; value.SubDefInfoMap.m_sorted = 1; value.Attitudes.m_begin = (void*)0x10;
    unsigned char raw[3 * sizeof(CBlurredAttitudesDef)]; CBlurredAttitudesDef* dst = (CBlurredAttitudesDef*)raw;
    CBlurredAttitudesDef* end = UninitFillN(dst, 3, value, CRetailAllocator());
    if (sizeof(CBlurredAttitudesDef) != 0x48 || (char*)&value.Attitudes - (char*)&value != 0x3c || (char*)&value.SubDefInfoMap.m_sorted - (char*)&value != 0x36
        || end != dst + 3 || g_baseCopies != 3 || g_mapCopies != 3 || g_attCopies != 3 || dst[2].InstantiationName != 0x55 || dst[1].SubDefInfoMap.m_sorted != 1 || dst[0].Attitudes.m_begin != (void*)0x10
        || UninitFillN(dst, 0, value, CRetailAllocator()) != dst) { std::printf("0067ca70_TEST FAIL\n"); return 1; }
    std::printf("0067ca70_TEST PASS\n"); return 0;
}