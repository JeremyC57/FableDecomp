// std::vector<bool>::_M_insert_overflow(bool* position, const bool& x, const __true_type&, unsigned int fillLength, bool atEnd) -- STLport growth path of a bool vector (one byte per element): allocate old+max(old,n), copy the head, fill n copies of x, copy the tail unless inserting at the end, free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) @ 0x00467831, 177 bytes, `ret 14h` (__fastcall: position, &x, the __true_type tag, n, atEnd).
// Identity: vector<bool> (sweep3; the landed rows said vector<unsigned char>): this size-optimised TU group (resize 0x00467686, erase 0x004676BB,
//   _M_fill_insert 0x004676EF, _M_insert_overflow 0x00467831, push_back 0x0053CB4F) serves vector<bool> members only: its callers
//   hold Ego_r vector<bool> members (CTCSpecialAbilities::AbilityAvailability 0x0072C6E0, CVertexGroupBlockOptimiser::RemainingBlocks
//   0x00ADA240, CTCWeaponTrail::ActiveTrail, CTCTavernTable::SeatsCurrentlyUsed, NUISystem::CList::ScrollingChildrenVisibility via
//   push_back) and GFSerialiseVectorBinaryIn<bool> 0x004675F6 calls resize (already bound as ?resize@?$vector@_N@std@@...). The
//   fill is STLport's generic byte loop (x re-read each pass), while the char-type overloads fill/fill_n use memset (the real
//   vector<unsigned char> overflow 0x00485B6C calls fill 0x0047CE40); retail STLport stores one byte per bool.
// Callees: malloc @ 0x00BFEA0E, memmove @ 0x00BFEAE6 (twice, the inlined __copy_trivial), free @ 0x00BFEA14.
//   Caller: the vector's fill insert @ 0x004676EF (labelled Vector_FillInsert).
// Shape: STLport 4.x POD `_M_insert_overflow`: `len = old + max(old, n)` with the const-reference `max` (`a < b ? b : a`,
//   retail `lea ecx,[ebp+14h] ; jb ; lea ecx,[ebp-4]`), `allocate(len)` = `len ? malloc(len) : 0`, `__copy_trivial`
//   (`last == first ? result : memmove(...) + n`) for the head and the tail, the generic `fill_n` loop for the insert
//   (x re-read through the reference every pass), then `_M_clear()` (`if (_M_start) free(_M_start)`) and `_M_set`.
//   Size TU (`and dword ptr [ebp-4],0`, `pop ecx` cleanup) with a frame pointer: `optimize("y", off)` + `("s", on)`;
//   the helpers are `__forceinline` because the size TU would call them out of line.
// Model: the three STLport vector words; no engine header (library type).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                            // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() {} };
template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
__forceinline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}
template <class T> __forceinline T* fill_n(T* first, unsigned int n, const T& value)
{
    for (; n > 0; --n, ++first)
        *first = value;
    return first;
}
template <class T> class vector {
public:
    T* _M_start;                    // +0x00
    T* _M_finish;                   // +0x04
    T* _M_end_of_storage;           // +0x08
    unsigned int size() const { return _M_finish - _M_start; }
    static __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    __forceinline void _M_clear() { if (_M_start != 0) free(_M_start); }
    __forceinline void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fillLength, bool atEnd);
};
template <class T> void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fillLength, bool atEnd)
{
    const unsigned int oldSize = size();
    const unsigned int length = oldSize + (max)(oldSize, fillLength);
    T* newStart = allocate(length);
    T* newFinish = (T*)__copy_trivial(_M_start, position, newStart);
    newFinish = fill_n(newFinish, fillLength, x);
    if (!atEnd)
        newFinish = (T*)__copy_trivial(position, _M_finish, newFinish);
    _M_clear();
    _M_set(newStart, newFinish, newStart + length);
}
template void vector<bool>::_M_insert_overflow(bool*, const bool&, const __true_type&, unsigned int, bool);
}