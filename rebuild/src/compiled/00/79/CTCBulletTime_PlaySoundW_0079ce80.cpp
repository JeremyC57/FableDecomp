// CTCBulletTime::PlaySoundW(const CCharString& name, bool loop) -- play a sound criteria through the parent thing's sound player
// (interface 16), or return the invalid sound handle when it has none.
// Leaf: ?PlaySoundW@CTCBulletTime@@AAEKABVCCharString@@_N@Z
// Retail 0x0079CE80, 91 bytes (`ret 8`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (GetTC expanded),
// CTCSoundPlayer::PlayAttachedCriteria @ 0x0066BD00 (name, false, loop, false, -1). Global 0x0129D25C = the invalid sound handle.
// Name (agentNF32, 2026-11-12; was ?PlaySoundW@CTCBulletTime@@QAEKKK@Z, public with two unsigned longs): Ego_r.pdb 0x007A2910 and
//   FableWin.pdb 0x026A84A0 ?PlaySoundW@CTCBulletTime@@AAEKABVCCharString@@_N@Z (private; a CCharString& and a bool); the landed UnPause
//   0x0079D7E0 binds this address under that symbol through engine/CTCBulletTime.h (FABLE_RETAIL_CTCBULLETTIME_UNPAUSE). The callee is
//   the shared engine/CTCSoundPlayer.h PlayAttachedCriteria (Ego_r / FableWin; landed binding 0x008DD6E0). The name is the
//   Unicode-renamed "PlaySound" (windows.h maps it to PlaySoundW). The byte-identical 0x0079DE00 is CTCPhysicalShieldSpell's.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#define FABLE_RETAIL_CTCBULLETTIME_UNPAUSE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#include "engine/CThing.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCSoundPlayer.h"

extern unsigned long g_InvalidSoundHandle;          // 0x0129D25C

unsigned long CTCBulletTime::PlaySoundW(const CCharString& name, bool loop)
{
    CTCSoundPlayer* sounds;
    if (ParentThing->GetTC(TCI_SOUND_PLAYER, &sounds))
        return sounds->PlayAttachedCriteria(name, false, loop, false, -1);
    return g_InvalidSoundHandle;
}
