// CTCUnholyPower::PlaySoundW(const CCharString& name, bool loop) -- make a "SIMPLE_NOISE" noise at the parent creature and play the sound
//   criteria through its sound player (interface 16), or return the invalid sound handle when it has none
// Leaf: ?PlaySoundW@CTCUnholyPower@@AAEKABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007940F0, 131 bytes (`ret 8`; byte twin of 0x0078EBF0 in the CTCDivineWrath TU).
// Identity (NF120, PRIORITY q6222): Ego_r.pdb 0x00799BA0 / FableWin.pdb 0x02694880 ?PlaySoundW@CTCUnholyPower@@AAEKABVCCharString@@_N@Z
//   (private; CTCDivineWrath has its own ?PlaySoundW@CTCDivineWrath@@AAEKABVCCharString@@_N@Z, the byte twin 0x0078EBF0). Every caller of
//   this address (0x00794987 / 0x007949B9 in the ctor 0x007941D0, 0x0079547C, 0x00795801, 0x00795841, 0x0079770C) is in the CTCUnholyPower
//   TU and passes this = CTCUnholyPower, a CCharString and a bool; the landed ctor 0x007941D0 already binds this spelling (RELOCATION_MATCH).
//   Was ?PlaySoundW@CTCDivineWrath@@QAEKKK@Z (the propagated label, two unsigned long arguments) over local CThing / CTCSound models.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline GetTC, TCI_SOUND_PLAYER 16), CCharString(const char*, long)
//   0x0099EBF0 ("SIMPLE_NOISE" 0x01268A5C) / ~CCharString 0x0099EAE0, CThingCreatureBase::MakeNoise 0x00662110,
//   CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00 (name, false, loop, false, -1). Global 0x0129D25C = the invalid sound handle.
// Header: engine/CTCUnholyPower.h (the complete PDB class) with the CTCSoundPlayer / CThingCreatureBase retail branches; the recipe is the
//   landed CTCPhysicalShieldSpell::PlaySoundW 0x0079DE00 plus the noise of CTCWillEnflame::PlaySoundW 0x007A4520.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCUnholyPower.h"

extern unsigned long g_InvalidSoundHandle;          // 0x0129D25C

unsigned long CTCUnholyPower::PlaySoundW(const CCharString& name, bool loop)
{
    CThing* thing = ParentThing;
    CTCSoundPlayer* sounds;
    if (thing->GetTC(TCI_SOUND_PLAYER, &sounds)) {
        ((CThingCreatureBase*)thing)->MakeNoise("SIMPLE_NOISE");
        return sounds->PlayAttachedCriteria(name, false, loop, false, -1);
    }
    return g_InvalidSoundHandle;
}
