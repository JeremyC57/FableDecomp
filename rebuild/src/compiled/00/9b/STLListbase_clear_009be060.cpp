// std::_List_base::clear() -- list<CSubTitle>::clear: destroy every node's CSubTitle (sound name, formatted text, text-entry reference), free the node and re-link the sentinel to itself
// Leaf: ?clear@?$_List_base@UCSubTitle@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BE060, 88 bytes, thiscall plain `ret`.
// Identity: STLport `_List_base<CSubTitle, allocator<CSubTitle> >::clear()` (FableWin.pdb has ?clear@?$list@UCSubTitle@@...
//   for the Dinkumware build).  Callers: the list<CSubTitle> destructors 0x009BE150 / 0x009BE190 (landed as
//   std::list<T_009be150>::~list), CMovieSubtitles::ParseSubtitles 0x009BD460 (re-parse clears SubTitles), 0x009BD090,
//   ~CBankFileEntryUpdateData 0x009BDB40 and CVideoSys::~CVideoSys 0x00A3BC20 (its CMovieSubtitles member's SubTitles list,
//   Ego_r.pdb CMovieSubtitles +0); catalogued as `list_node_destructor`.
// Element: Ego_r.pdb CSubTitle (28 bytes): TextEntry CIVCountedPointer<const NGameText::CTextEntry> +0, FormattedText
//   CWideString +4, Sound CCharString +8, FrameStart/FrameEnd/StartTime/EndTime -- node data at +8, so the members die
//   at +0x10 (~CCharString 0x0099EAE0), +0x0C (~CWideString 0x0099B510), +0x08 (the counted pointer's inline Release:
//   CIVCountedPointeeBase::Release() const = `--IVRefCount`, virtual Delete() (slot 1) at zero, then the null store).
// Callees: free @ 0x00BFEA14 (the node; unchecked because the node was dereferenced first).
// Shape: STLport _list.c clear(): walk from _M_node->_M_next, _Destroy the value, deallocate the node, then
//   `_M_next = _M_prev = _M_node` (the sentinel word re-read for each store).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
private:
    void* PStringData;
};

class CWideString {
public:
    ~CWideString();                                     // 0x0099B510
private:
    void* PStringData;
};

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                        // slot 1 (+0x04)
    long Release() const
    {
        if (--IVRefCount == 0) {
            Delete();
            return 0;
        }
        return (long)IVRefCount;
    }
private:
    mutable unsigned long IVRefCount;                   // +0x04
};

namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase {
};
}

template <class T> class CIVCountedPointer {
public:
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        if (Object) {
            InternalGetObject()->Release();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* InternalGetObject() const { return (CIVCountedPointeeBase*)Object; }
private:
    T* Object;
};

struct CSubTitle {                                      // Ego_r.pdb, 28 bytes
    CIVCountedPointer<const NGameText::CTextEntry> TextEntry;   // +0x00
    CWideString FormattedText;                          // +0x04
    CCharString Sound;                                  // +0x08
    long        FrameStart;                             // +0x0C
    long        FrameEnd;                               // +0x10
    float       StartTime;                              // +0x14
    float       EndTime;                                // +0x18
};

namespace std {

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
    void clear();
};

template <class _Tp, class _Alloc> void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

template void _List_base<CSubTitle, allocator<CSubTitle> >::clear();

}