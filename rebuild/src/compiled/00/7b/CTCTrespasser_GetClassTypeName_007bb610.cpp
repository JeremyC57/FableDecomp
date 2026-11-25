// CTCTrespasser::GetClassTypeName() const -- the class-type name "CTCTrespasser" as a CCharString (the in-class StatGetClassTypeName, inlined)
// Leaf: ?GetClassTypeName@CTCTrespasser@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BB610, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0123CB18 "CTCTrespasser"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCTrespasser@@UBE?AVCCharString@@XZ;
//   retail vftable 0x0126DE14 (RTTI .?AVCTCTrespasser@@) slot 22 (0x0126DE6C -> 0x007BB610) is CTCBase's pure GetClassTypeName slot
//   (engine/CTCBase.h); the literal 0x0123CB18 = "CTCTrespasser" = the class name (read from the exe).
// Form (NF417): FableWin.pdb spells the class's static ?StatGetClassTypeName@CTCTrespasser@@SI?AVCCharString@@XZ (Ego_r has none); the
//   25 retail bytes are that static's 19-byte body (push -1; push lit; call; mov eax) expanded into the virtual, so the static is an
//   in-class inline (as the landed static row 0x004D52C2 and the binder 0x004EE23F define it) and the virtual calls it; retail's
//   out-of-line copy is 0x004D52C2 (bound by 004ee23f). VC7.1 also emits the static's pick-any COMDAT copy in this object (identical to
//   the landed copies; the linker keeps one).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CTCTrespasser@@QAE?AVCCharString@@XZ (a non-virtual, non-const member of the
//   real class).
// Header: engine/CTCTrespasser.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCTRESPASSER
#define FABLE_RETAIL_CTCTRESPASSER_FRAMEUPDATE
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCTrespasser CTCTrespasserLayout
#include "engine/CTCTrespasser.h"
#undef CTCTrespasser

class CTCTrespasser : public CTCTrespasserLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCTrespasser", -1); }
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCTrespasser::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
