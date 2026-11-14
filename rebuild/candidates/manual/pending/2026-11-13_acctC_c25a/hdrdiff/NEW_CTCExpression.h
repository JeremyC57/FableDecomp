#ifndef FABLE_ENGINE_CTCEXPRESSION_H
#define FABLE_ENGINE_CTCEXPRESSION_H
// New header (agentC25, row 0x007EF200): Ego_r.pdb / FableWin.pdb CTCExpression (16 bytes: CTCBase + PDef
// CDefPointer<const CExpressionDef> +0x0C, struct_layouts_egor.tsv), interface TCI_EXPRESSION (0x8F). Inline accessors from
// FableWin.pdb (no Ego_r.pdb copies; expanded in retail): ?HasDef@CTCExpression@@QBE_NXZ, ?PeekDef@CTCExpression@@QBEABVCExpressionDef@@XZ,
// ?GetPDef@CTCExpression@@QBE?AV?$CDefPointer@$$CBVCExpressionDef@@@@XZ (by value: the row's `inc [eax+4]` copy at 0x007EF3CC).
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/ETCInterfaceType.h"
class CExpressionDef;
class CTCExpression : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_EXPRESSION; }
    bool HasDef() const { return !PDef.IsNull(); }
    const CExpressionDef& PeekDef() const { return *PDef; }
    CDefPointer<const CExpressionDef> GetPDef() const { return PDef; }
private:
    CDefPointer<const CExpressionDef> PDef;                              // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CTCExpression) == 16);
#endif
