// CAIStateGroup_OpinionReactionRoot::WifeToHerselfUpdate(EOpinionAttitudeType attitude) -- say "TEXT_AI_WIFE_OPINION_ACT_TOHERSELF_" + the attitude's name to the thing being reacted to; true once the speech action has finished (EAIActionReturn 2)
// Leaf: WifeToHerselfUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982F70, length 86 bytes.
// Identity: the catalog label "CPlayer::SayWifeOpinionToHerself" is a guess; Ego_r.pdb has
// `?WifeToHerselfUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NW4EOpinionAttitudeType@@@Z` after
// WifeFeelingUpdate and WifeGeneralUpdate, 0x60 bytes apart, exactly like 0x00982EB0 / F10 / F70,
// and the body calls the class's WaitForTextToLoadThenSpeak on `this`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the state group's getter
// @ 0x008FCFC0 (returns this->[+4]->[+0x1C]->[+0x24]; called for nothing: the result is dropped;
// manifest label "CPlayer::GetCurrentMode"), CCharString::operator+=(const CCharString&) @
// 0x0099F0A0, CAIStateGroup_OpinionReactionRoot::WaitForTextToLoadThenSpeak(const CCharString&,
// NSpeechGainManager::EDialogueLayer) @ 0x0095ED80 (layer 2), CCharString::~CCharString @ 0x0099EAE0; the attitude-name table @ 0x013BABD0
// (CCharString per EOpinionAttitudeType); the literal @ 0x01299964.
// Shape: a named CCharString local in the `push ecx` slot (explicit (literal, -1) constructor),
// the bool `== 2` kept in bl across the destructor (`sete bl ; ... mov al,bl`), `ret 4`.
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE = 0 };
enum EAIActionReturn { AI_ACTION_RETURN_RUNNING = 0, AI_ACTION_RETURN_FAILED = 1, AI_ACTION_RETURN_FINISHED = 2 };

class CCharString {
public:
    CCharString(const char* text, long length);           // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);     // 0x0099F0A0
private:
    void* m_pData;
};
extern CCharString g_OpinionAttitudeNames[];               // 0x013BABD0

namespace NSpeechGainManager { enum EDialogueLayer { DIALOGUE_LAYER_2 = 2 }; }

struct CAIStateGroup_OpinionReactionRoot {
    long GetReactionContext();                             // 0x008FCFC0
    EAIActionReturn WaitForTextToLoadThenSpeak(const CCharString& text, NSpeechGainManager::EDialogueLayer layer);   // 0x0095ED80
    bool WifeToHerselfUpdate(EOpinionAttitudeType attitude);
};

bool CAIStateGroup_OpinionReactionRoot::WifeToHerselfUpdate(EOpinionAttitudeType attitude)
{
    CCharString text("TEXT_AI_WIFE_OPINION_ACT_TOHERSELF_", -1);
    GetReactionContext();
    text += g_OpinionAttitudeNames[attitude];
    return WaitForTextToLoadThenSpeak(text, NSpeechGainManager::DIALOGUE_LAYER_2) == AI_ACTION_RETURN_FINISHED;
}
