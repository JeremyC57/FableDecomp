#include <cstdio>
#include "engine/CGamePlayerInterface.h"
class CASuspendableProcess;
struct C2DVector { float X; float Y; };
class CAFile {
public:
    virtual ~CAFile();
    virtual void _slot01();
    virtual void _slot02();
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);  // slot 3 (+0x0c)
};
class CMouse {
public:
    virtual ~CMouse();
    virtual void _slot01();
    virtual void _slot02();
    virtual void _slot03();
    virtual void SetPos(const C2DVector* pos);          // slot 4 (+0x10)
};
struct CInputManager {
    unsigned char _pad00[0x24];
    CMouse*       PMouse;                                // +0x24 scoped_ptr<CMouse>
    CMouse* GetMouse() const { return PMouse; }
};
extern CInputManager* g_pInputManager;                  // 0x013B8388
struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    CAFile& GetPacketFile() { return *reinterpret_cast<CAFile*>(PacketFile); }
    const C2DVector* GetLoadedMousePos() const { return reinterpret_cast<const C2DVector*>(LoadedInputEvent + 0x38); }
    void LoadInitialInfo();
};
void CGamePlayerInterface_Methods::LoadInitialInfo()
{
    GetPacketFile().Read(LoadedInputEvent, sizeof(LoadedInputEvent), 0);
    if (g_pInputManager->GetMouse())
        g_pInputManager->GetMouse()->SetPos(GetLoadedMousePos());
}

static int g_read, g_setpos; static void* g_buf; static long g_size; static const C2DVector* g_pos;
CAFile::~CAFile() {}
void CAFile::_slot01() {}
void CAFile::_slot02() {}
void CAFile::Read(void* b, long n, CASuspendableProcess* p) { ++g_read; g_buf = b; g_size = n; if (p) g_read = 100; }
CMouse::~CMouse() {}
void CMouse::_slot01() {}
void CMouse::_slot02() {}
void CMouse::_slot03() {}
void CMouse::SetPos(const C2DVector* p) { ++g_setpos; g_pos = p; }
CInputManager* g_pInputManager = 0;
#include <new>
int main()
{
    static double mem[sizeof(CGamePlayerInterface) / 8 + 1];
    CGamePlayerInterface_Methods* gpi = (CGamePlayerInterface_Methods*)mem;
    new (gpi->PacketFile) CAFile;
    CMouse mouse; CInputManager im; im.PMouse = &mouse; g_pInputManager = &im;
    gpi->LoadInitialInfo();
    bool ok = g_read == 1 && g_buf == gpi->LoadedInputEvent && g_size == 0x78 && g_setpos == 1
        && (const unsigned char*)g_pos == gpi->LoadedInputEvent + 0x38 && (const unsigned char*)g_pos == (const unsigned char*)gpi + 0x75c;
    im.PMouse = 0;
    gpi->LoadInitialInfo();
    ok = ok && g_read == 2 && g_setpos == 1;
    std::printf(ok ? "00445b40_TEST PASS\n" : "FAIL\n");
    return 0;
}