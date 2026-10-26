// CTCSimpleAppearanceMorph::OnCreate() -- give the component a fresh mesh morph set and add one base-type (3) composite blend per texture replacement in the thing def's CTextureReplacementDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007275C0, 240 bytes (plain `ret`); CTCSimpleAppearanceMorph vtable 0x012664A4 slot 4.
// Callees: operator new @ 0x00BFEA1A (the 0x18-byte morph set), CCountedPointer<CMeshMorphSet>::Reset(CMeshMorphSet*) @ 0x007222F0
// (manifest label: a propagated Reset instantiation), CDefClassBase::GetSubDef_007086f0(CDefPointer&) @ 0x007086F0 (the
// 'CTextureReplacementDef' instantiation), CEngineTextureMorphSet::AddBlend(unsigned long, const CCompositeBlendDesc&) @ 0x006CA950;
// the defs' virtual slot 1.
// Layout: CTCSimpleAppearanceMorph PMeshMorphSet (CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>) +0x0c; retail
// CMeshMorphSet (0x18): MeshParts (12-byte CArray) +0x00, OriginalMeshTextureMorphs (vector<CEngineTextureMorph>) +0x0c;
// CThing PDef +0x70; CTextureReplacementDef Entries (8-byte {texture, replacement} records) +0x28/+0x2c; CCompositeBlendDesc
// and CRGBColour as the landed AddBlend.
// Shape: the thing def is read through a by-value CDefPointer getter (add-ref and release of the temporary, both count
// stores kept) into a raw pointer; the replacement def out pointer is released once at the end; the loop re-reads the entries and passes each blend as a frame temporary whose diffuse colour is built in
// place by the R,G,B,A-assigning CRGBColour constructor (a by-component blend constructor; opaque black, no specular).
#include <stddef.h>
void* operator new(size_t size);                        // 0x00BFEA1A
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CCompositeBlendDesc { public:
    unsigned long TextureIndex;                         // +0x00
    long          BlendType;                            // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                        // +0x08
    CRGBColour    SpecularColour;                       // +0x0c
    CCompositeBlendDesc(unsigned long texture, long type, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        : TextureIndex(texture), BlendType(type), DiffuseColour(r, g, b, a), SpecularColour(0, 0, 0, 0) {}
};
class CEngineTextureMorph;
class CEngineTextureMorphSet { public:                                 // vector<CEngineTextureMorph>
    CEngineTextureMorph* m_first;
    CEngineTextureMorph* m_last;
    CEngineTextureMorph* m_end;
    CEngineTextureMorphSet() : m_first(0), m_last(0), m_end(0) { }
public:
    void AddTextureMorph(long, const CCompositeBlendDesc&);   // 0x006CA950
};
struct CMeshMorphSetParts;
struct CMeshMorphSet {                                  // CTCAppearanceMorphBase::CMeshMorphSet
    struct CPartArray {                                 // CArray<CMeshMorphSetParts> (12 bytes)
        CMeshMorphSetParts* m_first; CMeshMorphSetParts* m_last; CMeshMorphSetParts* m_end;
        CPartArray() : m_first(0), m_last(0), m_end(0) { }
    } MeshParts;                                        // +0x00
    CEngineTextureMorphSet OriginalMeshTextureMorphs;           // +0x0c
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    void Reset(T* object);                              // 0x007222F0
    T* operator->() const { return Data; }
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
template <class T> struct CDefOut {                     // the out slot (released by the holder below)
    T* Object;
    CDefOut() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def read once from the out slot, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CTextureReplacementDef : public CDefBase { public:
    class CEntry { public: unsigned long OriginalTexture; unsigned long ReplacementTexture; };
    unsigned char _pad08[0x28 - 0x08];
    struct CEntryArray {                                // CArray<CEntry> (12 bytes)
        CEntry* m_first; CEntry* m_last; CEntry* m_end;
        long size() const { return m_last - m_first; }
        const CEntry& operator[](long i) const { return *(m_first + i); }
    } Entries;                                          // +0x28
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_007086f0(CDefPointer<CTextureReplacementDef>& out);   // 0x007086F0
};
void* FindDef_CDefClassBase_GetSubDef_007086f0_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_007086f0(CDefPointer<CTextureReplacementDef>& out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_007086f0_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                    // +0x70
    CDefPointer<CDefClassBase> GetPDef() const { return PDef; }
};
struct CTCSimpleAppearanceMorph {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CCountedPointer<CMeshMorphSet> PMeshMorphSet;       // +0x0c
    void OnCreate();
};

void CTCSimpleAppearanceMorph::OnCreate()
{
    PMeshMorphSet.Reset(new CMeshMorphSet);
    CDefClassBase* thingDef = ParentThing->GetPDef().Object;
    CDefPointer<CTextureReplacementDef> replacements;
    thingDef->GetSubDef_007086f0(replacements);
    for (long i = 0; i < replacements->Entries.size(); ++i) {
        const CTextureReplacementDef::CEntry& entry = replacements->Entries[i];
        PMeshMorphSet->OriginalMeshTextureMorphs.AddTextureMorph(entry.OriginalTexture,
            CCompositeBlendDesc(entry.ReplacementTexture, 3, 0, 0, 0, 255));
    }
}