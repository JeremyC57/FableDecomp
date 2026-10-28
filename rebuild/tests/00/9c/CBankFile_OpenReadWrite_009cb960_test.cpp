// Behaviour test for 009cb960: src/009cb960.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CBankFile::OpenReadWrite(const CCharString& bankHandle, unsigned long flags, unsigned long alignment)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
class CCharString { public:
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class CWideString { public:
    void* m_pData;
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
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

// ---- stubs and driver ----
#include <cstdlib>
#include <new>
static int g_charAssigns = 0, g_wideAssigns = 0, g_wideDtors = 0, g_retailPaths = 0, g_diskPaths = 0, g_registers = 0;
static CBankFile* g_registered = 0;
static void* g_lastHandle = 0;
const CCharString& CCharString::operator=(const CCharString& other) { ++g_charAssigns; m_pData = other.m_pData; return *this; }
const CWideString& CWideString::operator=(const CWideString& other) { ++g_wideAssigns; m_pData = other.m_pData; return *this; }
static bool g_inStub = false;   // the stubs' own by-value return copies are not counted
CWideString::~CWideString() { if (g_inStub) g_inStub = false; else ++g_wideDtors; }
CWideString CBankFileManager::GetRetailBankPath(const CCharString& bankHandle)
{ ++g_retailPaths; g_lastHandle = bankHandle.m_pData; CWideString r; r.m_pData = (void*)0x5150; g_inStub = true; return r; }
CWideString CBankFileManager::GetBankPath(const CCharString& bankHandle)
{ ++g_diskPaths; g_lastHandle = bankHandle.m_pData; CWideString r; r.m_pData = (void*)0xD15C; g_inStub = true; return r; }
void CBankFileManager::RegisterRetailBank(const CCharString& bankHandle, CBankFile* bank)
{ ++g_registers; g_lastHandle = bankHandle.m_pData; g_registered = bank; }
CBankFileManager g_bankFileManager;
void CBankFile::v00() {} void CBankFile::v01() {} void CBankFile::v02() {} void CBankFile::v03() {}
void CBankFile::OpenFileReadWrite(const CWideString&, unsigned long, unsigned long) {}
void CBankFile::v05() {} void CBankFile::v06() {} void CBankFile::v07() {} void CBankFile::v08() {} void CBankFile::v09() {}
void CBankFile::v10() {} void CBankFile::OpenRetail() {}
static int g_retailOpens = 0, g_diskOpens = 0;
static void* g_diskName = 0;
static unsigned long g_diskFlags = 0, g_diskAlign = 0;
struct CTestBank : CBankFile {
    virtual void OpenFileReadWrite(const CWideString& filename, unsigned long flags, unsigned long alignment)
    { ++g_diskOpens; g_diskName = filename.m_pData; g_diskFlags = flags; g_diskAlign = alignment; }
    virtual void OpenRetail() { ++g_retailOpens; }
};
int main()
{
    void* mem = calloc(1, sizeof(CTestBank));
    memset(mem, 0xcc, sizeof(CTestBank));
    CTestBank* bank = new (mem) CTestBank;
    CCharString handle; handle.m_pData = (void*)0x4A4D;
    // not retail mode: disk open with the original flags and alignment, state reset
    g_bankFileManager.m_retailMode = false;
    bank->OpenReadWrite(handle, 0x3, 0x800);
    CHECK(bank->OpenFlags == 0x1fb && bank->BankHandle.m_pData == (void*)0x4A4D && bank->Alignment == 0x800);
    CHECK(!bank->ReadOnly && !bank->WrittenToFile && !bank->FileValid && !bank->UpdatingFlag && bank->ChangeCount == 0);
    CHECK(g_diskPaths == 1 && g_diskOpens == 1 && g_diskName == (void*)0xD15C && g_diskFlags == 0x3 && g_diskAlign == 0x800);
    CHECK(g_retailOpens == 0 && g_retailPaths == 0 && g_registers == 0 && g_wideDtors == 1 && g_charAssigns == 1);
    // retail mode: retail path assigned, registered, retail open
    g_bankFileManager.m_retailMode = true;
    bank->RetailMode = false; bank->Solid = false;
    bank->OpenReadWrite(handle, 0x1, 0x10);
    CHECK(bank->RetailMode && bank->Solid && bank->Filename.m_pData == (void*)0x5150 && bank->Alignment == 0x10);
    CHECK(g_retailPaths == 1 && g_registers == 1 && g_registered == bank && g_retailOpens == 1 && g_wideAssigns == 1);
    CHECK(g_diskOpens == 1 && g_wideDtors == 2 && g_lastHandle == (void*)0x4A4D);
    // retail mode but forced to disk (flag 4)
    bank->OpenReadWrite(handle, 0x4, 0x20);
    CHECK(g_diskOpens == 2 && g_diskFlags == 0x4 && g_diskAlign == 0x20 && bank->OpenFlags == 0x1fc && g_retailOpens == 1);
    if (fails == 0) printf("009cb960_TEST PASS\n");
    return fails;
}
