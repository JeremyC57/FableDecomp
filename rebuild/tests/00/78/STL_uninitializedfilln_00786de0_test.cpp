#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

// Model: STLport __uninitialized_fill_n over the 12-byte skeletal-morph entry.
static int g_copies = 0;

struct CCharString {
    const char* PStringData;
    CCharString(const char* t) : PStringData(t) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
};

struct CSkeletalMorph {
    long        Type;
    CCharString Name;
    float       Value;
    CSkeletalMorph(long t, const char* n, float v) : Type(t), Name(n), Value(v) { }
    CSkeletalMorph(const CSkeletalMorph& o) : Type(o.Type), Name(o.Name), Value(o.Value) { }
};

struct __false_type { __false_type() { } };

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

static CSkeletalMorph* __fastcall __uninitialized_fill_n(CSkeletalMorph* first, unsigned int n, const CSkeletalMorph& x,
                                                         const __false_type&)
{
    CSkeletalMorph* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

int main()
{
    CSkeletalMorph x(42, "Muscle", 0.75f);
    CSkeletalMorph* mem = (CSkeletalMorph*)std::malloc(6 * sizeof(CSkeletalMorph));
    std::memset(mem, 0xAB, 6 * sizeof(CSkeletalMorph));
    CSkeletalMorph* end = __uninitialized_fill_n(mem, 4, x, __false_type());
    bool ok = end == mem + 4 && g_copies == 4;
    for (int i = 0; ok && i < 4; ++i)
        ok = mem[i].Type == 42 && std::strcmp(mem[i].Name.PStringData, "Muscle") == 0 && mem[i].Value == 0.75f;
    ok = ok && *(unsigned char*)&mem[4] == 0xAB;        // nothing written past n
    CSkeletalMorph* none = __uninitialized_fill_n(mem + 5, 0, x, __false_type());
    ok = ok && none == mem + 5 && g_copies == 4;
    std::free(mem);
    if (!ok) {
        std::printf("00786de0_TEST FAIL\n");
        return 1;
    }
    std::printf("00786de0_TEST PASS\n");
    return 0;
}