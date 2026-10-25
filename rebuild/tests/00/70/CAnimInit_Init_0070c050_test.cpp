#include <cstdio>
#include <cstring>
#include <cstddef>
// Behaviour test for 0x0070C050 CAnimInit::Init: links against the source object.
enum EAnimUpdateMode { };
class CAnimFrameInfo { public: float Current; float Last; };
class CAnimInit {
public:
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping,
              unsigned long flags, long priority);
    EAnimUpdateMode UpdateMode; unsigned char Type; bool Looping; long Anim; float Speed; float Alpha; float LastAlpha;
    CAnimFrameInfo Frame; unsigned long Flags; unsigned long TypeFlags; long Priority;
};
int main()
{
    if (sizeof(CAnimInit) != 0x2c || offsetof(CAnimInit, Frame) != 0x18 || offsetof(CAnimInit, Priority) != 0x28) { std::printf("0070c050_TEST FAIL layout\n"); return 1; }
    CAnimInit r; std::memset(&r, 0xff, sizeof r);
    r.Init(9, 0x22u, 3, (EAnimUpdateMode)4, false, 0x66u, 7);
    if (r.UpdateMode != 4 || r.Type != 9 || r.Looping || r.Anim != 3 || r.Speed != 1.0f || r.Alpha != 1.0f || r.LastAlpha != 1.0f
        || r.Frame.Current != 0.0f || r.Frame.Last != 0.0f || r.Flags != 0x66u || r.TypeFlags != 0x22u || r.Priority != 7)
        { std::printf("0070c050_TEST FAIL\n"); return 1; }
    r.Speed = 2.0f; r.Frame.Current = 5.0f;
    r.Init(1, 0, -1, (EAnimUpdateMode)0, true, 0, 0);
    if (!r.Looping || r.Type != 1 || r.Anim != -1 || r.Speed != 1.0f || r.Frame.Current != 0.0f || r.UpdateMode != 0)
        { std::printf("0070c050_TEST FAIL reinit\n"); return 1; }
    std::printf("0070c050_TEST PASS\n");
    return 0;
}
