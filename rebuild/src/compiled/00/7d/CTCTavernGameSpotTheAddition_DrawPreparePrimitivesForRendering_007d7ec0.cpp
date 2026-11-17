// CTCTavernGameSpotTheAddition::DrawPreparePrimitivesForRendering(CRenderContext* context) -- DrawPreparePrimitivesForRendering override: base draw-prepare, then while the game is played fetches (and immediately drops) the spot-the-addition definition
// Leaf: ?vfunc_11@CTCTavernGameSpotTheAddition@@QAEXPAUCRenderContext@@@Z   (NF49: the row's own symbol, so a grade does not pick the header's larger GetSubDef COMDAT)
// Retail: Fable.exe @ 0x007D7EC0, 66 bytes (vtable 0x0126F814 slot 11, `ret 4`).
// Callees: CTCTavernGame::DrawPreparePrimitivesForRendering 0x005ECA50 (non-virtual base call),
//          CDefClassBase::GetSubDef<T>(CDefPointer<T>*) 0x007D9A80 (bool; result unused).
// The definition comes from the parent thing (+4) -> +0x70; whatever used it was compiled out, so
// only the CDefPointer's construction and destruction remain.
// Shape: the def pointer is a one-word class (null constructor, destructor `if (p && --p->ref == 0)
//        p->Destroy()`, virtual slot 1); VC7.1 homes it in the dead `context` argument slot
//        (`lea ecx,[esp+8]`, `mov [esp+0Ch],0` after the push). IsBeingPlayed is +0x20 (PDB +0x28).
// NF49 (round 7): the string and def classes come from the shared headers (were local CRenderContext / CDefObject / CDefPointer / CDefClassBase models
// and a same-TU stand-in GetSubDef body): the call at 0x007D9A80 is the member template
// ??$GetSubDef@VCTavernGameSpotTheAdditionDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameSpotTheAdditionDef@@@@@Z (Ego_r.pdb / FableWin.pdb), its body visible
// and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom); GFIntToCharString / the CWideString
// operator+ overloads / CCharString::ToWideString are the fable_string.h / engine/fable_script_text.h declarations.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/fable_script_text.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameSpotTheAdditionDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

struct CRenderContext;
class CThing { public:
    unsigned char _pad[0x70];
    CDefClassBase* m_pDefinition;                   // +0x70
};

class CTCTavernGame { public:
    void* __vftable;
    CThing* m_pParentThing;                         // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x18];
    bool IsBeingPlayed;                             // +0x20
    void DrawPreparePrimitivesForRendering(CRenderContext* context);    // 0x005ECA50
};

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    void vfunc_11(CRenderContext* context);
};

void CTCTavernGameSpotTheAddition::vfunc_11(CRenderContext* context)
{
    CTCTavernGame::DrawPreparePrimitivesForRendering(context);
    if (IsBeingPlayed) {
        CDefPointer<const CTavernGameSpotTheAdditionDef> def;
        m_pParentThing->m_pDefinition->GetSubDef(&def);
    }
}