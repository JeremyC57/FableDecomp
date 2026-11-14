#ifndef FABLE_ENGINE_CREACTIONMATCH_H
#define FABLE_ENGINE_CREACTIONMATCH_H
// New header (wfD5, row 0x00678760): Ego_r.pdb CReactionMatch, 8 bytes (ghidra_out/struct_layouts_egor.tsv: vptr +0,
// EOpinionReactionType ReactionType +4); class key from RTTI .?AVCReactionMatch@@ and the PDB list<PAVCReactionMatch> manglings.
// Virtuals in retail slot order (CReactionMatch_Sphere vtable 0x0125B7C4: 0 deleting dtor 0x0067C330, 1 Match 0x00700400,
// 2 GetMatchType 0x0067C320, 3 TransferBinaryIn 0x00701990, 4 TransferBinaryOut 0x007003A0). Ego_r.pdb / FableWin.pdb name
// ??1CReactionMatch@@UAE@XZ, ?TransferBinaryIn@CReactionMatch@@UAEXAAVCMemoryDataInputStream@@@Z (0x006FEFB0, landed) and
// ?TransferBinaryOut@CReactionMatch@@UBEXAAVCDataOutputStream@@@Z, but no CReactionMatch::Match / GetMatchType: pure in the base.
// The constructor ??0CReactionMatch@@QAE@W4EOpinionReactionType@@@Z (FableWin) is inline (the derived constructors store +4 in
// place, 0x0067C2FF). The concrete generated engine/CReactionMatch_*.h headers are flat layouts without this base.
#include <stddef.h>
#include "rebuild_abi.h"
enum EOpinionReactionType;                                   // MS forward enums
enum EReactionMatchType;
class COpinionSet;
class CMemoryDataInputStream;
class CDataOutputStream;
class CReactionMatch {
public:
    CReactionMatch(EOpinionReactionType type) : ReactionType(type) {}
    virtual ~CReactionMatch();
    virtual float Match(const COpinionSet& opinions) = 0;
    virtual EReactionMatchType GetMatchType() const = 0;
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;
    EOpinionReactionType ReactionType;                       // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CReactionMatch) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CReactionMatch, ReactionType) == 0x4);
#endif // FABLE_ENGINE_CREACTIONMATCH_H
