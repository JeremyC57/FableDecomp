// CThingGameObject::ConstructFromParams(long defIndex, const C3DVector* pos, long a3, unsigned long a4, const CCharString& scriptName, CThingPhysical* creator, CThingPhysical* originalCreator, bool a8) -- fail for a non-positive def index, else run CThingPhysical::ConstructFromParams with the other seven arguments and, on success, adopt a non-empty script name (CThing::ScriptName, +0x74)
// Leaf: ?ConstructFromParams@CThingGameObject@@IAE_NJABVC3DVector@@JKABVCCharString@@PAVCThingPhysical@@2_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008388D0, length 96 bytes.
// Callees: CThingPhysical::ConstructFromParams @ 0x006A5950 (7 arguments, `ret 1Ch`), CCharString::operator!=(const
// char*) const @ 0x0099E960 (literal 0x0122D70E = ""), CCharString::operator= @ 0x0099EFB0.
// Layout: local model. CThing::ScriptName sits at retail +0x74 (PDB +0x84 less the retail CThing's 0x10 narrower head;
// the def pointer at +0x70 matches the landed getters 0x00838B20/0x00838B60). engine/CThingGameObject.h only pads this
// region, so the base call and the name member are modelled locally. Parameter names past the fifth follow the
// manifest's CThingPhysical prototype; the 3rd/4th/8th are not established (descriptive).
// Shape: the fifth argument is not forwarded (the pushes skip [esp+18h]); `if (index > 0 && Base(...)) { if (name !=
// "") ScriptName = name; return true; } return false;`.
// NF70 (q10737 binder): ported onto engine/CThing.h / CThingPhysical.h / CThingGameObject.h. The callee is the protected
//   ?ConstructFromParams@CThingPhysical@@IAE_NJABVC3DVector@@JKPAV1@1_N@Z (Ego_r.pdb, FABLE_RETAIL_CTHINGPHYSICAL_CONSTRUCTFROMPARAMS),
//   and this definition takes Ego_r's own spelling ?ConstructFromParams@CThingGameObject@@IAE_NJABVC3DVector@@JKABVCCharString@@PAVCThingPhysical@@2_N@Z
//   (protected, position by const reference: FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT); ScriptName is CThing's (+0x74). Was a local
//   CThingPhysical / CThingGameObject / CCharString model with CThingPhysicalArg* creators and a C3DVector* position.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_CONSTRUCTFROMPARAMS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_CONSTRUCTFROMPARAMS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"
bool CThingGameObject::ConstructFromParams(long defIndex, const C3DVector& position, long a3, unsigned long a4,
                                           const CCharString& scriptName, CThingPhysical* creator,
                                           CThingPhysical* originalCreator, bool a8)
{
    if (defIndex > 0 && CThingPhysical::ConstructFromParams(defIndex, position, a3, a4, creator, originalCreator, a8)) {
        if (scriptName != "")
            ScriptName = scriptName;
        return true;
    }
    return false;
}
