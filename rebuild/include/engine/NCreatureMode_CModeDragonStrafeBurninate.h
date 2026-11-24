#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEDRAGONSTRAFEBURNINATE_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEDRAGONSTRAFEBURNINATE_H

#include "engine/NCreatureMode_CModeDragonFlying.h"
#include "engine/CIntelligentPointer.h"

namespace NCreatureMode {
// Ego_r layouts, FableWin TPI 0x2B58C and native new 0xC8 at 0x00950F59.
class CModeDragonStrafeBurninate : public CModeDragonFlying {
public:
    static unsigned long FABLE_FASTCALL StatGetType();
    virtual unsigned long GetType() const;
    static CTCCreatureModeManager::CModeBase* FABLE_FASTCALL Alloc(CThing& thing);
    CModeDragonStrafeBurninate(CThing& thing);
    CModeDragonStrafeBurninate(CThing& thing, std::vector<C3DVector>& points, const C3DVector& previous_point,
                             float burn_start_percentage, float burn_stop_percentage,
                             C3DVector& flame_start_pos, C3DVector& flame_end_pos); // 0x008E1580
    virtual const CAnimFilterBase& GetAnimFilter() const;
    virtual void OnActivate();
    virtual void FrameUpdate();
    virtual void OnDeactivate();
    virtual ~CModeDragonStrafeBurninate();
private:
    void StartBeamEffect();
    void KillBeamEffect();
    void UpdateBeam(float spline_progress);
    long Timer;
    CIntelligentPointer<CThing> PBeamWeaponEffect;
    long LastExplodeFrame;
    C3DVector FlameStartPos;
    C3DVector FlameEndPos;
    bool FlameActive;
    float FlameStartPercentage;
    float FlameStopPercentage;
};

FABLE_STATIC_ASSERT(sizeof(CModeDragonStrafeBurninate) == 0xC8);
}
#endif
