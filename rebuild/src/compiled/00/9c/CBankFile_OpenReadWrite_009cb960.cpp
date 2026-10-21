// CBankFile::OpenReadWrite(const CCharString& bankHandle, unsigned long flags, unsigned long alignment) -- reset the open state and either attach to the retail bank set (retail mode, not forced to disk) or open the bank's disk file read/write
// Leaf: ?OpenReadWrite@CBankFile@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CB960, length 218 bytes (+6 int3), virtual, ret 0xc.
// Callees: CCharString::operator= @ 0x0099EFB0, CWideString::operator= @ 0x0099B7D0, CWideString::~CWideString @ 0x0099B510;
// on the bank-file manager singleton @ 0x013CA79C (retail-mode byte +0x14 = 0x013CA7B0): GetRetailBankPath(const CCharString&)
// @ 0x009A7D40 (catalog "CBankFileManager_FindOrCreateBank"), GetBankPath(const CCharString&) @ 0x009A7CA0 (catalog
// "CBankFile::FindBank"), RegisterRetailBank(const CCharString&, CBankFile*) @ 0x009A8090 (catalog
// "CTCInventoryExperience::IsTutorialPlayed", propagated); this's virtual slots 11 (+0x2c, the retail open) and 4 (+0x10,
// OpenFileReadWrite(const CWideString&, ulong, ulong)).
// Layout: retail CBankFile = Ego_r.pdb CBankFile offsets -0x10 (the PDB CPackedUIntArray/CArray members are wider):
// OpenFlags +0x78, Filename (CWideString) +0x84, BankHandle (CCharString) +0x88, RetailMode +0x8c, ReadOnly +0x8d,
// WrittenToFile +0x8e, Solid +0x8f, ChangeCount +0x90, FileValid +0x94, UpdatingFlag +0x95, Alignment +0x98.
// rebuild/include/engine/CBankFile.h carries the PDB offsets, so the class is modelled locally.
// Shape: the by-value CWideString results are homed in dead argument slots (flags slot in the retail arm, the handle slot
// in the disk arm); the zero register ebx serves all the resets.
// The disk path is a named local built before the virtual call, so `alignment` is live across GetBankPath and VC7.1
// keeps it in ebp for the whole body (the argument pushes follow the path call, as retail).
struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CWideString {
    void* m_pData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                // 0x0099B510
};
class CBankFile;
struct CBankFileManager {
    unsigned char _pad[0x14];
    bool m_retailMode;                             // +0x14
    CWideString GetRetailBankPath(const CCharString& bankHandle);   // 0x009A7D40
    CWideString GetBankPath(const CCharString& bankHandle);         // 0x009A7CA0
    void RegisterRetailBank(const CCharString& bankHandle, CBankFile* bank);   // 0x009A8090
};
extern CBankFileManager g_bankFileManager;         // 0x013CA79C

class CBankFile {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void OpenFileReadWrite(const CWideString& filename, unsigned long flags, unsigned long alignment);   // slot 4 (+0x10)
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void OpenRetail();                     // slot 11 (+0x2c)
    virtual void OpenReadWrite(const CCharString& bankHandle, unsigned long flags, unsigned long alignment);
    unsigned char _pad04[0x74];                    // +0x04
    unsigned long OpenFlags;                       // +0x78
    void* BankFile_Data;                           // +0x7c CCountedPointer<CDiskFileWin32>
    void* BankFile_Info;                           // +0x80
    CWideString Filename;                          // +0x84
    CCharString BankHandle;                        // +0x88
    bool RetailMode;                               // +0x8c
    bool ReadOnly;                                 // +0x8d
    bool WrittenToFile;                            // +0x8e
    bool Solid;                                    // +0x8f
    unsigned long ChangeCount;                     // +0x90
    bool FileValid;                                // +0x94
    bool UpdatingFlag;                             // +0x95
    unsigned long Alignment;                       // +0x98
};

void CBankFile::OpenReadWrite(const CCharString& bankHandle, unsigned long flags, unsigned long alignment)
{
    ReadOnly = false;
    OpenFlags = flags | 0x1f8;
    BankHandle = bankHandle;
    ChangeCount = 0;
    FileValid = false;
    WrittenToFile = false;
    Alignment = alignment;
    UpdatingFlag = false;
    if (g_bankFileManager.m_retailMode && !(flags & 4)) {
        RetailMode = true;
        Solid = true;
        Filename = g_bankFileManager.GetRetailBankPath(bankHandle);
        g_bankFileManager.RegisterRetailBank(bankHandle, this);
        OpenRetail();
        return;
    }
    CWideString path = g_bankFileManager.GetBankPath(bankHandle);
    OpenFileReadWrite(path, flags, alignment);
}