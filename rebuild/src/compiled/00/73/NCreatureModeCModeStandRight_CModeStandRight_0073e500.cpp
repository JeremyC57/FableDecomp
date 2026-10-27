// NCreatureMode::CModeStandRight::CModeStandRight(CThing& thing) -- construct the CModeStandBase base, install this mode's vtable, then build Filter from the animation groups "STAND_RIGHT", "NOT_CARRYING", "STAND"
// Leaf: ??0CModeStandRight@NCreatureMode@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073e500, 129 bytes, thiscall, returns this, `ret 4`.
// Identity (proven): it stores vtable 0x01267534 (RTTI .?AVCModeStandRight@NCreatureMode@@, base CModeStandBase) after the base constructor 0x00861E00 (Ego_r.pdb ??0CModeStandBase@@QAE@AAVCThing@@@Z), then constructs NCreatureMode::CModeStandRight::Filter (Ego_r.pdb CAnimFilterOf3Groups at +0x68) from "STAND_RIGHT", "NOT_CARRYING", "STAND"; Ego_r.pdb ??0CModeStandRight@NCreatureMode@@QAE@AAVCThing@@@Z; caller the mode allocator 0x0073EC40 (catalogued with 0x0073EC10).
// Callees: CModeStandBase::CModeStandBase 0x00861E00, CCharString::CCharString(const char*, long) 0x0099EBF0 (three times, length -1),
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
class CModeStandBase {                                     // Ego_r.pdb, 0x68 bytes (members not touched here)
public:
    CModeStandBase(CThing& thing);                        // 0x00861E00
    virtual ~CModeStandBase();
    unsigned char Body[0x64];                             // +0x04
};
namespace NCreatureMode {
class CModeStandRight : public CModeStandBase {
public:
    CModeStandRight(CThing& thing);
    CAnimFilterOf3Groups Filter;                          // +0x68
};
}
NCreatureMode::CModeStandRight::CModeStandRight(CThing& thing)
    : CModeStandBase(thing),
      Filter("STAND_RIGHT", "NOT_CARRYING", "STAND")
{
}