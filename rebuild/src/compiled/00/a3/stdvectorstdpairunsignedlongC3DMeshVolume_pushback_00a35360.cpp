// std::vector<std::pair<unsigned long, C3DMeshVolume> >::push_back(const value_type&) -- appends a mesh-volume map entry
// Retail: Fable.exe 0x00A35360, 93 bytes: if _M_finish != _M_end_of_storage, the null-tested placement copy of the
// 0x10-byte pair (first; C3DMeshVolume copy: Name via CCharString copy ctor 0x0099EC30, ConvexVolume
// CCountedPointer copy with ++Info->RefCount) and _M_finish += 0x10; else _M_insert_overflow(_M_finish, x,
// __false_type, 1, true) (0x00A34F00); `ret 4`.
// Identity: the only caller 0x00A35410 is a CVectorMap insert (`push_back(v) ; Dirty = true` at +0xD), and the 0x10-byte
// element (unsigned long key, CCharString, counted pointer) is Ego_r.pdb pair<unsigned long, C3DMeshVolume> of
// C3DMeshStats::MeshVolumes (CVectorMap<unsigned long, C3DMeshVolume, CKeyPairCompareLess<...> >; C3DMeshVolume =
// Name CCharString +0, ConvexVolume CCountedPointer<CContainmentVolume> +4). The vector is the CVectorMap's CArray
// data (STLport vector<pair<..> >).
// Leaf: ?push_back@?$vector@U?$pair@KVC3DMeshVolume@@@std@@V?$allocator@U?$pair@KVC3DMeshVolume@@@std@@@2@@std@@QAEXABU?$pair@KVC3DMeshVolume@@@2@@Z
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    void* PStringData;
};

class CCPPointerInfo { public: long RefCount; };

template <class T> class CCountedPointer {
public:
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
};

class CContainmentVolume;

class C3DMeshVolume {
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

namespace std {
    struct __false_type { __false_type() { } };
    template <class _T1, class _T2> struct pair {
        _T1 first;
        _T2 second;
        pair(const pair& __o) : first(__o.first), second(__o.second) { }
    };
    template <class _Tp> class allocator { public: };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
    template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

    template <class _Tp, class _Alloc> class _Vector_base {
    public:
        _Tp* _M_start;                                  // +0x00
        _Tp* _M_finish;                                 // +0x04
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    };

    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : public _Vector_base<_Tp, _Alloc> {
    public:
        typedef _Tp* pointer;
        typedef unsigned int size_type;
        typedef __false_type _IsPODType;
        void push_back(const _Tp& __x)
        {
            if (this->_M_finish != this->_M_end_of_storage._M_data) {
                _Construct(this->_M_finish, __x);
                ++this->_M_finish;
            }
            else
                _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
        }
    protected:
        void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                                bool __atend = false);   // 0x00A34F00
    };

    template class vector<pair<unsigned long, C3DMeshVolume> >;
}