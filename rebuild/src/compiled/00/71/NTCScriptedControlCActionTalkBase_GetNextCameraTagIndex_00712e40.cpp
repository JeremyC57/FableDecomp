// NTCScriptedControl::CActionTalkBase::GetNextCameraTagIndex(long current_index) -- pick the next conversation camera tag: the current one when tags are off, 0 for a demon door, a random opening tag (0/1/2 when far enough away for the facing camera), otherwise keep the current tag one time in three or step through the side-specific set (left side 4/0/2/6, right side 1/3/2/6, re-pointing default tag 6 left or right), then latch the side from the first pick
// Leaf: ?GetNextCameraTagIndex@CActionTalkBase@NTCScriptedControl@@IAEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00712E40, 464 bytes incl. three jump tables, `ret 4`. Ego_r.pdb
//   ?GetNextCameraTagIndex@CActionTalkBase@NTCScriptedControl@@IAEJJ@Z (protected).
// Callees: rand 0x00BFEB16 (CRT import thunk). Globals: the camera-tags-enabled flag 0x01380C30, the conversation camera
//   side 0x013BAD00 (both descriptive: no PDB data names), the static DefaultCameraTags vector 0x013BAD18 (landed
//   InitialiseDefaultCameraTags 0x007167E0 / _M_insert_overflow 0x007196D0 spelling).
// Layout (retail, local model): FarEnoughAwayForFacingCamera +0x6d, ParentIsDemonDoor +0x8d (Ego_r.pdb +0x71 / +0x91: the
//   retail CArray<CSubtitledMessage> is 12 bytes, 4 less than the PDB's). CCameraTag is 24 bytes, CameraPosition +4 (so
//   DefaultCameraTags[6].CameraPosition is +0x94); the count is (end - begin) / 24 (the 0x2AAAAAAB multiply).
// Shape: each branch calls rand() % 3 itself (`switch (rand() % 3)` / `if (rand() % 3 == 0) return current_index;`); VC7.1
//   hoists the identical call + idiv above the `current_index == -1` test, and only then loads current_index into edi
//   before `or esi,-1` and the call, as retail does (a shared `long r = rand() % 3;` local scheduled the load after the call
//   and rotated the DefaultCameraTags base registers). The STLport `*(begin() + n)` operator[] and size() = end() - begin()
//   index the static vector.
#include <stdlib.h>

enum EConversationCameraPosition { CONVERSATION_CAMERA_POS_LEFT = 11, CONVERSATION_CAMERA_POS_RIGHT = 12 };
class CSubtitledMessage {
public:
    class CCameraTag {                                  // Ego_r.pdb, 24 bytes
    public:
        long CameraPositionPerson;                      // +0x00
        EConversationCameraPosition CameraPosition;     // +0x04
        long CameraFocusPerson;                         // +0x08
        long CameraFocusPosition;                       // +0x0C
        long CameraZoomType;                            // +0x10
        long CameraView;                                // +0x14
    };
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp> class vector {                     // STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, allocator<_Tp> > _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    unsigned int size() { return (unsigned int)(end() - begin()); }
    _Tp& operator[](unsigned int __n) { return *(begin() + __n); }
};
}

extern bool g_UseConversationCameraTags;               // 0x01380C30 (descriptive)
extern long g_ConversationCameraSide;                  // 0x013BAD00 (descriptive)

namespace NTCScriptedControl {
class CActionTalkBase {
public:
    unsigned char _pad00[0x6d];
    bool FarEnoughAwayForFacingCamera;                  // +0x6d (PDB +0x71)
    unsigned char _pad6e[0x8d - 0x6e];
    bool ParentIsDemonDoor;                             // +0x8d (PDB +0x91)
    static std::vector<CSubtitledMessage::CCameraTag> DefaultCameraTags;   // 0x013BAD18
protected:
    long GetNextCameraTagIndex(long current_index);
};

long CActionTalkBase::GetNextCameraTagIndex(long current_index)
{
    if (!g_UseConversationCameraTags)
        return current_index;
    if (ParentIsDemonDoor)
        return 0;
    long result = -1;
    if (current_index == -1) {
        switch (rand() % 3) {
        case 0: result = 0; break;
        case 1: result = 1; break;
        case 2: result = FarEnoughAwayForFacingCamera ? 2 : 0; break;
        }
    } else {
        if (rand() % 3 == 0)
            return current_index;
        switch (g_ConversationCameraSide) {
        case 0:
            result = rand() % (long)(DefaultCameraTags.size() - 1);
            break;
        case 1:
            switch (rand() % 4) {
            case 0: result = 4; break;
            case 1: result = 0; break;
            case 2:
                if (current_index == 4 || current_index == 0 || !FarEnoughAwayForFacingCamera) result = current_index;
                else result = 2;
                break;
            case 3:
                result = 6;
                DefaultCameraTags[6].CameraPosition = CONVERSATION_CAMERA_POS_LEFT;
                break;
            }
            break;
        case 2:
            switch (rand() % 4) {
            case 0: result = 1; break;
            case 1: result = 3; break;
            case 2:
                if (current_index == 1 || current_index == 3 || !FarEnoughAwayForFacingCamera) result = current_index;
                else result = 2;
                break;
            case 3:
                result = 6;
                DefaultCameraTags[6].CameraPosition = CONVERSATION_CAMERA_POS_RIGHT;
                break;
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
}