// CAIStateGroup_HoverHornetQueen::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: run CAIStateGroup_HoverHornetQueen::~CAIStateGroup_HoverHornetQueen (0x00903383), and on bit 0 free
//   the object through the global operator delete (MSVCR71 0x00BFE9BC); returns this
// Leaf: ??_GCAIStateGroup_HoverHornetQueen@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00903367, 28 bytes, thiscall `ret 4` (vtable slot 0).
// Fix (sweep 3): the landed source modelled this as a free __fastcall function calling an invented extern
//   `CAIStateGroup_HoverHornetQueen_Destruct` and an `OperatorDelete`; the callee is the landed (proven) destructor ??1CAIStateGroup_HoverHornetQueen@@UAE@XZ 0x00903383, its only
//   caller is this function (FINDINGS line 631 "_Destruct callees", agent951 G030), and Ego_r/FableWin hold
//   ??_GCAIStateGroup_HoverHornetQueen@@UAEPAXI@Z. The class is modelled only as far as the compiler needs to generate it: a virtual destructor
//   defined out of line (the landed row) and an inline constructor, whose use below emits the vtable and with it ??_G.
#pragma optimize("s", on)

class CAIStateGroup_HoverHornetQueen {
public:
    CAIStateGroup_HoverHornetQueen() { }
    virtual ~CAIStateGroup_HoverHornetQueen();                                  // 0x00903383
};

CAIStateGroup_HoverHornetQueen* Create_CAIStateGroup_HoverHornetQueen()
{
    return new CAIStateGroup_HoverHornetQueen;
}
