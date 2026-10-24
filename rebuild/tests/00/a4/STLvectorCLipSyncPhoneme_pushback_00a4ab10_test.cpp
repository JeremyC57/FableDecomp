// Behaviour test for 0x00A4AB10: _STL::vector<CLipSyncPhoneme>::push_back (placement copy, or the grow path when full)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

static int g_liveStrings = 0;
class CCharString {                                   // test model: a deep-copying string with a live count
public:
    char* m_pData;
    CCharString() : m_pData(0) { ++g_liveStrings; }
    CCharString(const char* s) : m_pData(Dup(s)) { ++g_liveStrings; }
    CCharString(const CCharString& other) : m_pData(Dup(other.m_pData)) { ++g_liveStrings; }
    ~CCharString() { free(m_pData); --g_liveStrings; }
    CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* p = Dup(other.m_pData);
            free(m_pData);
            m_pData = p;
        }
        return *this;
    }
    const char* c_str() const { return m_pData ? m_pData : ""; }
    bool operator<(const CCharString& o) const { return strcmp(c_str(), o.c_str()) < 0; }
    static char* Dup(const char* s)
    {
        if (s == 0) return 0;
        char* p = (char*)malloc(strlen(s) + 1);
        strcpy(p, s);
        return p;
    }
};
static bool Eq(const CCharString& s, const char* t) { return strcmp(s.c_str(), t) == 0; }

class CLipSyncPhoneme {
public:
    CCharString   Symbol;
    unsigned long ID;
    float         Weight;
    CLipSyncPhoneme() : ID(0), Weight(0.0f) { }
    CLipSyncPhoneme(const char* s, unsigned long id, float w) : Symbol(s), ID(id), Weight(w) { }
};
static bool Same(const CLipSyncPhoneme& p, const char* s, unsigned long id, float w)
{
    return Eq(p.Symbol, s) && p.ID == id && Near(p.Weight, w);
}

namespace _STL {
struct __false_type { __false_type() { } };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp>
class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
static int g_overflows = 0;
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() { this->_M_start = this->_M_finish = this->_M_end_of_storage._M_data = 0; }
    ~vector()
    {
        for (_Tp* p = this->_M_start; p != this->_M_finish; ++p)
            p->~_Tp();
        free(this->_M_start);
    }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    // test stand-in for the out-of-line grow (the retail COMDAT): double the storage, copy, append x
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
    {
        ++g_overflows;
        unsigned int old = size();
        unsigned int len = old + (old > __fill_len ? old : __fill_len);
        _Tp* ns = (_Tp*)malloc(len * sizeof(_Tp));
        _Tp* nf = ns;
        for (_Tp* p = this->_M_start; p != __position; ++p, ++nf)
            _Construct(nf, *p);
        for (unsigned int i = 0; i < __fill_len; ++i, ++nf)
            _Construct(nf, __x);
        if (!__atend)
            for (_Tp* q = __position; q != this->_M_finish; ++q, ++nf)
                _Construct(nf, *q);
        for (_Tp* d = this->_M_start; d != this->_M_finish; ++d)
            d->~_Tp();
        free(this->_M_start);
        this->_M_start = ns;
        this->_M_finish = nf;
        this->_M_end_of_storage._M_data = ns + len;
    }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

int main()
{
    {
        _STL::vector<CLipSyncPhoneme> v;
        CLipSyncPhoneme a("AA", 1, 0.25f), b("EE", 2, 0.5f), c("OO", 3, 0.75f);
        v.push_back(a);
        Check(_STL::g_overflows == 1 && v.size() == 1);
        v.push_back(b);
        Check(_STL::g_overflows == 2 && v.size() == 2);
        v.push_back(c);
        Check(_STL::g_overflows == 3 && v.size() == 3);
        v.push_back(a);                              // capacity 4: the in-place placement copy arm
        Check(_STL::g_overflows == 3 && v.size() == 4);
        Check(Same(v._M_start[0], "AA", 1, 0.25f) && Same(v._M_start[1], "EE", 2, 0.5f));
        Check(Same(v._M_start[2], "OO", 3, 0.75f) && Same(v._M_start[3], "AA", 1, 0.25f));
        Check(v._M_start[3].Symbol.m_pData != a.Symbol.m_pData);
    }
    Check(g_liveStrings == 0);
    if (g_fail) {
        printf("00a4ab10_TEST FAIL\n");
        return 1;
    }
    printf("00a4ab10_TEST PASS\n");
    return 0;
}