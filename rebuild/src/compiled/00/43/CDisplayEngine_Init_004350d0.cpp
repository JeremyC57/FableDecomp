// CDisplayEngine::Init() -- report progress, enable game drawing from frame 0, run the virtual engine initialisation, reset the refresh clock and create the view manager
// Leaf: ?Init@CDisplayEngine@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004350D0, length 192 bytes (180 + int3 padding), plain `ret` (__fastcall, void).
// Name: FableWin.pdb `?Init@CDisplayEngine@@QAEXXZ`; the progress strings are "Init Display Engine: Init Graphics" (0x012315AC) and
// "Init Display Engine: Init Engine" (0x01231588); single caller 0x004174E9 (the main game component's start-up, result unused --
// no return value is set). The catalog's "InitDisplayEngine_InitGraphics" is a label.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) @ 0x009E9F40 (__fastcall: text in ecx, the first bool in
// dl, the float and the second bool on the stack), the vtable 0x01231574 slot 1 (+0x04) = CDisplayEngine::InitEngine @ 0x00435190
// (FableWin `?InitEngine@CDisplayEngine@@MAE_NXZ`, protected virtual, result ignored), operator new @ 0x00BFEA1A,
// CDisplayViewManager::CDisplayViewManager @ 0x00640040 (8 bytes; the catalog's "CBaseIntelligentPointer" is propagated),
// CCountedPointer<CDisplayViewManager>::Reset @ 0x00436CC0 (landed generic Reset).
// Shape: /O2; each progress text is an implicit const char* -> CCharString temporary (default length -1); DrawGame/Frame are set
// between the two reports (VC schedules the two stores among the second temporary's argument pushes); `LastRefreshTime = 0.0` loads
// the double from the constant pool (`fld qword ; fstp`); the null-tested new expression feeds Reset, whose call + epilogue VC
// tail-duplicates into both arms.
// Layout: engine/CDisplayEngine.h members (Ego_r.pdb = retail here): Frame +0x68, DrawGame +0x6C, CameraInputsOn +0x90,
// LastRefreshTime +0x98, PViewManager +0xE0. That header flattens the vptr into a void* member and the counted pointer into two
// words, so it cannot carry the virtual InitEngine call nor the member Reset; a local class keeps the same offsets. The vptr comes
// from a vptr-only root base: a vptr declared in CDisplayEngine itself would be padded to the double's 8-byte alignment by MSVC,
// moving every member up by 4 (graded DIFFER 180v180 on the offsets alone).
class CCharString {
public:
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool flag1, bool flag2);   // 0x009E9F40
}
class CDisplayViewManager {                              // 8 bytes
public:
    CDisplayViewManager();                               // 0x00640040
    long m_first;
    long m_second;
};
template <class T> class CCountedPointer {
public:
    void Reset(T* data);                                 // 0x00436CC0 for CDisplayViewManager
    T*    Data;                                          // +0x00
    void* Info;                                          // +0x04
};
class CBaseClass {                                       // polymorphic root: vptr only (4-byte aligned)
public:
    virtual ~CBaseClass();                               // slot 0
};
class CDisplayEngine : public CBaseClass {               // vtable 0x01231574
protected:
    virtual bool InitEngine();                           // slot 1 (+0x04) 0x00435190
public:
    void Init();
    unsigned char                        _pad04[0x68 - 4];
    long                                 Frame;          // +0x68
    bool                                 DrawGame;       // +0x6C
    unsigned char                        LetterBox[0x90 - 0x6d];   // +0x6D..+0x8F (CLetterBoxModeInfo at +0x70)
    bool                                 CameraInputsOn; // +0x90
    double                               LastRefreshTime;          // +0x98
    unsigned char                        _padA0[0xe0 - 0xa0];
    CCountedPointer<CDisplayViewManager> PViewManager;   // +0xE0
};

void CDisplayEngine::Init()
{
    NProgressDisplay::DisplayProgress("Init Display Engine: Init Graphics", -1.0f, false, false);
    DrawGame = true;
    Frame = 0;
    NProgressDisplay::DisplayProgress("Init Display Engine: Init Engine", -1.0f, false, false);
    InitEngine();
    LastRefreshTime = 0.0;
    CameraInputsOn = true;
    PViewManager.Reset(new CDisplayViewManager);
}