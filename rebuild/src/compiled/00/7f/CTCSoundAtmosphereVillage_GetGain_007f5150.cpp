// CTCSoundAtmosphereVillage::GetGain(const CCharString&) -- the gain of the village's "STREETLIFE" atmosphere layer: the share of the village's living spaces filled by villagers who are not asleep in bed (recounted when the update timer has run out, which re-arms it to 5), clamped to [0, 1]; 0 for a village with no living spaces, -1 for any other layer name
// Leaf: ?GetGain@CTCSoundAtmosphereVillage@@UAEMABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) 0x007F5150, 355 bytes, `ret 4`, float result on the x87 stack. Identity: CTCSoundAtmosphereVillage
//   vtable 0x0124111C slot 28; Ego_r.pdb / FableWin.pdb ?GetGain@CTCSoundAtmosphereVillage@@UAEMABVCCharString@@@Z.
// Callees: CBasicString<char>::operator==(const char*) const 0x004115A0 (through the inline CCharString::operator==(const char*),
//   whose null-string arm is the strcmp intrinsic against "" 0x0122D70E; "STREETLIFE" 0x01271DE8);
//   CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020 (non-const; through the inline CThing::GetTC<T>(T**) for
//   TCI_VILLAGE 0x22 and TCI_ENTITY_MODE_MANAGER 0x31); CBaseIntelligentPointer::GetPItem 0x00A01B50;
//   CTCCreatureModeManager::GetCurrentMode() const 0x0073A6E0; CModeBase::GetType() const (vtable slot 1, +4).
//   Floats: -1.0f 0x0122DEE0, 0.0f 0x0122DEDC, 1.0f 0x0122DED8; 5.0f is the immediate 0x40A00000.
// Layout: this = Ego_r.pdb CTCSoundAtmosphereVillage (0x1C): CTCBase::ParentThing +0x04, TimeUntilNextUpdate +0x0C, PDef +0x10,
//   NumLivingSpaces +0x14, NumVillageMembers +0x18. engine/CThing.h UsingComponents +0x20 (bit 34 = byte +0x24 bit 2, bit 49 =
//   dword +0x24 bit 17), Components +0x44; engine/CTCVillage.h VillagePeople +0x2C (std::list<CIntelligentPointer<CThingAICreature> >).
// Shape: the whole body sits inside `if (name == "STREETLIFE")` with `return -1.0f` after it: retail lays the -1 return out between
//   the two arms of the expanded operator== (0x007F5179), which the early-return spelling `if (!(name == ..)) return -1.0f;` does not
//   give (graded: 351 v 355). `village` and `modes` are plain unset locals filled by GetTC(&ptr) (HasTC false leaves them unset: retail reads the dead
//   argument slot [esp+18h], which also holds both lookup keys); `modes` is declared outside the loop (loaded once before it,
//   0x007F5204). The loop is the STLport list iterator loop (end() re-read every pass, `cmp edi,[ebp+2Ch]`). The clamp assigns to
//   `gain` and returns it once: each clamp arm gets its own `fstp st(0); fld const` epilogue and the unclamped arm shares the -1
//   path's pops (0x007F517F); `return 0.0f` in the clamp is merged with the early `return 0.0f` (graded: 343 v 355).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TCI
#define FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCVillage.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"
#include "engine/CThingAICreature.h"

class CSoundAtmospheresDef;
// No shared header exists for the two sound-atmosphere components: local models with the Ego_r.pdb members
// (struct_layouts_egor.tsv: CTCSoundAtmosphere 0x14, CTCSoundAtmosphereVillage 0x1C) on the shared CTCBase.
class CTCSoundAtmosphere : public CTCBase {
public:
    virtual float GetGain(const CCharString& name);         // vtable slot 28 in retail (only the name matters to this row)
    float TimeUntilNextUpdate;                              // +0x0C
    const CSoundAtmospheresDef* PDef;                       // +0x10 CDefPointer<const CSoundAtmospheresDef> (one pointer)
};
class CTCSoundAtmosphereVillage : public CTCSoundAtmosphere {
public:
    virtual float GetGain(const CCharString& name);         // 0x007F5150
    long NumLivingSpaces;                                   // +0x14
    long NumVillageMembers;                                 // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CTCSoundAtmosphereVillage) == 0x1C);

float CTCSoundAtmosphereVillage::GetGain(const CCharString& name)
{
    if (name == "STREETLIFE") {
        if (NumLivingSpaces == 0)
            return 0.0f;
        if (TimeUntilNextUpdate <= 0.0f) {
            CTCVillage* village;
            GetParentThing().GetTC(&village);
            NumVillageMembers = 0;
            CTCCreatureModeManager* modes;
            for (std::list<CIntelligentPointer<CThingAICreature> >::iterator it = village->VillagePeople.begin(); it != village->VillagePeople.end(); ++it) {
                it->Get()->GetTC(&modes);
                if (modes->GetCurrentMode().GetType() != NCreatureMode::MODE_SLEEP_BED)
                    ++NumVillageMembers;
            }
            TimeUntilNextUpdate = 5.0f;
        }
        float gain = (float)NumVillageMembers / NumLivingSpaces;
        if (gain < 0.0f)
            gain = 0.0f;
        else if (gain > 1.0f)
            gain = 1.0f;
        return gain;
    }
    return -1.0f;
}