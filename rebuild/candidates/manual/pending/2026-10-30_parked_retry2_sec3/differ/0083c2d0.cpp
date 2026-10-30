// Grade: DIFFER(767v756) -- 171 difflines (was 760/242): parser.NextItemIs(";") passes the literal to the const CCharString& parameter (temp built first, as retail). Residue: the item string sits at +0x50 (retail +0x14, frame bottom) and its chars pointer is re-derived per else-if where retail jump-threads the null case (edi = "" before each repe cmpsb). Tried: literal for the CStringParser ctor default (770/246, worse)
// CThingSwitch::ConstructFromLevelScript(const char* levelScript) -- base CThingGameObject construction, clear Triggered, then parse "TriggerRadius", "TimeToChangeEnvironmentDef" (floats), "TriggeredByThing", "EnvironmentDef" (strings, "NULL" -> "") until "EndThing", each item followed by ";"
// Retail: Fable.exe @ 0x0083C2D0, 768 bytes (`ret 4`); mangled ?ConstructFromLevelScript@CThingSwitch@@MAE_NPBD@Z (protected, const char*).
// Callees: CThingGameObject::ConstructFromLevelScript 0x00838930, CStringParser ctor 0x00414E30 + inline dtor (0x004046B0, 0x0099A2E0),
//   NextItemExists 0x009BA4F0, ReadAsString 0x009BA6F0, ReadAsFloat 0x009BA650, NextItemIs 0x009B9BA0, CCharString 0x0099EBF0 / 0x0099EAE0 / operator= 0x0099EFB0, CBasicString<char>::operator== 0x004115A0.
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);                           // 0x0099EBF0
    ~CCharString();                                                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);                          // 0x0099EFB0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CBaseClass {
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
};
struct CStringParserCommentList {
    void* m_pHead;
    ~CStringParserCommentList();                   // 0x004046B0
};
struct CStringParser : CBaseClass {                // retail 0x28 bytes
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0c
    CCharString              TempCharString;       // +0x10
    CStringParserCommentList Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x18
    long                     CursorPos[3];         // +0x1c
    CStringParser(char* text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);            // 0x00414E30
    bool NextItemIs(const CCharString& item);      // 0x009B9BA0
    bool NextItemExists();                         // 0x009BA4F0
    float ReadAsFloat();                           // 0x009BA650
    CCharString ReadAsString();                    // 0x009BA6F0
};

struct CThingGameObject {
    unsigned char _pad04[0xd0 - 4];
    virtual bool ConstructFromLevelScript(const char* levelScript);   // 0x00838930
};
struct CThingSwitch : CThingGameObject {
    float         TriggerRadius;                         // +0xd0
    CCharString   EnvironmentDef;                        // +0xd4
    float         TimeToChangeEnvironmentDef;            // +0xd8
    bool          Triggered;                             // +0xdc
    unsigned char _pad_dd[3];
    CCharString   TriggerableObjectScriptName;           // +0xe0
    virtual bool ConstructFromLevelScript(const char* levelScript);
};

bool CThingSwitch::ConstructFromLevelScript(const char* levelScript)
{
    if (CThingGameObject::ConstructFromLevelScript(levelScript)) {
        Triggered = false;
        CStringParser parser((char*)levelScript, 0, CCharString(""), 0);
        while (parser.NextItemExists()) {
            CCharString item = parser.ReadAsString();
            if (item == "TriggerRadius")
                TriggerRadius = parser.ReadAsFloat();
            else if (item == "TimeToChangeEnvironmentDef")
                TimeToChangeEnvironmentDef = parser.ReadAsFloat();
            else if (item == "TriggeredByThing") {
                CCharString name = parser.ReadAsString();
                TriggerableObjectScriptName = name == "NULL" ? CCharString("") : name;
            }
            else if (item == "EnvironmentDef") {
                CCharString name = parser.ReadAsString();
                EnvironmentDef = name == "NULL" ? CCharString("") : name;
            }
            else if (item == "EndThing")
                return true;
            parser.NextItemIs(";");
        }
        return true;
    }
    return false;
}
