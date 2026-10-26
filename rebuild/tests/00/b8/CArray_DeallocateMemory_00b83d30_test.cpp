#include <cstdio>
#include <cstdlib>
// Behaviour test for CArray<CCountedPointer<CParticleSimulation> >::DeallocateMemory() @ 0x00B83D30: with capacity, every
// element is destroyed once (its reference dropped), the block freed and the array left with no storage; with no
// capacity nothing happens.
static int g_destroyed = 0;
static int g_freed = 0;
static void FreeBlock(void* p) { if (p) { ++g_freed; free(p); } }
class CParticleSimulation;
class CCPPointerInfo { public: long RefCount; };
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer() { ++g_destroyed; if (Info) --Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (Info != other.Info) {
            if (Info) --Info->RefCount;
            Data = other.Data; Info = other.Info;
            if (Info) ++Info->RefCount;
        }
        return *this;
    }
};
template <class _Tp> void Destroy(_Tp* first, _Tp* last) { for ( ; first != last; ++first) first->~_Tp(); }
template <class _Tp> _Tp* Copy(_Tp* first, _Tp* last, _Tp* result) { for ( ; first != last; ++first, ++result) *result = *first; return result; }
template <class _Tp> void Swap(_Tp& a, _Tp& b) { _Tp t = a; a = b; b = t; }
template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { Destroy(_M_start, _M_finish); if (_M_start != 0) FreeBlock(_M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void swap(vector& x) { Swap(_M_start, x._M_start); Swap(_M_finish, x._M_finish); Swap(_M_end_of_storage, x._M_end_of_storage); }
    _Tp* erase(_Tp* first, _Tp* last)
    {
        _Tp* i = Copy(last, _M_finish, first);
        Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
    void clear() { erase(_M_start, _M_finish); }
};
template <class T> class CArray : public vector<T> {
public:
    void DeallocateMemory()
    {
        if (this->capacity() > 0) {
            this->clear();
            CArray<T>().swap(*this);
        }
    }
};
typedef CCountedPointer<CParticleSimulation> CPtr;
int main()
{
    CCPPointerInfo block = { 4 };
    {
        CArray<CPtr> a;
        a._M_start = (CPtr*)malloc(6 * sizeof(CPtr));
        for (int i = 0; i < 3; ++i) { a._M_start[i].Data = 0; a._M_start[i].Info = &block; }
        a._M_finish = a._M_start + 3;
        a._M_end_of_storage = a._M_start + 6;
        a.DeallocateMemory();
        if (g_destroyed != 3 || g_freed != 1 || block.RefCount != 1) { std::printf("00b83d30_TEST FAIL\n"); return 1; }
        if (a._M_start != 0 || a._M_finish != 0 || a._M_end_of_storage != 0 || a.capacity() != 0) { std::printf("00b83d30_TEST FAIL\n"); return 1; }
        a.DeallocateMemory();                           // no capacity: nothing
        if (g_destroyed != 3 || g_freed != 1) { std::printf("00b83d30_TEST FAIL\n"); return 1; }
    }
    if (g_freed != 1 || g_destroyed != 3) { std::printf("00b83d30_TEST FAIL\n"); return 1; }
    {
        CArray<CPtr> e;                                 // capacity without elements: block freed
        e._M_start = (CPtr*)malloc(2 * sizeof(CPtr));
        e._M_finish = e._M_start; e._M_end_of_storage = e._M_start + 2;
        e.DeallocateMemory();
        if (g_freed != 2 || g_destroyed != 3 || e._M_start != 0) { std::printf("00b83d30_TEST FAIL\n"); return 1; }
    }
    std::printf("00b83d30_TEST PASS\n");
    return 0;
}