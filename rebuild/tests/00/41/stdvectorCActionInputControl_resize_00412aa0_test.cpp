#include <cstdio>
enum EGameAction {};
enum EControllerType {};
enum EInputKey {};
enum EXboxControllerButton {};
enum EMouseButtonControl {};
class C2DVector {
public:
    float X;                                             // +0x00
    float Y;                                             // +0x04
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CActionInputControl {
public:
    EGameAction           GameAction;                    // +0x00
    EControllerType       ControllerType;                // +0x04
    EInputKey             KeyboardKey;                   // +0x08
    EXboxControllerButton XboxButton;                    // +0x0c
    EMouseButtonControl   MouseButton;                   // +0x10
    C2DVector             ControlDirection;              // +0x14
    CActionInputControl()
        : GameAction((EGameAction)0), ControllerType((EControllerType)0), KeyboardKey((EInputKey)0),
          XboxButton((EXboxControllerButton)0), MouseButton((EMouseButtonControl)0), ControlDirection(0.0f, 0.0f) {}
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    void resize(unsigned int new_size, const T& x);      // 0x004129F0 for CActionInputControl
    void resize(unsigned int new_size) { resize(new_size, T()); }
};
}
template class std::vector<CActionInputControl>;

static int g_calls; static unsigned int g_n; static CActionInputControl g_x;
template <> void std::vector<CActionInputControl>::resize(unsigned int n, const CActionInputControl& x) { ++g_calls; g_n = n; g_x = x; }
int main()
{
    std::vector<CActionInputControl> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    g_x.GameAction = (EGameAction)5; g_x.MouseButton = (EMouseButtonControl)3; g_x.ControlDirection.Y = 2.0f;
    v.resize(7);
    bool ok = g_calls == 1 && g_n == 7 && g_x.GameAction == 0 && g_x.ControllerType == 0 && g_x.KeyboardKey == 0
        && g_x.XboxButton == 0 && g_x.MouseButton == 0 && g_x.ControlDirection.X == 0.0f && g_x.ControlDirection.Y == 0.0f;
    std::printf(ok ? "00412aa0_TEST PASS\n" : "FAIL\n");
    return 0;
}