// CLHFileManager::CreateFile1(const char* fileName) -- allocate a 0x14-byte CLHFile (operator new @ 0x00BFEA1A) and construct it from this manager and the file name (0x00ADD380); null when the allocation failed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAB7A0, length 48 bytes.
// Vtable 0x0129D2B4 slot 1 (labelled vfunc_1). Callees: operator new, CLHFile's constructor @ 0x00ADD380 (labelled "CPlayerGuiDef_LoadAnimationSet"). Shape: the `new` null test, `xor eax,eax`, `ret 4`.
// Naming wave 17: the constructor is c676's ??0CLHFile@@QAE@PAVCLHFileManager@@PBD@Z; the landed SFileSpec_00aab7a0 reference parameter is that
//   narrow file-name pointer (name-only: the bytes are unchanged).
class CLHFileManager;
class CLHFile {                                    // 0x14 bytes, vtable 0x0129F094 (RTTI .?AVCLHFile@@ : LHA::CFile)
public:
    unsigned char _body[0x14];
    CLHFile(CLHFileManager* manager, const char* fileName);            // 0x00ADD380 (??0CLHFile@@QAE@PAVCLHFileManager@@PBD@Z)
};
class CLHFileManager {                             // vtable 0x0129D2B4
public:
    virtual CLHFile* CreateFile0(const unsigned short* fileName);   // slot 0, 0x00AAB7D0
    virtual CLHFile* CreateFile1(const char* fileName);             // slot 1, 0x00AAB7A0
};

CLHFile* CLHFileManager::CreateFile1(const char* fileName)
{
    return new CLHFile(this, fileName);
}