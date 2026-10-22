// CBankFileAsync::OpenReadOnly(const CCharString& bankHandle, unsigned long flags) -- open the bank read-only through CBankFile, report progress, then take the asynchronous reader: the retail bank set's threaded file (and its DMA setting) when attached to one, else a new threaded file opened on the bank's own path; async reads enabled
// Leaf: ?OpenReadOnly@CBankFileAsync@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D56C0, length 352 bytes (+ int3 padding), virtual, ret 8.
// Callees: CBankFile::OpenReadOnly @ 0x009D06F0 (qualified base call), CCharString ctor @ 0x0099EBF0 ("..." @ 0x0129B884) /
//          dtor @ 0x0099EAE0, NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) @ 0x009E9F40
//          (__fastcall, (text, -1.0f, false, false)), CBankFileManager::FindContainedBank @ 0x009A7F80 on the manager
//          singleton @ 0x013CA79C, CCountedPointer<CThreadedFile>::operator= @ 0x009D6FD0 (catalog label is the
//          CDiskFileWin32 instantiation) / ~ @ 0x009A9C40, CCountedPointer<CRetailBank>::~ @ 0x009A9D60, operator new
//          @ 0x00BFEA1A, CThreadedFile ctor @ 0x0098DFD0 / Open(const CWideString&, bool) @ 0x0098E1E0,
//          CCountedPointer<CThreadedFile>::Reset @ 0x009A9C80, CBankFile::GetBankPathname @ 0x009CBF10 (CWideString by
//          hidden pointer), CWideString dtor @ 0x0099B510.
// Layout: retail CBankFileAsync as the landed ctor 0x009D5F80 / OpenFileReadOnly 0x009D5820 (CBankFile base 0x110:
//          RetailMode +0x8c; ThreadedFile +0x110, NonCached +0x148, DisableAsync +0x149); Ego_r.pdb CRetailBank: DiskFile
//          +4, ThreadedFile +0xc, UseDMA +0x14; FindContainedBank's 20-byte SBankEntryInfo.
// Shape: the progress text is a temporary homed in the dead `flags` argument slot; the contained bank's threaded file
//          comes through an inline by-value getter on a NAMED pointee local (`CRetailBank* retailBank = bank.Data;` -- the
//          temporary's two words are then loaded before either is stored, retail's `cmp eax,ebx` ahead of the stores),
//          assigned out of line and destroyed at once; the disk arm is the landed OpenFileReadOnly tail (GetBankPathname
//          temporary in the same dead slot).
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct CWideString {
    void* m_pData;
    ~CWideString();                                // 0x0099B510
};
void* operator new(unsigned int size);             // 0x00BFEA1A
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> struct CCountedPointer {        // {object, count block}
    T*           Data;
    SCountBlock* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer();                            // 0x009A9C40 (CThreadedFile), 0x009A9D60 (CRetailBank)
    CCountedPointer& operator=(const CCountedPointer& o);   // 0x009D6FD0 (CThreadedFile)
    void Reset(T* object);                         // 0x009A9C80 (CThreadedFile)
    T* operator->() const { return Data; }
};
struct CDiskFileWin32;
struct CThreadedFile {
    unsigned char _body[0x1c];
    CThreadedFile();                               // 0x0098DFD0
    bool Open(const CWideString& filename, bool nonCached);   // 0x0098E1E0
};
struct CRetailBank {                               // NBankFileManager::CRetailBank
    void* __vftable;
    CCountedPointer<CDiskFileWin32> DiskFile;      // +0x04
    CCountedPointer<CThreadedFile>  ThreadedFile;  // +0x0c
    bool UseDMA;                                   // +0x14
    CCountedPointer<CThreadedFile> GetThreadedFile() const { return ThreadedFile; }
};
struct SBankEntryInfo {                            // 20-byte per-entry record of a bank's index
    unsigned long Type;
    unsigned long FirstEntry;
    unsigned long Offset;
    unsigned long Size;
    unsigned long Alignment;
};
struct CBankFileManager {
    unsigned char _pad[0x14];
    bool m_retailMode;                             // +0x14
    bool FindContainedBank(const CCharString& name, SBankEntryInfo* outInfo, CCountedPointer<CRetailBank>* outBank);   // 0x009A7F80
};
extern CBankFileManager g_bankFileManager;         // 0x013CA79C

class CBankFile_Retail {                           // retail CBankFile (0x110 bytes)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual bool OpenReadOnly(const CCharString& bankHandle, unsigned long flags);   // 0x009D06F0
    unsigned char _pad04[0x88];                    // +0x04
    bool RetailMode;                               // +0x8c
    unsigned char _pad8d[0x110 - 0x8d];
    CWideString GetBankPathname();                 // 0x009CBF10
};
class CBankFileAsync : public CBankFile_Retail {
public:
    CCountedPointer<CThreadedFile> ThreadedFile;   // +0x110
    unsigned char _pad118[0x30];                   // +0x118
    bool NonCached;                                // +0x148
    bool DisableAsync;                             // +0x149
    virtual bool OpenReadOnly(const CCharString& bankHandle, unsigned long flags);
};

bool CBankFileAsync::OpenReadOnly(const CCharString& bankHandle, unsigned long flags)
{
    if (!CBankFile_Retail::OpenReadOnly(bankHandle, flags))
        return false;
    DisplayProgress("Opening bank", -1.0f, false, false);
    if (RetailMode) {
        SBankEntryInfo info;
        info.Type = 0;
        info.FirstEntry = 0;
        info.Offset = 0;
        info.Size = 0;
        info.Alignment = 0;
        CCountedPointer<CRetailBank> bank;
        if (!g_bankFileManager.FindContainedBank(bankHandle, &info, &bank))
            return false;
        CRetailBank* retailBank = bank.Data;
        ThreadedFile = retailBank->GetThreadedFile();
        NonCached = bank->UseDMA;
        DisableAsync = false;
        return true;
    }
    ThreadedFile.Reset(new CThreadedFile);
    ThreadedFile->Open(GetBankPathname(), NonCached);
    DisableAsync = false;
    return true;
}