// PKCWideString::operator=(const unsigned short* text) -- libpk wide string: assign a NUL-terminated wide text through Assign, return *this
// Leaf: ??4PKCWideString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C16970, 19 bytes, __fastcall `ret 4`, returns `this`.
// Identity: libpk (0x00C08AE0..0x00C40000, no PDB/RTTI; descriptive names as in hand chunk 433). The wide string keeps its
//   characters in a PKCAutoDeleteArray<wchar_t> at +0; Assign(const wchar_t*) @ 0x00C16820 (chunk 433) resizes it to
//   length+1 and copies. The wide-string family around it: the constructor from text 0x00C168C0 (zero both words, Assign),
//   the constructor / assignment from another string 0x00C16900 / 0x00C16950 (Assign(other.c_str() @ 0x00C16800)), and this
//   row, the assignment straight from text. Callers 0x00C16201 and 0x00C48508 pass a wide text pointer.
//   The catalog's "operator&&<CPredicate_And<...>, CIsThingAlive>" label is propagated.
// Callee: PKCWideString::Assign(const unsigned short*) @ 0x00C16820.
// Shape: `Assign(text); return *this;` -- `this` kept in esi across the call, the argument re-pushed from [esp+4].
struct PKCAutoDeleteArray {                 // PKCAutoDeleteArray<wchar_t>
    unsigned short* m_pData;                // +0x00
    unsigned long   m_count;                // +0x04
};

class PKCWideString {
public:
    PKCAutoDeleteArray m_chars;             // +0x00
    void Assign(const unsigned short* text);              // 0x00C16820
    PKCWideString& operator=(const unsigned short* text);
};

PKCWideString& PKCWideString::operator=(const unsigned short* text)
{
    Assign(text);
    return *this;
}