// CTCDeadShotInThingTracker::GetInterfaceType() const -- the component's interface type: 118 (0x76)
// Leaf: ?GetInterfaceType@CTCDeadShotInThingTracker@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3530, 6 bytes (`mov eax,76h; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCDeadShotInThingTracker::vfunc_21"): slot 21 of the CTCDeadShotInThingTracker vtable 0x0127040C, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCDeadShotInThingTracker@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCDeadShotInThingTracker {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCDeadShotInThingTracker::GetInterfaceType() const
{
    return (ETCInterfaceType)0x76;
}
