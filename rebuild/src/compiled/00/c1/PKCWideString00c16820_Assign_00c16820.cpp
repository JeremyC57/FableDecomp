// PKCWideString_00c16820::Assign(const unsigned short* text) -- copies a wide string into the PKCAutoDeleteArray character buffer: resizes it to length+1 when the size differs, block-copies the characters and writes the terminator (a null text becomes the empty string)
// Leaf: Assign
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16820, 108 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: a libpk wide string over a PKCAutoDeleteArray<wchar_t> at +0 (data +0, count +4; Resize @ 0x00C169D0, the
//   bounds-checked operator[] @ 0x00C16B10 logs "PKCAutoDeleteArray.h(77)" on overflow). Callers: the wide-string
//   constructor 0x00C168C0 (zeroes both words, then Assign) and the assignment forms 0x00C16934 / 0x00C1695F /
//   0x00C16978. Callee: MSVCR71 wcslen (import 0x01440084).
// Shape: `text ? wcslen(text) : 0` keeps the length in ebx; memcpy is the intrinsic (`shr ecx,2 ; rep movsd ; and ecx,3
//   ; rep movsb`) whose byte count is computed before the operator[] call and parked in the dead text slot; edi is
//   pushed only around the copy.
// Label correction: the catalog's `CDefString_SetFromWide` is propagated (no CDefString here); descriptive names.
extern "C" __declspec(dllimport) unsigned int __cdecl wcslen(const unsigned short* text);   // 0x01440084
extern "C" void* __cdecl memcpy(void* dest, const void* source, unsigned int count);
#pragma function(wcslen)
#pragma intrinsic(memcpy)

struct PKCAutoDeleteArray_00c16820 {       // PKCAutoDeleteArray<wchar_t>
    unsigned short* m_pData;               // +0x00
    unsigned long m_count;                 // +0x04
    void Resize(unsigned long count);      // 0x00C169D0
    unsigned short& operator[](unsigned long index);   // 0x00C16B10
};

struct PKCWideString_00c16820 {
    PKCAutoDeleteArray_00c16820 m_chars;   // +0x00
    void Assign(const unsigned short* text);
};

void PKCWideString_00c16820::Assign(const unsigned short* text)
{
    unsigned long length = text ? wcslen(text) : 0;
    if (length + 1 != m_chars.m_count)
        m_chars.Resize(length + 1);
    if (length)
        memcpy(&m_chars[0], text, length * sizeof(unsigned short));
    m_chars[length] = 0;
}