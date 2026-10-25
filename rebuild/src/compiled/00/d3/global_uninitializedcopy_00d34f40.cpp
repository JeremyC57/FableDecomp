// __uninitialized_copy(const bool* first, const bool* last, bool* result, const __true_type&) -- STLport trivial uninitialized copy of a byte range: memmove the bytes and return the end of the copy
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D34F40, 30 bytes, `__fastcall` (first in ecx, last in edx, result and the
//   tag on the stack: `ret 8`). Only caller: vector<bool>::vector(const vector<bool>&) @ 0x00D34F00 (CFireHeartPatternDef's
//   PadActive copy).
// Label correction: the catalog "Map_MoveOrCopyValue" is descriptive; this is STLport's __true_type
//   __uninitialized_copy, emitted out of line in this TU.
// Callees: memmove @ 0x00BFEAE6 (__cdecl, `add esp,0Ch`).
// Shape: STLport 4.6 `__copy_trivial` as written: `(last == first) ? result : memmove(result, first, last - first) +
//   (last - first)` -- the pointer compare comes first (`cmp edx,ecx ; je`, result already in eax), then the byte
//   count is formed in esi and kept across the call. (A `n = last - first; n ? ... : result` form tests `sub ; je`.)
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
namespace std {
struct __true_type { };
inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) +
        ((const char*)last - (const char*)first);
}
template <class T> T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __true_type&)
{
    return (T*)__copy_trivial(first, last, result);
}
template bool* __fastcall __uninitialized_copy<bool>(const bool* first, const bool* last, bool* result, const __true_type&);
} // namespace std