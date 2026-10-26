// std::fill(CShoveHaPennyRegionDef* first, CShoveHaPennyRegionDef* last, const CShoveHaPennyRegionDef& val) -- assign val to every shove-ha'penny region def of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x007D1420, 144 bytes, `ret 4`: __fastcall (first ecx, last edx -> edi, the value reference on
// the stack). Only call site 0x007D25FF in the vector insert path at 0x007D24F0 (catalogued vector_Insert_CBrainUpdateZone),
// reached from vector::resize 0x007D2670 whose wrapper 0x007D26E0 builds the default element: CResource constructor
// 0x009FBEC0, vtable 0x0124383C (rtti-vtables: CShoveHaPennyRegionDef), the three low flag bits cleared (`and dl,0F8h`).
// Identity: the catalog's `_Fill<pair<float, CSubtitledMessage::CCameraTag>*>` is propagated; the element is
// CShoveHaPennyRegionDef (Ego_r.pdb, 48 bytes: ten unnamed base words, Start +0x28, Score +0x2C) over CResource
// (Ego_r.pdb, 28 bytes: vptr, a count word set to 1 by the constructor, ResourceList +8, PrevResource +0xC,
// NextResource +0x10, ResourceSize +0x14, LastUsedFrame +0x18).
// Shape: the polymorphic element's implicit copy assignment never copies the vptr (+0 untouched); it copies the base
// words +4..+0x18, the two def words, each of the three one-bit flags at +0x24 by read-modify-write (`xor ; and 1/2/4 ;
// xor`, the other bits of the byte kept), then Start and Score through integer registers.
// Local base model, not engine/CResource.h: the generated header flattens the vptr into a plain `void* __vftable`,
// which the implicit assignment would copy -- retail never writes +0.
class CResourceList;
class CResource { public:                              // 28 bytes
    virtual void v00();
    long           m_refCount;                  // +0x04 (1 after construction)
    CResourceList* ResourceList;                // +0x08
    CResource*     PrevResource;                // +0x0C
    CResource*     NextResource;                // +0x10
    unsigned long  ResourceSize;                // +0x14
    unsigned long  LastUsedFrame;               // +0x18
};
class CShoveHaPennyRegionDef : public CResource { public:     // 48 bytes
    long          m_def1c;                      // +0x1C (unnamed in the PDB)
    long          m_def20;                      // +0x20 (unnamed in the PDB)
    unsigned char m_flag0 : 1;                  // +0x24 bit 0
    unsigned char m_flag1 : 1;                  // +0x24 bit 1
    unsigned char m_flag2 : 1;                  // +0x24 bit 2
    float         Start;                        // +0x28
    float         Score;                        // +0x2C
};

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CShoveHaPennyRegionDef*, CShoveHaPennyRegionDef*, const CShoveHaPennyRegionDef&);
}