#ifndef FABLE_ENGINE_CTCACTIONUSEBASE_H
#define FABLE_ENGINE_CTCACTIONUSEBASE_H
// CTCActionUseBase (new, chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): the base of the
// action-use components (TCI_ON_ACTION_USE 13: FableWin.pdb ?StatGetInterfaceType@CTCActionUseBase@@SI?AW4ETCInterfaceType@@XZ).
// Ego_r.pdb (struct_layouts_egor.tsv, 20 bytes, no STL member): PActionUseDef CDefPointer<const CActionUseDef> +0x0C, Usable +0x10
// (the row tests `[eax+10h]`: an unusable action-use thing gets no "use" cursor).
// Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/ETCInterfaceType.h"
class CActionUseDef;
#pragma pack(push, 1)
class CTCActionUseBase : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ON_ACTION_USE; }
    bool IsUsable() const { return Usable; }                             // PLACEHOLDER accessor name (inline member read in the row)
private:
    CDefPointer<const CActionUseDef> PActionUseDef;                      // +0x0c
    bool Usable;                                                         // +0x10
    unsigned char _pad_0x11[3];                                          // +0x11
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CTCActionUseBase) == 0x14);
#endif
