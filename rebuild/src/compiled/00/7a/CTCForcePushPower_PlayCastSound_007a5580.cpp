// CTCForcePushPower::PlayCastSound() -- play the force-push cast sound for the spell level through the parent's sound player
// Leaf: ?PlayCastSound@CTCForcePushPower@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A5580, length 304 bytes (jump table of 4 at 0x007A56A0 inside the row), plain `ret`.
// Naming: Ego_r.pdb / FableWin.pdb ?PlayCastSound@CTCForcePushPower@@AAEXXZ (private, no arguments); the body reads
//   CTCForcePushPower's SpellLevel and plays the FORCE_PUSH_CAST criteria.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetTC<CTCSoundPlayer>, TCI_SOUND_PLAYER 16),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00.
// Data: "HERO_ABILITY_FORCE_PUSH_CAST_NO_POWERUP" 0x0126CAA4, "_LEVEL_1" 0x0126CA7C, "_LEVEL_2" 0x0126CA54, "_LEVEL_3" 0x0126CA2C
//   (cases 0..3, paired with the retail table by hand).
// Layout: Ego_r.pdb CTCForcePushPower (CTCBase 0xC, so PDB offsets hold): PDef +0x0C, SpellLevel +0x10.
// Shape: VC7.1 tail-duplicates the epilogue into every case.
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
#include "engine/CTCSoundPlayer.h"

class CSpecialAbilitiesForcePushDef;
class CTCForcePushPower : public CTCBase { public:
    CDefPointer<const CSpecialAbilitiesForcePushDef> PDef;      // +0x0C
    long SpellLevel;                                            // +0x10
private:
    void PlayCastSound();
};

void CTCForcePushPower::PlayCastSound()
{
    CTCSoundPlayer* sound;
    if (ParentThing->GetTC(&sound)) {
        switch (SpellLevel) {
        case 0:
            sound->PlayAttachedCriteria("HERO_ABILITY_FORCE_PUSH_CAST_NO_POWERUP", true, false, false, -1);
            break;
        case 1:
            sound->PlayAttachedCriteria("HERO_ABILITY_FORCE_PUSH_CAST_LEVEL_1", true, false, false, -1);
            break;
        case 2:
            sound->PlayAttachedCriteria("HERO_ABILITY_FORCE_PUSH_CAST_LEVEL_2", true, false, false, -1);
            break;
        case 3:
            sound->PlayAttachedCriteria("HERO_ABILITY_FORCE_PUSH_CAST_LEVEL_3", true, false, false, -1);
            break;
        }
    }
}