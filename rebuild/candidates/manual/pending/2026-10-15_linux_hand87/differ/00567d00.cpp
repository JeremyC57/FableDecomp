// CNetString_Retail::RangeInitialize(const char* first, const char* last, const SForwardIteratorTag&) -- STLport basic_string<char>::_M_range_initialize: allocate last-first+1 bytes, copy the range, NUL-terminate
// Grade: DIFFER(ours v retail) -- 90v89: identical except the int3 the lane compiler emits after the `__declspec(noreturn)` ThrowLengthError call (retail's next byte is padding int3 too; the oracle trims it). Lane gap: trailing int3 after a noreturn call
// Leaf: RangeInitialize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00567D00, length 89 bytes.
// Callees: malloc @ 0x00BFEA0E (the allocator), memmove @ 0x00BFEAE6 (STLport __copy_trivial),
// _String_base::_M_throw_length_error() const @ 0x00565A30 (puts("basic_string") + abort with exceptions off;
// landed as BasicStringRaise -- called here as a member, `mov ecx,edi`, and never returns: nothing follows the call).
// The networking module's 12-byte string {start, finish, end_of_storage}; landed callers 0x0083F5C0/0x0083F600
// (CHttpUrl GetHost/GetPath) and 0x00569330 (substr) build it through the inline range constructor.
// Shape: STLport 4.x, inlined
//   n = l - f;  _M_allocate_block(n + 1):  if (n+1 <= max_size()+1 && n+1 > 0) { start = finish = allocate(n+1);
//   end_of_storage = start + n + 1; } else _M_throw_length_error();
//   finish = uninitialized_copy(f, l, start)  ->  __copy_trivial: l == f ? start : memmove(start, f, l-f) + (l-f);
//   *finish = 0.
// `max_size()+1` is size_t(-1): VC7.1 keeps the never-true `cmp esi,-1 ; ja` test. The forward-iterator tag is the
// empty third argument (`ret 0Ch`). The manifest label "CTCElectrocutedEffect::InitFromStringRange" is propagated.
extern "C" void* __cdecl malloc(unsigned int size);                                // 0x00BFEA0E
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int size);  // 0x00BFEAE6

struct SForwardIteratorTag { SForwardIteratorTag() { } };

struct CNetString_Retail {                         // 12 bytes: start, finish, end of storage
    char* m_start;
    char* m_finish;
    char* m_endOfStorage;
    __declspec(noreturn) void ThrowLengthError() const;   // 0x00565A30
    static unsigned int MaxSize() { return (unsigned int)(-1) / sizeof(char) - 1; }
    void AllocateBlock(unsigned int n)
    {
        if (n <= MaxSize() + 1 && n > 0) {
            m_start = (char*)malloc(n);
            m_finish = m_start;
            m_endOfStorage = m_start + n;
        } else
            ThrowLengthError();
    }
    static char* CopyTrivial(const char* first, const char* last, char* result)
    {
        return last == first ? result : (char*)memmove(result, first, last - first) + (last - first);
    }
    void RangeInitialize(const char* first, const char* last, const SForwardIteratorTag& tag);
};

void CNetString_Retail::RangeInitialize(const char* first, const char* last, const SForwardIteratorTag&)
{
    int n = last - first;
    AllocateBlock(n + 1);
    m_finish = CopyTrivial(first, last, m_start);
    *m_finish = 0;
}
