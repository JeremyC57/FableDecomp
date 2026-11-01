// NBankFileManager::GetHeaderDir() -- return a copy of the bank-file header directory SymbolHeaderDir
// Leaf: ?GetHeaderDir@NBankFileManager@@YI?AVCWideString@@XZ
// Retail: Fable.exe @ 0x009A76A0, 17 bytes: `push esi ; push 13CA7D4h ; mov esi,ecx ; call CWideString(const CWideString&) ; mov eax,esi ;
//   pop esi ; ret` -- a __fastcall function returning a CWideString by value (the hidden result in ecx).
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?GetHeaderDir@NBankFileManager@@YI?AVCWideString@@XZ (labels_chunk691; the PDB order
//   GetHeaderDir / SetHeaderDir is retail 0x009A76A0 / 0x009A76C0); the global is the PDB's NBankFileManager::SymbolHeaderDir (its
//   dynamic initializer 0x01224030 and the setter 0x009A76C0 name it). Was landed as a junk `C::Get` on a char stand-in g_str_9a76a0.
// Callees: CWideString(const CWideString&) 0x0099B720.
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
};
namespace NBankFileManager {
    extern CWideString SymbolHeaderDir;                 // 0x013CA7D4
    CWideString __fastcall GetHeaderDir();
}

CWideString __fastcall NBankFileManager::GetHeaderDir()
{
    return SymbolHeaderDir;
}
