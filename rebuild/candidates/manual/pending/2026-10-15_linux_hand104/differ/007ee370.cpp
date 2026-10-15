// CTCVillageMember::OnCreate() -- when the owner has a definition index, fetches its definition and resolves the village-member sub-definition into PDef
// Grade: DIFFER(ours v retail) -- the zeroed def local is stored before the argument pushes (retail: after them, `mov dword ptr [esp+0Ch],0` just before the manager call) and the def object is held in edi (an extra push/pop) where retail reloads it into the dead `this` register esi after `add esi,0Ch ; push esi` (88v86)
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EE370, length 86 bytes. CTCVillageMember vtable 0x0124007C slot 4, the
// CTCBase OnCreate slot (as CTCTrophy::OnCreate 0x006DDA20 in slot 4; PDB `?OnCreate@CTCVillageMember@@UAEXXZ`).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0 (landed naming, 0x005BDB4D), CDefinitionManager::GetDef<
// CDefClassBase>(long, CDefPointer*) @ 0x0042AF3C (template argument propagated), CDefClassBase::GetSubDef<
// CVillageMemberDef>(CDefPointer*) @ 0x007EE6E0 (template argument propagated), the def's virtual slot 1 (destroy).
// Shape: the def index is a zero-extended word of the owner (+0x8C); the counted local is zeroed after the argument
// pushes (`mov dword ptr [esp+0Ch],0`), GetDef's arguments are pushed before the manager call; the local's object
// is read once into esi for the GetSubDef call and its release (`test ; je ; dec [esi+4] ; jne ; call [vt+4]`):
// a CDefPointer destructor re-reads the escaped local after the call, so the holder is modelled as a plain record
// whose object is copied to a pointer local and released through it.
// Layout: Ego_r.pdb CTCVillageMember (retail = PDB): PDef +0x0C.
struct CVillageMemberDef;

struct CDefPointerVillageMemberDef {
    const CVillageMemberDef* Object;
};

struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long RefCount;                          // +0x04
    bool GetSubDef(CDefPointerVillageMemberDef* out);   // 0x007EE6E0
};

struct CDefPointerClassBase {
    CDefClassBase* Object;
};

inline void ReleaseDef(CDefClassBase* def)
{
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
}

struct CDefinitionManager {
    bool GetDef(long globalDefIndex, CDefPointerClassBase* out) const;   // 0x0042AF3C
};

struct CThing {
    unsigned char  _pad[0x8C];
    unsigned short DefIndex;                // +0x8C
};

struct CTCVillageMember {
    void*                       __vftable;  // +0x00
    CThing*                     Thing;      // +0x04
    unsigned char               _pad_0x08[0x4];
    CDefPointerVillageMemberDef PDef;       // +0x0C
    const CDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    void OnCreate();
};

void CTCVillageMember::OnCreate()
{
    if (Thing->DefIndex != 0) {
        CDefPointerClassBase def;
        def.Object = 0;
        GetDefinitionManager().GetDef(Thing->DefIndex, &def);
        CDefClassBase* classDef = def.Object;
        classDef->GetSubDef(&PDef);
        ReleaseDef(classDef);
    }
}
