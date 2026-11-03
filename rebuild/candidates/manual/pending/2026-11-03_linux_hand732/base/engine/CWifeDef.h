#ifndef FABLE_ENGINE_CWIFEDEF_H
#define FABLE_ENGINE_CWIFEDEF_H
// CWifeDef (chunk 728 round 2, row 0x006D0830 CTCWife::GlobalUpdate): RTTI .?AVCWifeDef@@ : CSubDefClassBase : CDefClassBase :
// CDefPointeeBase (ghidra_out/rtti_bases.tsv); Ego_r.pdb sizeof 0x2C with long Dowry at +0x28 (GlobalUpdate reads [def+28h] after
// ??$GetSubDef@VCWifeDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCWifeDef@@@@@Z 0x006D3DC0). Retail def model: needs
// FABLE_RETAIL_DEF_CLASS_BASE.
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
class CPersistContext;
// Members as the landed instantiation 0x006D3DC0 models them (Ego_r.pdb / FableWin.pdb spellings): the inline static
// StatGetTypeName (FableWin 0x02490170 ?StatGetTypeName@CWifeDef@@SI?AVCCharString@@XZ; GetSubDef<CWifeDef> builds "CWifeDef"
// in place) and the virtuals Transfer / Copy / GetSizeofClass (Ego_r 0x004F6153 / FableWin 0x01B9DA98 / 0x01B9DAB3).
class CWifeDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CWifeDef", -1); }   // ?StatGetTypeName@CWifeDef@@SI?AVCCharString@@XZ (inline)
    virtual void Transfer(CPersistContext&);                             // ?Transfer@CWifeDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                             // ?Copy@CWifeDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                              // ?GetSizeofClass@CWifeDef@@UAEKXZ
    long Dowry;                                                          // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CWifeDef) == 0x2c);
#endif
