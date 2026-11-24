// Test for 0x00739E90 CTCCreatureModeManager::CModeBase::IsAllowingPlayerSpecialMovementMoves on engine/CTCCreatureModeManager.h (NF240):
// links the row object; a qualified (non-virtual) call on a raw buffer. Cases: false on a zeroed mode and on a mode whose bytes are
// all set (the body reads nothing).
#include <string.h>
#include <stdio.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"

int main()
{
    static unsigned char buf[0x40];
    const CTCCreatureModeManager::CModeBase* mode = reinterpret_cast<const CTCCreatureModeManager::CModeBase*>(buf);
    if (mode->CTCCreatureModeManager::CModeBase::IsAllowingPlayerSpecialMovementMoves()) { printf("00739e90 FAIL 1\n"); return 1; }
    memset(buf, 0xff, sizeof(buf));
    if (mode->CTCCreatureModeManager::CModeBase::IsAllowingPlayerSpecialMovementMoves()) { printf("00739e90 FAIL 2\n"); return 1; }
    printf("PASS_00739e90\n");
    return 0;
}
