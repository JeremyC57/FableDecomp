#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
class CDefString { public: long m_index; };
struct CSkeletalMorph { long GroupID; CDefString Name; bool AllowVariation; };
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<long, CSkeletalMorph> SSkeletalMorphEntry;
typedef SSkeletalMorphEntry TElem;
static void Fill(TElem& e, int i)
{
    e.first = 3 * i; e.second.GroupID = i + 20; e.second.Name.m_index = 77 * i; e.second.AllowVariation = (i & 1) != 0;
}
static bool Check(const TElem& e, int i)
{
    return e.first == 3 * i && e.second.GroupID == i + 20 && e.second.Name.m_index == 77 * i && e.second.AllowVariation == ((i & 1) != 0);
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    
    if (!ok) { std::printf("00673ca0_TEST FAIL\n"); return 1; }
    std::printf("00673ca0_TEST PASS\n");
    return 0;
}