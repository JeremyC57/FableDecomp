// CNOVI_Barrel::OnPredicateFail() -- mark the quest (+0x14) bytes +0x74 / +0x75 and store the barrel thing's position (its slot 6) into the quest's unaligned vector at +0x76
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DB7DB0, length 64 bytes.
// Leaf: OnPredicateFail
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Shape: `this` in esi, the quest reloaded for each store, the position copied as three dwords, `ret`.
#include "engine/C3DVector.h"
#pragma pack(push, 1)
struct SQuest_00db7db0 {
    unsigned char _pad00[0x74];
    bool          m_74;                            // +0x74
    bool          m_75;                            // +0x75
    C3DVector     m_barrelPosition;                // +0x76 (unaligned)
};
#pragma pack(pop)
class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual C3DVector* GetPosition();              // slot 6 (+0x18)
    unsigned char _body[8];
};

struct CNOVI_Barrel {
    long               m_0;
    long               m_4;
    CScriptThing       Thing;                      // +0x08
    SQuest_00db7db0*   PQuest;                     // +0x14
    void OnPredicateFail();
};

void CNOVI_Barrel::OnPredicateFail()
{
    PQuest->m_74 = true;
    PQuest->m_75 = true;
    PQuest->m_barrelPosition = *Thing.GetPosition();
}