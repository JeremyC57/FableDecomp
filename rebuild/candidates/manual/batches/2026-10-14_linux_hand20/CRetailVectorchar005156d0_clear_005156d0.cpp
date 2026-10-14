// CRetailVector_char_005156d0::clear() -- erase(begin(), end()) on a byte vector with STLport's __copy_trivial inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005156D0, length 41 bytes.
// Callees: memmove @ 0x00BFEAE6 (never reached with a non-zero length).
// The manifest label (CComponent::SetParentColour) is propagated; the caller is
// CPersistContext::Transfer<signed char> @ 0x005E9840 on its second argument. The body is
// `finish = __copy_trivial(end(), end(), begin())`: the `(last == first) ? result : memmove(...) + n`
// form keeps the never-true `cmp eax,eax` test and the zero-length `memmove` + `add` tail that VC7.1
// cannot fold through the by-value pointer temporaries. Named by what it does.
extern "C" void* memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

// STLport __copy_trivial: byte block move, returns the advanced destination.
static inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}

struct CRetailVector_char_005156d0 {           // retail std::vector<char>
    char* first;
    char* last;
    char* end_of_storage;
    void clear();
};

void CRetailVector_char_005156d0::clear()
{
    char* newLast = (char*)CopyTrivial(last, last, first);   // erase(begin(), end()): copy [end, end) down to begin
    last = newLast;
}
