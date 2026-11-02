// CCharString::Insert(long position, char c) -- make the text block this string's own (or a fresh empty one), then insert the one-character string c at position
// Leaf: ?Insert@CCharString@@QAEXJD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099F3D0, 42 bytes (int3 padding to 0x0099F400), thiscall `ret 8`; the two-character buffer
//   is homed in the dead slot of argument c ([esp+0xC] after the push).
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb both list ?Insert@CCharString@@QAEXJD@Z (position long, char);
//   the body calls 0x0099EB40 = CCharString::MakeOwnCopyOfDataOrEmptyString (protected IAE in both PDBs) and then
//   0x009A06B0 = CBasicString<char>::Insert(unsigned long, char*) (Ego_r 0x00AFBA00 / FableWin 0x02F64BB0 ?Insert@?$CBasicString@D@@QAEXKPAD@Z)
//   on PStringData->Data. The catalog name is right.
// Callees: CCharString::MakeOwnCopyOfDataOrEmptyString() 0x0099EB40, CBasicString<char>::Insert(unsigned long, char*) 0x009A06B0.
// Layout (Ego_r.pdb, pack 1): CBasicString<char> {PData +0, StringLength +4, DataLength:31 / UseFastExtend:1 +8} 13 bytes;
//   CCharStringData {Data +0, NoRefs +0x0D}; CCharString {PStringData +0}. Shared fable_string.h models CCharString without
//   these PDB names or the protected MakeOwnCopyOfDataOrEmptyString, so the class is restated here (header debt, NOTES.md).
// Shape: the buffer in a block scope (the scope ends before return, letting VC7.1 reuse the argument slot), text[0] = c,
//   text[1] = 0, Insert(position, text) -- the form that gives retail's 42 bytes (a function-scope buffer gives 44).
#pragma pack(push, 1)
template <class T> class CBasicString {                  // Ego_r.pdb CBasicString<char>, 13 bytes packed
public:
    T*            PData;                                 // +0x00
    unsigned long StringLength;                          // +0x04
    unsigned long DataLength : 31;                       // +0x08
    bool          UseFastExtend : 1;                     // +0x0c
    void Insert(unsigned long position, T* text);        // 0x009A06B0 ?Insert@?$CBasicString@D@@QAEXKPAD@Z
};
class CCharStringData {                                  // Ego_r.pdb, 17 bytes packed
public:
    CBasicString<char> Data;                             // +0x00
    long               NoRefs;                           // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    void Insert(long position, char c);
protected:
    void MakeOwnCopyOfDataOrEmptyString();               // 0x0099EB40
private:
    CCharStringData* PStringData;                        // +0x00
};

void CCharString::Insert(long position, char c)
{
    MakeOwnCopyOfDataOrEmptyString();
    {
        char text[2];
        text[0] = c;
        text[1] = 0;
        PStringData->Data.Insert(position, text);
    }
}