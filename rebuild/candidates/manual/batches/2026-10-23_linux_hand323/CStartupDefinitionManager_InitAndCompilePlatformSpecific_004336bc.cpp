// CStartupDefinitionManager::InitAndCompilePlatformSpecific(bool) -- build the PC front-end header list (bank-directory headers, Data\Defs front-end defs; the test front-end def or the PC front-end/config/controller set) and the def list to compile, register the five startup definition types with their creators, hand both lists to the definition manager and run the base InitAndCompile
// Leaf: InitAndCompilePlatformSpecific
// Retail: Fable.exe (TLC, VC7.1) @ 0x004336BC, 1505 bytes, `ret 4`; size-optimised TU with an ebp frame (#pragma optimize("s", on) + ("y", off)).
// Callees: CWideString(const unsigned short*) 0x0099B6B0 / dtor 0x0099B510, the bank directory 0x009A76A0 and defs directory "Data\Defs\" 0x0041A080
//   (__fastcall, hidden result in ecx), operator+(const CWideString&, const CWideString&) 0x0099BE70 / (const CWideString&, const unsigned short*)
//   0x0099BF30 (__fastcall, hidden result in ecx), vector<CWideString>::push_back 0x004128A0 / dtor 0x00412130 / operator= 0x00433DF0,
//   CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0, CDefinitionManager::AddDefinitionType 0x009B0AC0,
//   CDefinitionManager::InitAndCompile 0x009B08C0; creators 0x00434540 / 0x004343F0 / 0x00433670 / 0x00432180 / 0x00434660.
#pragma optimize("s", on)
#pragma optimize("y", off)

class CWideString {
public:
    unsigned short* m_pData;
    CWideString(const unsigned short* text);                  // 0x0099B6B0
    ~CWideString();                                    // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);       // 0x0099BF30
CWideString __fastcall GetBankDirectory();             // 0x009A76A0
CWideString __fastcall GetDefsDirectory();             // 0x0041A080

class CCharString {
public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
class CWideStringVector {                              // STLport vector<CWideString> (12 bytes)
public:
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    CWideStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CWideStringVector();                              // 0x00412130
    void push_back(const CWideString& x);              // 0x004128A0
    CWideStringVector& operator=(const CWideStringVector& other);   // 0x00433DF0
};
class CDefinitionManager;
typedef void* (*TDefCreator)();
struct SDefinitionType {
    CCharString Name;
    TDefCreator Creator;
    SDefinitionType(const CCharString& name, TDefCreator creator) : Name(name), Creator(creator) { }
};
void* CreateEngineVideoOptionsDef();                   // 0x00434540
void* CreateEngineDef();                               // 0x004343F0
void* CreateUIDef();                                   // 0x00433670
void* CreateControlSchemeDef();                        // 0x00432180
void* CreateConfigOptionsDefaultsDef();                // 0x00434660

class CDefinitionManager {
public:
    virtual ~CDefinitionManager();
    unsigned char _pad04[0x40 - 0x04];
    CWideStringVector HeaderFiles;                     // +0x40
    CWideStringVector DefFiles;                        // +0x4c
    unsigned char _pad58[0xf0 - 0x58];
    void AddDefinitionType(const SDefinitionType& type);   // 0x009B0AC0
    void InitAndCompile();                             // 0x009B08C0
};
class CStartupDefinitionManager : public CDefinitionManager {
public:
    void InitAndCompilePlatformSpecific(bool testFrontEnd);
};

void CStartupDefinitionManager::InitAndCompilePlatformSpecific(bool testFrontEnd)
{
    CWideStringVector headers;
    CWideStringVector defs;
    CWideString platform(L"pc\\");
    headers.push_back(GetBankDirectory() + platform + L"textures.h");
    headers.push_back(GetBankDirectory() + L"fonts.h");
    headers.push_back(GetBankDirectory() + platform + L"front_end_bank.h");
    headers.push_back(GetBankDirectory() + platform + L"particles_frontend.h");
    headers.push_back(GetDefsDirectory() + L"FrontEndDefs\\engine.def");
    headers.push_back(GetDefsDirectory() + L"FrontEndDefs\\engine_video_options.def");
    headers.push_back(GetDefsDirectory() + L"FrontEndDefs\\front_end.def");
    headers.push_back(GetDefsDirectory() + L"ui_types.h");
    headers.push_back(GetDefsDirectory() + L"ui_dialogs.def");
    if (testFrontEnd) {
        headers.push_back(GetDefsDirectory() + L"FrontEndDefs\\frontend_test.def");
    } else {
        headers.push_back(GetDefsDirectory() + L"FrontEndDefs\\pc_frontend.def");
        headers.push_back(GetDefsDirectory() + L"config_options_defaults.def");
        headers.push_back(GetDefsDirectory() + L"controller_def.h");
        headers.push_back(GetDefsDirectory() + L"keyboard_keys.h");
        headers.push_back(GetDefsDirectory() + L"pc_controls.def");
    }
    defs.push_back(GetDefsDirectory() + L"FrontEndDefs\\engine.def");
    defs.push_back(GetDefsDirectory() + L"FrontEndDefs\\engine_video_options.def");
    defs.push_back(GetDefsDirectory() + L"FrontEndDefs\\front_end.def");
    defs.push_back(GetDefsDirectory() + L"ui_dialogs.def");
    if (testFrontEnd) {
        defs.push_back(GetDefsDirectory() + L"FrontEndDefs\\frontend_test.def");
    } else {
        defs.push_back(GetDefsDirectory() + L"FrontEndDefs\\pc_frontend.def");
        defs.push_back(GetDefsDirectory() + L"pc_controls.def");
        defs.push_back(GetDefsDirectory() + L"config_options_defaults.def");
    }
    AddDefinitionType(SDefinitionType("ENGINE_VIDEO_OPTIONS", CreateEngineVideoOptionsDef));
    AddDefinitionType(SDefinitionType("ENGINE", CreateEngineDef));
    AddDefinitionType(SDefinitionType("UI", CreateUIDef));
    AddDefinitionType(SDefinitionType("CONTROL_SCHEME", CreateControlSchemeDef));
    AddDefinitionType(SDefinitionType("CONFIG_OPTIONS_DEFAULTS_DEF", CreateConfigOptionsDefaultsDef));
    HeaderFiles = headers;
    DefFiles = defs;
    InitAndCompile();
}
