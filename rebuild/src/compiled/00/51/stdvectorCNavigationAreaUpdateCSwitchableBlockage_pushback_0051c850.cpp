// std::vector<CNavigationAreaUpdate::CSwitchableBlockage>::push_back(const value_type& x) -- STLport 4.6 push_back kept out of line: copy-construct x at _M_finish when there is room (the out-of-line blockage copy 0x005199E0), else _M_insert_overflow(_M_finish, x, __false_type(), 1, true) 0x0051C1F0
// Leaf: ?push_back@?$vector@VCSwitchableBlockage@CNavigationAreaUpdate@@V?$allocator@VCSwitchableBlockage@CNavigationAreaUpdate@@@std@@@std@@QAEXABVCSwitchableBlockage@CNavigationAreaUpdate@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051C850, 59 bytes, thiscall, `ret 4`.
// Identity: landed as the generated _global::PushBack_CElement16_005199e0_0051c850 over a CRetailVector stand-in. The element
//   is Ego_r.pdb CNavigationAreaUpdate::CSwitchableBlockage (16 bytes: UniqueID +0, STLport list<C2DLineF> +8); its copy
//   0x005199E0 and growth callee 0x0051C1F0 are that vector's (see their rows), and Ego_r / FableWin both name
//   ?push_back@?$vector@VCSwitchableBlockage@CNavigationAreaUpdate@@V?$allocator@...@std@@@std@@QAEXABVCSwitchableBlockage@...@Z.
//   The byte twin 0x00A84FB0 is the CNavQuadTree TU's vector<pair<unsigned __int64, list<C2DLineF> > >::push_back.
// Shape: placement new (hence the null test) through the declared out-of-line copy constructor, `_M_finish` advanced by 0x10;
//   the full case pushes `1, 1` (atend, fill_len), the __false_type temporary homed in the dead x argument slot
//   (`lea edx,[esp+10h]`), x and _M_finish -- the tag's user constructor keeps it uninitialised.
inline void* operator new(unsigned int, void* where) { return where; }

class C2DLineF;

namespace std {
struct __false_type { __false_type() {} };

template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                          // +0x00 (STLport: the header node pointer)
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);   // 0x0051C1F0
};
}

class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {                     // Ego_r.pdb (retail 0x10 bytes)
    public:
        unsigned __int64     UniqueID;              // +0x00
        std::list<C2DLineF>  LineList;              // +0x08
        CSwitchableBlockage(const CSwitchableBlockage& o);   // 0x005199E0 (the implicit copy, out of line)
    };
};

template void std::vector<CNavigationAreaUpdate::CSwitchableBlockage>::push_back(const CNavigationAreaUpdate::CSwitchableBlockage&);
