// CNewFrontendGameComponent::CNewFrontendGameComponent(CGame* game, CNewFrontendGameComponentInit* init) -- construct the front-end game component: game-component base, empty counted pointers / vectors / strings, cursor timing from the clock, inputs allowed, three stop watches, a 1/3 s repeat delay and the media-player flag; the main-game init strings are cleared in the body
#pragma optimize("s", on)
// Leaf: ??0CNewFrontendGameComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042EA8F, 295 bytes, `ret 8` (the init argument is unused here).
// Pragma: size TU (`fldz ; fstp` zero floats), as the landed destructor 0x0042F50B.
// Callees: CGameComponent::CGameComponent(CGame*) 0x0051D8E0 (out of line), GFGetTime 0x009E1BC0 (`__fastcall`, double),
//   CMainGameComponentInit() 0x004131A0, CWideString(const wchar_t*) 0x0099B6B0 and operator=(const wchar_t*) 0x0099B800
//   (L"" 0x0122D70C), CStopWatch() 0x0062F740.
// Layout: as the landed destructor 0x0042F50B (Ego_r.pdb with 12-byte STLport vectors): vtables 0x01230CA0 / 0x01230C94,
//   m_pMenuFont +0x10, CurrentMovement +0x18, StartedRepeatingMovement +0x1c, TimeLastMovementHappened +0x20,
//   m_bAllowInputs +0x28, m_bCreateGame +0x29, CreateDevFrontEnd +0x2a, m_bInitMainGame +0x2c (two CWideStrings at +0 and
//   +0xc), m_pFrontEndDef +0x3c, banks +0x40..+0x64, m_pInputManager +0x68, FileNames +0x6c, FileTimes +0x78, SaveGame +0x84,
//   PAVITextureShow +0x88, PAVITextureDecode +0x8c, AVISrcBox +0x90, AVIDstBox +0xa0, AVIPlaying +0xb0, XMVCodeLoaded +0xb1,
//   m_pFrontEndManager +0xb4, m_fLastUpdateTime +0xb8, m_fLastRenderTime +0xbc, m_fLastRealUpdateTime +0xc0,
//   CreatedMarkerFile +0xc8, m_LastInterp +0xcc, m_RepeatTimer +0xd8, m_fRepeatDelay +0xf8, m_dwDPadTriggerState +0xfc,
//   m_attractModeTimer +0x100, m_inputDelayTimer +0x120, LastActiveJoystickDeviceNumber +0x140, WindowsMediaPlayerInstalled +0x144.
// Shape: the STLport FileTimes vector through its real base (allocator proxy) gives the `lea eax` member address; the
//   interpolation set's inline ctor zeroes three floats with `fldz`.
class CGame;
class CNewFrontendGameComponentInit;
double __fastcall GFGetTime();                             // 0x009E1BC0
struct CBase {
    virtual ~CBase();                                      // 0x0099A300
};
struct CGameComponentInterface {
    virtual void v00();
};
class CGameComponent : public CBase, public CGameComponentInterface { public:   // vtables 0x0122ECB0 / 0x0122ECA4
    bool  m_bShutDown;                                     // +0x08
    CGame* m_pGame;                                        // +0x0c
    CGameComponent(CGame* game);                           // 0x0051D8E0
};
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CFontBank; class CGraphicDataBank; class CMeshDataBank; class CASoundBank; class CIEngine; class CInputManager;
struct CFrontEndManager; class CTexture;
namespace NGameText { class CDataBank; }
class CFrontEndDef;
template <class T> class CDefPointer { public:
    T* Data;
    CDefPointer() : Data(0) { }
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);               // 0x0099B6B0
    const CWideString& operator=(const unsigned short* text);    // 0x0099B800
};
class CMainGameComponentInit { public:
    CWideString   StartLevel;                              // +0x00
    unsigned long _data04[2];
    CWideString   StartPosition;                           // +0x0c
    CMainGameComponentInit();                              // 0x004131A0
};
struct CWideStringVector {                                 // STLport vector<CWideString>
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    CWideStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
template <class T> struct CAllocator {                     // STLport allocator<T> (empty)
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                    // STLport _Vector_base
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CRetailVector : CVectorBase<T> {
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
};
class CDateAndTime { public: unsigned char _data[0x10]; };
class C2DBoxI { public: long X0, Y0, X1, Y1; };
struct CEngineInterpolationInfoSet {
    float A, B, C;
    CEngineInterpolationInfoSet() : A(0.0f), B(0.0f), C(0.0f) { }
};
struct CStopWatch {
    unsigned char _data[0x20];
    CStopWatch();                                          // 0x0062F740
};
enum ECursorMovement { CURSOR_MOVEMENT_NONE = 0 };

class CNewFrontendGameComponent : public CGameComponent { public:        // vtables 0x01230CA0 / 0x01230C94
    CCountedPointer<const CFontBank>       m_pMenuFont;                // +0x10
    ECursorMovement                        CurrentMovement;            // +0x18
    bool                                   StartedRepeatingMovement;   // +0x1c
    double                                 TimeLastMovementHappened;   // +0x20
    bool                                   m_bAllowInputs;             // +0x28
    bool                                   m_bCreateGame;              // +0x29
    bool                                   CreateDevFrontEnd;          // +0x2a
    CMainGameComponentInit                 m_bInitMainGame;            // +0x2c
    CDefPointer<const CFrontEndDef>        m_pFrontEndDef;             // +0x3c
    CCountedPointer<CGraphicDataBank>      m_pFrontEndGraphicBank;     // +0x40
    CCountedPointer<CMeshDataBank>         m_pMeshBank;                // +0x48
    CCountedPointer<CASoundBank>           PSampleBank;                // +0x50
    CCountedPointer<CIEngine>              m_pEngine;                  // +0x58
    CCountedPointer<NGameText::CDataBank>  m_pTextBank;                // +0x60
    CInputManager*                         m_pInputManager;            // +0x68
    CRetailVector<CWideString>             FileNames;                  // +0x6c
    CRetailVector<CDateAndTime>            FileTimes;                  // +0x78
    CWideString                            SaveGame;                   // +0x84
    CTexture*                              PAVITextureShow;            // +0x88
    CTexture*                              PAVITextureDecode;          // +0x8c
    C2DBoxI                                AVISrcBox;                  // +0x90
    C2DBoxI                                AVIDstBox;                  // +0xa0
    bool                                   AVIPlaying;                 // +0xb0
    bool                                   XMVCodeLoaded;              // +0xb1
    CFrontEndManager*                      m_pFrontEndManager;         // +0xb4
    float                                  m_fLastUpdateTime;          // +0xb8
    float                                  m_fLastRenderTime;          // +0xbc
    double                                 m_fLastRealUpdateTime;      // +0xc0
    bool                                   CreatedMarkerFile;          // +0xc8
    CEngineInterpolationInfoSet            m_LastInterp;               // +0xcc
    CStopWatch                             m_RepeatTimer;              // +0xd8
    float                                  m_fRepeatDelay;             // +0xf8
    unsigned long                          m_dwDPadTriggerState;       // +0xfc
    CStopWatch                             m_attractModeTimer;         // +0x100
    CStopWatch                             m_inputDelayTimer;          // +0x120
    long                                   LastActiveJoystickDeviceNumber;   // +0x140
    bool                                   WindowsMediaPlayerInstalled;      // +0x144
    CNewFrontendGameComponent(CGame* game, CNewFrontendGameComponentInit* init);
};

static const unsigned short g_emptyWide[1] = { 0 };       // L"" 0x0122D70C

CNewFrontendGameComponent::CNewFrontendGameComponent(CGame* game, CNewFrontendGameComponentInit* init)
    : CGameComponent(game), CurrentMovement(CURSOR_MOVEMENT_NONE), StartedRepeatingMovement(false),
      TimeLastMovementHappened(GFGetTime()), m_bAllowInputs(true), m_bCreateGame(false),
      SaveGame(g_emptyWide), PAVITextureShow(0), PAVITextureDecode(0), AVIPlaying(false), XMVCodeLoaded(false),
      m_pFrontEndManager(0), m_fLastRealUpdateTime(GFGetTime()), m_fRepeatDelay(0.333f), m_dwDPadTriggerState(0),
      LastActiveJoystickDeviceNumber(0), WindowsMediaPlayerInstalled(true)
{
    m_bInitMainGame.StartLevel = g_emptyWide;
    m_bInitMainGame.StartPosition = g_emptyWide;
    CreatedMarkerFile = false;
    CreateDevFrontEnd = false;
}