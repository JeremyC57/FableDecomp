// CMapInfo_00697c60::Clear() -- when the counted file pointer at +0xdc holds an object: the owner's slot at +0xd4 is assigned the pointer (CCountedPointer<CDiskFileWin32>::operator= @ 0x0069C380), then the +0xdc pointer is released (count block at +0xe0: references decremented, destroyed through the block's function and freed at zero) and cleared; the two bytes +0xe8 / +0xe9 are cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00697C60, length 96 bytes.
// Callees: 0x0069C380 (counted-pointer assignment), operator delete @ 0x00BFE9BC. The chunk label (CMapInfo::Clear) is propagated; the layout does not match CMapInfo. Shape: `this` in edi, the pointer's address in esi, one zero register (ebx), `ret`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CDiskFileWin32;
struct SCountBlock_00697c60 {                      // the shared count block of a CCountedPointer
    long m_references;                             // +0x00
    void (__fastcall* m_pfnDestroy)(CDiskFileWin32*);   // +0x04
    CDiskFileWin32* m_pObject;                     // +0x08
};
struct CCountedPointer_CDiskFileWin32 {            // {object, count block}
    CDiskFileWin32*       m_pObject;               // +0x00
    SCountBlock_00697c60* m_pCount;                // +0x04
    CCountedPointer_CDiskFileWin32& operator=(const CCountedPointer_CDiskFileWin32& other);   // 0x0069C380
    __forceinline void Release()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
};

struct CMapInfo_00697c60 {
    unsigned char                  _pad00[0xd4];
    CCountedPointer_CDiskFileWin32 m_owner;        // +0xd4
    CCountedPointer_CDiskFileWin32 m_file;         // +0xdc
    unsigned char                  _pade4[4];
    bool                           m_e8;           // +0xe8
    bool                           m_e9;           // +0xe9
    void Clear();
};

void CMapInfo_00697c60::Clear()
{
    if (m_file.m_pObject) {
        m_owner = m_file;
        m_file.Release();
    }
    m_e8 = false;
    m_e9 = false;
}
