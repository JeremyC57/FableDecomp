// CTCWeapon::OnCreate() -- when the owner has a def class index, fetch that def class, take its weapon sub-def into WeaponDef and copy the def's projectile-loaded flag
// Leaf: OnCreate
// Retail: Fable.exe @ 0x005DA6E0, 99 bytes; CTCWeapon vtable slot 4 (the catalog's "vfunc_4" = CTCBase::OnCreate).
// Callees: CTCBase::GetDefinitionManager() 0x006869D0; CDefinitionManager::GetDef<CDefClassBase>(long, CDefPointer*) 0x0042AF3C
//          (template label propagated); CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer*) 0x0042B1A6 (template label
//          propagated); the def class's slot 1 on release.
// Layout: engine/CTCWeapon.h (Ego_r.pdb; offsets hold in retail): WeaponDef (CDefPointer<const CWeaponDef>) +0x0c,
//         ProjectileWeaponLoaded +0x10; the owner thing's def class index (unsigned short) +0x8c; CWeaponDef byte +0xc5.
// Shape: the def class is copied once out of the escaped out slot into a holder (ebx) that is used for the GetSubDef call
//        and released with a null test (`test ebx,ebx` scheduled before the flag copy).
class CWeaponDef { public:
    unsigned char _pad_0x00[0xc5];
    bool          ProjectileWeaponLoaded;       // +0xc5
};
struct CWeaponDefPointer {                      // CDefPointer<const CWeaponDef>
    CWeaponDef* Object;
    CWeaponDef* operator->() const { return Object; }
};

class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
    bool GetSubDef(CWeaponDefPointer* out);     // 0x0042B1A6
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CWeaponDefPointer* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
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
class CDefinitionManager { public:
    bool GetDef(long index, CDefClassPointer* out);   // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefClassPointer* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad_0x00[0x8c];
    unsigned short DefClassIndex;               // +0x8c
};

class CTCWeapon { public:
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