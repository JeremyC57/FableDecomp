// _global::OpenBankFile(CBankOwner_00c17640* owner, const unsigned short* path, long bankId, long flags) -- builds a local bank file, lets the bank loader @ 0x00C174A0 open `path` into it, records the path's multibyte name on it and reports whether it ended up open
// Grade: DIFFER(ours v 203) -- LTCG register arguments (lane gap): retail passes the loader 0x00C174A0 its path in esi and its first stack argument in ebx (the loader pushes/uses both without setting them); VC7.1 C++ cannot emit that convention, so this reference body passes them on the stack. One grade, no dummy parameters.
// Leaf: OpenBankFile
// Flags: /EHsc /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17640, 203 bytes, `__fastcall` (owner ecx, path edx, two stack
//   arguments, `ret 8`), EH-framed (the local bank file) and /GS-cookied (the 0x100-byte name buffer).
// Caller: 0x00C08F1F (the sound bank set-up; keeps the bool in bl). Callees: bank file ctor @ 0x00C308F0 / dtor
//   @ 0x00C30940 (0x34-byte local at esp+0xc), the loader @ 0x00C174A0 (ecx owner, edx &file, ebx bankId, esi path,
//   one stack argument = flags, `ret 4`), WideToMultiByte @ 0x00C16760 (this chunk), SetName @ 0x00C308A0,
//   IsOpen @ 0x00C307C0.
// Label correction: the catalog's `Process_BankFileWithGraphicsSetup` is propagated; descriptive name.
struct CBankOwner_00c17640;

struct CBankFile_00c17640 {
    unsigned char _data[0x34];
    CBankFile_00c17640();                  // 0x00C308F0
    ~CBankFile_00c17640();                 // 0x00C30940
    void SetName(const char* name);        // 0x00C308A0
    bool IsOpen();                         // 0x00C307C0
};

void __fastcall WideToMultiByte(const unsigned short* source, char* dest, int destSize);   // 0x00C16760
// retail convention: ecx owner, edx file, ebx bankId, esi path, [esp+4] flags
void __fastcall LoadBankFile(CBankOwner_00c17640* owner, CBankFile_00c17640* file, long flags,
                             long bankId, const unsigned short* path);                      // 0x00C174A0

bool __fastcall OpenBankFile(CBankOwner_00c17640* owner, const unsigned short* path, long bankId, long flags)
{
    CBankFile_00c17640 file;
    LoadBankFile(owner, &file, flags, bankId, path);
    char name[0x100];
    WideToMultiByte(path, name, 0x100);
    file.SetName(name);
    if (!file.IsOpen())
        return false;
    return true;
}
