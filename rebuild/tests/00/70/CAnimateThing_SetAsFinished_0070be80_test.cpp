// Behaviour test for CAnimateThing::SetAsFinished (0x0070BE80): sets only the Finished bit of the main animation set.
#include <cstdio>
#include <cstring>
class CAnimSet { public:
    int            UpdateMode;
    unsigned char  Anims[0xC];
    unsigned char  InternalAlphas[0xC];
    float          f[7];
    unsigned int   FrameInternalAlphasUpdated;
    void*          PAnimator;
    unsigned int   Flags, TypeFlags;
    int            AnimDelay, Priority;
    unsigned char  Type;
    bool           Looping : 1;
    bool           Finished : 1;
    bool           Forwards : 1;
    bool           HadUpdate : 1;
    bool           OwnedByMainAnim : 1;
};
class CCompleteAnimation { public: CAnimSet MainAnimSet; };
class CAnimateThing { public:
    CCompleteAnimation CurrentAnim;
    void SetAsFinished() { CurrentAnim.MainAnimSet.Finished = true; }
};
int main()
{
    CAnimateThing t;
    std::memset(&t, 0, sizeof(t));
    t.CurrentAnim.MainAnimSet.Looping = true;
    t.CurrentAnim.MainAnimSet.Type = 7;
    t.SetAsFinished();
    CAnimSet& s = t.CurrentAnim.MainAnimSet;
    if (!s.Finished || !s.Looping || s.Forwards || s.HadUpdate || s.OwnedByMainAnim || s.Type != 7) {
        std::printf("0070be80_TEST FAIL\n");
        return 1;
    }
    t.SetAsFinished();
    if (!s.Finished) { std::printf("0070be80_TEST FAIL\n"); return 1; }
    std::printf("0070be80_TEST PASS\n");
    return 0;
}