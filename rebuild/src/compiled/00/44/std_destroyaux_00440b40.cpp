// std::__destroy_aux(_Deque_iterator<CSubtitleRenderer::CSetTextInfo> first, _Deque_iterator<CSubtitleRenderer::CSetTextInfo> last, const __false_type&) -- STLport range destroy over a deque of subtitle text requests: run each 28-byte element's destructor (its four CWideStrings), stepping to the next 4-element block at each block end
// Leaf: __destroy_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00440B40, length 96 bytes (82 + int3 padding), callee-cleaned `ret 20h` (two
// 16-byte iterators by value; the __false_type reference rides in ecx and is never read).
// Callees: CWideString::~CWideString @ 0x0099B510, four times per element (CSetTextInfo's implicit destructor, members in
// reverse declaration order: Button3Text +0xC, Button2Text +8, Button1Text +4, Text +0).
// Label: the catalog "Vector_DestructStringRange" is behavioural. Callers 0x00441504 and 0x00442504 (the two-line
// `_Destroy(first, last)` / `__destroy(first, last, T*)` forwarders that copy both iterators and pass `lea ecx,[esp+34h]`, the
// empty tag) and 0x00442830 (a deque teardown path). Element = Ego_r.pdb CSubtitleRenderer::CSetTextInfo (28 bytes; field
// layout as the landed CSetTextInfo ctor 0x0048DDF6), the element of CSubtitleRenderer::TextInfoQueue
// (queue<CSetTextInfo, deque<CSetTextInfo> >). No engine header defines CSetTextInfo (CSubtitleRenderer.h keeps the queue as
// raw bytes), so it is modelled locally.
// Model: the landed sibling std::__destroy_aux<_Deque_iterator<pair<CCharString, long> > > @ 0x00440C00 -- STLport 4.x
// `__destroy_aux(first, last, const __false_type&)` (`for (; first != last; ++first) _Destroy(&*first);`) emitted out of line
// as `__fastcall`; the deque iterator is {_M_cur, _M_first, _M_last, _M_node}, `++` steps `_M_cur` and at `_M_last` calls
// `_M_set_node(_M_node + 1)` (buffer = 128 / sizeof(T) = 4 elements = 0x70 bytes: `lea ebx,[esi+70h]`) then `_M_cur =
// _M_first`; `!=` compares `_M_cur` only. Shape: speed TU (/O2); `last._M_cur` in ebp, the cursor in esi, `_M_last` in ebx,
// `_M_node` in edi (the dead `_M_first` store is dropped).
class CWideString { public:
    void* m_pStringData;
    ~CWideString();                                // 0x0099B510
};

class CSubtitleRenderer {
public:
    class CSetTextInfo { public:                          // 28 bytes
        CWideString   Text;                        // +0x00
        CWideString   Button1Text;                 // +0x04
        CWideString   Button2Text;                 // +0x08
        CWideString   Button3Text;                 // +0x0C
        unsigned long DisplayFlags;                // +0x10
        float         DisplayTime;                 // +0x14
        bool          UpdateWhilePaused;           // +0x18
        bool          ChangePlayerMode;            // +0x19
    };
};

namespace std {
struct __false_type { };

template <class T> struct _Deque_iterator {
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
    static unsigned int _S_buffer_size() { return sizeof(T) < 128 ? 128 / sizeof(T) : 1; }
    void _M_set_node(T** new_node)
    {
        _M_last = (_M_first = *(_M_node = new_node)) + (int)_S_buffer_size();
    }
    T& operator*() const { return *_M_cur; }
    _Deque_iterator& operator++()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
        return *this;
    }
    bool operator!=(const _Deque_iterator& other) const { return _M_cur != other._M_cur; }
};

template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

template void __fastcall __destroy_aux<_Deque_iterator<CSubtitleRenderer::CSetTextInfo> >(
    _Deque_iterator<CSubtitleRenderer::CSetTextInfo> first, _Deque_iterator<CSubtitleRenderer::CSetTextInfo> last,
    const __false_type&);
}