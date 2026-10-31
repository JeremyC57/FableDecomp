// PKCDefaultFileManager::CloseFile(unsigned long key) -- under the manager's lock, release one reference to an open file: trace an unknown key (line 65) and return; trace and break on a reference count already at zero (line 68); on the last reference remove it from the open-file map, CloseHandle the Win32 handle and delete the record
// Leaf: ?CloseFile@PKCDefaultFileManager@@QAEXK@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1A370, 404 bytes, thiscall `ret 4`, EH-framed (the lock, the two trace strings).
// Identity (class proven, method name descriptive; libpk has no PDB or RTTI): the traces name ".\PKCDefaultFileManager.cpp"
//   (0x012B30A8), and the body uses the landed GetFileSize 0x00C1A070's members -- the critical section +4 under
//   PKCProtectionInstance 0x00C0FFC0 / 0x00C0FFE0 and the open-file map +0x20 (PKCTRRedblackMap::Find 0x00C1AE50, 0 on a
//   hit, the key passed by the address of its own argument slot), whose value is the open file record (Win32 handle +0,
//   reference count +4). Caller: the async read thread 0x00C1A9C0, after a request's completion callback. The manifest
//   label "FileManager_CloseHandle" is descriptive.
// Callees: PKCTRRedblackMap::Find 0x00C1AE50 (bound) and the map's remove-by-key 0x00C1B7D0 (unbound: descriptive
//   `Remove`), the libpk trace stream (CDebugStream 0x00C0DEC0 / 0x00C0DF00 / ~0x00C0BA40, buffer g_pDebugBuffer 0x0139C900,
//   hook g_pfnErrorReport 0x0143BB2C, CDebugBreak 0x00C16750: the dominant landed spellings), KERNEL32 CloseHandle (import
//   0x0143FE30), operator delete 0x00BFE9BC.
// Shape: the map's Find and remove-by-key have same-TU bodies (`#pragma auto_inline(off)`; Find is the landed GetFileSize
//   body, Remove a stand-in that never keeps the key's address): VC7.1 then homes the first trace string in the dead `key`
//   argument slot as retail (declared only, it took the `file` slot). The zero test is `m_refCount <= 0` on the unsigned
//   count (retail `test ; ja`). The first trace has no debugger break. Land the leaf only.
class PKCCriticalSection;
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* hObject);   // 0x0143FE30
void __cdecl operator delete(void* block);                                   // 0x00BFE9BC

class PKCProtectionInstance { public:              // 8-byte scoped lock
    void* m_pSection;
    unsigned long m_state;
    PKCProtectionInstance(PKCCriticalSection* section);   // 0x00C0FFC0
    ~PKCProtectionInstance();                              // 0x00C0FFE0
};
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);       // 0x00C0DF00
    ~CDebugStream();                                       // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); };                     // 0x00C16750
extern char* g_pDebugBuffer;                               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*);   // 0x0143BB2C

struct SOpenFile_00c1a370 {
    void*         m_hFile;                 // +0x00 Win32 handle
    unsigned long m_refCount;              // +0x04
};
struct PKCTRRedblackNode_00c1a370 {
    PKCTRRedblackNode_00c1a370* Left;      // +0x00
    PKCTRRedblackNode_00c1a370* Right;     // +0x04
    PKCTRRedblackNode_00c1a370* Parent;    // +0x08
    long                        Colour;    // +0x0c
    unsigned long               Key;       // +0x10
    void*                       Value;     // +0x14
};
struct PKCTRRedblackMap {                  // +0x20: PKCTRRedblackMap (0x24 bytes)
    void*                       PAllocator;    // +0x00
    PKCTRRedblackNode_00c1a370  Sentinel;      // +0x04
    PKCTRRedblackNode_00c1a370* Root;          // +0x1c
    PKCTRRedblackNode_00c1a370* LastFound;     // +0x20
    long Find(const unsigned long* key, void** value);   // 0x00C1AE50
    long Remove(const unsigned long* key);               // 0x00C1B7D0
    void DeleteNode(PKCTRRedblackNode_00c1a370* node);   // stand-in helper (declaration only, never graded)
};
#pragma auto_inline(off)
long PKCTRRedblackMap::Find(const unsigned long* key, void** value)   // same-TU body (the landed GetFileSize 0x00C1A070 form)
{
    PKCTRRedblackNode_00c1a370* node = Root;
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
// Same-TU stand-in for the remove-by-key 0x00C1B7D0 (context only, not landed): it reads the key and does not keep its
// address, so VC7.1 may reuse the key's argument slot on the not-found path, as retail.
long PKCTRRedblackMap::Remove(const unsigned long* key)
{
    PKCTRRedblackNode_00c1a370* node = Root;
    while (node != &Sentinel && node->Key != *key)
        node = *key < node->Key ? node->Left : node->Right;
    if (node == &Sentinel)
        return 3;
    DeleteNode(node);
    return 0;
}
#pragma auto_inline(on)
struct PKCDefaultFileManager {
    void* m_vtable;                        // +0x00 (vtable 0x012B30E4)
    unsigned char m_section[0x18];         // +0x04 CRITICAL_SECTION
    void* m_pNodeAllocator;                // +0x1c
    PKCTRRedblackMap m_files;              // +0x20
    void CloseFile(unsigned long key);
};

void PKCDefaultFileManager::CloseFile(unsigned long key)
{
    SOpenFile_00c1a370* file;
    PKCProtectionInstance lock((PKCCriticalSection*)m_section);
    if (m_files.Find(&key, (void**)&file) != 0) {
        CDebugStream s(g_pDebugBuffer);
        s << ".\\PKCDefaultFileManager.cpp" << "(" << 65 << ") : " << "something has gone wrong here" << "\n";
        if (g_pfnErrorReport)
            g_pfnErrorReport(s.m_pBuffer);
        return;
    }
    if (file->m_refCount <= 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << ".\\PKCDefaultFileManager.cpp" << "(" << 68 << ") : " << "Reference count for file out of sync" << "\n";
            if (g_pfnErrorReport)
                g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    if (--file->m_refCount == 0) {
        m_files.Remove(&key);
        CloseHandle(file->m_hFile);
        delete file;
    }
}