// Behaviour test for the CSavedInputEvent constructor @ 0x00447BB0: the input event is constructed, the camera lock flags cleared.
#include <cstdio>
#include <new>

static int g_eventCtors = 0;
class CInputEvent { public:
    long Data[8]; long DeviceType, DeviceNumber, Type; float CurrentTime, StartTime;
    CInputEvent() : DeviceNumber(0), Type(0) { ++g_eventCtors; for (int i = 0; i < 8; ++i) Data[i] = 0; }
};
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
class CCamera { public:
    C3DVector WorldPosition; float RHSet[6]; bool HeightLocked; bool ViewVecZLocked;
    unsigned long FOVFlags; float HorizontalFOV, VerticalFOV, Zoom;
    CCamera() : HeightLocked(false), ViewVecZLocked(false) {}
};
struct CSavedInputEvent {
    unsigned long Frame; CInputEvent InputEvent; C2DVector MousePos; CCamera Camera;
    CSavedInputEvent() {}
};

int main()
{
    long storage[sizeof(CSavedInputEvent) / sizeof(long) + 1];
    unsigned char* bytes = reinterpret_cast<unsigned char*>(storage);
    for (unsigned i = 0; i < sizeof(storage); ++i) bytes[i] = 0xAB;      // garbage: the flags must be cleared
    CSavedInputEvent* e = new (static_cast<void*>(storage)) CSavedInputEvent;
    if (g_eventCtors != 1 || e->Camera.HeightLocked || e->Camera.ViewVecZLocked || e->InputEvent.Type != 0) { std::printf("00447bb0_TEST FAIL\n"); return 1; }
    std::printf("00447bb0_TEST PASS\n");
    return 0;
}