#include <cstdio>
#include <stddef.h>
class CThing { public: int x; };
struct CTCBase_Calls { virtual void v00() { } CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; CTCBase_Calls(CThing* p) : ParentThing(p), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { } };
class C3DVector { public: float X, Y, Z; };
class CTCSleep : public CTCBase_Calls { public:
    bool IsReadyToSleep; bool FastTrack; long LastFrameWokeUp; C3DVector PosAtFastTrackSetting; virtual void v00() { }
    CTCSleep(CThing* p) : CTCBase_Calls(p), IsReadyToSleep(false), FastTrack(false), LastFrameWokeUp(-1000000) { }
};
int main()
{
    CThing thing; CTCSleep s(&thing);
    if (s.ParentThing != &thing || s.IsReadyToSleep || s.FastTrack || s.LastFrameWokeUp != -1000000 || (long)s.LastFrameWokeUp != (long)0xfff0bdc0
        || offsetof(CTCSleep, IsReadyToSleep) != 0xc || offsetof(CTCSleep, LastFrameWokeUp) != 0x10 || sizeof(CTCSleep) != 0x20) { std::printf("004d60e5_TEST FAIL\n"); return 1; }
    std::printf("004d60e5_TEST PASS\n"); return 0;
}