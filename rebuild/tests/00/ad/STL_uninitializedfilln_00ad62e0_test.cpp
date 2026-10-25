// Behaviour test for _STL::__uninitialized_fill_n(CUniqueVertex*, unsigned int, const CUniqueVertex&, const __false_type&)
// (0x00AD62E0): n copies of x are constructed in raw storage (the flag and both index sets copied), the end pointer
// first + n is returned, and n = 0 constructs nothing and returns first.
#include <cstdio>
#include <cstdlib>
#include <new>

static int gSetCopies = 0;

namespace _STL {
struct __false_type { __false_type() { } };

template <class _Key> class set {                   // deep-copying stand-in for set<unsigned long>
public:
    _Key* Items;
    unsigned int Count;
    set() : Items(0), Count(0) { }
    set(const set& __x) : Items(0), Count(__x.Count)
    {
        ++gSetCopies;
        if (Count) {
            Items = (_Key*)std::malloc(Count * sizeof(_Key));
            for (unsigned int i = 0; i < Count; ++i)
                Items[i] = __x.Items[i];
        }
    }
    ~set() { std::free(Items); }
    void Add(_Key k)
    {
        _Key* n = (_Key*)std::malloc((Count + 1) * sizeof(_Key));
        for (unsigned int i = 0; i < Count; ++i)
            n[i] = Items[i];
        n[Count++] = k;
        std::free(Items);
        Items = n;
    }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

namespace QuadricOptimiserInternals {
class CIndexList {
public:
    _STL::set<unsigned long> Indices;
};
class CUniqueVertex {
public:
    bool       Exists;
    CIndexList VertexIndices;
    CIndexList FaceList;
};
}

int main()
{
    using QuadricOptimiserInternals::CUniqueVertex;
    int fails = 0;
    {
        CUniqueVertex x;
        x.Exists = true;
        x.VertexIndices.Indices.Add(4u);
        x.VertexIndices.Indices.Add(9u);
        x.FaceList.Indices.Add(2u);
        static double storage[64];
        CUniqueVertex* first = (CUniqueVertex*)storage;
        CUniqueVertex* end = _STL::__uninitialized_fill_n(first, 3u, x, _STL::__false_type());
        if (end != first + 3) ++fails;
        if (gSetCopies != 6) ++fails;
        for (int i = 0; i < 3; ++i) {
            if (!first[i].Exists) ++fails;
            if (first[i].VertexIndices.Indices.Count != 2u || first[i].VertexIndices.Indices.Items[1] != 9u) ++fails;
            if (first[i].FaceList.Indices.Count != 1u || first[i].FaceList.Indices.Items[0] != 2u) ++fails;
            if (first[i].FaceList.Indices.Items == x.FaceList.Indices.Items) ++fails;
        }
        CUniqueVertex* none = _STL::__uninitialized_fill_n(first + 3, 0u, x, _STL::__false_type());
        if (none != first + 3 || gSetCopies != 6) ++fails;
        for (int i = 0; i < 3; ++i)
            first[i].~CUniqueVertex();
    }
    if (fails) { std::printf("00ad62e0_TEST FAIL\n"); return 1; }
    std::printf("00ad62e0_TEST PASS\n");
    return 0;
}