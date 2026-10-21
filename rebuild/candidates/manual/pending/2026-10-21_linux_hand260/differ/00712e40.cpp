// Grade: DIFFER(464v464) -- only reg alloc: or esi,-1 scheduled before mov edi,[current]; Start[6].Kind store temp in eax/ecx vs retail edx/eax
// CActionTalkBase::GetNextCameraTagIndex(long) -- pick the next conversation camera tag
// Layout: no engine/CActionTalkBase.h; retail CArray is 12 bytes so members after Messages sit 4 below the PDB.
#include <stdlib.h>

struct CCameraTag { long A; long Kind; long C[4]; };
struct CCameraTagVector { CCameraTag* Start; CCameraTag* Finish; };

extern bool g_ConversationCameraTagsEnabled;   // 0x1380C30
extern long g_ConversationCameraState;         // 0x13BAD00
extern CCameraTagVector g_ConversationCameraTags;  // 0x13BAD18

class CActionTalkBase
{
public:
    char _pad0[0x6d];
    bool FarEnoughAwayForFacingCamera;   // retail +0x6D (PDB +0x71)
    char _pad6e[0x8d - 0x6e];
    bool ParentIsDemonDoor;              // retail +0x8D (PDB +0x91)
    long GetNextCameraTagIndex(long current);
};

long CActionTalkBase::GetNextCameraTagIndex(long current)
{
    if (!g_ConversationCameraTagsEnabled)
        return current;
    if (ParentIsDemonDoor)
        return 0;
    long result = -1;
    long r = rand() % 3;
    if (current == -1) {
        switch (r) {
        case 0: result = 0; break;
        case 1: result = 1; break;
        case 2: result = FarEnoughAwayForFacingCamera ? 2 : 0; break;
        }
    } else {
        if (r == 0)
            return current;
        switch (g_ConversationCameraState) {
        case 0: {
            long count = g_ConversationCameraTags.Finish - g_ConversationCameraTags.Start;
            result = rand() % (count - 1);
            break;
        }
        case 1:
            switch (rand() % 4) {
            case 0: result = 4; break;
            case 1: result = 0; break;
            case 2:
                if (current == 4 || current == 0 || !FarEnoughAwayForFacingCamera) result = current;
                else result = 2;
                break;
            case 3:
                result = 6;
                g_ConversationCameraTags.Start[6].Kind = 11;
                break;
            }
            break;
        case 2:
            switch (rand() % 4) {
            case 0: result = 1; break;
            case 1: result = 3; break;
            case 2:
                if (current == 1 || current == 3 || !FarEnoughAwayForFacingCamera) result = current;
                else result = 2;
                break;
            case 3:
                result = 6;
                g_ConversationCameraTags.Start[6].Kind = 12;
                break;
            }
            break;
        }
    }
    if (g_ConversationCameraState == 0) {
        switch (result) {
        case 0: case 4: g_ConversationCameraState = 1; break;
        case 1: case 3: g_ConversationCameraState = 2; break;
        }
    }
    return result;
}
