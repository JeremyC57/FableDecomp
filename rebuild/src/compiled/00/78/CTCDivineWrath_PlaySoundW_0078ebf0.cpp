// CTCDivineWrath::PlaySoundW(const CCharString& name, bool loop) -- if the parent thing has a sound player (interface 16), make a "SIMPLE_NOISE"
// noise at the parent creature and play the named criteria sound through the player; else return the invalid sound handle
// Leaf: ?PlaySoundW@CTCDivineWrath@@AAEKABVCCharString@@_N@Z
// Retail 0x0078EBF0, 131 bytes (`ret 8`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the expanded GetTC of
// TCI_SOUND_PLAYER on the thing's interface map at +0x44), CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("SIMPLE_NOISE"
// @ 0x01268A5C), CThingCreatureBase::MakeNoise(const CCharString&) @ 0x00662110, ~CCharString @ 0x0099EAE0,
// CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long) @ 0x0066BD00 (name, false, loop, false, -1).
// Global 0x0129D25C = the invalid sound handle (g_InvalidSoundHandle, as the landed CTCBulletTime::PlaySoundW 0x0079CE80).
// Shape: the landed CTCBulletTime::PlaySoundW (0x0079CE80) with a noise in between: the parent is a named local (edi), the player is
// taken right after the lookup (esi) and the noise name is a CCharString in its own `{}` block, sharing the lookup key's `push ecx`
// frame word. The name is the Unicode-renamed "PlaySound" (windows.h maps it to PlaySoundW).
// Names (NF119, PRIORITY q6158): Ego_r.pdb / FableWin.pdb ?PlaySoundW@CTCDivineWrath@@AAEKABVCCharString@@_N@Z (private; engine/CTCDivineWrath.h
//   declares it) and ?MakeNoise@CThingCreatureBase@@QAEXABVCCharString@@@Z (Ego_r 0x0053CF90 / FableWin 0x01D9C460); the callee
//   CTCSoundPlayer::PlayAttachedCriteria is the catalog name of 0x0066BD00 (bound so by 0x008DD6E0, 0x007A4520). Was an all-local model
//   (?PlaySoundW@CTCDivineWrath@@QAEKKK@Z, ?MakeNoise@CThing@@..., ?PlaySound@CTCSound@@QAEKKJKJJ@Z stand-ins). The parent is a
//   creature (the divine-wrath caster); the cast to CThingCreatureBase is the one the landed sibling CTCWillEnflame::PlaySoundW
//   0x007A4520 makes.
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
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCDivineWrath.h"

extern unsigned long g_InvalidSoundHandle;          // 0x0129D25C

unsigned long CTCDivineWrath::PlaySoundW(const CCharString& name, bool loop)
{
    CThing* parent = ParentThing;
    CTCSoundPlayer* sounds;
    if (parent->GetTC(TCI_SOUND_PLAYER, &sounds)) {
        {
            CCharString noise("SIMPLE_NOISE");
            ((CThingCreatureBase*)parent)->MakeNoise(noise);
        }
        return sounds->PlayAttachedCriteria(name, false, loop, false, -1);
    }
    return g_InvalidSoundHandle;
}
