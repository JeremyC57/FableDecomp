// Behaviour test for 0x00445A60 CGamePlayerInterface::SavePacketEvent.
#include <cstdio>
#include <cstring>
static int g_eventDtors = 0;
class CInputEvent { public: unsigned char body[0x34]; CInputEvent() { std::memset(body, 0, sizeof(body)); } ~CInputEvent() { ++g_eventDtors; } };
class C2DVector { public: float X, Y; };
class CCamera { public: bool HeightLocked, ViewVecZLocked; CCamera() : HeightLocked(false), ViewVecZLocked(false) {} };
struct CSavedInputEvent { unsigned int Frame; CInputEvent InputEvent; C2DVector MousePos; CCamera Camera; };
struct CMouseDevice { void GetPosition(C2DVector* p) { p->X = 3.0f; p->Y = 4.0f; } };
struct CLoader { CMouseDevice* m_pMouse; };
static CLoader g_loader; static CLoader* g_Loader = &g_loader;
class CMainGameComponent { public: unsigned int frame; unsigned int GetFrame() { return frame; } };
class CDiskFileWin32 { public:
    CSavedInputEvent last; unsigned int size; int writes;
    void Write(const void* buffer, unsigned int s, int) { std::memcpy(&last, buffer, sizeof(last)); size = s; ++writes; }
};
class CGamePlayerInterface { public:
    CMainGameComponent* Component; CDiskFileWin32 PacketFile;
    void SavePacketEvent(const CInputEvent& event)
    {
        CSavedInputEvent saved;
        saved.InputEvent = event;
        saved.Frame = Component->GetFrame();
        CMouseDevice* mouse = g_Loader->m_pMouse;
        if (mouse) mouse->GetPosition(&saved.MousePos);
        PacketFile.Write(&saved, sizeof(saved), 0);
    }
};
static int Fail() { std::printf("00445a60_TEST FAIL\n"); return 1; }
int main()
{
    static CGamePlayerInterface gpi; CMainGameComponent comp; comp.frame = 77;
    gpi.Component = &comp; gpi.PacketFile.writes = 0;
    CInputEvent ev; ev.body[0] = 9; ev.body[0x33] = 8;
    CMouseDevice mouse; g_loader.m_pMouse = &mouse;
    int before = g_eventDtors;
    gpi.SavePacketEvent(ev);
    if (gpi.PacketFile.writes != 1 || gpi.PacketFile.size != sizeof(CSavedInputEvent) || gpi.PacketFile.last.Frame != 77 ||
        gpi.PacketFile.last.InputEvent.body[0] != 9 || gpi.PacketFile.last.InputEvent.body[0x33] != 8 ||
        gpi.PacketFile.last.MousePos.X != 3.0f || gpi.PacketFile.last.Camera.HeightLocked || g_eventDtors - before != 1) return Fail();
    std::printf("00445a60_TEST PASS\n");
    return 0;
}