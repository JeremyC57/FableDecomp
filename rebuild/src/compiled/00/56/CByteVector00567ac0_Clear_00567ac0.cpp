// CByteVector_00567ac0::Clear() -- erase(begin(), end()) of a byte vector: the copy-down of the (empty) tail and the new end pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00567AC0, length 41 bytes (+ int3 padding).
// Callees: memmove @ 0x00BFEAE6 (__cdecl, three arguments popped together; its result is the new end
// pointer base). The manifest's "CComponent::SetParentColour" label is propagated; the only caller is
// a persist-style switch @ 0x00569F3B that clears the vector passed as its second argument, and the
// class is not recoverable from the body, so it is named by its address.
// Shape: Dinkumware erase(first, last) = `last_ = copy(last, end(), first)` with the copy-down
// helper written as an early-out (`if (src == srcEnd) return dest;`, GOTCHAS wave 17): both `last`
// and `end()` are the same load, giving the retail `cmp eax,eax ; jne`; the never-taken slow path
// moves `end - last` (zero) bytes and adds that count to memmove's result. One-byte elements: the
// count is not scaled.
#include <string.h>

struct CByteVector_00567ac0 {                                   // {first, last, end} of unsigned char
    unsigned char* first;
    unsigned char* last;
    unsigned char* end_of_storage;

    static unsigned char* CopyDown(unsigned char* src, unsigned char* srcEnd, unsigned char* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long count = srcEnd - src;
        return (unsigned char*)memmove(dest, src, count) + count;
    }
    void Erase(unsigned char* eraseFirst, unsigned char* eraseLast)
    {
        last = CopyDown(eraseLast, last, eraseFirst);
    }
    void Clear();
};

void CByteVector_00567ac0::Clear()
{
    Erase(first, last);
}