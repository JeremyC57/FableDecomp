// CTCPerceiveThing::OnCreate() -- fetch the parent's thing def, take its CPerceivedThingDef sub-def into PDef, reset the perception state, load the four radii from the def (keeping ExtendedSightRadius >= SightRadius), the give-up-chase radius, and clear DirectAwareness
// Leaf: ?OnCreate@CTCPerceiveThing@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00744ED0, 157 bytes (plain `ret`); CTCBase slot 4 override (Ego_r.pdb / FableWin.pdb
// ?OnCreate@CTCPerceiveThing@@UAEXXZ).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C,
// CDefClassBase::GetSubDef<CPerceivedThingDef> @ 0x00746100, CTCPerceiveThing::ResetPerceptionVariables @ 0x00744DB0; the four radius
// setters (SetSightRadius 0x00744540, SetSoundRadius 0x00744470, SetSmellRadius 0x007444A0, SetExtendedSightRadius 0x00744500, all in
// this retail TU) are expanded with radius -1 (take the def's value).
// Shape: the setters' bodies are the landed rows' (same retail TU), defined above OnCreate so that VC7.1 expands them as retail does;
// GiveUpChaseRadius is read from the def in place.
// Layout: engine/CTCPerceiveThing.h (FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS): DirectAwareness +0x30, PDef +0x34, SoundRadius +0x3c,
// SightRadius +0x40, ExtendedSightRadius +0x44, SmellRadius +0x48, GiveUpChaseRadius +0x4c; CPerceivedThingDef GiveUpChaseRadius +0x2c,
// SightRadius +0x30, ExtendedSightRadius +0x34, SoundRadius +0x38, SmellRadius +0x3c (engine/CPerceivedThingDef.h).
// Name fix (agentNF10 2026-11-11): moved off the local CDefClassBase / CThingDef / CThingDefOut / CDefinitionManager / CTCPerceiveThing
// stand-ins onto the shared headers (CTCPerceiveThing.h FABLE_RETAIL_CTCPERCEIVETHING_RADII, CPerceivedThingDef.h
// FABLE_RETAIL_CPERCEIVEDTHINGDEF). 0x0042AF3C binds GetDef<CThingBaseDef> (was ?GetDef@CDefinitionManager@@QAE_NJPAUCThingDefOut@@@Z),
// 0x006869D0 the PDB const GetDefinitionManager, 0x00746100 ??$GetSubDef@VCPerceivedThingDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCPerceivedThingDef@@@@@Z;
// the PDB-less GetDef() accessor is gone (PDef is read in place).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CPERCEIVEDTHINGDEF
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_RADII
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPerceivedThingDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCPerceiveThing.h"

void CTCPerceiveThing::SetSoundRadius(float radius)
{
    if (radius != -1.0f)
        SoundRadius = radius;
    else
        SoundRadius = PDef->SoundRadius;
}

void CTCPerceiveThing::SetSmellRadius(float radius)
{
    if (radius != -1.0f)
        SmellRadius = radius;
    else
        SmellRadius = PDef->SmellRadius;
}

void CTCPerceiveThing::SetSightRadius(float radius)
{
    if (radius != -1.0f) {
        SightRadius = radius;
        return;
    }
    SightRadius = PDef->SightRadius;
    if (SightRadius > ExtendedSightRadius)
        ExtendedSightRadius = SightRadius;
}

void CTCPerceiveThing::SetExtendedSightRadius(float radius)
{
    if (radius != -1.0f) {
        ExtendedSightRadius = radius;
        return;
    }
    ExtendedSightRadius = PDef->ExtendedSightRadius;
    if (ExtendedSightRadius < SightRadius)
        ExtendedSightRadius = SightRadius;
}

void CTCPerceiveThing::OnCreate()
{
    CThing* thing = ParentThing;
    CDefPointer<const CThingBaseDef> def;
    GetDefinitionManager().GetDef(thing->GetDefGlobalIndex(), &def);
    def->GetSubDef(&PDef);
    ResetPerceptionVariables();
    SetSightRadius(-1.0f);
    SetSoundRadius(-1.0f);
    SetSmellRadius(-1.0f);
    SetExtendedSightRadius(-1.0f);
    GiveUpChaseRadius = PDef->GiveUpChaseRadius;
    DirectAwareness = false;
}
