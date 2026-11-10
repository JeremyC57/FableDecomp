#include <cstdio>
// Host model of CSoundEmitterLH::PlayAtmos 0x00AAC1D0: look the atmos up by name on the manager, play it at the emitter position,
// and on a non-negative handle track a new active sound and mark the emitter playing.
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
class CAtmos { public: long Result; long LastEmitter; float LastVolume; C3DVector LastPos; int Plays;
    CAtmos() : Result(0), LastEmitter(0), LastVolume(0), LastPos(0, 0, 0), Plays(0) {}
    long Play(long emitterID, float volume, const C3DVector* p, long, long) { ++Plays; LastEmitter = emitterID; LastVolume = volume; LastPos = *p; return Result; } };
class CSoundManagerLH { public: CAtmos* Found; CAtmos* FindByName(const char* name) { return name[0] == 'r' ? Found : 0; } };
class CSoundEmitterLH {
public:
    CSoundManagerLH* PManager; C3DVector Position; int Tracked; long LastTracked; bool Playing; long EmitterID;
    CSoundEmitterLH() : PManager(0), Position(1, 2, 3), Tracked(0), LastTracked(0), Playing(false), EmitterID(9) {}
    const C3DVector GetPos() const { return Position; }
    unsigned long PlayAtmos(const char* name)
    {
        CAtmos* atmos = PManager->FindByName(name);
        if (atmos) {
            C3DVector position(0.0f, 0.0f, 0.0f);
            position.X = GetPos().X; position.Y = GetPos().Y; position.Z = GetPos().Z;
            long id = atmos->Play(EmitterID, 1.0f, &position, 0, 0);
            if (id >= 0) { ++Tracked; LastTracked = id; Playing = true; return id; }
        }
        return (unsigned long)-1;
    }
};
int main()
{
    CAtmos atmos; CSoundManagerLH m; m.Found = &atmos; CSoundEmitterLH e; e.PManager = &m;
    bool ok = e.PlayAtmos("none") == (unsigned long)-1 && atmos.Plays == 0;
    atmos.Result = -2; ok = ok && e.PlayAtmos("rain") == (unsigned long)-1 && atmos.Plays == 1 && e.Tracked == 0 && !e.Playing;
    atmos.Result = 5; ok = ok && e.PlayAtmos("rain") == 5 && e.Tracked == 1 && e.LastTracked == 5 && e.Playing && atmos.LastEmitter == 9 && atmos.LastPos.Y == 2.0f;
    if (!ok) { std::printf("00aac1d0_TEST FAIL\n"); return 1; }
    std::printf("00aac1d0_TEST PASS\n");
    return 0;
}
