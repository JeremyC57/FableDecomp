// `anonymous namespace'::`dynamic atexit destructor for 'CrimeText'' -- destroy the ten CCharStrings of CrimeText[10] through the vector destructor iterator
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01228ED8, 20 bytes: push ~CCharString ; push 10 ; push 4 ; push 0x013B8B10 ;
//   call `vector destructor iterator' ; ret.
// Identity (proven, chunk c674): the one atexit destructor the CrimeText initializer 0x0121A7CA registers (its _atexit push);
//   the ten consecutive CCharStrings 0x013B8B10..0x013B8B34 are the anonymous-namespace array CrimeText[10] (Ego_r.pdb initializer
//   order DEFAULT_DEATH_RECOVERY_MARKER_NAME -> CrimeText -> CrimeTotalText; see the 0x0121A7CA row).
// Callees: `vector destructor iterator' 0x004012F0-family (??_I, __stdcall: array, 4, 10, dtor), CCharString::~CCharString 0x0099EAE0.
// Shape: the same TU as the landed initializer 0x0121A7CA (its source, verbatim below): the compiler emits this _$E2 with the
//   _$E1 initializer; size-optimised TU.
// Naming wave 16: landed as the invented CDebugManager::ThrowException (FableThrowException_1228ed8 calling a stdcall
//   FableThrow_1228ed8 with immediate arguments); re-authored as the compiler-generated atexit destructor (name-only).
#pragma optimize("s", on)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A7CA, 177 bytes, in `.text$yc`, reached only from the .CRT$XCU table entry
//   0x01374324, plain `ret`. The start is genuine: the .CRT$XCU slot points at it and the previous initializer 0x0121A7AD ends
//   with `ret` at 0x0121A7C9 (initializers of a size-optimised TU are packed with no alignment padding).
// Identity (proven, two sources):
//   (1) initializer order: Ego_r.pdb runs `anonymous namespace'::DEFAULT_DEATH_RECOVERY_MARKER_NAME 0100DD76 -> CrimeText
//       0100DD93 -> CrimeTotalText 0100DE44 -> CrimeTimeLeftText 0100DE61 (CrimeText is 0xB1 = 177 bytes, the others 0x1D;
//       FableWin.pdb has the same pair at 03FE0B21 / 03FE0B4A); retail's .CRT$XCU runs 0x0121A7AD (29 bytes, CCharString
//       "HERO_GUILD_RECOVERY_MARKER" at 0x013B8B0C) -> this 177-byte body -> 0x0121A87B (29 bytes, "TEXT_GUI_CRIME_TOTAL_FINE"
//       at 0x013B8B38) -> 0x0121A898.
//   (2) the objects are ten consecutive CCharStrings 0x013B8B10..0x013B8B34 torn down by ONE atexit destructor 0x01228ED8
//       (`push ~CCharString ; push 10 ; push 4 ; push 0x013B8B10 ; call `vector destructor iterator'`), i.e. one array
//       CCharString[10], read by index from 0x0057DCA4 (the hero crime display code).
//   The catalog row was a gap-scan placeholder (_gapscan::sub_0121a7ca).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (x10), _atexit 0x004012BC (registers 0x01228ED8, which
//   is landed as the invented `CDebugManager::ThrowException`; it is this array's `dynamic atexit destructor for 'CrimeText''`).
// Shape: an aggregate-initialised static array; each element is built in place by the converting constructor
//   (`push -1 ; push lit ; mov ecx,&CrimeText[i] ; call`), -1 kept in esi. Size-optimised TU (packed address and the
//   vector-destructor-iterator teardown; GOTCHAS 0x0122A20A).
class CCharString {
public:
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
private:
    void* PStringData;
};

namespace {
CCharString CrimeText[10] = {                          // 0x013B8B10, indexed by crime type
    "",
    "TEXT_GUI_CRIME_WEAPON_OUT",
    "TEXT_GUI_CRIME_TRESPASS",
    "TEXT_GUI_CRIME_VANDALISM",
    "TEXT_GUI_CRIME_LOCKPICKING",
    "TEXT_GUI_CRIME_PICK_POCKETS",
    "TEXT_GUI_CRIME_THEFT",
    "TEXT_GUI_CRIME_ASSAULT",
    "TEXT_GUI_CRIME_GBH",
    "TEXT_GUI_CRIME_MURDER",
};
}
