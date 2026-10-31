// CTCHeroMorph::UpdateTattoos(CVectorMap<long, CTextureLayer>& layers) -- add one composite texture layer per worn tattoo: bank tattoos use their bank texture; a valid custom tattoo reuses the cached texture while the saved file is unchanged, otherwise loads the saved picture and the base alpha picture, builds the alpha channel, registers the texture and caches its id with the file time
// Grade: RELOCATION_MATCH -- verification copy: engine/CDefPointer.h text inlined with the proposed ENullDef ctor (not for landing)
// Was: DIFFER(1017v1027) -- register assignment: retail keeps the tattoo def in ebx and the texture id in eax / the dead tattoo-slot word (ours: id in ebx, def in esi), so the frame packs differently; structure, callees, temporaries and the release paths as retail
// Leaf: ?UpdateTattoos@CTCHeroMorph
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071D3B0, 1027 bytes, `ret 4`, aligned (and esp,-8) frame.
// Callees: GetSubDef<const CTattooDef> 0x005B1EFC, CTattooDef::IsInvalidCustomTattoo 0x00581765, CUserProfileManager::Get
//   0x0040D2A0, GetValidProfileName 0x00406350, save / custom-tattoo directories 0x0041A650 / 0x0041A6A0, operator+ 0x0099BE70 /
//   0x0099BF30, CCharString::ToWideString 0x0099E480, map find 0x0071FAC0 / operator[] 0x00721640, GetFileModifiedTime 0x009992A0,
//   CDateAndTime::operator== 0x009E9AD0, CPixelFormat::Initialise 0x009E3830, CTexture::LoadFromBitmapFile 0x009FA340 /
//   Uninitialise 0x009F9F70, CTCHeroMorph::CreateAlphaChannel 0x0071BCD0, CCompositeBlendDesc copy 0x005BA48A,
//   _M_insert_overflow 0x00721060 (landed AddCompositeTexture 0x0071DBD0 model).
// Layout: CTCHeroMorph Tattoos +0x1c, CustomToBankIndex +0x28 (landed AddTattoo 0x0071DA70); CTattooDef (engine/CTattooDef.h)
//   TextureLayer +0x2c, ReplacingTextureIndex +0x30, BankIndex +0x34, Blend +0x38, CustomFileName +0x4c, CustomFileNameBase +0x50.
#include <stddef.h>
#include <new>
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
CWideString __fastcall GetValidProfileName(const CWideString& name);   // 0x00406350
namespace NResourceDirectoryNames {
CWideString __fastcall GetSaveDir_0041a650();               // 0x0041A650
CWideString __fastcall GetCustomTattooDir();                // 0x0041A6A0
}
struct CUserProfileManager {
    unsigned char _pad00[8];
    CWideString ProfileName;                                // +0x08
    static CUserProfileManager* Get();                      // 0x0040D2A0
};
struct CDateAndTime {                                       // 28 bytes
    long Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime();
    bool operator==(const CDateAndTime& other) const;       // 0x009E9AD0
};
CDateAndTime __fastcall GetFileModifiedTime(const CWideString& path);   // 0x009992A0

struct CPixelFormat {
    long FormatIndex;
    void Initialise(long d3dFormat);                        // 0x009E3830
    CPixelFormat(long d3dFormat) { Initialise(d3dFormat); }
};
struct C2DExtentsI;
struct CTextureFileInfo;
struct IDirect3DTexture9;
class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                         // +0x00
    unsigned long      ByteLength : 28;                     // +0x04
    unsigned long      AllocationSource : 4;
    CTexture() : PD3DTexture(0) { *(unsigned long*)((char*)this + 4) = 0; }
    ~CTexture() { Uninitialise(); }
    void Uninitialise();                                    // 0x009F9F70
    bool LoadFromBitmapFile(const CWideString& filename, const C2DExtentsI* size, const CPixelFormat* format, unsigned long mipLevels,
                            unsigned long usage, long pool, unsigned long filter, unsigned long mipFilter, CTextureFileInfo* info,
                            long unused);                   // 0x009FA340
};
struct CTextureManager {                                    // virtual slot 36 (+0x90) registers a texture under a name
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual long AddTexture(CTexture& texture, const CCharString& name);   // slot 36
};
struct CEngineRenderer { unsigned char _pad00[0x2c]; CTextureManager* PTextures; };   // +0x2c
struct CEngine { unsigned char _pad00[0x28]; CEngineRenderer* PRenderer; };            // +0x28
extern CEngine* g_pEngine;                                  // 0x013B86A0

#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
class CTattooDef : public CSubDefClassBase {                // RTTI .?AVCTattooDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CTattooDef", -1); }   // inline
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    unsigned char _pad25[0x28 - 0x25];
    CCharString         TattooName;                         // +0x28
    long                TextureLayer;                       // +0x2c
    long                ReplacingTextureIndex;              // +0x30
    long                BankIndex;                          // +0x34
    ECompositeBlendType Blend;                              // +0x38
    bool RemovesAllTattoos;                                 // +0x3c
    bool Permanent;                                         // +0x3d
    long CoversBodyAreaFlags;                               // +0x40
    float VisibilityMultiplier;                             // +0x44
    long SpecificCoversBodyAreaFlags;                       // +0x48
    CCharString         CustomFileName;                     // +0x4c
    CCharString         CustomFileNameBase;                 // +0x50
    bool IsInvalidCustomTattoo() const;                     // 0x00581765
};
class CThingBaseDef : public CDefClassBase { };
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
// The retail GetSubDef<CTattooDef> body (landed 0x005B1EFC), noinline: only UpdateTattoos is graded.
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CTattooDef>(CDefPointer<const CTattooDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTattooDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
typedef CDefPointer<const CThingBaseDef> CThingDefPointer;
struct CRGBColour {                                         // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
struct CCompositeBlendDesc {
    unsigned long       TextureIndex;                       // +0x00
    ECompositeBlendType BlendType;                          // +0x04
    CRGBColour          DiffuseColour;                      // +0x08
    CRGBColour          SpecularColour;                     // +0x0c
    __forceinline CCompositeBlendDesc(ECompositeBlendType blendType, unsigned long textureIndex, const CRGBColour& diffuse,
                                      const CRGBColour& specular)
        : TextureIndex(textureIndex), BlendType(blendType), DiffuseColour(diffuse), SpecularColour(specular) { }
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};
#pragma auto_inline(off)
CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& other)
    : TextureIndex(other.TextureIndex)
    , BlendType(other.BlendType)
    , DiffuseColour(other.DiffuseColour)
    , SpecularColour(other.SpecularColour)
{
}
#pragma auto_inline(on)
struct __false_type { __false_type() { } };
struct CTextureLayer {
    long                TextureID;                          // +0x00
    CCompositeBlendDesc Blend;                              // +0x04
    CTextureLayer(long textureID, const CCompositeBlendDesc& blend) : TextureID(textureID), Blend(blend) { }
};
struct CTextureLayerPair {                                  // pair<long, CTextureLayer> (24 bytes)
    long          first;
    CTextureLayer second;
    CTextureLayerPair(const long& a, const CTextureLayer& b) : first(a), second(b) { }
};
struct CTextureLayerMap {                                   // CVectorMap<long, CTextureLayer, CKeyPairCompareLess<...> >
    CTextureLayerPair* _M_start;
    CTextureLayerPair* _M_finish;
    CTextureLayerPair* _M_end_of_storage;
    unsigned char      Compare;                             // +0x0c
    bool               Dirty;                               // +0x0d
    void _M_insert_overflow(CTextureLayerPair* pos, const CTextureLayerPair& x, const __false_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x00721060
    __forceinline void push_back(const CTextureLayerPair& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) CTextureLayerPair(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
    __forceinline void Add(const long& key, const CTextureLayer& value)
    {
        push_back(CTextureLayerPair(key, value));
        Dirty = true;
    }
};
void* GrowVectorStorage(void* vector, unsigned long count);
#pragma auto_inline(off)
void CTextureLayerMap::_M_insert_overflow(CTextureLayerPair* pos, const CTextureLayerPair& x, const __false_type& tag,
                                          unsigned long fillLength, bool atEnd)
{
    CTextureLayerPair* p = (CTextureLayerPair*)GrowVectorStorage(this, fillLength);
    if (p)
        *(long*)p = x.first;
}
#pragma auto_inline(on)

struct CBankEntry {                                         // pair<long, CDateAndTime> (32 bytes)
    long         first;
    CDateAndTime second;
    CBankEntry(const long& a, const CDateAndTime& b) : first(a), second(b) { }
};
struct CBankNode {                                          // _Rb_tree_node<pair<const CCharString, pair<long, CDateAndTime> > >
    unsigned char _base[0x10];
    CCharString   Key;                                      // +0x10
    CBankEntry    Value;                                    // +0x14
};
struct CBankMap {                                           // map<CCharString, pair<long, CDateAndTime> > (12 bytes)
    CBankNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    CBankNode* find(const CCharString& key);                // 0x0071FAC0
    CBankNode* end() { return _M_header; }
    CBankEntry& operator[](const CCharString& key);         // 0x00721640
};
struct CThingDefVector {
    CThingDefPointer* _M_start;
    CThingDefPointer* _M_finish;
    CThingDefPointer* _M_end_of_storage;
};

class CTCHeroMorph {
public:
    void* __vftable;
    void* ParentThing;                                      // +0x04
    unsigned char _pad08[0x1c - 0x08];
    CThingDefVector Tattoos;                                // +0x1c
    CBankMap CustomToBankIndex;                             // +0x28
    bool CreateAlphaChannel(CTexture& texture, CTexture& alpha);   // 0x0071BCD0
    void UpdateTattoos(CTextureLayerMap& layers);
};

void CTCHeroMorph::UpdateTattoos(CTextureLayerMap& layers)
{
    for (CThingDefPointer* it = Tattoos._M_start; it != Tattoos._M_finish; ++it) {
        CThingDefPointer item = *it;
        CDefPointer<const CTattooDef> tattoo(NULL_DEF);
        item->GetSubDef(&tattoo);
        if (tattoo->ReplacingTextureIndex != 0 && tattoo->BankIndex != 0) {
            long textureID = tattoo->BankIndex;
            if (tattoo->BankIndex == -1) {
                if (tattoo->IsInvalidCustomTattoo())
                    goto next;
                CUserProfileManager* profiles = CUserProfileManager::Get();
                CWideString path = NResourceDirectoryNames::GetSaveDir_0041a650() + GetValidProfileName(profiles->ProfileName) + L"\\"
                    + tattoo->CustomFileName.ToWideString();
                CBankNode* found = CustomToBankIndex.find(tattoo->CustomFileName);
                if (found != CustomToBankIndex.end() && found->Value.second == GetFileModifiedTime(path)) {
                    textureID = found->Value.first;
                } else {
                    CTexture texture;
                    CPixelFormat format(0x15);
                    if (!texture.LoadFromBitmapFile(path, 0, &format, 1, 0, 2, 1, 1, 0, 0))
                        continue;
                    CTexture alpha;
                    if (!alpha.LoadFromBitmapFile(NResourceDirectoryNames::GetCustomTattooDir() + tattoo->CustomFileNameBase.ToWideString(),
                                                  0, &format, 1, 0, 2, 1, 1, 0, 0))
                        continue;
                    CreateAlphaChannel(texture, alpha);
                    textureID = g_pEngine->PRenderer->PTextures->AddTexture(texture, tattoo->CustomFileName);
                    CustomToBankIndex[tattoo->CustomFileName] = CBankEntry(textureID, GetFileModifiedTime(path));
                }
            }
            layers.Add(tattoo->TextureLayer,
                       CTextureLayer(tattoo->ReplacingTextureIndex,
                                     CCompositeBlendDesc(tattoo->Blend, textureID, CRGBColour(0xff, 0xff, 0xff, 0xff), CRGBColour(0, 0, 0, 0))));
        }
    next:
        ;
    }
}
// x
