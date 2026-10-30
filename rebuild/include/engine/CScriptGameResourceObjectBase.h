#ifndef FABLE_ENGINE_CSCRIPTGAMERESOURCEOBJECTBASE_H
#define FABLE_ENGINE_CSCRIPTGAMERESOURCEOBJECTBASE_H
// Retail model (hand-written 2026-10-30, class-model migration wave 5; opt-in FABLE_RETAIL_BASE_OBJECT, the CBaseObject
// family). RTTI .?AVCScriptGameResourceObjectBase@@ : CBaseObject (ghidra_out/rtti_bases.tsv, 2 bases). Vtable 0x0126008C,
// 2 slots: slot 0 scalar deleting dtor 0x006E70A0 (`mov [esi],0x0126008C; call ~CBaseObject 0x0099A430`), slot 1 inherited
// CBaseObject::Validate 0x0099A400. No members of its own (Ego_r.pdb has no layout row; the derived classes put PImp at +8).
// The dtor is user-written and inline: every expansion keeps this class's vtable store before the CBaseObject dtor (the
// deleting dtor 0x006E70A0 and the derived dtors 0x006E7B80 / 0x007E74D0: `mov [esi],0x0126008C; jmp 0x0099A430`), and
// VC7.1 drops that store from an implicit dtor (tested: the implicit form gives a bare `jmp 0x0099A430`).
// The other members are the implicit ones: the default ctor 0x006E7A60 is `call CBaseObject() 0x0099A380; mov [esi],0x0126008C`,
// and both derived ctors (0x006E7B60, 0x007E72A0) expand it inline (they call 0x0099A380 directly); the derived copy ctor
// 0x00905330 and operator= 0x008ABD10 call CBaseObject's copy 0x0099A390 / operator= 0x0099A3B0 directly (this class's
// copy members inline); the derived dtors 0x006E7B80 / 0x007E74D0 end with this class's dtor inline (`mov [esi],0x0126008C;
// jmp 0x0099A430`). The PDB spellings ??0 / ??0(ABV0) / ??4 / ??1 (UAE, public virtual) / ??_G (Ego_r.pdb 0x00710370,
// FableWin.pdb 0x02467C20-0x02467D30, 0x02A8C0D0, 0x02B47030) are what the implicit members mangle to.
#ifndef FABLE_RETAIL_BASE_OBJECT
#error engine/CScriptGameResourceObjectBase.h is the retail CBaseObject-family model: define FABLE_RETAIL_BASE_OBJECT first
#else
#include "rebuild_abi.h"
#include "engine/CBaseObject.h"
class CScriptGameResourceObjectBase : public CBaseObject {
public:
    virtual ~CScriptGameResourceObjectBase() { }                         // slot 0  0x006E70A0 (scalar deleting)
};
FABLE_STATIC_ASSERT(sizeof(CScriptGameResourceObjectBase) == 0x8);
#endif // FABLE_RETAIL_BASE_OBJECT
#endif // FABLE_ENGINE_CSCRIPTGAMERESOURCEOBJECTBASE_H
