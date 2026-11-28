#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEDRAGONFLYING_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEDRAGONFLYING_H

#include "engine/NCreatureMode_CModeDragonFlyingBase.h"
#include "engine/CAnimFilterOfGroup.h"

class CCreatureActionBase;
namespace NCreatureMode {
// Ego_r layouts and FableWin TPI 0x2B565, with retail-sized CModeDragonFlyingBase.
class CModeDragonFlying : public CModeDragonFlyingBase {
public:
    static unsigned long FABLE_FASTCALL StatGetType();
    virtual unsigned long GetType() const;
    static CTCCreatureModeManager::CModeBase* FABLE_FASTCALL Alloc(CThing& thing);
    CModeDragonFlying(CThing& thing);
    CModeDragonFlying(CThing& thing, std::vector<C3DVector>& points, const C3DVector& previous_point,
                      float speed, CCountedPointer<CCreatureActionBase> p_end_action);
    CModeDragonFlying(CThing& thing, std::vector<C3DVector>& points, float speed,
                      CCountedPointer<CCreatureActionBase> p_end_action);
    virtual const CAnimFilterBase& GetAnimFilter() const;
    virtual void OnActivate();
    virtual void FrameUpdate();
    virtual void OnDeactivate();
    virtual ~CModeDragonFlying();
private:
#ifdef FABLE_RETAIL_DRAGON_FLYING_PROTECTED_PASS30
    // Ego_d field list 0x2b564 declares these inherited flight fields protected.
protected:
#endif
    CAnimFilterOfGroup Filter;
    C3DVector Destination;
    C3DVector StartPos;
    C3DVector PreviousPos;
    float DistanceToTravel;
    float Speed;
    float StartSpeed;
    float MovementDuringAnimationLoop;
    float Acceleration;
    float AnimLength;
    CCountedPointer<CCreatureActionBase> PEndAction;
    long NumAnimLoops;
    long NumAnimLoopsToDo;
    bool FirstFrame;
    bool UsePreviousPos;
};

FABLE_STATIC_ASSERT_AT(43, sizeof(CModeDragonFlying) == 0x94);
}
#endif
