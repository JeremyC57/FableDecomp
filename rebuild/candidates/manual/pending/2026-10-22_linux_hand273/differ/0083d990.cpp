// CThingPhysicalSwitch::ConstructFromLevelScript(char* script) -- base game-object construction, then parse the level-script block: "TriggeredByThing <name>" (NULL = none) sets the triggering thing's name, "EndThing" ends the block
// Grade: DIFFER(464v460) -- calls, strings, conditional "" temporary and parser teardown are right; differs in block layout and slot packing: retail keeps the token in the frame and homes the TriggeredByThing value in the dead `script` slot (ours swaps them), caches the token's string block in esi across both compares, pushes edi only inside the loop, and places the EndThing return block right after the null-block strcmp chain with the TriggeredByThing body after it; tried an early-return base test, an inverted `!(item == ...)` arm, an inline reader helper (not inlined)
// Leaf: ConstructFromLevelScript
// Callees: CThingGameObject::ConstructFromLevelScript @ 0x00838930, CStringParser::CStringParser @ 0x00414E30, CStringParser::
// NextItemExists @ 0x009BA4F0, CStringParser::ReadAsString @ 0x009BA6F0 (by value), CBasicString<char>::operator==(const char*)
// @ 0x004115A0, CCharString::operator= @ 0x0099EFB0, the parser's keyword search @ 0x009B9BA0, CCharString(const char*, long)
// @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, list<CStringParserCommentBoundaries>::~list @ 0x004046B0, ~CBase @ 0x0099A2E0.
// Strings: "" 0x0122D70E, "TriggeredByThing" 0x012730B0, "EndThing" 0x01239B8C, "NULL" 0x012393E4, ";" 0x0123542C.
// Layout: retail CStringParser (landed 0x00414E30 model, 0x28 bytes); CThingPhysicalSwitch flag byte +0xd0, triggering
// thing name +0xd4 (descriptive).
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

class CBase {
public:
    virtual ~CBase();                           // 0x0099A2E0
};
struct CStringParserCommentList {
    void* m_pHead;
    ~CStringParserCommentList();                // 0x004046B0
};
struct CCursorPos {
    bool          InQuotes;
    unsigned long StringPos;
    unsigned long Line;
};
typedef void (*TStringParserErrorCallback)(CCharString* message);
class CStringParser : public CBase {            // vtable 0x0122D874
public:
    char*                      String;          // +0x04
    unsigned long              BaseLineOffset;  // +0x08
    CCharString                Filename;        // +0x0C
    CCharString                TempCharString;  // +0x10
    CStringParserCommentList   Comments;        // +0x14
    TStringParserErrorCallback ErrorCallback;   // +0x18
    CCursorPos                 CursorPos;       // +0x1C
    CStringParser(char* text, TStringParserErrorCallback errorCallback, const CCharString& filename, long baseLineOffset);   // 0x00414E30
    bool NextItemExists();                      // 0x009BA4F0
    CCharString ReadAsString();                 // 0x009BA6F0
    bool FindKeyword(const CCharString& keyword);   // 0x009B9BA0
};

class CThingGameObject {
public:
    virtual void v00();
    unsigned char _pad04[0xd0 - 0x04];
    bool ConstructFromLevelScript(char* script);   // 0x00838930
};
class CThingPhysicalSwitch : public CThingGameObject {
public:
    bool        TriggeredBySet;                 // +0xd0
    unsigned char _padd1[3];
    CCharString TriggeredByThing;               // +0xd4
    bool ConstructFromLevelScript(char* script);
};

bool CThingPhysicalSwitch::ConstructFromLevelScript(char* script)
{
    if (CThingGameObject::ConstructFromLevelScript(script)) {
        TriggeredBySet = false;
        CStringParser parser(script, 0, CCharString(""), 0);
        while (parser.NextItemExists()) {
            CCharString item = parser.ReadAsString();
            if (item == "TriggeredByThing") {
                CCharString value = parser.ReadAsString();
                TriggeredByThing = value == "NULL" ? CCharString("") : value;
            }
            else if (item == "EndThing")
                return true;
            parser.FindKeyword(";");
        }
        return true;
    }
    return false;
}
