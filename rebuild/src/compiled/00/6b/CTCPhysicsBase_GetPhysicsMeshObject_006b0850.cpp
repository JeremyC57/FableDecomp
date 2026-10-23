// CTCPhysicsBase::GetPhysicsMeshObject() -- build a CPhysicsMeshObject for the thing: its physics mesh from the world's mesh bank (by the mesh info's bank index), the current position, the orientation set and the render size scaled by 0.01
// Leaf: GetPhysicsMeshObject
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B0850, 128 bytes.
// Callees: CTCPhysicsBase virtual slots 93 (+0x174, GetPhysicsMeshInfo -> CThingPhysicsMeshInfo*) and 72 (+0x120, the orientation
//   set; base 0x006B1290), CTCBase::GetWorld @ 0x00686A80, the mesh bank's physics-mesh-info lookup @ 0x00A24250 (catalog
//   "CTCInventoryAbilities::GetAbilityLevel" is propagated), CPhysicsMeshInfo::GetMesh() @ 0x00A262B0 (CIVCountedPointer<CPhysicsMesh
//   const> by value), CPhysicsMeshObject::CPhysicsMeshObject(const CIVCountedPointer<CPhysicsMesh const>&, const C3DVector&,
//   const CRightHandedSet&, float) @ 0x00A12CC0. 0.01f from the pool 0x01230A5C.
// Layout: CTCPhysicsBase Position +0x0c; CThingPhysicsMeshInfo (PDB) BankIndex +0x00 (ushort), RenderSizeX +0x04.
// Shape: the mesh info, the bank and the bank's physics mesh info are named locals (edi / eax / ebx) taken before the argument list; the ctor's
//   arguments evaluate right to left (scale, orientation, position, the GetMesh temporary); the temporary counted pointer is
//   released with the inline ReleaseTemporary (DecRef / GetRefCount on the ref-count base at +4).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CPhysicsMesh : CRefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CIVCountedPointer {
    T* Data;
    ~CIVCountedPointer() { if (Data) Data->ReleaseTemporary(); }
};
struct CPhysicsMeshInfo {
    CIVCountedPointer<CPhysicsMesh> GetMesh() const;   // 0x00A262B0
};
struct CMeshBank {
    CPhysicsMeshInfo* GetPhysicsMeshInfo(long index);  // 0x00A24250
};
struct CWorld {
    unsigned char _pad00[0x3c];
    CMeshBank* PMeshBank;                       // +0x3c
};
struct CPhysicsMeshObject {
    unsigned char _body[0x40];
    CPhysicsMeshObject(const CIVCountedPointer<CPhysicsMesh>& mesh, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00A12CC0
};
struct CThingPhysicsMeshInfo {
    unsigned short BankIndex;                   // +0x00
    float          RenderSizeX;                 // +0x04
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual const CRightHandedSet& GetRightHandedSet();   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual void v091(); virtual void v092();
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo();   // slot 93 (+0x174)
    void*     ParentThing;                      // +0x04
    unsigned char _pad08[0x4];
    C3DVector Position;                         // +0x0c
    CWorld* GetWorld_00686a80();                         // 0x00686A80 (CTCBase)
    CPhysicsMeshObject GetPhysicsMeshObject();
};

CPhysicsMeshObject CTCPhysicsBase::GetPhysicsMeshObject()
{
    const CThingPhysicsMeshInfo* info = GetPhysicsMeshInfo();
    CMeshBank* bank = GetWorld_00686a80()->PMeshBank;
    CPhysicsMeshInfo* meshInfo = bank->GetPhysicsMeshInfo(info->BankIndex);
    return CPhysicsMeshObject(meshInfo->GetMesh(), Position, GetRightHandedSet(), info->RenderSizeX * 0.01f);
}