// CRetailVector_SOcclusionEntry_009c2f50::~CRetailVector_SOcclusionEntry_009c2f50() -- destroy each 60-byte element, then free the storage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C2F50, length 58 bytes.
// Callees: the element's texture-handle release @ 0x009F9F70 (member at +0x18), CWideString::~CWideString
// @ 0x0099B510 (member at +0x14), free @ 0x00BFEA14 on the storage.
// The manifest label (CCombatWheel::ResetRings) is propagated: the three callers are
// CEnginePrimitiveManagerOcclusionBase::~CEnginePrimitiveManagerOcclusionBase @ 0x009C2FD0 on its members
// at +0x3e7c / +0x3e88 / +0x3e94. The element destructor runs its members in reverse order (handle, then
// string) with the bounds held in registers (they are by-value arguments of the inlined destroy loop);
// the base part re-reads `first` and frees it when non-null (SGI _Vector_base). Named by what it does.
extern "C" void free(void* block);             // 0x00BFEA14

struct CWideString {
    void* m_pData;
    ~CWideString();                            // 0x0099B510
};

struct CTextureHandle {                        // {texture, flags}: release through the texture's vslot 2, clear the top flag nibble
    void*         m_pTexture;
    unsigned long m_flags;
    ~CTextureHandle();                         // 0x009F9F70
};

struct SOcclusionEntry {                       // 60 bytes
    unsigned char  _pad0[0x14];
    CWideString    m_name;                     // +0x14
    CTextureHandle m_texture;                  // +0x18
    unsigned char  _pad1[0x1c];                // +0x20..+0x3b
};

struct CRetailVectorBase_SOcclusionEntry {
    SOcclusionEntry* first;
    SOcclusionEntry* last;
    SOcclusionEntry* end_of_storage;
    ~CRetailVectorBase_SOcclusionEntry()
    {
        if (first)
            free(first);
    }
};

struct CRetailVector_SOcclusionEntry_009c2f50 : CRetailVectorBase_SOcclusionEntry {
    ~CRetailVector_SOcclusionEntry_009c2f50();
};

CRetailVector_SOcclusionEntry_009c2f50::~CRetailVector_SOcclusionEntry_009c2f50()
{
    SOcclusionEntry* entry = first;
    SOcclusionEntry* entryEnd = last;
    for (; entry != entryEnd; ++entry)
        entry->~SOcclusionEntry();
}