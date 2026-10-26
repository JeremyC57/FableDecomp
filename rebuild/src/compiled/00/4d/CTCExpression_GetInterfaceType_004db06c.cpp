// CTCExpression::GetInterfaceType() const -- the component's interface type: 143 (0x8F)
// Leaf: ?GetInterfaceType@CTCExpression@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB06C, 6 bytes (`mov eax,8Fh; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCExpression::vfunc_21"): slot 21 of the CTCExpression vtable 0x012401F4, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCExpression@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCExpression {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCExpression::GetInterfaceType() const
{
    return (ETCInterfaceType)0x8F;
}
