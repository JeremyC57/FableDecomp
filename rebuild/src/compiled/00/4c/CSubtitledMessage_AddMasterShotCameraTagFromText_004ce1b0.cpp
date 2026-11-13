// CSubtitledMessage::AddMasterShotCameraTagFromText(float time, const CCharString& text) -- parse "(position, focus[, view])" from a
// subtitle tag's text, resolve the names through the misc_def_types symbol map (an empty view means MEDIUM) and insert a master-shot
// camera tag (no protagonists, no zoom) at `time` into CameraTags; false when the text or a name does not parse.
// Leaf: ?AddMasterShotCameraTagFromText@CSubtitledMessage@@AAE_NMABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CE1B0, 814 bytes, thiscall `ret 8`; sole caller CSubtitledMessage(const NGameText::CTextEntry&)
//   0x004CE550. Identity: Ego_r.pdb 004eb890 / FableWin.pdb 01b70630 (same mangled name).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator!=(const char*) 0x0099E960,
//   CStringParser(const CCharString&, cb, const CCharString&, long) 0x00404720, ~CStringParser 0x004047A0 (expanded once at the
//   merged early exits: list<CStringParserCommentBoundaries>::~list 0x004046B0, ~CBaseClass 0x0099A2E0), SkipWhitespace 0x009B9C60,
//   SkipPastNextItemIfIs 0x009B9E00, ReadAsStringUntilString 0x009BA330, CCRCSymbolMap::FindSymbol 0x00A38420 on the map at
//   0x013B8A54, CArray<pair<float,CCameraTag> >::push_back 0x004D18E0 (through CVectorMap::Insert, then Dirty +0x41).
// Shape: the CCameraTag local is scalar-replaced; only CameraPosition reaches its frame home (+0x24) because the second FindSymbol
//   call follows. The protagonist fields are set at the declaration (one shared zero in eax at the pair build), zoom / default view
//   live in ebp / edi across the third lookup. The lookups are a nested positive `if` (failure blocks after the success path, the
//   first lookup's exit merged with the ")" exit) with the second lookup's failure an early return inside it (its exit cross-jumps
//   into the same block with ecx = &third preloaded, shared with operator!=).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CStringParser.h"
#include "engine/CVectorMap.h"

class CCRCSymbolMap {
public:
    bool FindSymbol(const CCharString& name, long& value) const;   // 0x00A38420
};
extern CCRCSymbolMap g_MiscDefTypesSymbolMap;           // 0x013B8A54

// Enumerators: installed-game misc_def_types.h, landed spelling (0x00712340 / 0x00719A70).
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
    unsigned char _pad00[0x34];
    CVectorMap<float, CCameraTag> CameraTags;               // +0x34 (Dirty +0x41)
private:
    bool AddMasterShotCameraTagFromText(float time, const CCharString& text);
};

bool CSubtitledMessage::AddMasterShotCameraTagFromText(float time, const CCharString& text)
{
    CCameraTag tag;
    tag.CameraPositionPerson = CONVERSATION_PROTAGONIST_NONE;
    tag.CameraFocusPerson = CONVERSATION_PROTAGONIST_NONE;
    long position;
    long focus;
    CStringParser parser(text, 0, CCharString(""), 0);
    parser.SkipWhitespace();
    if (!parser.SkipPastNextItemIfIs("("))
        return false;
    CCharString first = parser.ReadAsStringUntilString(",");
    if (!parser.SkipPastNextItemIfIs(","))
        return false;
    parser.SkipWhitespace();
    CCharString second = parser.ReadAsStringUntilString(",");
    if (!parser.SkipPastNextItemIfIs(","))
        return false;
    parser.SkipWhitespace();
    CCharString third = parser.ReadAsStringUntilString(")");
    if (!parser.SkipPastNextItemIfIs(")"))
        return false;
    if (g_MiscDefTypesSymbolMap.FindSymbol(first, position)) {
        tag.CameraPosition = (EConversationCameraPosition)position;
        if (!g_MiscDefTypesSymbolMap.FindSymbol(second, focus))
            return false;
        tag.CameraFocusPosition = (EConversationCameraFocus)focus;
        tag.CameraZoomType = CONVERSATION_CAMERA_NO_ZOOM;
        tag.CameraView = CONVERSATION_CAMERA_VIEW_MEDIUM;
        if (third != "") {
            long view;
            if (g_MiscDefTypesSymbolMap.FindSymbol(third, view))
                tag.CameraView = (EConversationCameraView)view;
            else
                return false;
        }
        CameraTags.Insert(time, tag);
        return true;
    }
    return false;
}