// CTCWeapon::OnCreate() -- when the owner has a def index, fetch its thing def, take its weapon sub-def into WeaponDef and copy the def's start-loaded flag
// Leaf: ?OnCreate@CTCWeapon@@EAEXXZ
// Retail: Fable.exe @ 0x005DA6E0, 99 bytes; CTCWeapon vtable slot 4 (CTCBase::OnCreate).
// Identity: Ego_r.pdb ?OnCreate@CTCWeapon@@EAEXXZ (private virtual override).
// Callees: CTCBase::GetDefinitionManager() 0x006869D0; CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer*) 0x0042AF3C;
//          CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer*) 0x0042B1A6; the def's Release.
// Layout: engine/CTCWeapon.h (Ego_r.pdb; offsets hold in retail): WeaponDef (CDefPointer<const CWeaponDef>) +0x0c,
//         ProjectileWeaponLoaded +0x10; the owner thing's DefGlobalIndex (unsigned short) +0x8c; CWeaponDef::ProjectileWeaponStartLoaded +0xc5.
// Name fix (agentNF8 2026-11-11): moved off the local CDefClassBase / CDefinitionManager / CWeaponDef stand-ins (same-TU stand-in GetDef
// / GetSubDef bodies) onto the shared headers: GetDefinitionManager / GetDef<CThingBaseDef> / GetSubDef<CWeaponDef> bind the PDB
// spellings, the row's own symbol is the PDB private virtual (EAE), and the copied byte +0xc5 is CWeaponDef::ProjectileWeaponStartLoaded
// (Ego_r.pdb name; the local model called it ProjectileWeaponLoaded).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_ONCREATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CTCWeapon.h"
#include "engine/CGameDefinitionManager.h"

void CTCWeapon::OnCreate()
{
    unsigned short index = (unsigned short)ParentThing->GetDefGlobalIndex();
    if (index) {
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(index, &def);
        def->GetSubDef(&WeaponDef);
        ProjectileWeaponLoaded = WeaponDef->ProjectileWeaponStartLoaded;
    }
}
