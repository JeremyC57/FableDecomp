#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEICETROLLGROUNDSMASHICICLEASSAULT_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEICETROLLGROUNDSMASHICICLEASSAULT_H
#ifdef FABLE_RETAIL_ICICLE_ASSAULT_PASS30
#include "engine/CMainGameComponent.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CSnowTrollDef.h"
#include "engine/CDefPointer.h"
namespace NCreatureMode {
class CModeIceTrollGroundSmashIcicleAssault : public CModeStandBase {
public:
    class CIcicleData {
    public:
        CIcicleData();
#ifdef FABLE_RETAIL_ICICLE_COPY_CTOR_DECL_PASS30
        // The retail vector appender calls the separate copy constructor at 008e2fc0.
        CIcicleData(const CIcicleData&);
#endif
        CIntelligentPointer<CThing> PEffect;
        CIntelligentPointer<CThing> PTrap;
        long EffectTimer;
        long TrapTimer;
        C3DVector Position;
    };
    virtual void FrameUpdate();
private:
    bool IsTimeToQuit() const {
        return Timer > CMainGameComponent::GetConstantFPS() * PDef->SecondsForIcicleAssault && Icicles.empty();
    }
    CIntelligentPointer<CThing> PTarget;
    CAnimFilterOfGroup Filter;
    long Timer;
    CDefPointer<const CSnowTrollDef> PDef;
    std::vector<CIcicleData> Icicles;
    std::vector<C3DVector> ViablePoints;
};
}
FABLE_STATIC_ASSERT_AT(30, sizeof(NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::CIcicleData) == 0x24);
FABLE_STATIC_ASSERT_AT(31, sizeof(NCreatureMode::CModeIceTrollGroundSmashIcicleAssault) == 0x9c);
#endif
#endif
