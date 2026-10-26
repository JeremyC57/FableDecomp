// CTCCreatureGenerator::GetInterfaceType() const -- the component's interface type: 61 (0x3D)
// Leaf: ?GetInterfaceType@CTCCreatureGenerator@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074D9F0, 6 bytes (`mov eax,3Dh; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCCreatureGenerator::vfunc_21"): slot 21 of the CTCCreatureGenerator vtable 0x01268C24, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCCreatureGenerator@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCCreatureGenerator {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCCreatureGenerator::GetInterfaceType() const
{
    return (ETCInterfaceType)0x3D;
}
