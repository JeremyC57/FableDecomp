#pragma once
#ifdef FABLE_RETAIL_CSUBTITLEDMESSAGE_PASS29
#ifdef FABLE_RETAIL_SUBTITLE_GETMESSAGES_PASS30
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/CVectorMap.h"
#include "fable_string.h"
namespace NGameText { class CTextEntry; }
template<class T> class CDefPointer;
class CGlobalDef;
#endif
class CASoundBank;
#ifdef FABLE_RETAIL_SUBTITLED_MESSAGE_PASS29
enum EConversationAttitude {CONVERSATION_ATTITUDE_NULL = 0, CONVERSATION_ATTITUDE_NEUTRAL = 1, CONVERSATION_ATTITUDE_LOVE = 2, CONVERSATION_ATTITUDE_ANGER = 3, CONVERSATION_ATTITUDE_FEAR = 4, CONVERSATION_ATTITUDE_GLEE = 5, CONVERSATION_ATTITUDE_SADNESS = 6, CONVERSATION_ATTITUDE_RIDICULE = 7, CONVERSATION_ATTITUDE_FRIENDLY = 8, CONVERSATION_ATTITUDE_BELLIGERENT = 9, CONVERSATION_ATTITUDE_CAGED = 10, CONVERSATION_ATTITUDE_HOLDING_SWORD = 11, MAX_CONVERSATION_ATTITUDES = 12};
#else
enum EConversationAttitude;
#endif
namespace NConversationTypes {
enum EConversationProtagonist { NONE = 0, SPEAKER = 1, SPEAKEE = 2 };
enum EConversationCameraPosition { IN_FRONT_OF_FACE = 0, LEFT_SHOULDER = 1, RIGHT_SHOULDER = 2, BEHIND_LEFT_SHOULDER_NEAR = 3, BEHIND_LEFT_SHOULDER_FAR = 4, BEHIND_RIGHT_SHOULDER_NEAR = 5, BEHIND_RIGHT_SHOULDER_FAR = 6, LEFT_OF_TORSO_NEAR = 7, LEFT_OF_TORSO_FAR = 8, RIGHT_OF_TORSO_NEAR = 9, RIGHT_OF_TORSO_FAR = 10, POS_LEFT = 11, POS_RIGHT = 12 };
enum EConversationCameraFocus { FOCUS_FACE = 0, FOCUS_10 = 1, FOCUS_25 = 2, FOCUS_33 = 3, FOCUS_50 = 4, FOCUS_HIGH = 5, FOCUS_LEVEL = 6, FOCUS_LOW = 7 };
enum EConversationCameraZoomType { NO_ZOOM = 0, SLOW_ZOOM = 1, FAST_ZOOM = 2 };
enum EConversationCameraView { VIEW_WIDE = 0, VIEW_MEDIUM = 1, VIEW_CLOSE = 2 };
}

class CSubtitledMessage {
public:
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_INIT_PASS30
    static void FABLE_FASTCALL Initialise();  // 004CDB10.
#endif
    class CCameraTag {
    public:
        NConversationTypes::EConversationProtagonist    CameraPositionPerson;   // +0x00
        NConversationTypes::EConversationCameraPosition CameraPosition;         // +0x04
        NConversationTypes::EConversationProtagonist    CameraFocusPerson;      // +0x08
        NConversationTypes::EConversationCameraFocus    CameraFocusPosition;    // +0x0c
        NConversationTypes::EConversationCameraZoomType CameraZoomType;         // +0x10
        NConversationTypes::EConversationCameraView     CameraView;             // +0x14
    };
#ifdef FABLE_RETAIL_SUBTITLED_MESSAGE_PASS29
    CSubtitledMessage(const NGameText::CTextEntry&);
#endif
#ifdef FABLE_RETAIL_SUBTITLE_GETMESSAGES_PASS30
    CSubtitledMessage(const CWideString&);
    CSubtitledMessage(const CSubtitledMessage&);
    static void FABLE_FASTCALL GetMessagesFromTextName(CArray<CSubtitledMessage>&, const char*, const char*, ETextGroupSelectionMethod);
    static CSubtitledMessage FABLE_FASTCALL GetMessageFromTextIndex(unsigned long, const char*);
private:
    static long LastReturnedIndex;
    static CCharString MostRecentGroupQueried;
#endif
private:
    CASoundBank* PSpeechBank;
    long SpeechIndex;
    bool Speech3D;
    CWideString Text;
    CCharString Narrator;
    CVectorMap<float, EConversationAttitude> AttitudeTags;
    CVectorMap<float, CCharString> AnimationTags;
    CVectorMap<float, CCameraTag> CameraTags;               // +0x34 (Dirty +0x41)
private:
#ifdef FABLE_RETAIL_SUBTITLED_MESSAGE_PASS29
    bool AddMasterShotCameraTagFromText(float, const CCharString&);
    bool AddCameraTagFromIndex(long, float, CDefPointer<const CGlobalDef>);
#endif
    bool AddCameraTagFromText(float time, const CCharString& text);
#ifdef FABLE_RETAIL_SUBTITLE_GETMESSAGES_PASS30
public:
    ~CSubtitledMessage();
#endif
};


#endif
