// PKCDefaultFileManager::ReleaseFile(unsigned long key) -- under the manager's lock look the open file up by key (false when unknown), assert its reference count is positive ("Reference count for file out of sync", PKCDefaultFileManager.cpp line 145), drop one reference and, at zero, remove it from the open-file map, close its Win32 handle and free the record; true when found
// Grade: DIFFER(325v321) -- frame 0x14 vs retail 0x10: VC7.1 does not home the trace CString in the dead `value` slot (retail: value and line both at frame+4, CDebugBreak byte at frame+3), so every esp offset shifts by 4; the extra slot also moves &m_files from retail edi to ebp, and the record test CSEs `lea esi,[file+4]` / `cmp [esi],0` / `dec [esi]` where retail keeps the record in esi (`mov esi,[esp+0xc]; mov eax,[esi+4]; test eax,eax; ja` then `dec [esi+4]`). Tried: value in its own sibling block (332), inline typed Lookup helper on the map (337, gets retail edi/esi push order but same frame), count read into a local (326, gets `mov eax; test` but keeps the CSE). Next step: the "several short-lived EH locals in one dead slot" lane gap (chunk 589 0x00C4AEB0); try the trace block as an inline assert helper (its CString then lives in an inlinee scope) or Find as a same-TU nothrow body with the EH state store pinned before it.
// Leaf: ?ReleaseFile@PKCDefaultFileManager@@QAE_NK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1A870, 321 bytes, thiscall `ret 4`, EH-framed (state 0: the lock, 1: the trace
//   string).
// Identity: the libpk default file manager of the landed sweep1 / chunk 589 model (QueueRead 0x00C1A110: the critical
//   section +4, the open-file map +0x20 with PKCTRRedblackMap::Find 0x00C1AE50, the file record {Win32 handle +0, count
//   +4}); its own trace names `.\PKCDefaultFileManager.cpp` (0x012B30A8) and "Reference count for file out of sync"
//   (0x012B312C). Only caller: the file handle's close, slot 3 of vtable 0x012B4F4C (0x00C32FA0: `m_pManager->
//   ReleaseFile(m_key)` then 0x00C19FA0). libpk has no PDB or RTTI: the method name is descriptive. The catalog's
//   "FileManager_CloseHandleOnException" is a guess.
// Callees: PKCProtectionInstance 0x00C0FFC0 / 0x00C0FFE0, PKCTRRedblackMap::Find 0x00C1AE50 (declared only: retail stores
//   EH state 0 before it), PKCTRRedblackMap::Remove 0x00C1B7D0 (descriptive), CloseHandle (import 0x0143FE30), operator
//   delete 0x00BFE9BC, and the trace block of the chunk 589/590 model: CString::operator<< 0x00C0DEC0 (text) / 0x00C0DF00
//   (long), ~CString 0x00C0BA40, CDebugBreak 0x00C16750, _afxPchNil 0x0139C900, g_pfnAssertHook 0x0143BB2C.
// Shape: Find fills a `void*` local that is copied into the typed record pointer (kept in esi), so the trace string reuses
//   the dead `value` slot and the CDebugBreak temporary the byte below it (a `(void**)&file` out-argument keeps the record
//   in memory: 343v321); the count test is unsigned `> 0` (`test ; ja`).
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // 0x0143FE30
void __cdecl operator delete(void* block);                                  // 0x00BFE9BC

extern char* _afxPchNil;                        // 0x0139C900
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                         // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};

struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

struct SOpenFile {                              // the open-file record (map value)
    void*         m_hFile;                      // +0x00 Win32 handle
    unsigned long m_refCount;                   // +0x04 (QueueRead adds one per queued request)
};

class PKCCriticalSection {                      // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
};

class PKCProtectionInstance {                   // scoped guard: section + entry tick
public:
    PKCCriticalSection* m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);   // 0x00C0FFC0
    ~PKCProtectionInstance();                   // 0x00C0FFE0
};

class CMapNodeAllocator {                       // vtable 0x012B30EC: the map's heap node allocator
public:
    virtual void Free(void* node);              // slot 0 (0x00C1B320)
    virtual void* Allocate();                   // slot 1 (0x00C1B330)
};

struct PKCTRRedblackNode {                      // 0x18 bytes
    PKCTRRedblackNode* Left;                    // +0x00
    PKCTRRedblackNode* Right;                   // +0x04
    PKCTRRedblackNode* Parent;                  // +0x08
    long               Colour;                  // +0x0c
    unsigned long      Key;                     // +0x10
    void*              Value;                   // +0x14
};

class PKCTRRedblackMap {                        // 0x24 bytes
public:
    CMapNodeAllocator* PAllocator;              // +0x00
    PKCTRRedblackNode  Sentinel;                // +0x04
    PKCTRRedblackNode* Root;                    // +0x1c
    PKCTRRedblackNode* LastFound;               // +0x20
    long Find(const unsigned long* key, void** value);   // 0x00C1AE50: 0 on a hit
    long Remove(const unsigned long* key);               // 0x00C1B7D0
};

class PKCFileManager {                          // vtable 0x012B2FB8: the abstract file-manager interface
public:
    virtual ~PKCFileManager() { }
};

class PKCDefaultFileManager : public PKCFileManager {   // vtable 0x012B30E4
public:
    PKCCriticalSection m_lock;                  // +0x04
    CMapNodeAllocator  m_allocator;             // +0x1c
    PKCTRRedblackMap   m_files;                 // +0x20
    virtual ~PKCDefaultFileManager();
    bool ReleaseFile(unsigned long key);
};

bool PKCDefaultFileManager::ReleaseFile(unsigned long key)
{
    PKCProtectionInstance lock(&m_lock);
    void* value;
    if (m_files.Find(&key, &value) != 0)
        return false;
    SOpenFile* file = (SOpenFile*)value;
    if (!(file->m_refCount > 0)) {
        {
            CString line;
            line << ".\\PKCDefaultFileManager.cpp" << "(" << (long)145 << ") : " << "Reference count for file out of sync"
                 << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    if (--file->m_refCount == 0) {
        m_files.Remove(&key);
        CloseHandle(file->m_hFile);
        delete file;
    }
    return true;
}
