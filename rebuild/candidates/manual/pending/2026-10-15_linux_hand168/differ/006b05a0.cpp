// CTCPhysicsBase::CreatePPhysicsMeshInfo(const CGraphicDataRef& graphic) -- look the thing's static (type 5) or animated (type 4) mesh info up in the world's mesh bank; use its physics mesh when it has one, otherwise fall back to the physics def's collision setting
// Grade: DIFFER(ours v retail) -- 112v110: register choice only: retail keeps the graphic reference in edx and the type byte in al (eax free after the bank load); ours reference in eax, type in dl, and the slot-85 argument push lands before the vtable load; tried if/else, switch, a type local, a mesh local
// Leaf: CreatePPhysicsMeshInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B05A0, 110 bytes.
// Callees: CTCBase::GetWorld @ 0x00686A80 (catalog "CViewBase::GetDisplayEngine"); the world's mesh bank (+0x3c) accessors
//   @ 0x00A24280 (type 5) and 0x00A242B0 (type 4) (`mov eax,[ecx+380h/...] ; mov eax,[eax+idx*4]` table lookups; catalog
//   "CTCInventoryAbilities::GetAbilityLevel" is propagated); C3DMeshInfo::HasPhysicsMesh @ 0x00A23C40,
//   C3DMeshInfo::PeekPhysicsMesh @ 0x00A24A70; CTCPhysicsBase virtual slots 85 (+0x154, base 0x006B1340 `ret 4`) and 43 (+0xac, SetInteractionFlags:
//   base 0x006B1110 `ret 4`), names not in the PDB dump.
// Layout: CTCPhysicsBase PhysicsDef +0x38 (CDefPointer<CPhysicsDef>; the def word at +0x3c is PDB InteractionFlags);
//   the graphic reference holds the mesh index at +0 and the mesh type byte at +0xd; CPhysicsMeshInfo word +8 is passed on.
// Shape: the bank pointer is loaded before the type compare (a named local); the mesh-info arm ends in its own `ret 4`.
struct CPhysicsMeshInfo {
    unsigned char _pad00[0x08];
    void* PMesh;                                // +0x08
};
struct C3DMeshInfo {
    bool HasPhysicsMesh() const;                // 0x00A23C40
    CPhysicsMeshInfo* PeekPhysicsMesh() const;  // 0x00A24A70
};
struct CMeshBank {
    C3DMeshInfo* GetStaticMeshInfo(long index);     // 0x00A24280
    C3DMeshInfo* GetAnimatedMeshInfo(long index);   // 0x00A242B0
};
struct CWorld {
    unsigned char _pad00[0x3c];
    CMeshBank* PMeshBank;                       // +0x3c
};
struct CGraphicDataRef {
    long          Index;                        // +0x00
    unsigned char _pad04[9];
    unsigned char Type;                         // +0x0d (4 = animated, 5 = static)
};
struct CPhysicsDef {
    unsigned char _pad00[0x3c];
    long InteractionFlags;                      // +0x3c
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042();
    virtual void SetInteractionFlags(long flags);   // slot 43 (+0xac)
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void SetPhysicsMesh(void* mesh);   // slot 85 (+0x154)
    void*     ParentThing;                      // +0x04
    unsigned char _pad08[0x30];
    const CPhysicsDef* PhysicsDef;              // +0x38 CDefPointer
    CWorld* GetWorld();                         // 0x00686A80 (CTCBase)
    void CreatePPhysicsMeshInfo(const CGraphicDataRef& graphic);
};

void CTCPhysicsBase::CreatePPhysicsMeshInfo(const CGraphicDataRef& graphic)
{
    CMeshBank* bank = GetWorld()->PMeshBank;
    C3DMeshInfo* info;
    unsigned char type = graphic.Type;
    if (type == 5)
        info = bank->GetStaticMeshInfo(graphic.Index);
    else if (type == 4)
        info = bank->GetAnimatedMeshInfo(graphic.Index);
    else
        return;
    if (info->HasPhysicsMesh()) {
        void* mesh = info->PeekPhysicsMesh()->PMesh;
        SetPhysicsMesh(mesh);
        return;
    }
    if (PhysicsDef)
        SetInteractionFlags(PhysicsDef->InteractionFlags);
}
