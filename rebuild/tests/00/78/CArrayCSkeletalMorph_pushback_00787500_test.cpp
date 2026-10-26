#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Model: the 12-byte STLport vector (CArray) of skeletal-morph entries {long, CCharString, float}.
static int g_liveStrings = 0;

class CCharString { public:
    const char* PStringData;
    CCharString(const char* t) : PStringData(t) { ++g_liveStrings; }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};

struct __false_type { __false_type() { } };

static int g_overflows = 0;

template <class T> class CArray { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    // stand-in for the growth path: reallocate to twice the size and append fill_len copies of x
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        ++g_overflows;
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = size ? size * 2 : 1;
        T* mem = (T*)std::malloc(cap * sizeof(T));
        T* d = mem;
        for (T* s = _M_start; s != position; ++s, ++d) {
            new (d) T(*s);
            s->~T();
        }
        for (unsigned int i = 0; i < fill_len; ++i, ++d)
            new (d) T(x);
        std::free(_M_start);
        _M_start = mem;
        _M_finish = d;
        _M_end_of_storage = mem + cap;
        (void)atend;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    void destroy()
    {
        for (T* p = _M_start; p != _M_finish; ++p)
            p->~T();
        std::free(_M_start);
        _M_start = _M_finish = _M_end_of_storage = 0;
    }
};

struct CSkeletalMorph {
    long        Type;
    CCharString Name;
    float       Value;
    CSkeletalMorph(long type, const CCharString& name, float value) : Type(type), Name(name), Value(value) { }
    CSkeletalMorph(const CSkeletalMorph& o) : Type(o.Type), Name(o.Name), Value(o.Value) { }
};

int main()
{
    CArray<CSkeletalMorph> morphs;
    const char* names[5] = { "Fat", "Thin", "Old", "Evil", "Good" };
    {
        for (int i = 0; i < 5; ++i) {
            CSkeletalMorph m(i * 10, CCharString(names[i]), 0.25f * (float)i);
            morphs.push_back(m);
        }
    }
    bool ok = (morphs._M_finish - morphs._M_start) == 5 && g_overflows == 4 && g_liveStrings == 5;
    for (int i = 0; ok && i < 5; ++i) {
        const CSkeletalMorph& m = morphs._M_start[i];
        ok = m.Type == i * 10 && std::strcmp(m.Name.PStringData, names[i]) == 0 && m.Value == 0.25f * (float)i;
    }
    morphs.destroy();
    if (!ok || g_liveStrings != 0) {
        std::printf("00787500_TEST FAIL\n");
        return 1;
    }
    std::printf("00787500_TEST PASS\n");
    return 0;
}