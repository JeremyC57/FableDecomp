// CTCHeroOnlineScoreboard::GetCachedImage(const CWideString&, long) -- returns the logo texture for an id: from PictureCache when present, else (loading SaveDir\<folder>\<id>\logo.* into a new 256x256 A8R8G8B8 texture when the file exists) caching the result first
// Leaf: GetCachedImage
// Flags: /GS
// Retail: Fable.exe @ 0x0055FC90, 560 bytes, `ret 0Ch` (hidden CTexturePointer result), /GS cookie (find data).
// Ego_r.pdb `?GetCachedImage@CTCHeroOnlineScoreboard@@AAE?AV?$CCountedPointer@VCTexture@@@@ABVCWideString@@J@Z`.
// Callees: map<long,CTexturePointer>::find 0x00566F00 (iterator by hidden pointer) / operator[] 0x005698A0,
// GetSaveDir 0x0041A5B0, operator+(const CWideString&, const CWideString&) 0x0099BE70 / (const CWideString&, const
// wchar_t*) 0x0099BF30, ~CWideString 0x0099B510, GFIntToCharString 0x0099F830, CCharString::ToWideString 0x0099E480,
// ~CCharString 0x0099EAE0, CWideString::operator+= 0x0099B8D0 / 0x0099B940, FindFirstFile 0x00999760, CWideString(const
// wchar_t*) 0x0099B6B0, operator new 0x00BFEA1A, CTexturePointer::Reset 0x005669C0, CPixelFormat::Initialise
// 0x009E3790, CTexture::Initialise 0x009FA280, CTexture::GetSurface 0x009F9E00, CSurface::LoadFromBitmapFileTo
// PreallocatedSurface 0x009F2DD0, CSurface::Uninitialise 0x009F2E20 (inline ~CSurface, vtable 0x0122F84C), the count
// block's destroy function, operator delete 0x00BFE9BC.
// Layout: retail PictureCache +0x10 (12-byte STLport map; node value +0x14); CCountedPointer {object, count block}, the
// count block {references, destroy(object) __fastcall, object} as the landed Reset 0x005669C0.
// Shape: same-TU noinline stand-ins for Reset and find (the texture's and iterator's addresses never escape, so its
// fields stay in edi/esi and the iterator shares the path temporary's frame word); operator= reads the source fields
// first (landed 0x008C0D10 form), giving the load/inc/store add-ref on the hit path.
#include <stddef.h>
typedef unsigned short wchar16;

struct CWideString;
struct CCharString {
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
    CWideString ToWideString() const;                // 0x0099E480
};
struct CWideString {
    void* PStringData;
    CWideString(const wchar16* text);                // 0x0099B6B0
    ~CWideString();                                  // 0x0099B510
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
    const CWideString& operator+=(const wchar16* text);        // 0x0099B940
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);       // 0x0099BF30
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
CWideString __fastcall GetSaveDir();                                                   // 0x0041A5B0
struct CFindFileData {
    wchar16       FileName[260];
    unsigned long Attributes;
};
struct CFindFileHandle {
    long _body[2];
};
bool __fastcall FindFirstFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);   // 0x00999760

struct C2DExtentsI {
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
};
struct CPixelFormat {
    long FormatIndex;
    void Initialise(long r, long g, long b, long a, bool alpha);   // 0x009E3790
};
struct CSurface {
    void*         PD3DSurface;
    unsigned long AllocationSource;
    void*         PAllocatedMemory;
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise();                             // 0x009F2E20
    void LoadFromBitmapFileToPreallocatedSurface(const CWideString& filename, unsigned long flags);   // 0x009F2DD0
};
struct CTexture {
    void*         PD3DTexture;
    unsigned long ByteLength : 28;
    unsigned long AllocationSource : 4;
    CTexture() : PD3DTexture(0), ByteLength(0), AllocationSource(0) { }
    bool Initialise(const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage, long pool, bool dynamic, bool renderTarget);   // 0x009FA280
    CSurface GetSurface(long level);                 // 0x009F9E00
};

struct SCountBlock {
    long m_references;                               // +0
    void (__fastcall* m_pfnDestroy)(void*);          // +4
    void* m_pObject;                                 // +8
};
struct CTexturePointer {                            // CTexturePointer
    CTexture*    Object;
    SCountBlock* Info;
    CTexturePointer() : Object(0), Info(0) { }
    CTexturePointer(const CTexturePointer& other) : Object(other.Object), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CTexturePointer()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
    CTexturePointer& operator=(const CTexturePointer& other)
    {
        SCountBlock* count = other.Info;
        CTexture* object = other.Object;
        if (Info != count) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Object = object;
            Info = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
    CTexture* operator->() const { return Object; }
    void Reset(CTexture* object);                    // 0x005669C0
};

struct SPictureNode {
    long                       _M_color;
    SPictureNode*              _M_parent;
    SPictureNode*              _M_left;
    SPictureNode*              _M_right;
    long                       first;                // +0x10
    CTexturePointer  second;               // +0x14
};
struct CPictureMap {                                 // std::map<long, CTexturePointer > (12 bytes)
    struct iterator {
        SPictureNode* _M_node;
        iterator() { }
    };
    SPictureNode* _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    iterator find(const long& key);                  // 0x00566F00
    CTexturePointer& operator[](const long& key);   // 0x005698A0
};

void __fastcall DestroyTexture(void* object);        // the count block's CTexture deleter

#pragma auto_inline(off)
// Same-TU stand-in for the out-of-line Reset 0x005669C0 (landed body shape).
__declspec(noinline) void CTexturePointer::Reset(CTexture* object)
{
    if (Info) {
        --Info->m_references;
        if (Info->m_references == 0) {
            Info->m_pfnDestroy(Info->m_pObject);
            delete Info;
        }
    }
    Info = 0;
    Object = object;
    if (object) {
        SCountBlock* block = new SCountBlock;
        block->m_references = 1;
        block->m_pfnDestroy = DestroyTexture;
        block->m_pObject = Object;
        Info = block;
    }
}
// Same-TU stand-in for the out-of-line map find 0x00566F00 (STLport lower_bound + key test).
__declspec(noinline) CPictureMap::iterator CPictureMap::find(const long& key)
{
    SPictureNode* y = _M_header;
    SPictureNode* x = _M_header->_M_parent;
    while (x != 0) {
        if (x->first < key)
            x = x->_M_right;
        else {
            y = x;
            x = x->_M_left;
        }
    }
    iterator result;
    result._M_node = (y == _M_header || key < y->first) ? _M_header : y;
    return result;
}
#pragma auto_inline(on)

class CTCHeroOnlineScoreboard {
public:
    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x10 - 0x04];
    CPictureMap   PictureCache;                      // +0x10
    CTexturePointer GetCachedImage(const CWideString& folder, long id);
};

CTexturePointer CTCHeroOnlineScoreboard::GetCachedImage(const CWideString& folder, long id)
{
    CTexturePointer texture;
    CPictureMap::iterator it = PictureCache.find(id);
    if (it._M_node == PictureCache._M_header) {
        CWideString path = GetSaveDir() + folder + (const wchar16*)L"\\";
        path += GFIntToCharString(id).ToWideString();
        path += (const wchar16*)L"\\logo.*";
        CFindFileHandle handle;
        CFindFileData data;
        if (FindFirstFile(path, handle, data)) {
            CWideString name(data.FileName);
            texture.Reset(new CTexture);
            CPixelFormat format;
            format.Initialise(8, 8, 8, 8, false);
            texture->Initialise(C2DExtentsI(256, 256), 1, format, 0, 1, false, false);
            texture->GetSurface(0).LoadFromBitmapFileToPreallocatedSurface(name, 3);
        }
        PictureCache[id] = texture;
    } else {
        texture = it._M_node->second;
    }
    return texture;
}