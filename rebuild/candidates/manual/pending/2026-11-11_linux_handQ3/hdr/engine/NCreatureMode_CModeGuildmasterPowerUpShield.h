#ifndef FABLE_ENGINE_NCREATUREMODE_CMODEGUILDMASTERPOWERUPSHIELD_H
#define FABLE_ENGINE_NCREATUREMODE_CMODEGUILDMASTERPOWERUPSHIELD_H
// New header (chunk Q3, rows 0x008E20F0 Shield / 0x008E24F0 Speed FrameUpdate): NCreatureMode::CModeGuildmasterPowerUpShield and
// CModeGuildmasterPowerUpSpeed : CModeStandBase (Ego_r.pdb layout, ghidra_out/struct_layouts_egor.tsv: base 0x68, Filter +0x68,
// Timer +0x74, LightningBeamParticleEmitters +0x78, then with the retail 12-byte STLport vector Guards +0x84; sizeof 0x90).
// Member functions: Ego_r.pdb spellings (?FrameUpdate@CModeGuildmasterPowerUpShield@NCreatureMode@@UAEXXZ, slot 3, Shield vtable
// 0x012685F0 cell 0x012685FC; Speed vtable cell 0x01268630). Needs FABLE_RETAIL_CMODEBASE_VIRTUALS, FABLE_RETAIL_CMODESTANDBASE and
// FABLE_RETAIL_CINTELLIGENTPOINTER.
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThingAICreature;
namespace NCreatureMode {
class CModeGuildmasterPowerUpShield : public CModeStandBase {
public:
    CModeGuildmasterPowerUpShield(CThing& thing);
    CModeGuildmasterPowerUpShield(CThing& thing, float time);
    virtual ~CModeGuildmasterPowerUpShield();
    virtual unsigned long GetType() const;
    virtual void FrameUpdate();                                          // 0x008E20F0
    virtual void OnActivate();                                           // 0x008E0060
    virtual void OnDeactivate();                                         // 0x008D7A90
private:
    CAnimFilterOfGroup Filter;                                           // +0x68
    long Timer;                                                          // +0x74
    std::vector<CIntelligentPointer<CThing> > LightningBeamParticleEmitters;   // +0x78
    std::vector<CIntelligentPointer<CThingAICreature> > Guards;          // +0x84
    static void CheckRetailLayout();
};
inline void CModeGuildmasterPowerUpShield::CheckRetailLayout() {
    FABLE_STATIC_ASSERT(sizeof(CModeGuildmasterPowerUpShield) == 0x90);
    FABLE_STATIC_ASSERT(offsetof(CModeGuildmasterPowerUpShield, Timer) == 0x74);
    FABLE_STATIC_ASSERT(offsetof(CModeGuildmasterPowerUpShield, Guards) == 0x84);
}
class CModeGuildmasterPowerUpSpeed : public CModeStandBase {
public:
    CModeGuildmasterPowerUpSpeed(CThing& thing);
    CModeGuildmasterPowerUpSpeed(CThing& thing, float time);
    virtual ~CModeGuildmasterPowerUpSpeed();
    virtual unsigned long GetType() const;
    virtual void FrameUpdate();                                          // 0x008E24F0
    virtual void OnActivate();
    virtual void OnDeactivate();                                         // 0x008D7B20
private:
    CAnimFilterOfGroup Filter;                                           // +0x68
    long Timer;                                                          // +0x74
    std::vector<CIntelligentPointer<CThing> > LightningBeamParticleEmitters;   // +0x78
    std::vector<CIntelligentPointer<CThingAICreature> > Guards;          // +0x84
    static void CheckRetailLayout();
};
inline void CModeGuildmasterPowerUpSpeed::CheckRetailLayout() {
    FABLE_STATIC_ASSERT(sizeof(CModeGuildmasterPowerUpSpeed) == 0x90);
    FABLE_STATIC_ASSERT(offsetof(CModeGuildmasterPowerUpSpeed, Guards) == 0x84);
}
}
#endif
