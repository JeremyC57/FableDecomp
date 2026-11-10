// Behaviour test for 0x00AABD40 CSoundEmitterLH::Update(): the whole body is the prune of the stopped active sounds
// (RemoveInactiveSounds); a model emitter counts the prunes and drops the sounds whose event handle is -1.
#include <cstdio>
class CSoundEmitterLH {
public:
    long Handles[4];
    long Count;
    int Prunes;
    void RemoveInactiveSounds()
    {
        ++Prunes;
        long kept = 0;
        for (long i = 0; i < Count; ++i)
            if (Handles[i] != -1) Handles[kept++] = Handles[i];
        Count = kept;
    }
    virtual void Update() { RemoveInactiveSounds(); }
    virtual ~CSoundEmitterLH() { }
};
int main()
{
    CSoundEmitterLH e;
    e.Handles[0] = 5; e.Handles[1] = -1; e.Handles[2] = 9; e.Count = 3; e.Prunes = 0;
    e.Update();
    if (e.Prunes != 1 || e.Count != 2 || e.Handles[0] != 5 || e.Handles[1] != 9) { std::printf("00aabd40_TEST FAIL\n"); return 1; }
    e.Update();
    if (e.Prunes != 2 || e.Count != 2) { std::printf("00aabd40_TEST FAIL\n"); return 1; }
    std::printf("00aabd40_TEST PASS\n");
    return 0;
}
