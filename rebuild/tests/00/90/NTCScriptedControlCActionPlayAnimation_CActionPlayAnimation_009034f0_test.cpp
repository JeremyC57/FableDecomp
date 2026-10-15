// Behaviour test for NTCScriptedControl::CActionPlayAnimation::CActionPlayAnimation (0x009034F0).
#include <cstdio>
#include <cstring>

struct CCharString {
    char text[32];
    CCharString(const char* t) { std::strcpy(text, t); }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); }
};

struct CBase { virtual ~CBase() {} };

struct CActionBase : CBase {
    void* PTCScript; bool Activated; bool Terminated;
    CActionBase() : PTCScript(0), Activated(false), Terminated(false) {}
};

struct CActionPlayAnimation : CActionBase {
    bool PlayedAnim; CCharString Animation; bool StayOnLastFrame; bool Looping; long NumLoops; bool UseMovement;
    bool UsePhysics; long Priority; bool AddAsQueuedAction; bool WaitForAnimationToFinish;
    bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait; bool AllowLooking;
    CActionPlayAnimation(const CCharString& animation, bool stayOnLastFrame, bool looping, long numLoops,
                         bool useMovement, long priority, bool addAsQueuedAction, bool waitForAnimationToFinish,
                         bool usePhysics, bool animMayNeedCameraPositionUpdated, bool allowLooking)
        : CActionBase(), PlayedAnim(false), Animation(animation), StayOnLastFrame(stayOnLastFrame), Looping(looping),
          NumLoops(numLoops), UseMovement(useMovement), UsePhysics(usePhysics), Priority(priority),
          AddAsQueuedAction(addAsQueuedAction), WaitForAnimationToFinish(waitForAnimationToFinish),
          AnimMayNeedCameraPositionUpdatedInSpeakAndWait(animMayNeedCameraPositionUpdated), AllowLooking(allowLooking) {}
};

int main()
{
    CCharString name("WAVE");
    CActionPlayAnimation a(name, true, false, 3, true, 7, false, true, true, false, true);
    if (std::strcmp(a.Animation.text, "WAVE") || !a.StayOnLastFrame || a.Looping || a.NumLoops != 3 || !a.UseMovement
        || a.Priority != 7 || a.AddAsQueuedAction || !a.WaitForAnimationToFinish || !a.UsePhysics
        || a.AnimMayNeedCameraPositionUpdatedInSpeakAndWait || !a.AllowLooking || a.PlayedAnim || a.PTCScript
        || a.Activated || a.Terminated) {
        std::printf("009034f0_TEST FAIL\n");
        return 1;
    }
    std::printf("009034f0_TEST PASS\n");
    return 0;
}