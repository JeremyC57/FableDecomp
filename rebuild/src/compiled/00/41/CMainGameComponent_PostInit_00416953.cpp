// CMainGameComponent::PostInit() -- hand the display engine to the world, then either load the save named in the init
//   structure (flagging a failed load) or load the world file (the init structure's world, else the remembered one, else
//   updatedscenic.wld from the levels directory); in static-map export mode copy the world's map files to their static
//   paths and stop, otherwise run the post-load step and activate the initial quest; finally register the component
// Leaf: ?PostInit@CMainGameComponent@@MAEXXZ
// Retail: Fable.exe @ 0x00416953, 738 bytes, frame pointer, no EH frame (the TU has no unwind tables).
// Layout: CMainGameComponent as in the landed ForceRender 0x004164E0 (retail offsets); InitStructure +0x161d0 holds
//   (retail loads) a world name +0x0 and a second wide string +0x4, the initial quest CCharString +0x8 and the save name +0xc.
// Names: GetMapPath / GetMapPath2 (0x0049DDD0 / 0x0049DEC0), PostLoad 0x0049F180, Register 0x004BBC00
//   and the globals 0x013B89C0 / 0x013B8648 / 0x013B8616 / 0x013B8674 are descriptive (no PDB name); literals 0x0122EE48
//   "Loading save", 0x0122EE38 "Loading world", 0x0122EE14 L"updatedscenic.wld", 0x0122EDFC "Activate Initial Quests",
//   0x0122D70E "", -1.0f 0x0122DEE0.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString { public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const char* text) const;           // 0x0099E960
};
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
    const CWideString& operator=(const CWideString& other);  // 0x0099B7D0
    long GetLength() const;                            // 0x0099B220
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);  // 0x0099BF30 (wchar_t is unsigned short under VC7.1 defaults)
CWideString __fastcall GetLevelsDirectory();                                     // 0x0041A060
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CDiskFileWin32 { public:                                // 0x1c bytes
    unsigned char _body[0x18];
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual ~CDiskFileWin32();                                 // 0x00401630 (out-of-line copy)
};
void __fastcall CopyFileW(CDiskFileWin32& from, CDiskFileWin32& to);   // 0x00998E70
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
void __fastcall LoadingStage(const CCharString& stage, long level);     // 0x009D8240
void __fastcall LoadingStageDone(long level);                           // 0x009D8250
class CQuestManager { public: void ActivateQuest(const CCharString& name, bool a, bool b); };   // 0x004B4A10
extern CQuestManager* g_pQuestManager;                 // 0x013B89FC
extern bool        g_bSaveLoadFailed;                  // 0x013B89C0
extern CWideString g_WorldToLoad;                      // 0x013B8668
extern CWideString g_WorldToLoad2;                     // 0x013B866C
extern bool        g_bExportStaticMaps;                // 0x013B8648
extern bool        g_b13B8616;                         // 0x013B8616
struct CRegistration;
extern CRegistration g_Registration;                   // 0x013B8674
class CDisplayEngine;
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual void SetDisplayEngine(CDisplayEngine* engine);                  // slot 7 (+0x1c)
    bool LoadGame(const CWideString& name, bool b);                          // 0x004A3200
    void Load(const CWideString& path);                                      // 0x004A1840
    CWideString GetMapPath(const CWideString& path) const;                   // 0x0049DDD0
    CWideString ConvertWorldToStaticMapPath(const CWideString& path, bool a, bool b) const;   // 0x0049D550
    CWideString GetMapPath2(const CWideString& path) const;                  // 0x0049DEC0
    CWideString ConvertWorldToStaticMapPath2(const CWideString& path, bool a, bool b) const;  // 0x0049D6B0
    void PostLoad(long a);                                                   // 0x0049F180
};
template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
};
class CMainGameComponentInit { public:
    CWideString WorldName;                             // +0x0
    CWideString WorldName2;                            // +0x4
    CCharString InitialQuest;                          // +0x8
    CWideString SaveName;                              // +0xc
};
class CMainGameComponent {
public:
    virtual ~CMainGameComponent();
    unsigned char                  _pad004[0x24 - 4];
    scoped_ptr<CWorld>             PWorld;             // +0x24
    scoped_ptr<CDisplayEngine>     PDisplayEngine;     // +0x28
    unsigned char                  _pad02c[0x161d0 - 0x2c];
    CMainGameComponentInit         InitStructure;      // +0x161d0
    void Register(CRegistration* registration);        // 0x004BBC00

protected:
    virtual void PostInit();
};

void CMainGameComponent::PostInit()
{
    PWorld->SetDisplayEngine(PDisplayEngine.get());
    if (InitStructure.SaveName.GetLength() > 0) {
        NProgressDisplay::DisplayProgress("Loading save", -1.0f, false, false);
        if (!PWorld->LoadGame(InitStructure.SaveName, true))
            g_bSaveLoadFailed = true;
    } else {
        NProgressDisplay::DisplayProgress("Loading world", -1.0f, false, false);
        CWideString path;
        if (InitStructure.WorldName.GetLength() > 0) {
            path = GetLevelsDirectory() + InitStructure.WorldName;
            g_WorldToLoad = InitStructure.WorldName;
            if (InitStructure.WorldName2.GetLength() > 0)
                g_WorldToLoad2 = InitStructure.WorldName2;
        } else if (g_WorldToLoad.GetLength() > 0) {
            path = GetLevelsDirectory() + g_WorldToLoad;
        } else {
            path = GetLevelsDirectory() + L"updatedscenic.wld";
        }
        PWorld->Load(path);
        if (g_bExportStaticMaps) {
            CWideString mapPath = PWorld->GetMapPath(path);
            CDiskFileWin32 mapFrom(mapPath, FILE_ACCESS_READ, 2);
            CWideString staticMapPath = PWorld->ConvertWorldToStaticMapPath(path, true, g_b13B8616);
            CDiskFileWin32 mapTo(staticMapPath, FILE_ACCESS_WRITE, 2);
            CopyFileW(mapFrom, mapTo);
            CWideString mapPath2 = PWorld->GetMapPath2(path);
            CDiskFileWin32 mapFrom2(mapPath2, FILE_ACCESS_READ, 2);
            CWideString staticMapPath2 = PWorld->ConvertWorldToStaticMapPath2(path, true, g_b13B8616);
            CDiskFileWin32 mapTo2(staticMapPath2, FILE_ACCESS_WRITE, 2);
            CopyFileW(mapFrom2, mapTo2);
            return;
        }
        PWorld->PostLoad(0);
        LoadingStage("Activate Initial Quests", 0);
        if (InitStructure.InitialQuest != "")
            g_pQuestManager->ActivateQuest(InitStructure.InitialQuest, false, true);
        LoadingStageDone(0);
    }
    Register(&g_Registration);
}