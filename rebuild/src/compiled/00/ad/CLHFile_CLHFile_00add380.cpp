// CLHFile::CLHFile(CLHFileManager* manager, const char* fileName) -- create the counted CThreadedFile, flag a ".WMA" extension (upper-cased) for streamed-read priority, and open the file (read-only) under its wide name
// Leaf: ??0CLHFile@@QAE@PAVCLHFileManager@@PBD@Z
// Retail: Fable.exe (TLC, VC7.1). Two adjacent CLHFile constructors of one TU: 0x00ADD380 (276 bytes, narrow file name) and
//   0x00ADD4A0 (274 bytes, wide file name), both __fastcall `ret 8`, followed by 14 int3 bytes and an UNLISTED 10-byte
//   function at 0x00ADD5C0 (`mov edx,[ecx] ; xor eax,eax ; test edx,edx ; sete al ; ret`, no references), then the
//   CLHFileCompletionCallback ??_G 0x00ADD5D0 and the CLHFile ??_G 0x00ADD5F0.
// Identity: both bodies store vtable 0x0129F094 first (RTTI .?AVCLHFile@@ : LHA::CFile, rtti-vtables slots 0..4 = the landed
//   GetSize 0x00ADD160 / Read 0x00ADD240 / GetBlockSize 0x00ADD190 / Close 0x00ADD330 / ??_G 0x00ADD5F0) and return this; their
//   only callers are CLHFileManager's vtable 0x0129D2B4 slots 1 (0x00AAB7A0 -> 0x00ADD380) and 0 (0x00AAB7D0 -> 0x00ADD4A0),
//   each `new(0x14)` then `push name ; push this(manager)`. The second argument is the file name: it feeds
//   CCharString(const char*, long) / CWideString(const unsigned short*) and the matching CAFile::SplitPath overload. The
//   first argument (the manager) is never read. CLHFile / LHA have no PDB symbols, so the parameter types are the ones
//   the bytes and callers prove (manager = the caller's CLHFileManager this, no conversion). Catalog labels
//   CPlayerGuiDef_LoadAnimationSet / PlayerGuiDef_ctor are propagated and wrong.
// Callees: operator new 0x00BFEA1A (0x1C = sizeof(CThreadedFile), FableWin/Ego_r layout), ??0CThreadedFile@@QAE@XZ
//   0x0098DFD0, ?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z 0x009A9C80 (out of line here, as retail),
//   CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / ToUpper 0x0099EE20 / ToWideString 0x0099E480,
//   CWideString() 0x0099AED0 / (const unsigned short*) 0x0099B6B0 / ~ 0x0099B510 / ToCharString 0x0099B2C0,
//   CAFile::SplitPath 0x009976D0 / 0x00997620 (static __fastcall), CBasicString<char>::operator==(const char*) 0x004115A0,
//   CThreadedFile::Open(const CWideString&, bool) 0x0098E1E0. Literals ".WMA" 0x0129F0A8, "" 0x0122D70E.
// Shape: m_unknown0C is the only init-list member (stored between the counted pointer's zeroing and `new`); the Reset and
//   the m_bIsWMA = false store are body statements. The forced-inline CCharString == ".WMA" tests the data block (none:
//   the intrinsic strcmp against "", `repe cmpsb` over 5 bytes). The narrow form opens CCharString(fileName).ToWideString()
//   (the hidden result lands in the dead argument slot after `push 1`); the wide form passes fileName through the implicit
//   CWideString(const unsigned short*) conversion, which VC7.1 constructs in the argument slot BEFORE `push 1` (an explicit
//   CWideString(fileName) temporary is built after it, a named local takes a new frame slot: probes 3 / 4).
// Layout: CThreadedFile restates the Ego_r.pdb members (no engine/CThreadedFile.h); CCountedPointer from engine/CCountedPointer.h.
//   LHA::CFile vtable 0x0129F06C = four _purecall slots + the virtual destructor (slot 4).
#include "engine/CCountedPointer.h"
#include <string.h>

template <class T> class CBasicString { public:
    T*            PData;
    unsigned long StringLength;
    bool operator==(const T* text) const;                         // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
};
class CWideString;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                                 // 0x0099E4B0
    CCharString(const char* text, long length = -1);               // 0x0099EBF0
    ~CCharString();                                                // 0x0099EAE0
    CCharString ToUpper() const;                                   // 0x0099EE20
    CWideString ToWideString() const;                              // 0x0099E480
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CWideString {
public:
    void* PStringData;
    CWideString();                                                 // 0x0099AED0
    CWideString(const unsigned short* text);                       // 0x0099B6B0
    ~CWideString();                                                // 0x0099B510
    CCharString ToCharString() const;                              // 0x0099B2C0
};
enum EThreadedFileDeviceID { };
class CAFile {
public:
    static void __fastcall SplitPath(const CCharString& path, CCharString* drive, CCharString* dir, CCharString* name, CCharString* ext);   // 0x009976D0
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir, CWideString* name, CWideString* ext);   // 0x00997620
};
class CThreadedFile {
public:
    CThreadedFile();                                               // 0x0098DFD0
    virtual ~CThreadedFile();
    bool Open(const CWideString& fileName, bool readOnly);         // 0x0098E1E0
    EThreadedFileDeviceID DeviceID;                                // +0x04
    void*                 FileHandle;                              // +0x08
    CWideString           Filename;                                // +0x0C
    unsigned long         Length;                                  // +0x10
    unsigned long         PhysicalSortKey;                         // +0x14
    bool                  OpenedForWrite;                          // +0x18
    bool                  OpenFlag;                                // +0x19
};
namespace LHA {
class CFileReadCallback;
class CFile {                                                      // vtable 0x0129F06C
public:
    virtual bool GetSize(unsigned long* size) = 0;
    virtual bool Read(unsigned long block, unsigned long blockCount, unsigned long reserved, void* buffer,
                      void* userData, CFileReadCallback* callback) = 0;
    virtual long GetBlockSize() = 0;
    virtual void Close() = 0;
    virtual ~CFile() { }
};
}
class CLHFileManager;                                              // RTTI .?AVCLHFileManager@@ : LHA::CFileManager

class CLHFile : public LHA::CFile {                                // vtable 0x0129F094
public:
    CCountedPointer<CThreadedFile> m_pFile;                        // +0x04
    unsigned long                  m_unknown0C;                    // +0x0C
    bool                           m_bIsWMA;                       // +0x10
    CLHFile(CLHFileManager* manager, const char* fileName);
    CLHFile(CLHFileManager* manager, const unsigned short* fileName);
    virtual bool GetSize(unsigned long* size);
    virtual bool Read(unsigned long block, unsigned long blockCount, unsigned long reserved, void* buffer,
                      void* userData, LHA::CFileReadCallback* callback);
    virtual long GetBlockSize();
    virtual void Close();
    virtual ~CLHFile();                                            // 0x00ADD1F0 (landed)
};

CLHFile::CLHFile(CLHFileManager*, const char* fileName)
    : m_unknown0C(0)
{
    m_pFile.Reset(new CThreadedFile);
    m_bIsWMA = false;
    CCharString ext;
    CAFile::SplitPath(CCharString(fileName), 0, 0, 0, &ext);
    if (ext.ToUpper() == ".WMA")
        m_bIsWMA = true;
    m_pFile->Open(CCharString(fileName).ToWideString(), true);
}

CLHFile::CLHFile(CLHFileManager*, const unsigned short* fileName)
    : m_unknown0C(0)
{
    m_pFile.Reset(new CThreadedFile);
    m_bIsWMA = false;
    CWideString ext;
    CAFile::SplitPath(CWideString(fileName), 0, 0, 0, &ext);
    if (ext.ToCharString().ToUpper() == ".WMA")
        m_bIsWMA = true;
    m_pFile->Open(fileName, true);
}