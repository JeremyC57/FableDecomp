// CTCWillEnflame::PlaySoundW() -- play the enflame cast sound for the spell level through the parent's sound player, then make a SIMPLE_NOISE
// Leaf: ?PlaySoundW@CTCWillEnflame@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A4520, length 316 bytes (jump table of 4 at 0x007A464C inside the row), plain `ret`.
// Naming: Ego_r.pdb / FableWin.pdb ?PlaySoundW@CTCWillEnflame@@QAEXXZ (public, no arguments; windows.h maps PlaySound to
//   PlaySoundW); the body reads CTCWillEnflame's SpellLevel and plays the ENFLAME_CAST criteria.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetTC<CTCSoundPlayer>, TCI_SOUND_PLAYER 16),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00,
//   CThingCreatureBase::MakeNoise 0x00662110 (declared in the engine/CThingCreatureBase.h retail branch, chunk 774).
// Data: "HERO_ABILITY_ENFLAME_CAST_NO_POWERUP" 0x0126C900, "_LEVEL_1" 0x0126C8DC, "_LEVEL_2" 0x0126C8B8, "_LEVEL_3" 0x0126C894
//   (cases 0..3, paired with the retail table by hand), "SIMPLE_NOISE" 0x01268A5C.
// Layout: Ego_r.pdb CTCWillEnflame (CTCBase 0xC, so PDB offsets hold): SpellLevel +0x14.
// Shape: the parent is a named local (ebx) kept for the trailing noise, made whether or not the thing has a sound player.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSoundPlayer.h"

class CSpecialAbilitiesEnflameDef;
class CTCWillEnflame : public CTCBase { public:
    bool EnflameActionInitialised;                              // +0x0C
    bool Released;                                              // +0x0D
    CDefPointer<const CSpecialAbilitiesEnflameDef> PDef;        // +0x10
    long SpellLevel;                                            // +0x14
    void PlaySoundW();
};

void CTCWillEnflame::PlaySoundW()
{
    CThing* parent = ParentThing;
    CTCSoundPlayer* sound;
    if (parent->GetTC(&sound)) {
        switch (SpellLevel) {
        case 0:
            sound->PlayAttachedCriteria("HERO_ABILITY_ENFLAME_CAST_NO_POWERUP", true, false, false, -1);
            break;
        case 1:
            sound->PlayAttachedCriteria("HERO_ABILITY_ENFLAME_CAST_LEVEL_1", true, false, false, -1);
            break;
        case 2:
            sound->PlayAttachedCriteria("HERO_ABILITY_ENFLAME_CAST_LEVEL_2", true, false, false, -1);
            break;
        case 3:
            sound->PlayAttachedCriteria("HERO_ABILITY_ENFLAME_CAST_LEVEL_3", true, false, false, -1);
            break;
        }
    }
    ((CThingCreatureBase*)parent)->MakeNoise("SIMPLE_NOISE");
}