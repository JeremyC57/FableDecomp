// CFileManager_00c1a070::GetFileSize(unsigned long key, unsigned long* size) -- under the manager's critical-section lock, looks the key up in the open-file map at +0x20; an unknown key or a failing Win32 GetFileSize (0xFFFFFFFF, still stored to *size) returns false, else true
// Leaf: GetFileSize@CFileManager_00c1a070
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1A070, 158 bytes (+ int3 padding), __fastcall `ret 8`, EH-framed.
// Identity: the libpk default file manager (vtable 0x012B30E4; landed constructor 0x00C1AC40 / destructor 0x00C1A230):
//   the critical section +4 taken by the scoped lock (@ 0x00C0FFC0 / 0x00C0FFE0, the same 8-byte lock the inventory
//   walks use over the global section), the open-file map +0x20 (PKCTRRedblackMap::Find @ 0x00C1AE50: 0 on a hit, the key
//   passed by the address of its own argument slot), whose value is the open file record (Win32 handle at +0).
//   Caller: 0x00C32F4C. Callee: KERNEL32 GetFileSize (import 0x0143FDF0).
// Shape: retail never stores EH state 0 after the lock constructor and starts the frame with `mov eax,fs:[0]`: the map's
//   Find is compiled in the same TU (a nothrow body above, auto-inlining off) and GetFileSize is extern "C" (nothrow
//   under /EHsc), so no call between the lock's construction and destruction can throw -- a declaration-only Find gives
//   the `mov [esp+20h],0` store and the plain prologue order. The two `return false` paths tail-merge into one lock-destructor call (the size-failure path jumps into the
//   not-found block after its state store); the size test compares the call result, not the stored word.
// Label: the catalog's `FileManager_GetFileSizeByKey` is descriptive; the class is named by address.
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileSize(void* hFile, unsigned long* sizeHigh);   // 0x0143FDF0

struct CScopedLock_00c1a070 {              // 8-byte scoped lock
    void* m_pSection;
    unsigned long m_state;
    CScopedLock_00c1a070(void* section);   // 0x00C0FFC0
    ~CScopedLock_00c1a070();               // 0x00C0FFE0
};

struct SOpenFile_00c1a070 {
    void* m_hFile;                         // +0x00 Win32 handle
};

struct PKCTRRedblackNode_00c1a070 {
    PKCTRRedblackNode_00c1a070* Left;      // +0x00
    PKCTRRedblackNode_00c1a070* Right;     // +0x04
    PKCTRRedblackNode_00c1a070* Parent;    // +0x08
    long                        Colour;    // +0x0c
    unsigned long               Key;       // +0x10
    void*                       Value;     // +0x14
};

struct COpenFileMap_00c1a070 {             // +0x20: PKCTRRedblackMap (0x24 bytes; Find matched @ 0x00C1AE50)
    void*                       PAllocator;    // +0x00
    PKCTRRedblackNode_00c1a070  Sentinel;      // +0x04
    PKCTRRedblackNode_00c1a070* Root;          // +0x1c
    PKCTRRedblackNode_00c1a070* LastFound;     // +0x20
    long Find(const unsigned long* key, void** value);   // 0x00C1AE50
};

#pragma auto_inline(off)
long COpenFileMap_00c1a070::Find(const unsigned long* key, void** value)   // same-TU body (retail compiles it here)
{
    PKCTRRedblackNode_00c1a070* node = Root;
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

struct CFileManager_00c1a070 {
    void* m_vtable;                        // +0x00 (vtable 0x012B30E4)
    unsigned char m_section[0x18];         // +0x04 CRITICAL_SECTION
    void* m_pNodeAllocator;                // +0x1c (vptr-only allocator, vtable 0x012B30EC)
    COpenFileMap_00c1a070 m_files;         // +0x20
    bool GetFileSize(unsigned long key, unsigned long* size);
};

bool CFileManager_00c1a070::GetFileSize(unsigned long key, unsigned long* size)
{
    CScopedLock_00c1a070 lock(m_section);
    SOpenFile_00c1a070* file;
    if (m_files.Find(&key, (void**)&file) != 0)
        return false;
    unsigned long result = ::GetFileSize(file->m_hFile, 0);
    *size = result;
    if (result == 0xFFFFFFFF)
        return false;
    return true;
}