// Grade: DIFFER(1198v1379) 192 of 380 lines (was 302): PDB signature ?AddCameraTagFromText@CSubtitledMessage@@AAE_NMABVCCharString@@@Z (float time), Ego_r CCameraTag filled field by field (Person=camera, Position=target, FocusPerson=from, FocusPosition=to, ZoomType=extra or 0; CameraView is never assigned -- retail copies the uninitialised home word) and copied into std::pair<float, CCameraTag>; the parked inline_depth(0) region after the third parse check (later exits call ~CStringParser 0x004047A0 out of line as retail); residue: under inline_depth(0) the pair ctor stays out of line, so the tag lives in memory and the four lookup failures merge into one exit (retail: inline pair copy, tag fields in ebp/ebx/edi, one exit per lookup); restoring inline_depth around the push_back or a POD member copy makes the whole function inline (245)
// CSubtitledMessage::AddCameraTagFromText(long, const CCharString&) -- parse "(<a>,<b>) (<c>,<d>) [(<e>)]" from the text, look each name up in the camera-tag symbol table and append {time, values} to the message's camera-tag list
// Leaf: AddCameraTagFromText
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CDC40, 1377 bytes (+ int3 padding), `ret 8`.
// Literals "" 0x0122D70E, "(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020.
// Callees: CStringParser ctor 0x00404720 / out-of-line dtor 0x004047A0 (comment-list dtor 0x004046B0, ~CBase 0x0099A2E0 in the inline
//   copy), SkipWhitespace 0x009B9C60, SkipPastNextItemIfIs 0x009B9E00, ReadAsStringUntilString 0x009BA330, CCharString 0x0099EBF0 /
//   default ctor 0x0099E4B0 / dtor 0x0099EAE0 / operator= 0x0099EFB0 / operator!=(const char*) 0x0099E960, camera-tag symbol table
//   Lookup 0x00A38420 (object 0x013B8A54), camera-tag vector push_back 0x004D18E0 (landed: 28-byte pair element).
// Layout: CSubtitledMessage camera-tag list {first, last, end} at +0x34 with its needs-sort flag at +0x41.
struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    bool operator!=(const char* text) const;           // 0x0099E960
};
struct CBase {
    virtual void v00();
    ~CBase();                                          // 0x0099A2E0
};
struct CStringParserCommentList {                      // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                       // 0x004046B0
};
struct CStringParser : CBase {                         // retail 0x28 bytes; implicit dtor (out-of-line copy 0x004047A0)
    char* String;                                      // +0x04
    unsigned long BaseLineOffset;                      // +0x08
    CCharString Filename;                              // +0x0c
    CCharString TempCharString;                        // +0x10
    CStringParserCommentList Comments;                 // +0x14
    void (*ErrorCallback)(CCharString*);               // +0x18
    long CursorPos[3];                                 // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);                // 0x00404720
    long SkipWhitespace();                             // 0x009B9C60
    bool SkipPastNextItemIfIs(const CCharString& item);   // 0x009B9E00
    CCharString ReadAsStringUntilString(const CCharString& end);   // 0x009BA330
};
struct CCameraTagSymbolTable {
    bool Lookup(const CCharString& name, long* value) const;   // 0x00A38420
};
extern CCameraTagSymbolTable g_CameraTagSymbols;       // 0x013B8A54
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
    bool AddCameraTagFromText(float time, const CCharString& text);
};

bool CSubtitledMessage::AddCameraTagFromText(float time, const CCharString& text)
{
    CStringParser parser(text, 0, CCharString(""), 0);
    parser.SkipWhitespace();
    if (!parser.SkipPastNextItemIfIs("("))
        return false;
    CCharString cameraName = parser.ReadAsStringUntilString(",");
    if (!parser.SkipPastNextItemIfIs(","))
        return false;
    parser.SkipWhitespace();
    CCharString targetName = parser.ReadAsStringUntilString(")");
    if (!parser.SkipPastNextItemIfIs(")"))
        return false;
#pragma inline_depth(0)
    parser.SkipWhitespace();
    if (!parser.SkipPastNextItemIfIs("("))
        return false;
    CCharString fromName = parser.ReadAsStringUntilString(",");
    if (!parser.SkipPastNextItemIfIs(","))
        return false;
    parser.SkipWhitespace();
    CCharString toName = parser.ReadAsStringUntilString(")");
    if (!parser.SkipPastNextItemIfIs(")"))
        return false;
    parser.SkipWhitespace();
    CCharString extra;
    if (parser.SkipPastNextItemIfIs("(")) {
        extra = parser.ReadAsStringUntilString(")");
        if (!parser.SkipPastNextItemIfIs(")"))
            return false;
    }
    CCameraTag tag;
    long camera;
    if (!g_CameraTagSymbols.Lookup(cameraName, &camera))
        return false;
    tag.CameraPositionPerson = (EConversationProtagonist)camera;
    long value;
    if (!g_CameraTagSymbols.Lookup(targetName, &value))
        return false;
    tag.CameraPosition = (EConversationCameraPosition)value;
    long from;
    if (!g_CameraTagSymbols.Lookup(fromName, &from))
        return false;
    tag.CameraFocusPerson = (EConversationProtagonist)from;
    if (!g_CameraTagSymbols.Lookup(toName, &value))
        return false;
    tag.CameraFocusPosition = (EConversationCameraFocus)value;
    tag.CameraZoomType = CONVERSATION_CAMERA_ZOOM_0;
    if (extra != "") {
        if (!g_CameraTagSymbols.Lookup(extra, &value))
            return false;
        tag.CameraZoomType = (EConversationCameraZoomType)value;
    }
    CameraTags.push_back(CCameraTagPair(time, tag));
    CameraTags.Dirty = true;
    return true;
}
