// CTavernGameCardBaseDef::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: run CTavernGameCardBaseDef::~CTavernGameCardBaseDef (0x004E2DE7), and on bit 0 free
//   the object through the global operator delete (MSVCR71 0x00BFE9BC); returns this
// Leaf: ??_GCTavernGameCardBaseDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2DCB, 28 bytes, thiscall `ret 4` (vtable slot 0).
// Fix (sweep 3): the landed source modelled this as a free __fastcall function calling an invented extern
//   `CTavernGameCardBaseDef_Destruct` and an `OperatorDelete`; the callee is the landed (proven) destructor ??1CTavernGameCardBaseDef@@UAE@XZ 0x004E2DE7, its only
//   caller is this function (FINDINGS line 631 "_Destruct callees", agent951 G030), and Ego_r/FableWin hold
//   ??_GCTavernGameCardBaseDef@@UAEPAXI@Z. The class is modelled only as far as the compiler needs to generate it: a virtual destructor
//   defined out of line (the landed row) and an inline constructor, whose use below emits the vtable and with it ??_G.
#pragma optimize("s", on)

class CTavernGameCardBaseDef {
public:
    CTavernGameCardBaseDef() { }
    virtual ~CTavernGameCardBaseDef();                                  // 0x004E2DE7
};

CTavernGameCardBaseDef* Create_CTavernGameCardBaseDef()
{
    return new CTavernGameCardBaseDef;
}
