#include <cstdio>
#include "engine/CGamePlayerInterface.h"
class CWideString;
class CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CAFile {
public:
    virtual ~CAFile();
    virtual void Open(const CWideString& name, FileAccessType access, unsigned long flags) = 0;
    virtual void Close() = 0;                                                    // slot 2
    virtual void Read(void* dst, long size, CASuspendableProcess* process) = 0;  // slot 3
    virtual void Write(const void* src, long size, CASuspendableProcess* process) = 0;   // slot 4 (+0x10)
};
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
class CCamera {
public:
    C3DVector     WorldPosition;                         // +0x00
    unsigned char RHSet[0x18];                           // +0x0c CRightHandedSet
    bool          HeightLocked;                          // +0x24
    bool          ViewVecZLocked;                        // +0x25
    unsigned long FOVFlags;                              // +0x28
    float         HorizontalFOV;                         // +0x2c
    float         VerticalFOV;                           // +0x30
    float         Zoom;                                  // +0x34
    CCamera() : HeightLocked(false), ViewVecZLocked(false) {}
};
class CInputEvent {
public:
    unsigned char Body[0x34];
    CInputEvent();                                       // 0x00A04410
    ~CInputEvent();                                      // 0x00A03B30
};
class CSavedInputEvent {
public:
    unsigned long Frame;                                 // +0x00
    CInputEvent   InputEvent;                            // +0x04
    C2DVector     MousePos;                              // +0x38
    CCamera       Camera;                                // +0x40
};
class CMouse {
public:
    virtual ~CMouse();
    virtual void GetMousePosition(C2DVector& position);  // slot 1 (+4); name not confirmed
};
class CInputManager {
public:
    unsigned char _pad00[0x24];
    CMouse*       PMouse;                                // +0x24 scoped_ptr<CMouse>
};
extern CInputManager* g_pInputManager;                   // 0x013B8388
struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    CAFile& GetPacketFile() { return *reinterpret_cast<CAFile*>(PacketFile); }
    void SaveInitialInfo();
};
void CGamePlayerInterface_Methods::SaveInitialInfo()
{
    CSavedInputEvent saved;
    CMouse* mouse = g_pInputManager->PMouse;
    if (mouse)
        mouse->GetMousePosition(saved.MousePos);
    GetPacketFile().Write(&saved, sizeof(saved), 0);
}

#include <new>
#include <string.h>
static int g_ctor, g_dtor, g_writes, g_size, g_mouse_calls;
static unsigned char g_written[0x78];
CInputEvent::CInputEvent() { ++g_ctor; memset(Body, 0x11, sizeof(Body)); }
CInputEvent::~CInputEvent() { ++g_dtor; }
CAFile::~CAFile() {}
class CTestFile : public CAFile {
public:
    void Open(const CWideString&, FileAccessType, unsigned long) {}
    void Close() {}
    void Read(void*, long, CASuspendableProcess*) {}
    void Write(const void* src, long size, CASuspendableProcess*) { ++g_writes; g_size = size; memcpy(g_written, src, 0x78); }
};
CMouse::~CMouse() {}
void CMouse::GetMousePosition(C2DVector& p) { ++g_mouse_calls; p.X = 3.0f; p.Y = 4.0f; }
static CInputManager g_im;
CInputManager* g_pInputManager = &g_im;
int main()
{
    static unsigned char raw[sizeof(CGamePlayerInterface)];
    CGamePlayerInterface_Methods* gpi = reinterpret_cast<CGamePlayerInterface_Methods*>(raw);
    new (gpi->PacketFile) CTestFile;
    g_im.PMouse = 0;
    gpi->SaveInitialInfo();
    bool ok = g_ctor == 1 && g_dtor == 1 && g_writes == 1 && g_size == 0x78 && g_mouse_calls == 0 && g_written[0x64] == 0 && g_written[0x65] == 0 && g_written[4] == 0x11;
    CMouse m; g_im.PMouse = &m;
    gpi->SaveInitialInfo();
    C2DVector pos; memcpy(&pos, g_written + 0x38, 8);
    ok = ok && g_ctor == 2 && g_dtor == 2 && g_writes == 2 && g_mouse_calls == 1 && pos.X == 3.0f && pos.Y == 4.0f;
    std::printf(ok ? "00445ae0_TEST PASS\n" : "00445ae0_TEST FAIL\n");
    return ok ? 0 : 1;
}