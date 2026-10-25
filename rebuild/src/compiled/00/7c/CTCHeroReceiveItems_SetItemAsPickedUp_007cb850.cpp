// CTCHeroReceiveItems::SetItemAsPickedUp(long item) -- remember an item as picked up: append it to the picked-up list unless it is already there
// Leaf: ?SetItemAsPickedUp@CTCHeroReceiveItems
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CB850, 90 bytes (+6 int3), thiscall `ret 4`.
// Identity: Ego_r.pdb name set ?SetItemAsPickedUp@CTCHeroReceiveItems@@QAEXJ@Z (donor 0x007C8F10); both callers are in
//   CThingPlayerCreature::OnCollisionWithObject (0x006ACCF6, 0x006ACEC4). The catalog label "std_vector_Insert" is
//   descriptive.
// Callees: STLport find(first, last, const long&, const random_access_iterator_tag&) @ 0x0050E540 (__fastcall: first
//   in ecx, last in edx), vector<long>::_M_insert_overflow(pointer, const long&, const __true_type&, size_type, bool)
//   @ 0x00440990.
// Layout: retail CTCHeroReceiveItems PickedUpItems (STLport vector<long>, 12 bytes) +0x10 (the landed GiveItems model).
// Shape: `if (find(begin(), end(), item) == end()) push_back(item);` with the inline 3-argument find building the
//   iterator tag and STLport push_back (placement-new null test, the finish pointer re-read after the store). Both
//   empty tags are homed on the live `item` argument slot: VC7.1 does that only when the callees have same-TU noinline
//   bodies that read the value and leave the tag unnamed (stand-ins below; only the target row is graded).
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __true_type { __true_type() { } };

template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall find(_RandomAccessIter __first, _RandomAccessIter __last,
                                                      const _Tp& __val, const random_access_iterator_tag&)   // 0x0050E540
{
    for (; __first != __last; ++__first)
        if (*__first == __val)
            return __first;
    return __last;
}

template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return find(__first, __last, __val, random_access_iterator_tag());
}

template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    __declspec(noinline) void _M_insert_overflow(T* __position, const T& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x00440990
    void push_back(const T& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            new (this->_M_finish) T(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};

template <class T>
void vector<T>::_M_insert_overflow(T* __position, const T& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    if (__x == 0 && __atend)
        this->_M_finish = __position + __fill_len;
}
}

class CTCHeroReceiveItems {
public:
    virtual void v00();
    void*                ParentThing;                       // +0x04 (CTCBase)
    unsigned char        _pad08[4];                         // +0x08
    unsigned char        Items[4];                          // +0x0c STLport list
    std::vector<long>   PickedUpItems;                     // +0x10
    void SetItemAsPickedUp(long item);
};

void CTCHeroReceiveItems::SetItemAsPickedUp(long item)
{
    if (std::find(PickedUpItems.begin(), PickedUpItems.end(), item) == PickedUpItems.end())
        PickedUpItems.push_back(item);
}