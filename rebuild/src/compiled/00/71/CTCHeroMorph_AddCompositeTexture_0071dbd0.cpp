// CTCHeroMorph::AddCompositeTexture(EHeroMorphType type, float strength, CVectorMap<long, CTextureLayer>& layers) const -- for a morph of a noticeable strength, add one composite texture layer per texture-morph entry of that type in the hero's morph def: the entry's bank texture blended in with a white diffuse colour whose alpha is the strength
// Leaf: AddCompositeTexture
//
// Retail 0x0071DBD0, 384 bytes (`ret 0Ch`; the only caller is 0x0071DEF8).
// Callees: CDefClassBase::GetSubDef<const CHeroMorphDef>(CDefPointer*) const @ 0x0071F930, CTextureMorphs::GetAllMorphsOfType
// @ 0x0081B0A0, CCompositeBlendDesc::CCompositeBlendDesc(ECompositeBlendType, unsigned long, const CRGBColour&, const CRGBColour&)
// @ 0x006CABA0 (landed), CCompositeBlendDesc copy ctor @ 0x005BA48A (landed), the inlined push_back's growth path
// `_M_insert_overflow` @ 0x00721060, `_ftol` @ 0x00BFEA70, CRT `free` @ 0x00BFEA14 (the local vector's storage), the def's
// virtual slot 1 on release.
// Shape: the two colours live in the dead `type` / `strength` argument slots; the descriptor, the CTextureLayer and the
// pair<long, CTextureLayer> are three frame temporaries per entry (the copies go through the out-of-line copy ctor); the vector
// map's push_back is inlined and the map is flagged dirty after every insert.
// Layout: retail CVectorMap = 12-byte STLport vector + Compare (+0x0c) + Dirty (+0x0d); CTextureMorphEntry (20 bytes) = MorphType,
// TextureLayer, ReplacingTextureIndex, BankIndex, BlendType; CHeroMorphDef TextureMorphs at +0x28; CThing PDef +0x70.
#include <new>
#include <stdlib.h>

enum EHeroMorphType { HERO_MORPH_NONE = 0 };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
struct __false_type { __false_type() { } };

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->m_refCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};

class CRGBColour { public:                                     // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
class CCompositeBlendDesc { public:
    unsigned long       TextureIndex;                   // +0x00
    ECompositeBlendType BlendType;                      // +0x04
    CRGBColour          DiffuseColour;                  // +0x08
    CRGBColour          SpecularColour;                 // +0x0c
    CCompositeBlendDesc(ECompositeBlendType blendType, unsigned long textureIndex, const CRGBColour& diffuse,
                        const CRGBColour& specular);    // 0x006CABA0
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};
#pragma auto_inline(off)
// Same-TU body of the copy constructor (inline in the header; the linker kept one COMDAT copy at 0x005BA48A).
CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& other)
    : TextureIndex(other.TextureIndex)
    , BlendType(other.BlendType)
    , DiffuseColour(other.DiffuseColour)
    , SpecularColour(other.SpecularColour)
{
}
#pragma auto_inline(on)

class CTextureMorphEntry { public:                             // 20 bytes
    EHeroMorphType      MorphType;                      // +0x00
    long                TextureLayer;                   // +0x04
    long                ReplacingTextureIndex;          // +0x08
    unsigned long       BankIndex;                      // +0x0c
    ECompositeBlendType Blend;                          // +0x10
};
struct CTextureMorphEntryVector {                       // STLport vector<CTextureMorphEntry> (12 bytes, malloc allocator)
    CTextureMorphEntry* _M_start;
    CTextureMorphEntry* _M_finish;
    CTextureMorphEntry* _M_end_of_storage;
    CTextureMorphEntryVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CTextureMorphEntryVector() { if (_M_start) free(_M_start); }
    CTextureMorphEntry* begin() const { return _M_start; }
    CTextureMorphEntry* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
class CTextureMorphs { public:
    bool GetAllMorphsOfType(EHeroMorphType type, CTextureMorphEntryVector* entries) const;   // 0x0081B0A0
};
class CHeroMorphDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    CTextureMorphs TextureMorphs;                       // +0x28
};
class CThingBaseDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<const CHeroMorphDef>* out) const;   // 0x0071F930
};

void* FindSubDef(const CThingBaseDef* def);
void ReleaseDef(void* def);
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef (retail's body is out of line): CDefPointer::operator= on the out pointer.
bool CThingBaseDef::GetSubDef(CDefPointer<const CHeroMorphDef>* out) const
{
    void* def = FindSubDef(this);
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)

struct CTextureLayer {
    long                TextureID;                      // +0x00
    CCompositeBlendDesc Blend;                          // +0x04
    CTextureLayer(long textureID, const CCompositeBlendDesc& blend) : TextureID(textureID), Blend(blend) { }
};
struct CTextureLayerPair {                              // pair<long, CTextureLayer> (24 bytes)
    long          first;
    CTextureLayer second;
    CTextureLayerPair(const long& a, const CTextureLayer& b) : first(a), second(b) { }
};
struct CTextureLayerMap {                               // CVectorMap<long, CTextureLayer, CKeyPairCompareLess<...> >
    CTextureLayerPair* _M_start;
    CTextureLayerPair* _M_finish;
    CTextureLayerPair* _M_end_of_storage;
    unsigned char      Compare;                         // +0x0c
    bool               Dirty;                           // +0x0d
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
// Same-TU body of the growth template: reads the value only through its const reference and never touches the empty tag.
void CTextureLayerMap::_M_insert_overflow(CTextureLayerPair* pos, const CTextureLayerPair& x, const __false_type& tag,
                                          unsigned long fillLength, bool atEnd)
{
    CTextureLayerPair* p = (CTextureLayerPair*)GrowVectorStorage(this, fillLength);
    if (p)
        *(long*)p = x.first;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad00[0x70];
    CDefPointer<const CThingBaseDef> PDef;              // +0x70
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
};
class CTCHeroMorph : public CTCBase {
public:
    void AddCompositeTexture(EHeroMorphType type, float strength, CTextureLayerMap& layers) const;
};

void CTCHeroMorph::AddCompositeTexture(EHeroMorphType type, float strength, CTextureLayerMap& layers) const
{
    if (strength > 1e-4f) {
        CDefPointer<const CHeroMorphDef> def;
        if (ParentThing->PDef->GetSubDef(&def)) {
            CTextureMorphEntryVector entries;
            if (def->TextureMorphs.GetAllMorphsOfType(type, &entries) && !entries.empty()) {
                CRGBColour diffuse(0xff, 0xff, 0xff, (unsigned char)(strength * 255.0f));
                CRGBColour specular(0, 0, 0, 0);
                for (CTextureMorphEntry* it = entries.begin(); it != entries.end(); ++it)
                    layers.Add(it->TextureLayer,
                               CTextureLayer(it->ReplacingTextureIndex,
                                             CCompositeBlendDesc(it->Blend, it->BankIndex, diffuse, specular)));
            }
        }
    }
}