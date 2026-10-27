// NCreatureMode::CModeSitChairStudious::CModeSitChairStudious(CThing& thing) -- construct the NCreatureMode::CModeSitBase base, install this mode's vtable, then build Filter from the animation groups "SIT_CHAIR", "SIT_CHAIR_NORMAL", "SIT_CHAIR_STUDIOUS"
// Leaf: ??0CModeSitChairStudious@NCreatureMode@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00910830, 129 bytes, thiscall, returns this, `ret 4`.
// Identity (proven): it stores vtable 0x0128F590 (RTTI .?AVCModeSitChairStudious@NCreatureMode@@, base NCreatureMode::CModeSitBase) after the base constructor 0x008D7CA0 (Ego_r.pdb ??0CModeSitBase@NCreatureMode@@QAE@AAVCThing@@@Z), then constructs NCreatureMode::CModeSitChairStudious::Filter (Ego_r.pdb CAnimFilterOf3Groups at +0x2C) from "SIT_CHAIR", "SIT_CHAIR_NORMAL", "SIT_CHAIR_STUDIOUS"; Ego_r.pdb ??0CModeSitChairStudious@NCreatureMode@@QAE@AAVCThing@@@Z; caller CAIStateGroup_MinionSitAtSchool::AllocChairSittingMode 0x0090F7D0.
// Callees: NCreatureMode::CModeSitBase::CModeSitBase 0x008D7CA0, CCharString::CCharString(const char*, long) 0x0099EBF0 (three times, length -1),
//   CAnimFilterOf3Groups::CAnimFilterOf3Groups(const CCharString&, const CCharString&, const CCharString&) 0x0073E0D0,
//   CCharString::~CCharString 0x0099EAE0 (three times).
// Shape (family L055): base ctor, vptr store, the member initialiser's three implicit const char* -> CCharString conversions built right to left
//   (two in the frame, the third in the dead `thing` slot) and destroyed in reverse after the filter constructor.
class CThing;
class CCharString {
public:
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
private:
    void* PStringData;                                    // +0x00
};
class CAnimFilterOf3Groups {                               // Ego_r.pdb, 28 bytes: vptr, Group1..3, Group1CRC..Group3CRC
public:
    CAnimFilterOf3Groups(const CCharString& group1, const CCharString& group2, const CCharString& group3);   // 0x0073E0D0
    virtual ~CAnimFilterOf3Groups();
    unsigned char Body[0x18];                             // +0x04 (members not touched here)
};
namespace NCreatureMode {
class CModeSitBase {                                       // Ego_r.pdb, 0x2C bytes in retail (members not touched here)
public:
    CModeSitBase(CThing& thing);                          // 0x008D7CA0
    virtual ~CModeSitBase();
    unsigned char Body[0x28];                             // +0x04
};
}
namespace NCreatureMode {
class CModeSitChairStudious : public NCreatureMode::CModeSitBase {
public:
    CModeSitChairStudious(CThing& thing);
    CAnimFilterOf3Groups Filter;                          // +0x2C
};
}
NCreatureMode::CModeSitChairStudious::CModeSitChairStudious(CThing& thing)
    : CModeSitBase(thing),
      Filter("SIT_CHAIR", "SIT_CHAIR_NORMAL", "SIT_CHAIR_STUDIOUS")
{
}