// CTCIdleScheduler::GetInterfaceType() const -- the component's interface type: 211 (0xD3)
// Leaf: ?GetInterfaceType@CTCIdleScheduler@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1566, 6 bytes (`mov eax,D3h; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCIdleScheduler::vfunc_21"): slot 21 of the CTCIdleScheduler vtable 0x012426C4, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCIdleScheduler@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCIdleScheduler {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCIdleScheduler::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD3;
}
