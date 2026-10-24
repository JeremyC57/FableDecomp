// CBasicString<char>::Append(char c, unsigned long count) -- appends `count` copies of `c`, keeping the text NUL-terminated and the slack up to the next dword boundary zeroed
// Leaf: ?Append@?$CBasicString@D@@QAEXDK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A0460, 115 bytes (+ int3 padding), `ret 8`. Catalog name "String_FillRange";
// the Ego_d/FableWin PDB name set has `?Append@?$CBasicString@D@@QAEXDK@Z` (= `void Append(char, unsigned long)`),
// and the body sits between SetString(count, char) @ 0x009A0390 and Append(const char*, unsigned long) @ 0x009A04E0.
// Callees: CBasicString<char>::ExtendData(unsigned long) @ 0x009A01C0 (grows the buffer; `this` stays in ecx).
// Layout: CBasicString<char> (13 bytes, packed): PData +0, StringLength +4, DataLength:31 +8, UseFastExtend:1 +0xc.
// Shape: ExtendData(StringLength + count); then the PDB's small inline helpers: Fill(dest, count, c) is a plain
// byte loop that VC7.1 turns into the fill idiom (the loop entry test is the `jbe`, the byte-replicating
// `mov al ; mov bl,al ; mov bh,bl` rep stosd/stosb; an explicit `memset` sinks the dest add into the guarded
// block and widens the char with `movsx`),
// SetStringLength(length) stores the length and the terminator, and ClearStringEnd(data, length) zeroes the bytes
// after the terminator up to Align(length + 1, 4) (the second rep stosd/stosb with eax = 0).
#include <string.h>

template <class T>
class CBasicString {
public:
    T*            PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength    : 31;           // +0x08 bits 0..30
    unsigned long m_reserved    : 1;            // +0x08 bit 31
    unsigned char UseFastExtend : 1;            // +0x0c bit 0

    void Append(T c, unsigned long count);
protected:
    void ExtendData(unsigned long length);      // 0x009A01C0
    static unsigned long __fastcall Align(unsigned long value, unsigned long alignment)
    {
        return (value + alignment - 1) & ~(alignment - 1);
    }
    static void __fastcall Fill(T* dest, unsigned long count, T c)
    {
        for (unsigned long i = 0; i < count; ++i)
            dest[i] = c;
    }
    void ClearStringEnd(T* data, unsigned long length)
    {
        unsigned long start = length + 1;
        memset(data + start, 0, Align(start, 4) - start);
    }
    void SetStringLength(long length)
    {
        StringLength = length;
        PData[length] = 0;
        ClearStringEnd(PData, StringLength);
    }
};

template <class T>
void CBasicString<T>::Append(T c, unsigned long count)
{
    ExtendData(StringLength + count);
    Fill(PData + StringLength, count, c);
    SetStringLength(StringLength + count);
}

template void CBasicString<char>::Append(char c, unsigned long count);