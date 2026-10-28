// Test: CArray<unsigned char>::DeallocateMemory -- when capacity is non-zero, clear and swap with an empty array (storage freed, all pointers null)
#include <cstdio>
#include <cstdlib>
template <class T> class CArray { public:
    T* start; T* finish; T* end_of_storage; int* frees;
    unsigned int capacity() const { return (unsigned int)(end_of_storage - start); }
    void DeallocateMemory()
    {
        if (capacity() > 0) {
            finish = start;                             // clear()
            T* old = start;                             // CArray<T>().swap(*this); the temporary frees the old block
            start = finish = end_of_storage = 0;
            if (old) { std::free(old); ++*frees; }
        }
    }
};
int main()
{
    int frees = 0;
    CArray<unsigned char> a; a.start = (unsigned char*)std::malloc(8); a.finish = a.start + 3; a.end_of_storage = a.start + 8; a.frees = &frees;
    a.DeallocateMemory();
    CArray<unsigned char> e; e.start = e.finish = e.end_of_storage = 0; e.frees = &frees;
    e.DeallocateMemory();
    if (a.start || a.finish || a.end_of_storage || frees != 1) { std::printf("009b4ae0_TEST FAIL\n"); return 1; }
    std::printf("009b4ae0_TEST PASS\n");
    return 0;
}