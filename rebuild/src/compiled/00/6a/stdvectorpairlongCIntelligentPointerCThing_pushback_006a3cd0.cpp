// std::vector<pair<long,CIntelligentPointer<CThing> > >::push_back(const value_type& x) -- STLport push_back: copy-construct at the end when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3CD0, length 93 bytes (+ int3 padding), `ret 4`.
// Callees: vector::_M_insert_overflow(pointer, const T&, const __false_type&, size_type, bool) @ 0x006A3A70,
//   CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90;
//   CIntelligentPointer<CThing> vtable 0x01238C6C.
// Identity: the catalog's "Vector_PushBackElement" is a descriptive auto label. 0x006A3A70 (catalog
//   "Vector_ReserveAndFill") takes (position = _M_finish, x, the __false_type tag, 1, true): STLport's
//   `_M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true)`. The vector is the storage of the
//   CVectorMap<long, CIntelligentPointer<CThing>> the neighbouring sort functions order.
// Element: pair<long, CIntelligentPointer<CThing> > with STLport's declared memberwise copy constructor (the landed
//   __uninitialized_copy 0x006A31D0 model: key, then the pointer copy through a second destination pointer).
// Shape: _Construct's placement null test; the __false_type temporary is homed in the dead x-argument slot
//   (`lea edx,[esp+10h]`) after x has been loaded into ecx.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FablePush_006a3cd0 {
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {                        // 8 bytes: vtable + item
    void* PData;                                        // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {  // vtable 0x01238C6C for CIntelligentPointer<CThing>
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<long, CIntelligentPointer<CThing> > SThingEntry;

template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x006A3A70
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            new ((void*)this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
template class vector<SThingEntry>;
}