// CSubtitledMessage::AddCameraTagFromText(long, const CCharString&) -- parse "(<a>,<b>) (<c>,<d>) [(<e>)]" from the text, look each name up in the camera-tag symbol table and append {time, values} to the message's camera-tag list
// Grade: DIFFER(1349v1379) -- per-return inline_depth(0) + POD pair copy; ours spills `this` (sub esp,0x8c) and assigns the six Lookup out-values to other registers/slots (retail: this in esi, From/To/Extra in ebp/ebx/edi, Camera/Target/Spare in frame); probe 4cd3 (single inline_depth(0) region after the third check) matches the first 0x341 bytes but leaves the pair ctor out of line (1198v1379)
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
struct SCameraTagValues {                              // 24-byte POD
    long Camera, Target, From, To, Extra, Spare;
};
struct SCameraTag {                                    // 28-byte pair {time, values}
    long Time;
    SCameraTagValues Values;
};
struct CCameraTagList {
    SCameraTag* first;
    SCameraTag* last;
    SCameraTag* end;
    unsigned char _pad0c;
    bool NeedsSort;                                    // +0x0d
    void push_back(const SCameraTag& tag);             // 0x004D18E0
};
struct CSubtitledMessage {
    unsigned char _pad00[0x34];
    CCameraTagList CameraTags;                         // +0x34
    bool AddCameraTagFromText(long time, const CCharString& text);
};

bool CSubtitledMessage::AddCameraTagFromText(long time, const CCharString& text)
{
    CStringParser parser(text, 0, CCharString(""), 0);
    parser.SkipWhitespace();
    if (!parser.SkipPastNextItemIfIs("("))
        return false;
    CCharString camera = parser.ReadAsStringUntilString(",");
    if (!parser.SkipPastNextItemIfIs(","))
        return false;
    parser.SkipWhitespace();
    CCharString target = parser.ReadAsStringUntilString(")");
    if (!parser.SkipPastNextItemIfIs(")"))
        return false;
#pragma inline_depth(0)
    parser.SkipWhitespace();
    if (!parser.SkipPastNextItemIfIs("("))
        return false;
    CCharString from = parser.ReadAsStringUntilString(",");
    if (!parser.SkipPastNextItemIfIs(","))
        return false;
    parser.SkipWhitespace();
    CCharString to = parser.ReadAsStringUntilString(")");
    if (!parser.SkipPastNextItemIfIs(")"))
        return false;
    parser.SkipWhitespace();
    CCharString extra;
    if (parser.SkipPastNextItemIfIs("(")) {
        extra = parser.ReadAsStringUntilString(")");
        if (!parser.SkipPastNextItemIfIs(")"))
            return false;
    }
    SCameraTagValues values;
    long cameraValue;
    if (!g_CameraTagSymbols.Lookup(camera, &cameraValue))
        return false;
    values.Camera = cameraValue;
    long targetValue;
    if (!g_CameraTagSymbols.Lookup(target, &targetValue))
        return false;
    values.Target = targetValue;
    long fromValue;
    if (!g_CameraTagSymbols.Lookup(from, &fromValue))
        return false;
    values.From = fromValue;
    long toValue;
    if (!g_CameraTagSymbols.Lookup(to, &toValue))
        return false;
    values.To = toValue;
    values.Extra = 0;
    if (extra != "") {
        long extraValue;
        if (!g_CameraTagSymbols.Lookup(extra, &extraValue))
            return false;
        values.Extra = extraValue;
    }
    SCameraTag tag;
    tag.Time = time;
    tag.Values = values;
    CameraTags.push_back(tag);
    CameraTags.NeedsSort = true;
    return true;
}
#pragma inline_depth()
