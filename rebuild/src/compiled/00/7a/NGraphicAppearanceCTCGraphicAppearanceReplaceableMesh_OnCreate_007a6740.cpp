// NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::OnCreate() -- vtable slot 4: once the owner exists, fetch the component's CReplaceableMeshDef sub-def from the owner's def
// Leaf: ?OnCreate@CTCGraphicAppearanceReplaceableMesh@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A6740, 22 bytes (+10 int3 padding).
// Name: slot 4 of the vtable 0x0126CB7C (rtti-vtables.tsv); slot 4 of the sibling CTCGraphicAppearanceSprite
// vtable is the landed OnCreate 0x0077C480 (empty), so this slot is the component's OnCreate.
// Callees: CDefClassBase::GetSubDef<CReplaceableMeshDef>(CDefPointer<T>&) @ 0x007A6CA0 (landed as
// GetSubDef_007a6ca0; its bool result is dropped).
// Layout: as the landed constructor 0x007A66A0 (checked: PDef at +0x1C): CTCBase (vptr, ParentThing +4), the appearance-component
// base vptr +0x0C, PrimHandles +0x10, PDef (CDefPointer<CReplaceableMeshDef const>) +0x1C. Retail CThing holds its
// def pointer (a CDefClassBase*) at +0x70.
// Shape: `this` is copied to eax first (kept for the `add eax,1Ch` argument), the owner's def is loaded
// into ecx and null-tested, the call is the `if` body; plain `ret`.
struct CDefClassBase;

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                                // +0x70 (retail)
};

struct CSubDef;

struct CDefPointer {                                    // CDefPointer<T>, as the landed GetSubDef 0x007A6CA0
    CSubDef* m_p;
};

struct CDefClassBase {
    bool GetSubDef_007a6ca0(CDefPointer& out);          // 0x007A6CA0
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};

namespace NGraphicAppearance {

struct CPrimitive;

struct CAGraphicAppearanceComponent {
    virtual void v00();
};

struct CTCGraphicAppearanceReplaceableMesh : CTCBase, CAGraphicAppearanceComponent {
    CPrimitive** PrimHandles[3];                        // +0x10 retail vector<CPrimitive*>
    CDefPointer  PDef;                                  // +0x1C
    void OnCreate();
};

void CTCGraphicAppearanceReplaceableMesh::OnCreate()
{
    if (ParentThing->PDef)
        ParentThing->PDef->GetSubDef_007a6ca0(PDef);
}

} // namespace NGraphicAppearance