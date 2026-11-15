#ifndef FABLE_ENGINE_COPINIONREACTIONBEHAVIOUR_H
#define FABLE_ENGINE_COPINIONREACTIONBEHAVIOUR_H
// COpinionReactionBehaviour (agentE12, row 0x01221D20): one entry of CAIStateGroup_OpinionReactionRoot::Behaviours (0x013BB748,
// 79 entries, one per opinion reaction type). Ego_r.pdb layout (ghidra_out/struct_layouts_egor.tsv, 40 bytes): UpdateFunc +0x00,
// IsRunnableFunc +0x10 (both `bool (CAIStateGroup_OpinionReactionRoot::*)()`, Anniversary PDB LF_MFUNCTION), Name +0x20 (CCharString).
// The pointers to member are 16-byte unspecified-inheritance pointers (the class is only forward-declared here; VC7.1 aligns them
// to 8, hence 40 bytes, not 36); include this header BEFORE engine/CAIStateGroup_OpinionReactionRoot.h. Constructor: FableWin.pdb
// ??0COpinionReactionBehaviour@@QAE@P8CAIStateGroup_OpinionReactionRoot@@AE_NXZABVCCharString@@0@Z (update, name, isRunnable;
// the Anniversary PDB has the same three arguments); inline in retail (the initializer 0x01221D20 stores the two pointers and
// copy-constructs Name, 0x0099EC30, from the converted literal). The destructor is compiler-generated (Anniversary PDB), as
// the atexit loop 0x0122A460 shows (~CCharString inline on +0x20). Needs fable_string.h's CCharString (FABLE_RETAIL_CCHARSTRING
// for the default length that lets a literal convert).
#include "rebuild_abi.h"
#include "fable_string.h"
class CAIStateGroup_OpinionReactionRoot;
class COpinionReactionBehaviour {
public:
    COpinionReactionBehaviour(bool (CAIStateGroup_OpinionReactionRoot::*updateFunc)(), const CCharString& name,
                              bool (CAIStateGroup_OpinionReactionRoot::*isRunnableFunc)())
        : UpdateFunc(updateFunc), IsRunnableFunc(isRunnableFunc), Name(name) {}
    bool (CAIStateGroup_OpinionReactionRoot::*UpdateFunc)();                 // +0x00
    bool (CAIStateGroup_OpinionReactionRoot::*IsRunnableFunc)();             // +0x10
    CCharString Name;                                                        // +0x20
};
FABLE_STATIC_ASSERT(sizeof(COpinionReactionBehaviour) == 0x28);
#endif
