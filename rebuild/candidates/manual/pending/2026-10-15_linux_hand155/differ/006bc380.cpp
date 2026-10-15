// CRegion::GetWorldMapNameGraphic() const -- look up the graphic of this region's world-map name in the UI locale graphics definition (0 when the region has no def or the name is not listed)
// Grade: DIFFER(119v129) -- the two hand releases: retail `lea eax,[esi+4] ; mov ecx,esi ; mov esi,[eax] ; dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi ; jne` (count via its address, `this` copied to ecx before the decrement), ours `mov eax,[esi+4] ; dec eax ; mov [esi+4],eax ; jne`; also `this` in esi (retail) vs edi. Lane gap: the inline counted release (GOTCHAS chunk 147) in another register assignment.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BC380, length 129 bytes.
// Callees: GetUIGraphics(CDefPointer<const NUISystem::CUILocaleGraphicsDef>*) @ 0x00415640 (`__fastcall`), the
// map<EWorldMapNameGraphic, unsigned long>::find @ 0x006BD0F0 (iterator through a hidden pointer; catalog
// "RBTreeMap_LowerBound"); the def's virtual slot 1 (destroy at reference count zero).
// Layout: retail CRegion (12-byte vectors, so the PDB offsets less 8): PDef +0x24 (CDefPointer<const CRegionDef>).
// CRegionDef NameGraphic +0xbc (PDB +0xc8); CUILocaleGraphicsDef WorldMapRegionName +0x3c (PDB +0x40), its node value
// at +0x14 (STLport node: color/parent/left/right, then the pair).
// Shape: the graphics def pointer is a local filled by the getter; the function releases it by hand on each exit
// (found: no null test; fall-through: null-tested), each release a load/dec/store of the count through its address.
// Leaf: GetWorldMapNameGraphic
struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +0x04
};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
};

template <class T> __forceinline void ReleaseDef(T* def)
{
    long& count = def->m_refCount;
    long n = count - 1;
    count = n;
    if (n == 0)
        def->Destroy();
}

struct SMapNode {
    long          Color;
    SMapNode*     Parent;
    SMapNode*     Left;
    SMapNode*     Right;
    long          Key;                              // +0x10 EWorldMapNameGraphic
    unsigned long Value;                            // +0x14
};

struct CNameGraphicMap {                            // map<EWorldMapNameGraphic, unsigned long>
    struct iterator {
        SMapNode* m_node;
        iterator() {}
        iterator(const iterator& o) : m_node(o.m_node) {}
    };
    SMapNode*     m_header;                         // +0x00
    unsigned long m_count;                          // +0x04
    unsigned long m_compare;                        // +0x08
    iterator find(const long& key);                 // 0x006BD0F0
};

namespace NUISystem {
struct CUILocaleGraphicsDef : CDefPointee {
    unsigned char   _pad08[0x34];
    CNameGraphicMap WorldMapRegionName;             // +0x3c
};
}

struct CRegionDef : CDefPointee {
    unsigned char _pad08[0xb4];
    long          NameGraphic;                      // +0xbc EWorldMapNameGraphic
};

void __fastcall GetUIGraphics(CDefPointer<NUISystem::CUILocaleGraphicsDef>* graphics);   // 0x00415640

struct CRegion {
    unsigned char           _pad00[0x24];
    CDefPointer<CRegionDef> PDef;                   // +0x24
    unsigned long GetWorldMapNameGraphic() const;
};

unsigned long CRegion::GetWorldMapNameGraphic() const
{
    CDefPointer<NUISystem::CUILocaleGraphicsDef> graphicsDef;
    GetUIGraphics(&graphicsDef);
    NUISystem::CUILocaleGraphicsDef* graphics = graphicsDef.Object;
    if (PDef.Object) {
        CNameGraphicMap& names = graphics->WorldMapRegionName;
        CNameGraphicMap::iterator it = names.find(PDef.Object->NameGraphic);
        if (it.m_node != names.m_header) {
            unsigned long result = it.m_node->Value;
            ReleaseDef(graphics);
            return result;
        }
    }
    if (graphics)
        ReleaseDef(graphics);
    return 0;
}
