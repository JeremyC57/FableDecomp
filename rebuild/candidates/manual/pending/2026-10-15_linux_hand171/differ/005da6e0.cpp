// CTCWeapon::OnCreate() -- when the owner has a def class index, fetch that def class, take its weapon sub-def into WeaponDef and copy the def's projectile-loaded flag
// Leaf: OnCreate
// Grade: DIFFER(99v99) -- one register rotation: retail `mov ebx,[esp+0Ch] ; lea edi,[esi+0Ch] ; push edi ; mov ecx,ebx`, ours `mov ecx,[esp+0Ch] ; lea edi ; push edi ; mov ebx,ecx` (same instructions otherwise). Tried: holder copy of out.Object, plain local + hand release, holder + call through out.Object.
// Retail: Fable.exe @ 0x005DA6E0, 99 bytes; CTCWeapon vtable slot 4 (the catalog's "vfunc_4" = CTCBase::OnCreate).
// Callees: CTCBase::GetDefinitionManager() 0x006869D0; CDefinitionManager::GetDef<CDefClassBase>(long, CDefPointer*) 0x0042AF3C
//          (template label propagated); CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer*) 0x0042B1A6 (template label
//          propagated); the def class's slot 1 on release.
// Layout: engine/CTCWeapon.h (Ego_r.pdb; offsets hold in retail): WeaponDef (CDefPointer<const CWeaponDef>) +0x0c,
//         ProjectileWeaponLoaded +0x10; the owner thing's def class index (unsigned short) +0x8c; CWeaponDef byte +0xc5.
// Shape: the def class is copied once out of the escaped out slot into a holder (ebx) that is used for the GetSubDef call
//        and released with a null test (`test ebx,ebx` scheduled before the flag copy).
struct CWeaponDef {
    unsigned char _pad_0x00[0xc5];
    bool          ProjectileWeaponLoaded;       // +0xc5
};
struct CWeaponDefPointer {                      // CDefPointer<const CWeaponDef>
    CWeaponDef* Object;
    CWeaponDef* operator->() const { return Object; }
};

struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    bool GetSubDef(CWeaponDefPointer* out);     // 0x0042B1A6
};
struct CDefClassPointer {
    CDefClassBase* Object;
    CDefClassPointer() : Object(0) {}
};
struct CDefClassReference {                     // holder of the taken def class
    CDefClassBase* Object;
    CDefClassReference(CDefClassBase* object) : Object(object) {}
    ~CDefClassReference()
    {
        if (Object && --Object->m_count == 0)
            Object->Destroy();
    }
    CDefClassBase* operator->() const { return Object; }
};
struct CDefinitionManager {
    bool GetDef(long index, CDefClassPointer* out);   // 0x0042AF3C
};

struct CThing {
    unsigned char  _pad_0x00[0x8c];
    unsigned short DefClassIndex;               // +0x8c
};

struct CTCWeapon {
    virtual void v00();
    CThing*           ParentThing;              // +0x04
    unsigned char     _pad_0x08[0x4];
    CWeaponDefPointer WeaponDef;                // +0x0c
    bool              ProjectileWeaponLoaded;   // +0x10
    CDefinitionManager* GetDefinitionManager(); // 0x006869D0 (CTCBase)
    void OnCreate();
};

void CTCWeapon::OnCreate()
{
    unsigned short index = ParentThing->DefClassIndex;
    if (index) {
        CDefClassPointer out;
        GetDefinitionManager()->GetDef(index, &out);
        CDefClassReference defClass(out.Object);
        defClass->GetSubDef(&WeaponDef);
        ProjectileWeaponLoaded = WeaponDef->ProjectileWeaponLoaded;
    }
}
