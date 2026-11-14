#ifndef FABLE_ENGINE_CREACTIONMATCH_MORALITYRENOWNBLOB_H
#define FABLE_ENGINE_CREACTIONMATCH_MORALITYRENOWNBLOB_H
// New header (wfD5, row 0x00678760): CReactionMatch_MoralityRenownBlob, absent from struct_layouts_egor.tsv; named by Ego_r.pdb
// 0x006A95D0 / FableWin ??0CReactionMatch_MoralityRenownBlob@@QAE@W4EOpinionReactionType@@MMMMMMM@Z (retail 0x00677760, landed:
// a CReactionMatch_MRPyramid built from the blob's renown / morality bounds; `new` size 0x34 at TransferIn 0x00678DDB).
// Needs FABLE_RETAIL_CREACTIONMATCH_ASYMMETRICPYRAMID_CLASS and FABLE_RETAIL_CREACTIONMATCH_MRPYRAMID_CLASS.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CReactionMatch_MRPyramid.h"
#pragma pack(push, 1)
class CReactionMatch_MoralityRenownBlob : public CReactionMatch_MRPyramid {
public:
    CReactionMatch_MoralityRenownBlob(EOpinionReactionType type, float yMin, float yMax, float xMin, float xMax,
                                      float scarinessShift, float agreeablenessShift, float attractivenessShift);   // 0x00677760
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CReactionMatch_MoralityRenownBlob) == 0x34);
#endif // FABLE_ENGINE_CREACTIONMATCH_MORALITYRENOWNBLOB_H
