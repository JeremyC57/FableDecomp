// Behaviour test for 0x00515AD0 std::vector<CMapInfo>(n): the default record is built (counted pointer null, Clear()
// run), copied into n slots through the copy constructor, then destroyed exactly once; finish == end == start + n.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_clears = 0, g_copies = 0, g_dtors = 0;

struct MapInfoModel {
    void*         PMapObject;
    void*         PMapInfo;
    unsigned long MapUID;
    bool          Activated;
    bool          IsSea;
    MapInfoModel() : PMapObject(0), PMapInfo(0) { Clear(); }
    MapInfoModel(const MapInfoModel& o) : PMapObject(o.PMapObject), PMapInfo(o.PMapInfo), MapUID(o.MapUID), Activated(o.Activated), IsSea(o.IsSea) { ++g_copies; }
    ~MapInfoModel() { ++g_dtors; }
    void Clear() { ++g_clears; MapUID = 0; Activated = true; IsSea = false; }
};

template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n_model(ForwardIter first, Size n, const T& x)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (&*cur) T(x);
    return cur;
}

struct vector_model {
    MapInfoModel* _M_start;
    MapInfoModel* _M_finish;
    MapInfoModel* _M_end_of_storage;
    explicit vector_model(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (MapInfoModel*)std::malloc(n * sizeof(MapInfoModel)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = uninitialized_fill_n_model(_M_start, n, MapInfoModel());
    }
};

int main()
{
    vector_model v(5);
    if (g_clears != 1 || g_copies != 5 || g_dtors != 1) { std::printf("00515ad0_TEST FAIL\n"); return 1; }
    if (v._M_finish != v._M_start + 5 || v._M_end_of_storage != v._M_start + 5) { std::printf("00515ad0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (v._M_start[i].PMapInfo != 0 || v._M_start[i].Activated != true || v._M_start[i].MapUID != 0) { std::printf("00515ad0_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    vector_model e(0);
    if (e._M_start != 0 || e._M_finish != 0 || g_copies != 5 || g_dtors != 2) { std::printf("00515ad0_TEST FAIL\n"); return 1; }
    std::printf("00515ad0_TEST PASS\n");
    return 0;
}