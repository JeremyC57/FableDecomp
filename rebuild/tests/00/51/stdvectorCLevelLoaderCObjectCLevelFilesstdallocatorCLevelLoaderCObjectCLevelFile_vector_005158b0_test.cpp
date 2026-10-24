// Behaviour test for 0x005158B0 std::vector<CLevelLoader::CObject::CLevelFiles>(n): n records copied from a default
// record (all three counted pointers null), the default record destroyed once, finish == end == start + n.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_destroyed = 0;

struct CountedPointerModel {
    void* Object;
    void* Info;
    CountedPointerModel() : Object(0), Info(0) { }
};

struct CLevelFilesModel {
    long                LevelID;
    CountedPointerModel PTopologyFile;
    CountedPointerModel PNavigationFile;
    CountedPointerModel PThingFile;
    ~CLevelFilesModel() { ++g_destroyed; }
};

struct false_type_model { false_type_model() { } };

template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n_model(ForwardIter first, Size n, const T& x, const false_type_model&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (&*cur) T(x);
    return cur;
}

struct vector_model {
    CLevelFilesModel* _M_start;
    CLevelFilesModel* _M_finish;
    CLevelFilesModel* _M_end_of_storage;
    explicit vector_model(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (CLevelFilesModel*)std::malloc(n * sizeof(CLevelFilesModel)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = uninitialized_fill_n_model(_M_start, n, CLevelFilesModel(), false_type_model());
    }
};

int main()
{
    vector_model v(3);
    if (g_destroyed != 1) { std::printf("005158b0_TEST FAIL\n"); return 1; }
    if (v._M_start == 0 || v._M_finish != v._M_start + 3 || v._M_end_of_storage != v._M_start + 3) { std::printf("005158b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (v._M_start[i].PTopologyFile.Object != 0 || v._M_start[i].PNavigationFile.Info != 0 || v._M_start[i].PThingFile.Object != 0) { std::printf("005158b0_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    vector_model e(0);
    if (e._M_start != 0 || e._M_finish != 0 || g_destroyed != 2) { std::printf("005158b0_TEST FAIL\n"); return 1; }
    std::printf("005158b0_TEST PASS\n");
    return 0;
}