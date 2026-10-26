// CProgressDisplay::CProgressDisplay() -- loading-screen progress display: empty texture/tag vectors, empty strings, cleared timers and flags
// Leaf: ??0CProgressDisplay
// Retail: Fable.exe @ 0x00499CE0, 163 bytes; vtable 0x012388B4 (CProgressDisplay, RTTI), callers 0x00413150 / 0x0049E85A.
// The catalog label C3DMeshStats::C3DMeshStats is propagated. The five STLport vectors and the two indices are
// initialised (zeroed from one register), then CWideString (0x0099AED0) and two CCharString (0x0099E4B0) default
// ctors; the body sets the three timers from the shared 0.0 double global 0x0122ED70 (`fld qword`, not fldz, so it
// is a named constant), FullScreen and DisplayText true, the frame/stage counters 0 and the other flags false (the
// two IgnoreNext*RenderCall flags are left alone).
// Shape: the body's statement order fixes where VC7.1 slots the integer stores between each `fld` and its `fstp`
// (two timers, the two true flags, the third timer, the counters, then the false flags).
// Layout: Ego_r.pdb CProgressDisplay with 12-byte retail STLport vectors, as the landed destructor 0x00499FB0
// (no engine header).
class CCharString { public:
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};
class CWideString { public:
    void* m_pStringData;
    CWideString();                                 // 0x0099AED0
    ~CWideString();                                // 0x0099B510
};
class CTexture { public:                                  // 8 bytes
    void* m_pTexture;
    void* m_pHandle;
    ~CTexture();                                   // 0x009F9F70
};
template <class T> struct CVector {                // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
extern const double g_ProgressTimeZero;            // 0x0122ED70 (0.0)
class CAProgressDisplay { public:                         // vtable 0x012387A0
    virtual ~CAProgressDisplay() {}
    virtual void v01();
    virtual void StopProgress(bool a, bool b) = 0; // slot 2
};
struct CProgressDisplay : CAProgressDisplay {      // vtable 0x012388B4
    CVector<CTexture>    ProgressTextures;         // +0x04
    CVector<CTexture>    GuildSealTextures;        // +0x10
    CVector<CTexture>    LoadingScreenTextures;    // +0x1c
    long                 LoadingScreenIndex;       // +0x28
    CVector<CCharString> TextTags;                 // +0x2c
    CVector<CCharString> TextTagsPC;               // +0x38
    long                 TextTagIndex;             // +0x44
    CWideString          Text;                     // +0x48
    CCharString          TextTag;                  // +0x4c
    CCharString          LastTextDisplayed;        // +0x50
    long                 GuildSealFrameIndex;      // +0x54
    long                 StageCount;               // +0x58
    double               LastUpdateTime;           // +0x60
    double               LastScreenUpdate;         // +0x68
    double               LastProgressUpdate;       // +0x70
    bool                 DebugDisplay;             // +0x78
    bool                 Initialised;              // +0x79
    bool                 FullScreen;               // +0x7a
    bool                 BlackScreen;              // +0x7b
    bool                 DisplayText;              // +0x7c
    bool                 DisplayResetProgressTipText;   // +0x7d
    bool                 ForRelaunch;              // +0x7e
    bool                 ForFrontEnd;              // +0x7f
    bool                 ForFileInstaller;         // +0x80
    bool                 FileInstallerFinished;    // +0x81
    bool                 IgnoreNextBeginRenderCall;     // +0x82
    bool                 IgnoreNextEndRenderCall;  // +0x83
    CProgressDisplay();
    virtual ~CProgressDisplay();
    virtual void v01();
    virtual void StopProgress(bool a, bool b);     // 0x00499D90
};
CProgressDisplay::CProgressDisplay()
    : LoadingScreenIndex(0),
      TextTagIndex(0)
{
    LastUpdateTime = g_ProgressTimeZero;
    LastScreenUpdate = g_ProgressTimeZero;
    FullScreen = true;
    DisplayText = true;
    LastProgressUpdate = g_ProgressTimeZero;
    GuildSealFrameIndex = 0;
    StageCount = 0;
    DebugDisplay = false;
    Initialised = false;
    BlackScreen = false;
    DisplayResetProgressTipText = false;
    ForRelaunch = false;
    ForFrontEnd = false;
    ForFileInstaller = false;
    FileInstallerFinished = false;
}
