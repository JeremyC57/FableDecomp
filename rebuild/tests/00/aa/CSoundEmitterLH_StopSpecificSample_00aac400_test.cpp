#include <cstdio>
// Host model of CSoundEmitterLH (vtable 0x0129E6EC) and the libpk event manager it drives; the method under test is the row's body.
class C3DVector { public: float X, Y, Z; C3DVector() : X(0), Y(0), Z(0) {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
class CASoundBank { public: int Id; };
class CSoundEventManager {
public:
    int Stops, Releases, Volumes, Pitches, Positions, Values, States; long Last; float LastFloat; long State; C3DVector LastPos;
    CSoundEventManager() : Stops(0), Releases(0), Volumes(0), Pitches(0), Positions(0), Values(0), States(0), Last(0), LastFloat(0), State(0) {}
    virtual ~CSoundEventManager() {}
    virtual void StopEvent(long id) { ++Stops; Last = id; }
    virtual void ReleaseEventLoop(long id) { ++Releases; Last = id; }
    virtual void SetEventVolume(long id, float v) { ++Volumes; Last = id; LastFloat = v; }
    virtual void SetEventPitch(long id, float p) { ++Pitches; Last = id; LastFloat = p; }
    virtual void SetEventPosition(long id, const C3DVector& p) { ++Positions; Last = id; LastPos = p; }
    virtual float GetEventValue(long id, float* v) { ++Values; Last = id; *v = 0.5f; return 0.5f; }
    virtual long GetEventState(long id) { ++States; Last = id; return State; }
};
class CSoundSystem { public: CSoundEventManager Events; virtual ~CSoundSystem() {} virtual CSoundEventManager* GetEventManager() { return &Events; } };
class CSoundManagerLH { public: CSoundSystem* m_pSoundSystem; };
class CActiveSound { public: long State; long SampleID; CASoundBank* PBank; float StartTime; bool Started; bool Stopping; long Sample; };
struct SNode { SNode* Next; CActiveSound* Data; };
class CSoundEmitterLH {
public:
    CSoundManagerLH* PManager; C3DVector Position; SNode* ActiveSounds; bool Positional; bool Playing; bool Active; long EmitterID;
    int StopAllCalls, SetPosCalls, Prunes; C3DVector SetPosArg;
    CSoundEmitterLH() : PManager(0), ActiveSounds(0), Positional(false), Playing(false), Active(false), EmitterID(0), StopAllCalls(0), SetPosCalls(0), Prunes(0) {}
    virtual ~CSoundEmitterLH() {}
    virtual void StopAllSamples() { ++StopAllCalls; }
    virtual void SetPos(const C3DVector& p) { ++SetPosCalls; SetPosArg = p; }
    virtual const C3DVector GetPos() const { return Position; }
    void RemoveInactiveSounds() { ++Prunes; }
    void StopSpecificSample(unsigned long id) { if (PManager->m_pSoundSystem) { for (SNode* it = ActiveSounds->Next; it != ActiveSounds; it = it->Next) { CActiveSound* sound = it->Data; if (sound->SampleID == (long)id) { PManager->m_pSoundSystem->GetEventManager()->StopEvent(id); sound->SampleID = -1; } } RemoveInactiveSounds(); } }
};
static CSoundSystem gSystem; static CSoundManagerLH gManager; static CASoundBank gBankA, gBankB;
static CActiveSound gS1, gS2, gS3; static SNode gHead, gN1, gN2, gN3;
static void Setup(CSoundEmitterLH& e, bool systemUp)
{
    gManager.m_pSoundSystem = systemUp ? &gSystem : 0; gSystem.Events = CSoundEventManager();
    CActiveSound a = { 0, 11, &gBankA, 0.0f, true, false, 3 }; CActiveSound b = { 1, 22, &gBankB, 0.0f, true, true, 3 };
    CActiveSound c = { 0, 11, &gBankA, 0.0f, false, false, 4 };
    gS1 = a; gS2 = b; gS3 = c;
    gHead.Next = &gN1; gN1.Next = &gN2; gN2.Next = &gN3; gN3.Next = &gHead; gN1.Data = &gS1; gN2.Data = &gS2; gN3.Data = &gS3;
    e.PManager = &gManager; e.ActiveSounds = &gHead;
}
int main()
{
    bool ok = true;
    { CSoundEmitterLH e; Setup(e, false); e.StopSpecificSample(11); ok = e.Prunes == 0 && gS1.SampleID == 11;
      Setup(e, true); e.StopSpecificSample(11); ok = ok && e.Prunes == 1 && gSystem.Events.Stops == 2 && gSystem.Events.Last == 11 && gS1.SampleID == -1 && gS2.SampleID == 22 && gS3.SampleID == -1; }
    if (!ok) { std::printf("00aac400_TEST FAIL\n"); return 1; }
    std::printf("00aac400_TEST PASS\n");
    return 0;
}
