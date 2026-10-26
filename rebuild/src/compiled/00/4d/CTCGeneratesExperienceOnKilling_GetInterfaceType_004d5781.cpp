// CTCGeneratesExperienceOnKilling::GetInterfaceType() const -- the component's interface type: 201 (0xC9)
// Leaf: ?GetInterfaceType@CTCGeneratesExperienceOnKilling@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5781, 6 bytes (`mov eax,C9h; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCGeneratesExperienceOnKilling::vfunc_21"): slot 21 of the CTCGeneratesExperienceOnKilling vtable 0x0123D224, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCGeneratesExperienceOnKilling@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCGeneratesExperienceOnKilling {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCGeneratesExperienceOnKilling::GetInterfaceType() const
{
    return (ETCInterfaceType)0xC9;
}
