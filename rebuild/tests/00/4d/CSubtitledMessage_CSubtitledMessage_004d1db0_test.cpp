#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour test for 0x004D1DB0 CSubtitledMessage::~CSubtitledMessage: the implicit destructor destroys every
// AnimationTags name, frees the three tag vectors, then Narrator and Text.
static int g_stringDtors = 0;
static int g_wideDtors = 0;
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) ::free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
class CCharString {
public:
    CCharString() : PStringData(0) { }
    ~CCharString() { ++g_stringDtors; }
    void* PStringData;
};
class CWideString {
public:
    CWideString() : PData(0) { }
    ~CWideString() { ++g_wideDtors; }
    unsigned short* PData;
};
template <class T> class CArray : public std::vector<T> { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {
public:
    CArray<std::pair<K, V> > Vector;
    C Compare;
    bool Dirty;
};
enum EConversationAttitude { ATTITUDE_A };
class CASoundBank;
class CSubtitledMessage {
public:
    class CCameraTag { public: long Values[6]; };
    CASoundBank* PSpeechBank;
    long SpeechIndex;
    bool Speech3D;
    CWideString Text;
    CCharString Narrator;
    CVectorMap<float, EConversationAttitude, CKeyPairCompareLess<float, EConversationAttitude> > AttitudeTags;
    CVectorMap<float, CCharString, CKeyPairCompareLess<float, CCharString> > AnimationTags;
    CVectorMap<float, CCameraTag, CKeyPairCompareLess<float, CCameraTag> > CameraTags;
};
template <class P> static void Fill(std::vector<P>& v, int n)
{
    v._M_start = (P*)::malloc(sizeof(P) * (n ? n : 1));
    for (int i = 0; i < n; ++i) new (&v._M_start[i]) P;
    v._M_finish = v._M_start + n;
    v._M_end_of_storage._M_data = v._M_finish;
}
int main()
{
    void* raw = ::operator new(sizeof(CSubtitledMessage));
    CSubtitledMessage* m = new (raw) CSubtitledMessage;
    Fill(m->AttitudeTags.Vector, 2);
    Fill(m->AnimationTags.Vector, 3);
    Fill(m->CameraTags.Vector, 1);
    g_stringDtors = 0; g_wideDtors = 0;
    m->~CSubtitledMessage();
    int ok = g_stringDtors == 4 && g_wideDtors == 1;          // three animation names + Narrator; Text
    CSubtitledMessage* e = new (raw) CSubtitledMessage;       // empty vectors: nothing freed, strings still destroyed
    g_stringDtors = 0; g_wideDtors = 0;
    e->~CSubtitledMessage();
    ok = ok && g_stringDtors == 1 && g_wideDtors == 1;
    ::operator delete(raw);
    if (!ok) { std::printf("004d1db0_TEST FAIL\n"); return 1; }
    std::printf("004d1db0_TEST PASS\n");
    return 0;
}