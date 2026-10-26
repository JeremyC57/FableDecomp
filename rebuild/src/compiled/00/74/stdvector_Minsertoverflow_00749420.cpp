// std::vector::_M_insert_overflow(TDangerPair* position, const TDangerPair& x, const __false_type&, unsigned int fill_len, bool atend) -- grow CTCStealth's danger-thing vector and insert fill_len copies of x
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749420, 270 bytes, `ret 0x14`.
// Label correction: catalog "CIntelligentPointer_Vector_Resize" -> STLport 4.x
//   vector<pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> >::_M_insert_overflow (the landed
//   __uninitialized_fill_n 0x00748AA0 names this caller; 16-byte element: `sar eax,4`, `shl 4`).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy 0x00748A40, __uninitialized_fill_n 0x00748AA0,
//   CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, ::GetPItem 0x00A01B50, ::SetPItem 0x00A01B90,
//   ::~CBaseIntelligentPointer 0x00A01C10.
// Shape: STLport `_M_insert_overflow` (see 0x006FC6A0): len = size + max(size, fill_len) via the const-ref `max`,
//   allocate (new_start parked in the dead fill_len slot), copy the head, construct one element in place (the inlined
//   pair copy: CIntelligentPointer copy constructor -- base ctor, vtable 0x01237F1C, SetPItem(x.GetPItem()) -- then
//   the two CDangerStatus words) or fill_n, copy the tail unless atend; _M_clear's destroy loop is inlined (the pair's
//   generated destructor calls the out-of-line base destructor directly; `last` parked in the dead atend slot), then
//   free and _M_set. The copy / fill helpers are same-TU noinline stand-ins.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CThing;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE = 0 };
class CDangerStatus {                           // Ego_r.pdb, 8 bytes
public:
    EDangerStatus DangerStatus;                 // +0x00
    long          Timer;                        // +0x04
};
}

namespace std {

struct __false_type { __false_type() {} };

template<class T1, class T2> struct pair {
    T1 first;                                   // +0x00
    T2 second;                                  // +0x08
    pair(const pair& other) : first(other.first), second(other.second) {}
};

template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template<class T> inline void _Destroy(T* p) { p->~T(); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

#pragma auto_inline(off)
// Same-TU stand-ins (real bodies: 0x00748A40, 0x00748AA0).
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        _Construct(result, *first);
    return result;
}
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first, unsigned int n, const T& x, const __false_type&)
{
    for (; n > 0; --n, ++first)
        _Construct(first, x);
    return first;
}
#pragma auto_inline(on)

template<class T> inline void __destroy_aux(T* first, T* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(first);
}
template<class T> inline void _Destroy(T* first, T* last) { __destroy_aux(first, last, __false_type()); }

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    Alloc _M_alloc() const { return Alloc(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_alloc().deallocate(_M_start, (unsigned int)(_M_end_of_storage - _M_start));
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template<class T, class Alloc>
void vector<T, Alloc>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);

    T* new_start = _M_alloc().allocate(len);
    T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template class vector<pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> >;

}