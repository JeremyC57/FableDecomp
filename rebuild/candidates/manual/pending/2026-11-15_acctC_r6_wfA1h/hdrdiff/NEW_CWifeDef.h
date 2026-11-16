#ifndef FABLE_ENGINE_CWIFEDEF_H
#define FABLE_ENGINE_CWIFEDEF_H
// CWifeDef (agentC42, row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb TPI CWifeDef, 44 bytes, one own member `long Dowry` at +0x28
// behind the CSubDefClassBase part (Ego_r.pdb ?Alloc@CWifeDef@@SIPAVCDefClassBase@@XZ, ?Transfer@CWifeDef@@UAEXAAVCPersistContext@@@Z;
// GlobalUpdate reads Dowry at 0x006D0B9F after CDefClassBase::GetSubDef<CWifeDef> 0x006D3DC0). Replaces the generated padded model
// (engine/_quarantine/CWifeDef.h). Declarations only.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"
#include "fable_string.h"
class CPersistContext;
class CWifeDef : public CSubDefClassBase {
public:
    // wfA1: the inline class-name key (FableWin.pdb ?StatGetTypeName@CWifeDef@@SI?AVCCharString@@XZ; the 'CWifeDef' literal of the landed
    // GetSubDef<CWifeDef> 0x006D3DC0) and the Ego_r.pdb virtuals ?Transfer / ?Copy / ?GetSizeofClass@CWifeDef (as that landed row declares them).
    static CCharString FABLE_FASTCALL StatGetTypeName() { return CCharString("CWifeDef", -1); }
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long Dowry;                                                          // +0x28
};
FABLE_STATIC_ASSERT(offsetof(CWifeDef, Dowry) == 0x28);
FABLE_STATIC_ASSERT(sizeof(CWifeDef) == 0x2c);
#endif
