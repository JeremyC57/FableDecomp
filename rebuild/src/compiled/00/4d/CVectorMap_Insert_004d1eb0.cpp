// CVectorMap::Insert(const float& key, const CCameraTag& value) -- appends a {time, camera tag} pair to the map's array and marks it dirty (re-sorted on the next lookup)
// Leaf: ?Insert@?$CVectorMap@MVCCameraTag@CSubtitledMessage@@V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@@QAEXABMABVCCameraTag@CSubtitledMessage@@@Z
// Retail: Fable.exe @ 0x004D1EB0, 84 bytes (+ int3 padding). Callee: CArray<pair<float, CCameraTag> >::push_back @ 0x004D18E0.
// Identity (agentNF54 name fix; was the descriptive stand-in CVectorMap_Morph24::Add over a made-up 28-byte SMorphPair24 / CMorphPairVector24,
//   catalogued from the propagated "CTextureMorphs::Add"): the callee 0x004D18E0 is the push_back that CSubtitledMessage::
//   AddMasterShotCameraTagFromText 0x004CE1B0 reaches through its inline CVectorMap<float, CCameraTag>::Insert (landed binding
//   ?push_back@?$CArray@U?$pair@MVCCameraTag@CSubtitledMessage@@@std@@@@QAEXABU?$pair@MVCCameraTag@CSubtitledMessage@@@std@@@Z, Ego_r.pdb
//   0x004F04A0), and this body is that Insert's shape (the pair built, push_back, Dirty +0x0D): FableWin.pdb 0x01B72450 names the out-of-line
//   ?Insert@?$CVectorMap@MVCCameraTag@CSubtitledMessage@@V?$CKeyPairCompareLess@MVCCameraTag@CSubtitledMessage@@@@@@QAEXABMABVCCameraTag@CSubtitledMessage@@@Z.
//   No retail caller (callers.py).
// Shape: engine/CVectorMap.h's Insert(key, value) (FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE + _VALUE_TYPE: a value_type temporary,
//   then push_back, then Dirty), explicitly instantiated; push_back stays out of line (FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE).
//   CSubtitledMessage has no shared header: its nested CCameraTag (24 bytes, Ego_r.pdb member names) is the model of the landed 0x004CE1B0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"
enum EConversationProtagonist { CONVERSATION_PROTAGONIST_NONE = 0, CONVERSATION_PROTAGONIST_SPEAKER = 1, CONVERSATION_PROTAGONIST_SPEAKEE = 2 };
enum EConversationCameraPosition { CONVERSATION_CAMERA_IN_FRONT_OF_FACE = 0 };
enum EConversationCameraFocus { CONVERSATION_CAMERA_FOCUS_FACE = 0 };
enum EConversationCameraZoomType { CONVERSATION_CAMERA_NO_ZOOM = 0, CONVERSATION_CAMERA_SLOW_ZOOM = 1, CONVERSATION_CAMERA_FAST_ZOOM = 2 };
enum EConversationCameraView { CONVERSATION_CAMERA_VIEW_WIDE = 0, CONVERSATION_CAMERA_VIEW_MEDIUM = 1, CONVERSATION_CAMERA_VIEW_CLOSE = 2 };

class CSubtitledMessage {
public:
    class CCameraTag {
    public:
        EConversationProtagonist    CameraPositionPerson;   // +0x00
        EConversationCameraPosition CameraPosition;         // +0x04
        EConversationProtagonist    CameraFocusPerson;      // +0x08
        EConversationCameraFocus    CameraFocusPosition;    // +0x0c
        EConversationCameraZoomType CameraZoomType;         // +0x10
        EConversationCameraView     CameraView;             // +0x14
    };
};

template void CVectorMap<float, CSubtitledMessage::CCameraTag>::Insert(const float& key, const CSubtitledMessage::CCameraTag& value);
