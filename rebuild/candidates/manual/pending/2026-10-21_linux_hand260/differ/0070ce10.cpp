// Grade: DIFFER(475v495) -- empty allocator temp gets its own frame byte (retail reuses the distance arg slot); speeds start cached in ebp but retail stores then reloads
// CAnimSet::UpdateDistanceMoved(float) -- advance the anim set's completion from the distance moved
// Layout: no engine/CAnimSet.h; retail STLport vector is 12 bytes so InternalAlphas sits at +0x10 and later members 8 below the PDB.
#include <stdlib.h>
#include <math.h>

struct C3DVector { float X, Y, Z; float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); } };

struct C3DAnimationInfo { char _pad[0x18]; C3DVector Movement; };
struct CMeshDataBank { const C3DAnimationInfo* PeekAnimation(unsigned long) const; };   // 0xA242C0
struct CWorld { char _pad[0x3c]; CMeshDataBank* PMainMeshBank; };
struct CMainGameComponent { char _pad[0x24]; CWorld* PWorld; };
extern CMainGameComponent* g_PMainGameComponent;   // 0x13B86A0

struct CFloatAllocator { CFloatAllocator() {} };
struct CFloatVector {
    float* _M_start; float* _M_finish; float* _M_end_of_storage;
    CFloatVector(unsigned int n, const float& value, const CFloatAllocator& a);   // 0x4516E7
    __forceinline ~CFloatVector() { if (_M_start) free(_M_start); }
    float& operator[](unsigned int i) { return _M_start[i]; }
};
struct CShortVector {
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    short& operator[](unsigned int i) { return _M_start[i]; }
};
struct CTransitionBlendAlpha { float Alpha; long Frame; };
struct CAlphaVector { CTransitionBlendAlpha* _M_start; CTransitionBlendAlpha* _M_finish; CTransitionBlendAlpha* _M_end_of_storage; };
struct CAnimateThing { char _pad[0x88]; float DistancePerCycle; };

class CAnimSet
{
public:
    long UpdateMode;
    CShortVector Anims;
    CAlphaVector InternalAlphas;
    float CurrentPercentageComplete;
    float LastPercentageComplete;
    float Speed;
    unsigned long FrameInternalAlphasUpdated;
    float CurrentAlpha;
    float LastAlpha;
    float LastUpdateChange;
    float ChangeAlphaBy;
    CAnimateThing* PAnimator;
    unsigned long Flags;
    unsigned long TypeFlags;
    long AnimDelay;
    long Priority;
    unsigned char Type;
    bool Looping : 1;
    bool Finished : 1;
    bool Forwards : 1;
    bool HadUpdate : 1;
    bool OwnedByMainAnim : 1;
    void UpdateDistanceMoved(float distance);
};

void CAnimSet::UpdateDistanceMoved(float distance)
{
    CFloatVector speeds(Anims.size(), 0.0f, CFloatAllocator());
    float* speedData = speeds._M_start;
    bool noMovement = true;
    for (unsigned int i = 0; i < Anims.size(); i++) {
        short anim = Anims[i];
        if (anim != 0) {
            const C3DAnimationInfo* info = g_PMainGameComponent->PWorld->PMainMeshBank->PeekAnimation(anim);
            speedData[i] = info->Movement.GetLength() * 0.01f;
            if (speedData[i] > 0.0f)
                noMovement = false;
        }
    }
    float change;
    if (noMovement) {
        change = 0.1f;
    } else {
        float total = 0.0f;
        for (unsigned int i = 0; i < Anims.size(); i++) {
            if (Anims[i] != 0)
                total += InternalAlphas._M_start[i].Alpha * speedData[i];
        }
        if (total > 0.0001f)
            change = distance / (total * PAnimator->DistancePerCycle);
        else
            change = 0.0f;
    }
    LastUpdateChange = change;
    LastPercentageComplete = CurrentPercentageComplete;
    float current = change + CurrentPercentageComplete;
    CurrentPercentageComplete = current;
    if (Looping) {
        if (LastPercentageComplete > 1.0f) {
            LastPercentageComplete -= 1.0f;
            CurrentPercentageComplete = current - 1.0f;
        } else if (LastPercentageComplete < 0.0f) {
            LastPercentageComplete += 1.0f;
            CurrentPercentageComplete = current + 1.0f;
        }
    } else {
        if (CurrentPercentageComplete < 0.0f)
            CurrentPercentageComplete = 0.0f;
        else if (CurrentPercentageComplete > 1.0f)
            CurrentPercentageComplete = 1.0f;
        if (CurrentPercentageComplete >= 1.0f - 0.0001f)
            Finished = true;
    }
}
