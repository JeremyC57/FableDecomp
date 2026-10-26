// CCompositeBlendEntry::CCompositeBlendEntry(unsigned long key, const CCompositeBlendDesc& desc) -- build a {key, blend description} record: store the key, copy the description
// Leaf: ??0CCompositeBlendEntry
// Retail: Fable.exe (TLC, VC7.1) @ 0x00720050, 28 bytes (32 with padding), `ret 8`, returns `this`.
// Label: the manifest's "NUISystem::CUIStateDef::vfunc_25" comes from a bogus vtable read: slots 25/27/28/30 of
//   "vtable" 0x0125871C are UTF-16 text (0x00720050 = L"Pr"), and no code calls this body directly. The class
//   name is descriptive: the body is a two-member constructor whose second member is copied by
//   CCompositeBlendDesc's copy constructor 0x005BA48A (landed), in the composite-blend code band around 0x0071F000.
// Callees: CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc&) 0x005BA48A.
// Shape: the key is passed BY VALUE (stored straight from its slot, `mov [esi],eax`; a `const&` key would load
//   through the pointer), the description by reference; the key store is scheduled after the member's `lea`.
class CRGBColour { public:
    unsigned char B, G, R, A;
};

class CCompositeBlendDesc { public:
    unsigned long TextureIndex;                                 // +0x00
    long          BlendType;                                    // +0x04 (ECompositeBlendType)
    CRGBColour    DiffuseColour;                                // +0x08
    CRGBColour    SpecularColour;                               // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other);      // 0x005BA48A
};

struct CCompositeBlendEntry {
    unsigned long       Key;                                    // +0x00
    CCompositeBlendDesc Desc;                                   // +0x04
    CCompositeBlendEntry(unsigned long key, const CCompositeBlendDesc& desc);
};

CCompositeBlendEntry::CCompositeBlendEntry(unsigned long key, const CCompositeBlendDesc& desc)
    : Key(key), Desc(desc)
{
}