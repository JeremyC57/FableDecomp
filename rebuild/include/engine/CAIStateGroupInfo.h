#ifndef FABLE_ENGINE_CAISTATEGROUPINFO_H
#define FABLE_ENGINE_CAISTATEGROUPINFO_H
// CAIStateGroupInfo (agentE12, row 0x0121F86F): one entry of CAIBrain::StateGroupInfo (0x013BB008, 203 entries: a state-group
// name and the allocator that creates it). Ego_r.pdb layout (ghidra_out/struct_layouts_egor.tsv, 8 bytes; Anniversary PDB the same
// members): StateGroupName +0x00 (CCharString), AllocFunc +0x04 (`CAIStateGroupBase* (__fastcall*)()`). Constructor Ego_r.pdb
// ??0CAIStateGroupInfo@@QAE@VCCharString@@P6IPAVCAIStateGroupBase@@XZ@Z, out of line in retail (0x008FD21F, landed). Destructor
// user-declared and INLINE: the initializer 0x0121F86F destroys each temporary with a direct ~CCharString call (0x0099EAE0), while
// the atexit thunk 0x0122A20A passes the vector destructor iterator the out-of-line copy ??1CAIStateGroupInfo@@QAE@XZ 0x008FE93B
// (a jump to ~CCharString); an out-of-line destructor makes the initializer call ??1 instead, an implicit one makes the thunk an
// inline loop. The copy constructor is the implicit one (0x008FE940, landed). Needs fable_string.h's CCharString.
#include "rebuild_abi.h"
#include "fable_string.h"
class CAIStateGroupBase;
class CAIStateGroupInfo {
public:
    CAIStateGroupInfo(CCharString name, CAIStateGroupBase* (FABLE_FASTCALL* allocFunc)());   // 0x008FD21F
    ~CAIStateGroupInfo() {}                                                                 // 0x008FE93B (out-of-line copy)
    CCharString StateGroupName;                                                             // +0x00
    CAIStateGroupBase* (FABLE_FASTCALL* AllocFunc)();                                       // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroupInfo) == 0x8);
#endif
