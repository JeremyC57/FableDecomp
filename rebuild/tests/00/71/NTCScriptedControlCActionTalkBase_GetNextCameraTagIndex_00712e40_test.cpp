// Behaviour test for NTCScriptedControl::CActionTalkBase::GetNextCameraTagIndex (0x00712E40), with a scripted rand().
#include <cstdio>

static int g_seq[16]; static int g_pos;
static int test_rand() { return g_seq[g_pos++]; }

struct CCameraTag { long CameraPositionPerson; long CameraPosition; long CameraFocusPerson; long CameraFocusPosition; long CameraZoomType; long CameraView; };
static CCameraTag g_tags[9];
struct CTagVector { CCameraTag* b; CCameraTag* e; CCameraTag* begin() { return b; } CCameraTag* end() { return e; } unsigned int size() { return (unsigned int)(e - b); } CCameraTag& operator[](unsigned int n) { return *(begin() + n); } };
static CTagVector DefaultCameraTags = { g_tags, g_tags + 9 };
static bool g_UseConversationCameraTags = true;
static long g_ConversationCameraSide = 0;

struct CActionTalkBase {
    bool FarEnoughAwayForFacingCamera; bool ParentIsDemonDoor;
    long GetNextCameraTagIndex(long current_index)
    {
        if (!g_UseConversationCameraTags) return current_index;
        if (ParentIsDemonDoor) return 0;
        long result = -1;
        if (current_index == -1) {
            switch (test_rand() % 3) {
            case 0: result = 0; break;
            case 1: result = 1; break;
            case 2: result = FarEnoughAwayForFacingCamera ? 2 : 0; break;
            }
        } else {
            if (test_rand() % 3 == 0) return current_index;
            switch (g_ConversationCameraSide) {
            case 0: result = test_rand() % (long)(DefaultCameraTags.size() - 1); break;
            case 1:
                switch (test_rand() % 4) {
                case 0: result = 4; break;
                case 1: result = 0; break;
                case 2: if (current_index == 4 || current_index == 0 || !FarEnoughAwayForFacingCamera) result = current_index; else result = 2; break;
                case 3: result = 6; DefaultCameraTags[6].CameraPosition = 11; break;
                }
                break;
            case 2:
                switch (test_rand() % 4) {
                case 0: result = 1; break;
                case 1: result = 3; break;
                case 2: if (current_index == 1 || current_index == 3 || !FarEnoughAwayForFacingCamera) result = current_index; else result = 2; break;
                case 3: result = 6; DefaultCameraTags[6].CameraPosition = 12; break;
                }
                break;
            }
        }
        if (g_ConversationCameraSide == 0) {
            switch (result) {
            case 0: case 4: g_ConversationCameraSide = 1; break;
            case 1: case 3: g_ConversationCameraSide = 2; break;
            }
        }
        return result;
    }
};

static int fail() { std::printf("00712e40_TEST FAIL\n"); return 1; }

int main()
{
    CActionTalkBase a; a.FarEnoughAwayForFacingCamera = true; a.ParentIsDemonDoor = false;
    g_seq[0] = 5;                         // 5 % 3 = 2 -> facing camera 2, side stays 0
    if (a.GetNextCameraTagIndex(-1) != 2 || g_ConversationCameraSide != 0) return fail();
    g_pos = 0; g_seq[0] = 1; g_seq[1] = 11; // keep? 1%3 != 0; side 0 -> 11 % 8 = 3 -> side 2
    if (a.GetNextCameraTagIndex(2) != 3 || g_ConversationCameraSide != 2) return fail();
    g_pos = 0; g_seq[0] = 3;               // 3 % 3 == 0 -> keep current
    if (a.GetNextCameraTagIndex(3) != 3) return fail();
    g_pos = 0; g_seq[0] = 2; g_seq[1] = 7;  // side 2, 7 % 4 = 3 -> 6, tag 6 right
    if (a.GetNextCameraTagIndex(3) != 6 || g_tags[6].CameraPosition != 12) return fail();
    g_pos = 0; g_seq[0] = 2; g_seq[1] = 2;  // side 2, case 2 with current 1 -> current
    if (a.GetNextCameraTagIndex(1) != 1) return fail();
    g_ConversationCameraSide = 1; g_pos = 0; g_seq[0] = 1; g_seq[1] = 6; // side 1 case 2, current 5 -> 2
    if (a.GetNextCameraTagIndex(5) != 2) return fail();
    a.ParentIsDemonDoor = true;
    if (a.GetNextCameraTagIndex(5) != 0) return fail();
    g_UseConversationCameraTags = false;
    if (a.GetNextCameraTagIndex(5) != 5) return fail();
    std::printf("00712e40_TEST PASS\n");
    return 0;
}