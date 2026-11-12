// CMainGameComponent::PostInit() -- hand the display engine to the world, then either load the save named in the init
//   structure (flagging a failed load) or load the world file (the init structure's world, else the remembered one, else
//   updatedscenic.wld from the levels directory); in static-map export mode copy the world's map files to their static
//   paths and stop, otherwise run the post-load step and activate the initial quest; finally register the component
// Leaf: ?PostInit@CMainGameComponent@@MAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x00416953, 738 bytes, frame pointer, no EH frame (the TU has no unwind tables).
// Layout: CMainGameComponent as in the landed ForceRender 0x004164E0 (retail offsets); InitStructure +0x161d0 holds
//   (retail loads) a world name +0x0 and a second wide string +0x4, the initial quest CCharString +0x8 and the save name +0xc.
// Names: GetMapPath / GetMapPath2 (0x0049DDD0 / 0x0049DEC0), PostLoad 0x0049F180
//   and the globals 0x013B89C0 / 0x013B8648 / 0x013B8616 are descriptive (no PDB name); literals 0x0122EE48
//   "Loading save", 0x0122EE38 "Loading world", 0x0122EE14 L"updatedscenic.wld", 0x0122EDFC "Activate Initial Quests",
//   0x0122D70E "", -1.0f 0x0122DEE0.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013B8668 GForceLevel (global; initializer 0x01219FC0), was g_WorldToLoad.
//   0x013B866C GForceStartingHolySite (global; initializer 0x01219FE0), was g_WorldToLoad2.
//   0x013B8674 GRunLUAScript (global CCharString; initializer 0x0121A020), was g_Registration (a CRegistration). It goes to the protected
//   CMainGameComponent::InitLUA(const CCharString&) 0x004BBC00 (Ego_r.pdb + FableWin.pdb; empty `ret 4` in both builds), was Register.
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
class CAFile { public:                                         // Ego_r.pdb base of CDiskFileWin32 (row-local view)
    virtual ~CAFile();                                         // 0x0099A300
    static void __fastcall CopyFileW(CAFile& from, CAFile& to);   // 0x00998E70 Ego_r.pdb / FableWin.pdb ?CopyFileW@CAFile@@SIXAAV1@0@Z (was a free function on CDiskFileWin32&)
};
class CDiskFileWin32 : public CAFile { public:                // 0x1c bytes
    unsigned char _body[0x18];
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual ~CDiskFileWin32();                                 // 0x00401630 (out-of-line copy)
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
namespace NProfileTimer {
void __fastcall StartProfile(const CCharString& name, long level);      // 0x009D8240
void __fastcall EndProfile(long level);                                 // 0x009D8250
}
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public: void ActivateQuest(const CCharString& name, bool a, bool b); };   // 0x004B4A10
extern bool        g_bSaveLoadFailed;                  // 0x013B89C0
extern CWideString GForceLevel;                      // 0x013B8668
extern CWideString GForceStartingHolySite;                     // 0x013B866C
extern bool        g_bExportStaticMaps;                // 0x013B8648
extern bool        g_b13B8616;                         // 0x013B8616
extern CCharString GRunLUAScript;                      // 0x013B8674
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

protected:
    void InitLUA(const CCharString& script);           // 0x004BBC00
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
            GForceLevel = InitStructure.WorldName;
            if (InitStructure.WorldName2.GetLength() > 0)
                GForceStartingHolySite = InitStructure.WorldName2;
        } else if (GForceLevel.GetLength() > 0) {
            path = GetLevelsDirectory() + GForceLevel;
        } else {
            path = GetLevelsDirectory() + L"updatedscenic.wld";
        }
        PWorld->Load(path);
        if (g_bExportStaticMaps) {
            CWideString mapPath = PWorld->GetMapPath(path);
            CDiskFileWin32 mapFrom(mapPath, FILE_ACCESS_READ, 2);
            CWideString staticMapPath = PWorld->ConvertWorldToStaticMapPath(path, true, g_b13B8616);
            CDiskFileWin32 mapTo(staticMapPath, FILE_ACCESS_WRITE, 2);
            CAFile::CopyFileW(mapFrom, mapTo);
            CWideString mapPath2 = PWorld->GetMapPath2(path);
            CDiskFileWin32 mapFrom2(mapPath2, FILE_ACCESS_READ, 2);
            CWideString staticMapPath2 = PWorld->ConvertWorldToStaticMapPath2(path, true, g_b13B8616);
            CDiskFileWin32 mapTo2(staticMapPath2, FILE_ACCESS_WRITE, 2);
            CAFile::CopyFileW(mapFrom2, mapTo2);
            return;
        }
        PWorld->PostLoad(0);
        NProfileTimer::StartProfile("Activate Initial Quests", 0);
        if (InitStructure.InitialQuest != "")
            CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().ActivateQuest(InitStructure.InitialQuest, false, true);
        NProfileTimer::EndProfile(0);
    }
    InitLUA(GRunLUAScript);
}