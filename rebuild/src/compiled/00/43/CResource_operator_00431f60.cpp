// CResource::operator=(const CResource&) -- the implicit copy assignment: copy every member after the vptr, return *this
// Leaf: ??4CResource@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe @ 0x00431F60, 45 bytes, `ret 4`, returns this. Catalog label CTCPhysicsBase::ConstructFromParams is
// wrong: it copies the six dwords +0x04..+0x18 (not the vptr) -- exactly Ego_r.pdb CResource (IVRefCount, ResourceList,
// PrevResource, NextResource, ResourceSize, LastUsedFrame; 0x1c bytes) -- and its callers are the def assignment
// operators that start with their base copy (CThingBuildingDef::operator= 0x00431F10, CFrontEndDef 0x00432E60,
// CEngineVideoOptionsDef 0x00434450, CConfigOptionsDefaultsDef 0x004345A0, ...).
// Layout: Ego_r.pdb CResource with a real virtual destructor (out of line, 0x009FC550, as 0x0044FCD7); engine/CResource.h
// flattens the vptr into a data member, which the implicit operator= would then copy, so the class is local.
// Shape: /O2; the compiler-generated operator=, emitted out of line by an assignment in a helper.
class CResourceList;
class CResource {
public:
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    virtual ~CResource();                                // 0x009FC550
};
void AssignCResource(CResource& to, const CResource& from)   // emission helper, not a retail function
{
    to = from;
}