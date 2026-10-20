#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_dtors, g_saves, g_checksums;
static long g_savedTime, g_savedC4, g_savedCount, g_firstType;
static float g_savedMouse[2], g_savedJoy[2];

struct CAFile { int id; };
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
struct CChecksum { long Checksum1, Checksum2, Checksum3, Checksum4; };
struct CInputEvent { long type; unsigned char _body[0x30]; ~CInputEvent() { ++g_dtors; } };

template <class T> inline void _Destroy(T* first, T* last) { for (; first != last; ++first) first->~T(); }
template <class T>
struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    vector(const vector& other)
    {
        size_t n = other._M_finish - other._M_start;
        _M_start = n ? (T*)std::malloc(n * sizeof(T)) : 0;
        for (size_t i = 0; i < n; ++i) std::memcpy((void*)(_M_start + i), (const void*)(other._M_start + i), sizeof(T));
        _M_finish = _M_end_of_storage = _M_start + n;
    }
    ~vector() { _Destroy(_M_start, _M_finish); if (_M_start) std::free(_M_start); }
};
struct CMouse { float x, y; void GetPosition(C2DVector& p) { p.X = x; p.Y = y; } };
struct CJoystick { float x, y; float GetAxisX() { return x; } float GetAxisY() { return y; } };
struct CCountedJoystick { CJoystick* P; void* Count; CJoystick* operator->() const { return P; } CJoystick* Get() const { return P; } };
struct CJoystickVector { CCountedJoystick* _M_start; CCountedJoystick* _M_finish; CCountedJoystick* _M_end_of_storage;
    CCountedJoystick& operator[](unsigned int n) { return *(_M_start + n); } };

struct CInputEventPackage {
    vector<CInputEvent> Events; long TimeStamp; CChecksum Checksum; C2DVector MousePos; C2DVector JoystickPos;
    CInputEventPackage(const vector<CInputEvent>& events, long timeStamp, const CChecksum& checksum,
                       const C2DVector& mousePos, const C2DVector& joystickPos)
        : Events(events), TimeStamp(timeStamp), Checksum(checksum), MousePos(mousePos), JoystickPos(joystickPos) {}
    void Save(CAFile& file)
    {
        if (file.id != 9) return;
        ++g_saves; g_savedTime = TimeStamp; g_savedC4 = Checksum.Checksum4; g_savedCount = (long)(Events._M_finish - Events._M_start);
        g_firstType = g_savedCount ? Events._M_start->type : -1;
        g_savedMouse[0] = MousePos.X; g_savedMouse[1] = MousePos.Y; g_savedJoy[0] = JoystickPos.X; g_savedJoy[1] = JoystickPos.Y;
    }
};
struct CInputManager {
    vector<CInputEvent> EventStore; long NoStoredEvents; CMouse* PMouse; CJoystickVector PJoysticks;
    CChecksum Checksum; long TimeStamp; CAFile PackageFile;
    long CalculateInternalChecksum() { ++g_checksums; return 1234 + NoStoredEvents; }
    void SaveEventPackage()
    {
        if (NoStoredEvents == 0)
            return;
        C2DVector mousePos(0.0f, 0.0f);
        C2DVector joystickPos(0.0f, 0.0f);
        if (PMouse)
            PMouse->GetPosition(mousePos);
        if (PJoysticks[0].Get()) {
            joystickPos.X = PJoysticks[0]->GetAxisX();
            joystickPos.Y = PJoysticks[0]->GetAxisY();
        }
        Checksum.Checksum4 = CalculateInternalChecksum();
        CInputEventPackage package(EventStore, TimeStamp, Checksum, mousePos, joystickPos);
        package.Save(PackageFile);
    }
};
static int Fail(int n) { std::printf("009f53c0_TEST FAIL %d\n", n); return 1; }
int main()
{
    CInputEvent* store = (CInputEvent*)std::malloc(3 * sizeof(CInputEvent));
    for (int i = 0; i < 3; ++i) store[i].type = 10 + i;
    CJoystick joy = { 0.25f, -0.5f }; CCountedJoystick slots[1] = { { &joy, 0 } };
    CMouse mouse = { 320.0f, 240.0f };
    CInputManager m; m.EventStore._M_start = store; m.EventStore._M_finish = m.EventStore._M_end_of_storage = store + 3;
    m.NoStoredEvents = 3; m.PMouse = &mouse; m.PJoysticks._M_start = slots; m.PJoysticks._M_finish = m.PJoysticks._M_end_of_storage = slots + 1;
    m.Checksum.Checksum1 = 1; m.Checksum.Checksum2 = 2; m.Checksum.Checksum3 = 3; m.Checksum.Checksum4 = 0;
    m.TimeStamp = 77; m.PackageFile.id = 9;
    m.SaveEventPackage();
    if (g_saves != 1 || g_savedTime != 77 || g_savedC4 != 1237 || m.Checksum.Checksum4 != 1237 || g_savedCount != 3 || g_firstType != 10) return Fail(1);
    if (g_savedMouse[0] != 320.0f || g_savedMouse[1] != 240.0f || g_savedJoy[0] != 0.25f || g_savedJoy[1] != -0.5f) return Fail(2);
    if (g_dtors != 3) return Fail(3);                       // the package's copies destroyed
    m.PMouse = 0; slots[0].P = 0;                          // no devices: both positions stay zero
    m.SaveEventPackage();
    if (g_saves != 2 || g_savedMouse[0] != 0.0f || g_savedMouse[1] != 0.0f || g_savedJoy[0] != 0.0f || g_savedJoy[1] != 0.0f || g_dtors != 6) return Fail(4);
    m.NoStoredEvents = 0;                                  // nothing stored: no checksum, no save
    m.SaveEventPackage();
    if (g_saves != 2 || g_checksums != 2) return Fail(5);
    m.EventStore._M_start = m.EventStore._M_finish = 0;
    std::free(store);
    std::printf("009f53c0_TEST PASS\n");
    return 0;
}
