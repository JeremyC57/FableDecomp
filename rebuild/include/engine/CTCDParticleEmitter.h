#ifndef FABLE_ENGINE_CTCDPARTICLEEMITTER_H
#define FABLE_ENGINE_CTCDPARTICLEEMITTER_H
// New shared header (chunk 1024, row 0x007CD390 CTCMultiStrike::CTCMultiStrike): the particle-emitter component, interface
// TCI_PARTICLE_EMITTER (75; the row's inline GetTC looks up 0x4B). Declarations only, Ego_r.pdb / FableWin.pdb spellings:
// ?SetAlwaysUpdate@CTCDParticleEmitter@@QAEX_N@Z (retail 0x006E0590) and
// ?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z (retail 0x006E0BE0). Layout not modelled. Needs FABLE_RETAIL_CTCBASE.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CCharString;
class CTCDParticleEmitter : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_EMITTER; }
    void SetAlwaysUpdate(bool always_update);                                                          // 0x006E0590
    void AttachToThing(CThing& thing, long attach_type, const CCharString& helper_name, long helper_index, float offset);   // 0x006E0BE0
};
#endif
