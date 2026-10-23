// Grade: DIFFER(784v814) -- retail inlines ~CStringParser only at the first three exits and calls it out of line (0x004047A0) at the later ones (inline budget), keeps the first camera id in a frame word and the tag in its own 0x1c bytes (frame 0x74 v 0x5c); the parse/lookup shape matches
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
struct SCameraTag {
    long Time;
    long Unused04;
    long CameraA;
    long Unused0C;
    long CameraB;
    long Blend;
    long Target;
};
struct CCameraTagVector {
    SCameraTag* _M_start; SCameraTag* _M_finish; SCameraTag* _M_end;
    unsigned char HasMasterShot;                         // +0x0d of the vector block? (this+0x41)
    void push_back(const SCameraTag& tag);              // 0x004D18E0
};
struct CCameraTags {
    unsigned char _pad[0xd];
    bool HasMasterShot;                                 // +0x0d
    void push_back(const SCameraTag& tag);              // 0x004D18E0
};
class CSubtitledMessage {
public:
    unsigned char _pad00[0x34];
    CCameraTags   CameraTags;                           // +0x34
    bool AddMasterShotCameraTagFromText(const CCharString& text, long time);
};

bool CSubtitledMessage::AddMasterShotCameraTagFromText(const CCharString& text, long time)
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
    long cameraA;
    if (!g_CameraSymbolTable.Lookup(first, &cameraA))
        return false;
    long savedA = cameraA;
    long cameraB;
    if (!g_CameraSymbolTable.Lookup(second, &cameraB))
        return false;
    long b = cameraB;
    long blend = 0;
    long target = 1;
    if (third != "") {
        if (!g_CameraSymbolTable.Lookup(third, &cameraB))
            return false;
        target = cameraB;
    }
    SCameraTag tag;
    tag.Time = time;
    tag.CameraA = savedA;
    tag.Unused04 = 0;
    tag.Unused0C = 0;
    tag.CameraB = b;
    tag.Blend = blend;
    tag.Target = target;
    CameraTags.push_back(tag);
    CameraTags.HasMasterShot = true;
    return true;
}
