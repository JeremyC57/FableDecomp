// NBankFileManager::SetHeaderDir(const CWideString&) -- stores the bank-file header directory
// Retail: Fable.exe 0x009A76C0, 12 bytes: `push ecx ; mov ecx,13CA7D4h ; call CWideString::operator= ; ret`.
// Identity: Ego_r.pdb ?SetHeaderDir@NBankFileManager@@YIXABVCWideString@@@Z (namespace function, __fastcall: the
// argument arrives in ecx and is pushed for the assignment). The directory is a file-static CWideString at 0x013CA7D4
// (its dynamic initializer 0x01224030 default-constructs a CWideString): Ego_r.pdb / FableWin.pdb name its initializer
// NBankFileManager::`dynamic initializer for 'SymbolHeaderDir'' -- NBankFileManager::SymbolHeaderDir (naming wave 24, labels_chunk691;
// was the descriptive g_CCharString_013ca7d4).
// Callees: CWideString::operator= 0x0099B7D0 (result dropped, so `call ; ret`, not a tail jump).
// Leaf: ?SetHeaderDir@NBankFileManager@@YIXABVCWideString@@@Z
class CWideString {
public:
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    void* PStringData;
};

namespace NBankFileManager {
    extern CWideString SymbolHeaderDir;                 // 0x013CA7D4 bank header directory
    void __fastcall SetHeaderDir(const CWideString& dir);
}

void __fastcall NBankFileManager::SetHeaderDir(const CWideString& dir)
{
    SymbolHeaderDir = dir;
}