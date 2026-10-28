// CCharString::Insert(long position, char c) -- insert one character at `position` (taking a private, non-null copy of the data block first)
// Leaf: ?Insert@CCharString@@QAEXJD@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099F3D0, 42 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): both PDBs carry ?Insert@CCharString@@QAEXJD@Z (long position, char) and ?Insert@?$CBasicString@D@@QAEXKPAD@Z;
//   the body builds a one-character C string and hands it with the position to CBasicString<char>::Insert on the data block.
//   Caller 0x009EAB53.
// Callees: CCharString::MakeOwnCopyOfDataOrEmptyString 0x0099EB40 (protected), CBasicString<char>::Insert(unsigned long, char*)
//   0x009A06B0.
// Shape: the two-char buffer `{ c, 0 }` is a local array homed in the dead `c` argument slot (`mov [esp+14h],al ;
//   mov byte [esp+15h],0`), as the landed CWideString::operator!=(wchar_t) 0x0099B1D0; it is homed there only with a same-TU
//   noinline stand-in body for CBasicString<char>::Insert (declaration-only gives a `push ecx` frame slot).
// ---- CCharString model (Ego_r: CCharString = one CCharStringData*; CCharStringData = CBasicString<char> Data + NoRefs) ----
template <class T> class CBasicString {
public:
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    unsigned long UseFastExtend : 1;
    void Truncate(unsigned long length);                // 0x009A0080
    void Insert(unsigned long position, T* text);       // 0x009A06B0
};
class CCharStringData {
public:
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;
};
class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
    void Insert(long position, char c);
protected:
    void MakeOwnCopyOfDataOrEmptyString();              // 0x0099EB40
};

#pragma auto_inline(off)
// Same-TU stand-in body of CBasicString<char>::Insert 0x009A06B0 (retail's TU holds the real one): with the callee's body in
// the TU, VC7.1 sees that the character buffer is not captured and homes it in the dead `c` slot.
template <> __declspec(noinline) void CBasicString<char>::Insert(unsigned long position, char* text)
{
    PData[position] = *text;
}
#pragma auto_inline(on)

void CCharString::Insert(long position, char c)
{
    MakeOwnCopyOfDataOrEmptyString();
    char text[2];
    text[0] = c;
    text[1] = 0;
    PStringData->Data.Insert(position, text);
}
