// PKCDefaultFileManager::OpenFile(const wchar16* fileName, unsigned long* key) -- under the manager's lock: trace and fail when the file does not exist (line 101) or CreateFile fails (line 116); otherwise record the Win32 handle in a new open-file record with one reference, hand out the next file key through `key` and insert the record into the open-file map (tracing and breaking on an allocation or insert failure, lines 122 and 132); true when opened
// Leaf: ?OpenFile@PKCDefaultFileManager@@QAE_NPBGPAK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1A510, 850 bytes, thiscall `ret 8`, EH-framed (the lock, the four trace strings).
// Identity (class proven, method name descriptive; libpk has no PDB or RTTI): the traces name ".\PKCDefaultFileManager.cpp"
//   (lines 101, 116, 122, 132); the members are the landed CloseFile 0x00C1A370's (critical section +4, open-file map +0x20,
//   the open-file record {Win32 handle, reference count}) plus the next file key +0x80. Only caller: the landed
//   CreateFromFile 0x00C1ABB0 (`Resolve(name, &name)`: the key overwrites the name argument).
// Callees: PKCProtectionInstance 0x00C0FFC0 / 0x00C0FFE0, KERNEL32 GetFileAttributesW / GetLastError / CreateFileW (IAT
//   0x0143FDBC / 0x0143FD58 / 0x0143FE2C), operator new 0x00BFEA1A (8 bytes), PKCTRRedblackMap_00c1b720::Insert 0x00C1B720 (the landed body, same TU),
//   the libpk trace stream (CString operator<< text 0x00C0DEC0 / long 0x00C0DF00 / unsigned int 0x00C0DEF0 / pointer
//   0x00C0DF20 -- the wide file name streams as a pointer -- ~CString 0x00C0BA40, g_pDebugBuffer 0x0139C900,
//   g_pfnErrorReport 0x0143BB2C) and CDebugBreak 0x00C16750.
// Shape: the GetLastError argument of each trace chain is evaluated before the chain (right-to-left argument order);
//   `*key = m_nextKey; ++m_nextKey;` re-reads the counter after the store through `key`.
//   The record pointer itself is the map value: `Insert(key, (void* const*)&file)` with Insert's landed body in the TU
//   (`#pragma auto_inline(off)`; it only reads through the pointer) keeps `file` in edi and stores it to its home slot
//   right after `new`, as retail. A separate `void* value = file;` copy gives retail's code with esi/edi swapped (c2
//   priority: file 55 over this 51), and Insert declared only keeps `file` in memory (861v850).
typedef unsigned short wchar16;
#include "libpk/PKCProtectionInstance.h"
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileAttributesW(const wchar16* fileName);   // 0x0143FDBC
extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();                               // 0x0143FD58
extern "C" __declspec(dllimport) void* __stdcall CreateFileW(const wchar16* name, unsigned long access, unsigned long share,
                                                             void* security, unsigned long disposition, unsigned long attributes,
                                                             void* templateFile);                    // 0x0143FE2C
void* __cdecl operator new(unsigned int size);                                                         // 0x00BFEA1A

class CString { public:
    char* m_pBuffer;
    CString(char* buffer) : m_pBuffer(buffer) { }
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
    CString& __fastcall operator<<(unsigned int value); // 0x00C0DEF0
    CString& __fastcall operator<<(const void* value);  // 0x00C0DF20
    ~CString();                                         // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                     // 0x00C16750
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

struct SOpenFile_00c1a370 {
    void*         m_hFile;                 // +0x00 Win32 handle
    unsigned long m_refCount;              // +0x04
};
struct PKCTRRedblackNode_00c1b720 {
    PKCTRRedblackNode_00c1b720* Left;      // +0x00
    PKCTRRedblackNode_00c1b720* Right;     // +0x04
    PKCTRRedblackNode_00c1b720* Parent;    // +0x08
    long                        Colour;    // +0x0c (0 black, 1 red)
    unsigned long               Key;       // +0x10
    void*                       Value;     // +0x14
};
enum { PKCTR_BLACK = 0, PKCTR_RED = 1 };
class PKCTRAllocator_00c1b720 {            // the node allocator (vtable 0x012B30EC: Free 0x00C1B320, Alloc 0x00C1B330)
public:
    virtual void Free(void* node);         // slot 0
    virtual void* Alloc();                 // slot 1: a 0x18-byte node
};
class PKCTRRedblackMap_00c1b720 {          // +0x20: the open-file map (0x24 bytes)
public:
    PKCTRAllocator_00c1b720*    PAllocator;                    // +0x00
    PKCTRRedblackNode_00c1b720  Sentinel;                      // +0x04 (0x18 bytes)
    PKCTRRedblackNode_00c1b720* Root;                          // +0x1c
    PKCTRRedblackNode_00c1b720* LastFound;                     // +0x20 (the lookup cache)
    void InsertFixup(PKCTRRedblackNode_00c1b720* x);           // 0x00C1B470
    long Insert(const unsigned long* key, void* const* value); // 0x00C1B720
};
// Same-TU body of the landed Insert 0x00C1B720 (context only, landed separately).
#pragma auto_inline(off)
long PKCTRRedblackMap_00c1b720::Insert(const unsigned long* key, void* const* value)
{
    PKCTRRedblackNode_00c1b720* parent = 0;
    PKCTRRedblackNode_00c1b720* current = Root;
    while (current != &Sentinel) {
        if (*key == current->Key)
            return 2;
        parent = current;
        if (*key < current->Key)
            current = current->Left;
        else
            current = current->Right;
    }
    PKCTRRedblackNode_00c1b720* x = (PKCTRRedblackNode_00c1b720*)PAllocator->Alloc();
    if (x == 0)
        return 1;
    x->Parent = parent;
    x->Left = &Sentinel;
    x->Right = &Sentinel;
    x->Colour = PKCTR_RED;
    x->Key = *key;
    x->Value = *value;
    if (parent) {
        if (*key < parent->Key)
            parent->Left = x;
        else
            parent->Right = x;
    } else {
        Root = x;
    }
    InsertFixup(x);
    LastFound = 0;
    return 0;
}
#pragma auto_inline(on)
struct PKCDefaultFileManager {
    void* m_vtable;                        // +0x00 (vtable 0x012B30E4)
    PKCCriticalSection m_lock;             // +0x04 CRITICAL_SECTION
    void* m_pNodeAllocator;                // +0x1c
    PKCTRRedblackMap_00c1b720 m_files;     // +0x20
    unsigned char m_44[0x80 - 0x44];       // +0x44 request queue, mailbox +0x58, done semaphore +0x78
    unsigned long m_nextKey;               // +0x80
    bool OpenFile(const wchar16* fileName, unsigned long* key);
};

bool PKCDefaultFileManager::OpenFile(const wchar16* fileName, unsigned long* key)
{
    PKCProtectionInstance lock(&m_lock);
    if (GetFileAttributesW(fileName) == (unsigned long)-1) {
        CString s(g_pDebugBuffer);
        s << ".\\PKCDefaultFileManager.cpp" << "(" << 101L << ") : " << "GetFileAttributes for file '" << (const void*)fileName
          << "' failed: " << (unsigned int)GetLastError() << "\n";
        if (g_pfnErrorReport)
            g_pfnErrorReport(s.m_pBuffer);
        return false;
    }
    void* handle = CreateFileW(fileName, 0x80000000, 3, 0, 4, 0, 0);
    if (handle == (void*)-1) {
        CString s(g_pDebugBuffer);
        s << ".\\PKCDefaultFileManager.cpp" << "(" << 116L << ") : " << "CreateFile '" << (const void*)fileName
          << "' failed: " << (unsigned int)GetLastError() << "\n";
        if (g_pfnErrorReport)
            g_pfnErrorReport(s.m_pBuffer);
        return false;
    }
    SOpenFile_00c1a370* file = new SOpenFile_00c1a370;
    if (!file) {
        {
            CString s(g_pDebugBuffer);
            s << ".\\PKCDefaultFileManager.cpp" << "(" << 122L << ") : " << "EMEM" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    file->m_refCount = 1;
    file->m_hFile = handle;
    *key = m_nextKey;
    ++m_nextKey;
    long result = m_files.Insert(key, (void* const*)&file);
    if (result != 0) {
        {
            CString s(g_pDebugBuffer);
            s << ".\\PKCDefaultFileManager.cpp" << "(" << 132L << ") : " << "Inserting tree item failed (EMEM or duplicate)" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    return true;
}