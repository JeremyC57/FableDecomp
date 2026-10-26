// Behaviour test for 009d56c0: final.cpp copied verbatim (operator new from <new>), the declared callees stubbed below so
// calls and arguments are observable. CBankFileAsync::OpenReadOnly(const CCharString&, unsigned long)
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[2048];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
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
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class CWideString { public:
    void* m_pData;
    ~CWideString();                                // 0x0099B510
};
#include <new>

void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> class CCountedPointer { public:        // {object, count block}
    T*           Data;
    SCountBlock* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer();                            // 0x009A9C40 (CThreadedFile), 0x009A9D60 (CRetailBank)
    CCountedPointer& operator=(const CCountedPointer& o);   // 0x009D6FD0 (CThreadedFile)
    void Reset(T* object);                         // 0x009A9C80 (CThreadedFile)
    T* operator->() const { return Data; }
};
class CDiskFileWin32;
class CThreadedFile { public:
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

// ---- stubs ----
CCharString::CCharString(const char* text, long length) : m_pData((void*)text) { CHECK(length == -1); }
CCharString::~CCharString() { m_pData = 0; }
CWideString::~CWideString() { m_pData = 0; }   // silent: VC7.1 (no NRVO) destroys one extra copy of the by-value path
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b) { char buf[96]; _snprintf(buf, sizeof buf, "progress(%s,%.1f,%d,%d)", (const char*)text.m_pData, progress, (int)a, (int)b); note(buf); }
static int g_baseResult = 1;
bool CBankFile_Retail::OpenReadOnly(const CCharString& bankHandle, unsigned long flags) { char b[96]; _snprintf(b, sizeof b, "base(%s,%lu)", (const char*)bankHandle.m_pData, flags); note(b); return g_baseResult != 0; }
void CBankFile_Retail::v00() {} void CBankFile_Retail::v01() {} void CBankFile_Retail::v02() {} void CBankFile_Retail::v03() {} void CBankFile_Retail::v04() {} void CBankFile_Retail::v05() {}
void CBankFile_Retail::v06() {} void CBankFile_Retail::v07() {} void CBankFile_Retail::v08() {} void CBankFile_Retail::v09() {} void CBankFile_Retail::v10() {} void CBankFile_Retail::v11() {}
static const char* g_pathText = "bank path";
CWideString CBankFile_Retail::GetBankPathname() { note("pathname"); CWideString w; w.m_pData = (void*)g_pathText; return w; }
CBankFileManager g_bankFileManager;
static CRetailBank g_retailBank; static SCountBlock g_threadBlock; static CThreadedFile g_retailThreaded; static int g_findResult = 1;
static SCountBlock g_bankBlock;
bool CBankFileManager::FindContainedBank(const CCharString& name, SBankEntryInfo* outInfo, CCountedPointer<CRetailBank>* outBank)
{
    char b[96]; _snprintf(b, sizeof b, "find(%s)", (const char*)name.m_pData); note(b);
    CHECK(outInfo->Type == 0 && outInfo->FirstEntry == 0 && outInfo->Offset == 0 && outInfo->Size == 0 && outInfo->Alignment == 0);
    CHECK(outBank->Data == 0 && outBank->Info == 0);
    if (!g_findResult) return false;
    outBank->Data = &g_retailBank; outBank->Info = &g_bankBlock; ++g_bankBlock.m_references; return true;
}
static const char* PtrName(CRetailBank*) { return "~bank"; }
static const char* PtrName(CThreadedFile*) { return "~tmp"; }
static const char* PtrName(CDiskFileWin32*) { return "~disk"; }
template <class T> CCountedPointer<T>::~CCountedPointer() { if (Info) { note(PtrName((T*)0)); --Info->m_references; } Data = 0; Info = 0; }
template <class T> CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer<T>& o)
{
    note("assign"); CHECK((void*)o.Data == (void*)&g_retailThreaded && o.Info == &g_threadBlock); if (o.Info) ++o.Info->m_references; Data = o.Data; Info = o.Info; return *this;
}
template <class T> void CCountedPointer<T>::Reset(T* object) { note("reset"); Data = object; Info = 0; }
static int g_threadedCtors = 0;
CThreadedFile::CThreadedFile() { ++g_threadedCtors; }
static const void* g_openPath = 0; static int g_openNonCached = -1;
bool CThreadedFile::Open(const CWideString& filename, bool nonCached) { note("open"); g_openPath = filename.m_pData; g_openNonCached = nonCached; return true; }

static void reset() { g_log[0] = 0; g_bankBlock.m_references = 5; g_threadBlock.m_references = 7; g_retailBank.ThreadedFile.Data = &g_retailThreaded; g_retailBank.ThreadedFile.Info = &g_threadBlock; g_retailBank.UseDMA = true; g_threadedCtors = 0; g_openPath = 0; g_openNonCached = -1; }

int main()
{
    CBankFileAsync bank; memset(&bank, 0, sizeof bank);
    CCharString handle("AUDIO");
    // 1. the base open fails: nothing else
    reset(); g_baseResult = 0; CHECK(bank.OpenReadOnly(handle, 3) == false); CHECKLOG("base(AUDIO,3);");
    // 2. retail mode, bank found: the retail bank's threaded file and DMA flag are taken, async enabled
    reset(); g_baseResult = 1; bank.RetailMode = true; g_findResult = 1; bank.DisableAsync = true;
    CHECK(bank.OpenReadOnly(handle, 3) == true);
    CHECKLOG("base(AUDIO,3);progress(Opening bank,-1.0,0,0);find(AUDIO);assign;~tmp;~bank;");
    CHECK(bank.ThreadedFile.Data == &g_retailThreaded && bank.ThreadedFile.Info == &g_threadBlock); CHECK(g_threadBlock.m_references == 8);   // one new reference for the member, the temporary released
    CHECK(bank.NonCached == true && bank.DisableAsync == false); CHECK(g_bankBlock.m_references == 5);
    // 3. retail mode, bank not found
    reset(); bank.RetailMode = true; g_findResult = 0; bank.DisableAsync = true;
    CHECK(bank.OpenReadOnly(handle, 0) == false); CHECKLOG("base(AUDIO,0);progress(Opening bank,-1.0,0,0);find(AUDIO);");   // the empty bank pointer's dtor has nothing to release CHECK(bank.DisableAsync == true);
    // 4. disk mode: a new threaded file opened on the bank's path with the NonCached setting
    reset(); bank.RetailMode = false; bank.NonCached = false; bank.DisableAsync = true; bank.ThreadedFile.Data = 0;
    CHECK(bank.OpenReadOnly(handle, 4) == true);
    CHECKLOG("base(AUDIO,4);progress(Opening bank,-1.0,0,0);reset;pathname;open;");
    CHECK(g_threadedCtors == 1 && bank.ThreadedFile.Data != 0 && g_openPath == (const void*)g_pathText && g_openNonCached == 0 && bank.DisableAsync == false);
    delete bank.ThreadedFile.Data;
    if (fails == 0) printf("009d56c0_TEST PASS\n"); else printf("009d56c0_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}