// Grade: DIFFER(802v814) 120 of 236 lines (was 148, many are relocation-only push/mov lines): PDB signature ?AddMasterShotCameraTagFromText@CSubtitledMessage@@AAE_NMABVCCharString@@@Z (float time FIRST -- the parked model had the arguments swapped), Ego_r CCameraTag (six enums) inside std::pair<float, CCameraTag> pushed into the CVectorMap (Dirty +0x0d); a CCameraTag local filled field by field after each lookup and copied into the pair by a __forceinline pair ctor reproduces the 0x74 frame and the first camera id spilled to its tag-field home; residue: this in ebp (retail esi) with the three tag registers permuted (retail ebx/ebp/edi = focus/zoom/view), every early frame slot +4, two zero registers for the person fields
// CSubtitledMessage::AddMasterShotCameraTagFromText(const CCharString& text, long time) -- parse "(cameraA, cameraB, target)"
// from the message text, resolve the three names through the global symbol table (an empty target means 1) and append a
// master-shot camera tag at the given time; false when the text or a name does not parse
// Leaf: AddMasterShotCameraTagFromText
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CE1B0, 812 bytes, `ret 8`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CStringParser ctor 0x00404720 / dtor
//   0x004047A0 (inline at the first exits: comment list 0x004046B0, CBase::~CBase 0x0099A2E0), SkipWhitespace
//   0x009B9C60, SkipPastNextItemIfIs 0x009B9E00, ReadAsStringUntilString 0x009BA330, the symbol table's Lookup
//   0x00A38420 on the global 0x013B8A54, operator!=(const char*) 0x0099E960, vector<SCameraTag>::push_back 0x004D18E0.
// Layout: CStringParser as the landed CPhysicalPrimitiveCylinder::Init 0x00816150; the tag vector at +0x34, the
//   "has camera tags" flag at +0x41.
#include <string.h>
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    bool operator!=(const char* text) const;            // 0x0099E960
};
struct CBase { virtual ~CBase(); };                     // dtor 0x0099A2E0
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); };   // 0x004046B0
struct CStringParser : CBase {
    char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
    CStringParserCommentList Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
    CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long);   // 0x00404720
    void SkipWhitespace();                              // 0x009B9C60
    bool SkipPastNextItemIfIs(const CCharString&);      // 0x009B9E00
    CCharString ReadAsStringUntilString(const CCharString&);   // 0x009BA330
};
struct CSymbolTable { bool Lookup(const CCharString& symbol, long* index); };   // 0x00A38420
extern CSymbolTable g_CameraSymbolTable;                // 0x013B8A54
enum EConversationProtagonist { CONVERSATION_PROTAGONIST_0 = 0 };
enum EConversationCameraPosition { CONVERSATION_CAMERA_POSITION_0 = 0 };
enum EConversationCameraFocus { CONVERSATION_CAMERA_FOCUS_0 = 0 };
enum EConversationCameraZoomType { CONVERSATION_CAMERA_ZOOM_0 = 0 };
enum EConversationCameraView { CONVERSATION_CAMERA_VIEW_0 = 0, CONVERSATION_CAMERA_VIEW_1 = 1 };
class CSubtitledMessage;
class CSubtitledMessageCameraTagMap;
class CSubtitledMessage {
public:
    class CCameraTag {                                  // Ego_r.pdb, 24 bytes
    public:
        EConversationProtagonist    CameraPositionPerson;   // +0x00
        EConversationCameraPosition CameraPosition;         // +0x04
        EConversationProtagonist    CameraFocusPerson;      // +0x08
        EConversationCameraFocus    CameraFocusPosition;    // +0x0c
        EConversationCameraZoomType CameraZoomType;         // +0x10
        EConversationCameraView     CameraView;             // +0x14
    };
    struct CCameraTagPair {                              // std::pair<float, CCameraTag>
        float first; CCameraTag second;
        __forceinline CCameraTagPair(const float& a, const CCameraTag& b) : first(a), second(b) { }
    };
    struct CCameraTagMap {                               // CVectorMap<float, CCameraTag> (retail STLport vector 0xc)
        CCameraTagPair* _M_start; CCameraTagPair* _M_finish; CCameraTagPair* _M_end_of_storage;
        unsigned char   Compare;                         // +0x0c
        bool            Dirty;                           // +0x0d
        void push_back(const CCameraTagPair& value);     // 0x004D18E0
    };
    unsigned char _pad00[0x34];
    CCameraTagMap CameraTags;                           // +0x34
    bool AddMasterShotCameraTagFromText(float time, const CCharString& text);
};

bool CSubtitledMessage::AddMasterShotCameraTagFromText(float time, const CCharString& text)
{
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
    CCameraTag tag;
    long cameraA;
    if (!g_CameraSymbolTable.Lookup(first, &cameraA))
        return false;
    tag.CameraPosition = (EConversationCameraPosition)cameraA;
    long value;
    if (!g_CameraSymbolTable.Lookup(second, &value))
        return false;
    tag.CameraFocusPosition = (EConversationCameraFocus)value;
    tag.CameraZoomType = CONVERSATION_CAMERA_ZOOM_0;
    tag.CameraView = CONVERSATION_CAMERA_VIEW_1;
    if (third != "") {
        if (!g_CameraSymbolTable.Lookup(third, &value))
            return false;
        tag.CameraView = (EConversationCameraView)value;
    }
    tag.CameraPositionPerson = CONVERSATION_PROTAGONIST_0;
    tag.CameraFocusPerson = CONVERSATION_PROTAGONIST_0;
    CameraTags.push_back(CCameraTagPair(time, tag));
    CameraTags.Dirty = true;
    return true;
}
