#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Model: the 12-byte STLport vector (CArray) of skeletal-morph entries {long, CCharString, float}.
static int g_liveStrings = 0;

struct CCharString {
    const char* PStringData;
    CCharString(const char* t) : PStringData(t) { ++g_liveStrings; }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};

struct __false_type { __false_type() { } };

static int g_overflows = 0;

template <class T> struct CArray {
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

class CTCSkeletalMorph {
public:
    bool                   Changed;
    CArray<CSkeletalMorph> Morphs;

    void AddSkeletalMorph(long type, const CCharString& name, float value)
    {
        Morphs.push_back(CSkeletalMorph(type, name, value));
        Changed = true;
    }
};

int main()
{
    CTCSkeletalMorph c;
    c.Changed = false;
    {
        CCharString fat("Fat"), old("Old"), thin("Thin");
        c.AddSkeletalMorph(3, fat, 0.5f);
        c.AddSkeletalMorph(7, old, 1.0f);
        c.AddSkeletalMorph(-1, thin, 0.0f);
    }
    bool ok = c.Changed && (c.Morphs._M_finish - c.Morphs._M_start) == 3 && g_liveStrings == 3
              && c.Morphs._M_start[0].Type == 3 && std::strcmp(c.Morphs._M_start[0].Name.PStringData, "Fat") == 0
              && c.Morphs._M_start[0].Value == 0.5f && c.Morphs._M_start[1].Type == 7 && c.Morphs._M_start[1].Value == 1.0f
              && c.Morphs._M_start[2].Type == -1 && std::strcmp(c.Morphs._M_start[2].Name.PStringData, "Thin") == 0;
    c.Morphs.destroy();
    if (!ok || g_liveStrings != 0) {
        std::printf("00786670_TEST FAIL\n");
        return 1;
    }
    std::printf("00786670_TEST PASS\n");
    return 0;
}