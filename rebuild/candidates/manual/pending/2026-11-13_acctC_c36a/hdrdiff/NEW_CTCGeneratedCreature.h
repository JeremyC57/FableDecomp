#ifndef FABLE_ENGINE_CTCGENERATEDCREATURE_H
#define FABLE_ENGINE_CTCGENERATEDCREATURE_H
// CTCGeneratedCreature (agentC36, row 0x0074AF50): the component a creature generator adds to each creature it makes.
// Ego_r.pdb layout (struct_layouts_egor.tsv): 20 bytes, CTCBase + CIntelligentPointer<const CThing> PGenerator +0x0C.
// FableWin.pdb ?SetGenerator@CTCGeneratedCreature@@QAEXABVCThing@@@Z (retail 0x0074D0D0, which routes its argument through a
// temporary intelligent pointer into +0x0C), ?IsGeneratorActive@CTCGeneratedCreature@@QBE_NXZ. Class name string 0x0123AC80.
// Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_CINTELLIGENTPOINTER.
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CThing;
class CTCGeneratedCreature : public CTCBase {
public:
    void SetGenerator(const CThing&);                        // 0x0074D0D0
    CIntelligentPointer<const CThing> PGenerator;            // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CTCGeneratedCreature) == 0x14);
#endif // FABLE_ENGINE_CTCGENERATEDCREATURE_H
