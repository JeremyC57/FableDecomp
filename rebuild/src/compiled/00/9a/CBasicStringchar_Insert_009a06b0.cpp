// CBasicString<char>::Insert(unsigned long position, const char* text) -- insert a C string at a position: appended when the position is the end, otherwise grow the buffer, extend the length (terminator + zeroed alignment tail), move the tail up and copy the text in
// Leaf: ?Insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A06B0, length 196 bytes, `ret 8`.
// Callees: CBasicString<char>::AppendData(const char*, unsigned long) @ 0x009A04E0 (landed), CBasicString<char>::ExtendData(unsigned long)
// @ 0x009A01C0, memmove @ 0x00BFEAE6; memset / memcpy are the inline `rep stos` / `rep movs` intrinsics.
// Layout: Ego_r.pdb CBasicString<char> (packed 13 bytes, as the landed Truncate 0x009A0080): PData +0, StringLength +4,
// DataLength:31 +8, UseFastExtend:1 +0xc.
// Shape: the old length is read once (ebp) for the end test and the moved-tail size; each branch has its own inline `strlen`
// loop (a rotated `while (text[n]) ++n`); the length store, terminator and alignment-tail clear are the landed SetString /
// Truncate tail; the destination of the tail move is `PData + length + position`.
#include <string.h>
#pragma intrinsic(memcpy, memset)

#pragma pack(push, 1)
template<class T> struct CBasicString {
    T* PData;                                   // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool UseFastExtend : 1;
    void ExtendData(unsigned long newLength);                       // 0x009A01C0
    void AppendData(const T* data, unsigned long length);           // 0x009A04E0
    static unsigned long Length(const T* text)
    {
        unsigned long n = 0;
        while (text[n])
            ++n;
        return n;
    }
    void ClearTail()
    {
        unsigned long used = StringLength + 1;
        unsigned long end = (used + 3) & ~3UL;
        memset(PData + used, 0, end - used);
    }
    void Insert(unsigned long position, const T* text);
};
#pragma pack(pop)

template<class T> void CBasicString<T>::Insert(unsigned long position, const T* text)
{
    unsigned long oldLength = StringLength;
    if (position == oldLength) {
        AppendData(text, Length(text));
        return;
    }
    unsigned long length = Length(text);
    ExtendData(length + oldLength);
    unsigned long newLength = StringLength + length;
    StringLength = newLength;
    PData[newLength] = 0;
    ClearTail();
    memmove(PData + length + position, PData + position, oldLength - position);
    memcpy(PData + position, text, length);
}

template void CBasicString<char>::Insert(unsigned long position, const char* text);