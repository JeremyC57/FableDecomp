// CTCDeadShotInThingTracker::GetClassTypeName() const -- the class-type name "CTCDeadShotInThingTracker" as a CCharString (the in-class StatGetClassTypeName, inlined)
// Leaf: ?GetClassTypeName@CTCDeadShotInThingTracker@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3540, 25 bytes: push esi; mov esi,[esp+8]; push -1; push 0x0123E128 "CTCDeadShotInThingTracker"; mov ecx,esi;
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,esi; pop esi; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (kept in esi).
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CTCDeadShotInThingTracker@@UBE?AVCCharString@@XZ; retail vftable 0x0127040C (RTTI
//   .?AVCTCDeadShotInThingTracker@@) slot 22 (0x01270464 -> 0x007E3540) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h); the
//   literal 0x0123E128 = "CTCDeadShotInThingTracker" = the class name (read from the exe).
// Form (NF417): FableWin.pdb spells the class's static ?StatGetClassTypeName@CTCDeadShotInThingTracker@@SI?AVCCharString@@XZ (Ego_r has
//   none); the 25 retail bytes are that static's 19-byte body (push -1; push lit; call; mov eax) expanded into the virtual, so the
//   static is an in-class inline (as the landed static row 0x004D6353 and the binder 0x004EE23F define it) and the virtual calls it;
//   retail's out-of-line copy is 0x004D6353 (bound by 004ee23f). VC7.1 also emits the static's pick-any COMDAT copy in this object
//   (identical to the landed copies; the linker keeps one).
// Respell (NF417): was the stand-in self ?GetClassTypeName@CTCDeadShotInThingTracker_Methods@@QAE?AVCCharString@@XZ (a non-virtual
//   member of a made-up `_Methods` class).
// Header: engine/CTCDeadShotInThingTracker.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCDeadShotInThingTracker CTCDeadShotInThingTrackerLayout
#include "engine/CTCDeadShotInThingTracker.h"
#undef CTCDeadShotInThingTracker

class CTCDeadShotInThingTracker : public CTCDeadShotInThingTrackerLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName() { return CCharString("CTCDeadShotInThingTracker", -1); }
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCDeadShotInThingTracker::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
