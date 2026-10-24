// Behaviour test for 0x00519800 vector<CMapInfo>::reserve: a request within capacity changes nothing; an empty vector just
// gets an n-slot block (size stays 0); a populated one moves its records (allocate-and-copy) into an n-slot block, destroys
// the old records once and frees the old block, keeping size and setting capacity n.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __int64 long long
#endif
static int g_strdtors, g_destroys, g_frees;
class CCharString {
public:
    const char* PString;
    ~CCharString() { ++g_strdtors; }
};
class C2DBoxI {
public:
    long X0, Y0, X1, Y1;
};
class CMap;
template <class T> class CCountedPointer {
public:
    T*    Object;                              // +0x0
    void* Block;                               // +0x4 (count block)
    ~CCountedPointer() {}
};
class CMapInfo {                               // Ego_r.pdb, 72 bytes
public:
    C2DBoxI               Area;                // +0x00
    CCountedPointer<CMap> PMap;                // +0x10
    CCharString           LevelName;           // +0x18
    CCharString           LevelScriptName;     // +0x1C
    unsigned long         MapUID;              // +0x20
    bool                  Used;                // +0x24
    bool                  Placed;              // +0x25
    bool                  Activated;           // +0x26
    bool                  Loading;             // +0x27
    bool                  LoadedOnPlayerProximity;   // +0x28
    CCharString           SerialiseScript;     // +0x2C
    unsigned __int64      LevStamp;            // +0x30
    unsigned __int64      TngStamp;            // +0x38
    bool                  IsSea;               // +0x40
};

struct __false_type { __false_type() {} };

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator first, _ForwardIterator last, const __false_type&)
{
    ++g_destroys;
    for (; first != last; ++first) first->~CMapInfo();
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator first, _ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) { ++g_frees; std::free(p); } }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
};
template <class T> class vector : public _Vector_base<T> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIterator>
    T* _M_allocate_and_copy(unsigned int n, _ForwardIterator first, _ForwardIterator last)
    {
        T* r = this->_M_end_of_storage.allocate(n);
        T* c = r;
        for (; first != last; ++first, ++c) std::memcpy((void*)c, (const void*)first, sizeof(T));
        return r;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    void reserve(unsigned int n);
};

template <class T> void vector<T>::reserve(unsigned int n)
{
    if (capacity() < n) {
        const unsigned int old_size = size();
        T* tmp;
        if (this->_M_start) {
            tmp = _M_allocate_and_copy(n, this->_M_start, this->_M_finish);
            _M_clear();
        }
        else {
            tmp = this->_M_end_of_storage.allocate(n);
        }
        _M_set(tmp, tmp + old_size, tmp + n);
    }
}


typedef vector<CMapInfo> V;
int main()
{
    V v; v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;
    v.reserve(0);
    int ok = v._M_start == 0 && g_destroys == 0;
    v.reserve(3);
    ok &= v._M_start != 0 && v._M_finish == v._M_start && v._M_end_of_storage._M_data == v._M_start + 3 && g_destroys == 0 && g_frees == 0;
    v._M_start[0].MapUID = 11; v._M_start[1].MapUID = 22; v._M_finish = v._M_start + 2;
    CMapInfo* old = v._M_start;
    v.reserve(2);
    ok &= v._M_start == old && g_destroys == 0;
    v.reserve(8);
    ok &= v._M_start != old && v._M_finish == v._M_start + 2 && v._M_end_of_storage._M_data == v._M_start + 8;
    ok &= v._M_start[0].MapUID == 11 && v._M_start[1].MapUID == 22 && g_destroys == 1 && g_frees == 1 && g_strdtors == 6;
    std::free(v._M_start);
    if (!ok) { std::printf("00519800_TEST FAIL\n"); return 1; }
    std::printf("00519800_TEST PASS\n");
    return 0;
}