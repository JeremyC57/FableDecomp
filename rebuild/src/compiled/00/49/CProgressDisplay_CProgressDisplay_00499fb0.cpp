// CProgressDisplay::~CProgressDisplay() -- loading-screen progress display destructor: stop the progress, then destroy the members in reverse and fall back to the CAProgressDisplay vtable
// Retail 0x00499FB0, 256 bytes. Label correction: catalogued as CTCInventoryMap::ResetPrimitiveHandles
// (propagated); the only caller is the deleting destructor 0x0049B7D0, slot 0 of vtable 0x012388B4 =
// CProgressDisplay (rtti-vtables.tsv), and this body stores that vtable first and the base CAProgressDisplay
// vtable 0x012387A0 last.
// Callees: StopProgress(bool, bool) 0x00499D90 (slot 2, called directly inside the destructor), ~CCharString
// 0x0099EAE0, ~CWideString 0x0099B510, the 8-byte CTexture element destructor 0x009F9F70 (manifest
// "CTexture::Uninitialise", landed under that spelling), free 0x00BFEA14.
// Layout: Ego_r.pdb CProgressDisplay with 12-byte retail STLport vectors (PDB 16): ProgressTextures +0x04,
// GuildSealTextures +0x10, LoadingScreenTextures +0x1C, LoadingScreenIndex +0x28, TextTags +0x2C, TextTagsPC
// +0x38, TextTagIndex +0x44, Text +0x48, TextTag +0x4C, LastTextDisplayed +0x50 (no engine header for it).
// Shape: each vector destructor is the inline STLport `_Destroy(first, last)` element loop (finish into ebx,
// start into edi) followed by `if (_M_start) free(_M_start)`; the base has a user-declared inline virtual
// destructor (an implicit one would drop the base vtable store).
// Leaf: ??1CProgressDisplay
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
class CWideString { public:
    void* m_pStringData;
    ~CWideString();                                // 0x0099B510
};
class CTexture { public:                                  // 8 bytes
    void* m_pTexture;
    void* m_pHandle;
    void Uninitialise();                           // 0x009F9F70 ?Uninitialise@CTexture@@QAEXXZ
    ~CTexture() { Uninitialise(); }
};

template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T> struct CVector {                // STLport vector<T> (12 bytes, malloc allocator)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

class CAProgressDisplay { public:                         // vtable 0x012387A0
    virtual ~CAProgressDisplay() {}
    virtual void v01();
    virtual void StopProgress(bool a, bool b) = 0; // slot 2
};

struct CProgressDisplay : CAProgressDisplay {      // vtable 0x012388B4
    CVector<CTexture>    ProgressTextures;         // +0x04
    CVector<CTexture>    GuildSealTextures;        // +0x10
    CVector<CTexture>    LoadingScreenTextures;    // +0x1c
    long                 LoadingScreenIndex;       // +0x28
    CVector<CCharString> TextTags;                 // +0x2c
    CVector<CCharString> TextTagsPC;               // +0x38
    long                 TextTagIndex;             // +0x44
    CWideString          Text;                     // +0x48
    CCharString          TextTag;                  // +0x4c
    CCharString          LastTextDisplayed;        // +0x50
    virtual ~CProgressDisplay();
    virtual void v01();
    virtual void StopProgress(bool a, bool b);     // 0x00499D90
};

CProgressDisplay::~CProgressDisplay()
{
    StopProgress(false, false);
}