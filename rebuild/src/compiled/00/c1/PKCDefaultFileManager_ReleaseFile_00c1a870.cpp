// PKCDefaultFileManager::ReleaseFile(unsigned long key) -- under the manager's lock look the open file up by key (false when unknown), assert its reference count is positive ("Reference count for file out of sync", PKCDefaultFileManager.cpp line 145), drop one reference and, at zero, remove it from the open-file map, close its Win32 handle and free the record; true when found
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
// Callees: PKCProtectionInstance 0x00C0FFC0 / 0x00C0FFE0 (shared libpk/PKCProtectionInstance.h), PKCTRRedblackMap::Find
//   0x00C1AE50 (same-TU body, the landed GetFileSize / CloseFile form), PKCTRRedblackMap::Remove 0x00C1B7D0 (descriptive,
//   declared only), CloseHandle (import 0x0143FE30), operator delete 0x00BFE9BC, and the trace block of the chunk 589/590
//   model: CString::operator<< 0x00C0DEC0 (text) / 0x00C0DF00 (long), ~CString 0x00C0BA40, CDebugBreak 0x00C16750,
//   _afxPchNil 0x0139C900, g_pfnAssertHook 0x0143BB2C.
// Shape: Find fills a `void*` local that is copied into the typed record pointer (kept in esi). With Find's body in the TU
//   (`#pragma auto_inline(off)`, it only stores through `value` before returning) VC7.1 sees `value` die at the copy and
//   homes the trace string in its slot and the CDebugBreak temporary in the byte below it, retail's 0x10-byte frame; with
//   Find declared only it reserves a separate slot (325v321). The count test is unsigned `> 0` (`test ; ja`).
#include "libpk/PKCProtectionInstance.h"
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
    void DeleteNode(PKCTRRedblackNode* node);            // stand-in helper (declaration only, never graded)
};
#pragma auto_inline(off)
long PKCTRRedblackMap::Find(const unsigned long* key, void** value)
{
    PKCTRRedblackNode* node = Root;
    while (node != &Sentinel) {
        if (*key == node->Key) {
            *value = node->Value;
            LastFound = node;
            return 0;
        }
        if (*key < node->Key)
            node = node->Left;
        else
            node = node->Right;
    }
    return 3;
}
#pragma auto_inline(on)

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