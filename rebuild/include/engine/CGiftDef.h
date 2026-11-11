#ifndef FABLE_ENGINE_CGIFTDEF_SHARED_H
#define FABLE_ENGINE_CGIFTDEF_SHARED_H
// CGiftDef (NEW shared header, agentNF10, rows 0x00894BF0 / 0x00894C90 / 0x00894D30 CGameScriptInterface::IsGiftRomantic / Friendly /
// Offensive): RTTI .?AVCGiftDef@@ : CSubDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb CGiftDef
// (struct_layouts_egor.tsv, 0x30, no container: the same offsets in retail) Type (EGiftType) +0x28, IsWeddingRing +0x2C; retail reads
// Type `mov edx,[ecx+28h]` off GetSubDef<CGiftDef> 0x006D3EF0 (0x00894C3C). StatGetTypeName: FableWin.pdb
// ?StatGetTypeName@CGiftDef@@SI?AVCCharString@@XZ (inline in Ego_r.pdb and retail), the class-name literal. EGiftType's enumerators
// are in no name set on disk (the rows compare with 0 / 1 / 2), so the enum is declared without them. The generated flat model stays in
// engine/_quarantine/CGiftDef.h (a different include guard). Needs FABLE_RETAIL_DEF_CLASS_BASE and fable_string.h.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"
#ifdef _MSC_VER
enum EGiftType;                                                          // MS forward enum (enumerators unproven)
#else
enum EGiftType { };
#endif
class CGiftDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CGiftDef", -1); }
    unsigned char _pad_0x25[0x3];                                        // +0x25
    EGiftType Type;                                                      // +0x28
    bool IsWeddingRing;                                                  // +0x2c
};
FABLE_STATIC_ASSERT(sizeof(CGiftDef) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CGiftDef, Type) == 0x28);
#endif
