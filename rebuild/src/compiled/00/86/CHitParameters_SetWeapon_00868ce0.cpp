// CHitParameters::SetWeapon(CThing& weapon) -- remember the weapon (PWeapon) and, when it carries the object-augmentations component, append the Type of every augmentation def it holds to the hit's Augmentations list
// Leaf: ?SetWeapon@CHitParameters@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00868CE0, 360 bytes (0x00868CE0..0x00868E47), thiscall, `ret 4`, no frame pointer.
// Identity: Ego_r.pdb ?SetWeapon@CHitParameters@@QAEXAAVCThing@@@Z (public, reference parameter; same TU order as retail:
//   ClearAllDamage, SetWeapon, AddDamageWithAttribute) and the landed caller 0x00838EC0 GetShotHitParameters.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (the
//   CIntelligentPointer<CThing> temporary of `PWeapon = &weapon`, vtable 0x01238C6C, inline (T*) ctor and operator=),
//   CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0 (inside the expanded CThing::PeekTC),
//   CTCObjectAugmentations::GetAugmentations(std::vector<long>&) const 0x00766920, CThing::PeekDefinitionManager() const 0x004C7990,
//   CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const CThingBaseDef>*) const 0x0042AF3C,
//   CDefClassBase::GetSubDef<CAugmentationDef>(CDefPointer<const CAugmentationDef>*) const 0x0042B248,
//   std::vector<EObjectAugmentationType>::_M_insert_overflow 0x00869AC0 (inline push_back's grow path), free 0x00BFEA14
//   (the local vector's storage), the defs' virtual slot 1 (CDefPointer release).
// Layout: CHitParameters Augmentations +0x3C (std::vector<EObjectAugmentationType>), PWeapon +0x58; CThing UsingComponents +0x20
//   (bit 191 = sign of +0x34), Components +0x44; CAugmentationDef::Type +0x2C.
// Shape: the frame is 0x18: the 8-byte pointer temporary's first word is reused for the PeekTC key and both def out words
//   (GetDef / GetSubDef are the shared headers' visible noinline template bodies, so their out words do not escape and the defs
//   live in edi / esi); the def index is read into a local BEFORE the def pointer is declared (`mov eax,[ebx]` ahead of the
//   `lea ecx,[pdef]` push -- written as `GetDef(*it, &pdef)` VC7.1 loads it after the out pointer push); `weapon` is re-read
//   from its argument slot each pass; push_back's __false_type tag sits in the dead argument slot.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CAUGMENTATIONDEF
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#define FABLE_RETAIL_CHITPARAMETERS_SETWEAPON
#define FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_GETAUGMENTATIONS
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAugmentationDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CHitParameters.h"

void CHitParameters::SetWeapon(CThing& weapon)
{
    PWeapon = &weapon;
    const CTCObjectAugmentations* ptc;
    if (PWeapon->PeekTC(&ptc)) {
        std::vector<long> augmentations;
        ptc->GetAugmentations(augmentations);
        for (std::vector<long>::iterator it = augmentations.begin(); it != augmentations.end(); ++it) {
            long index = *it;
            CDefPointer<const CThingBaseDef> pdef;
            if (weapon.PeekDefinitionManager().GetDef(index, &pdef)) {
                CDefPointer<const CAugmentationDef> paug;
                if (pdef->GetSubDef(&paug))
                    Augmentations.push_back(paug->Type);
            }
        }
    }
}