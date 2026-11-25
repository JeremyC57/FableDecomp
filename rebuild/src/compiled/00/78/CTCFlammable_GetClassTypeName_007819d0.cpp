// CTCFlammable::GetClassTypeName() const -- the class-type name "CTCFlammable" as a CCharString (the in-class StatGetClassTypeName, inlined)
// Leaf: ?GetClassTypeName@CTCFlammable@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007819D0, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0123BEE0 "CTCFlammable"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCFlammable@@UBE?AVCCharString@@XZ;
//   retail vftable 0x0126B1A4 (RTTI .?AVCTCFlammable@@) slot 22 (0x0126B1FC -> 0x007819D0) is CTCBase's pure GetClassTypeName slot
//   (engine/CTCBase.h); the literal 0x0123BEE0 = "CTCFlammable" = the class name (read from the exe).
// Form (NF417): FableWin.pdb spells the class's static ?StatGetClassTypeName@CTCFlammable@@SI?AVCCharString@@XZ (Ego_r has none); the
//   25 retail bytes are that static's 19-byte body (push -1; push lit; call; mov eax) expanded into the virtual, so the static is an
//   in-class inline (as the landed static row 0x004D46BD and the binder 0x004EE23F define it) and the virtual calls it; retail's
//   out-of-line copy is 0x004D46BD (bound by 004ee23f). VC7.1 also emits the static's pick-any COMDAT copy in this object (identical to
//   the landed copies; the linker keeps one).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CTCFlammable_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a
//   made-up `_Methods` class).
// Header: engine/CTCFlammable.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCFlammable CTCFlammableLayout
#include "engine/CTCFlammable.h"
#undef CTCFlammable

class CTCFlammable : public CTCFlammableLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCFlammable", -1); }
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCFlammable::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
