// NBankFileManager::SetHeaderDir(const CWideString&) -- stores the bank-file header directory
// Retail: Fable.exe 0x009A76C0, 12 bytes: `push ecx ; mov ecx,13CA7D4h ; call CWideString::operator= ; ret`.
// Identity: Ego_r.pdb ?SetHeaderDir@NBankFileManager@@YIXABVCWideString@@@Z (namespace function, __fastcall: the
// argument arrives in ecx and is pushed for the assignment). The directory is a file-static CWideString at 0x013CA7D4
// (its dynamic initializer 0x01224030 default-constructs a CWideString); its real name is not in the PDB name sets, so
// the existing landed spelling g_CCharString_013ca7d4 (staticinit 0x01224030) is reused to keep one name per address.
// Callees: CWideString::operator= 0x0099B7D0 (result dropped, so `call ; ret`, not a tail jump).
// Leaf: ?SetHeaderDir@NBankFileManager@@YIXABVCWideString@@@Z
class CWideString {
public:
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    void* PStringData;
};

extern CWideString g_CCharString_013ca7d4;              // 0x013CA7D4 bank header directory

namespace NBankFileManager {
    void __fastcall SetHeaderDir(const CWideString& dir);
}

void __fastcall NBankFileManager::SetHeaderDir(const CWideString& dir)
{
    g_CCharString_013ca7d4 = dir;
}