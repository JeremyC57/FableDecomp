// CTavernGameShoveHaPennyDef::operator=(const CTavernGameShoveHaPennyDef&) -- def copy: tavern-game base, the region vector, then 20 board/coin tunables
// Leaf: ??4CTavernGameShoveHaPennyDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EAD07, 279 bytes, size-optimised def TU (/Os def region, like the landed
//         0x004E2C52 CFishingDef::operator=).
// Identity: reached through the 5-byte `jmp` thunk 0x004EAD02 in slot 19 of vtable 0x01242524 = CTavernGameShoveHaPennyDef
//   (rtti-vtables.tsv; landed as CTavernGameShoveHaPennyDef::Copy). The catalog label "CTavernGameDef::operator=" is the
//   base assignment's name propagated onto the derived one.
// Callees: CTavernGameDef::operator= @ 0x004EA6DD (copies the CDefClassBase part via 0x00431F10, then the text ids from
//   +0x28); vector<CShoveHaPennyRegionDef>::operator= @ 0x004EAE1E (0x30-byte elements).
// Layout (PDB CTavernGameShoveHaPennyDef 0x208): retail CTavernGameDef is 0x1A4 (PDB 0x1A8 - 4, def head 0x3C), Regions
//   +0x1A4 (12-byte STLport vector; PDB +0x1A8 at 16 bytes), so BoardDefIndex .. Rounds sit 8 below the PDB at +0x1B0..+0x1FC.
// Shape: the implicit memberwise assignment written out; floats move through eax like dwords. The generated engine
//   header flattens the bases, so the classes are modelled locally.
#pragma optimize("s", on)

struct CTavernGameDef {
    unsigned char _body[0x1a4];                  // CDefClassBase head + the tavern-game text ids
    CTavernGameDef& operator=(const CTavernGameDef& other);   // 0x004EA6DD
};
struct CShoveHaPennyRegionDefVector {            // retail std::vector<CShoveHaPennyRegionDef> (12 bytes)
    unsigned long m_words[3];
    CShoveHaPennyRegionDefVector& operator=(const CShoveHaPennyRegionDefVector& other);   // 0x004EAE1E
};

struct CTavernGameShoveHaPennyDef : CTavernGameDef {
    CShoveHaPennyRegionDefVector Regions;        // +0x1a4
    long  BoardDefIndex;                         // +0x1b0
    long  ArrowDefIndex;                         // +0x1b4
    float PositionCoinSpeedXbox;                 // +0x1b8
    float PositionCoinSpeedPC;                   // +0x1bc
    float PositionCoinMinimum;                   // +0x1c0
    float PositionCoinMaximum;                   // +0x1c4
    float PositionCoinDepth;                     // +0x1c8
    float ImpulseScale;                          // +0x1cc
    float TargetDistanceScale;                   // +0x1d0
    float JoystickScaleXboxX;                    // +0x1d4
    float JoystickScaleXboxY;                    // +0x1d8
    float JoystickScalePCX;                      // +0x1dc
    float JoystickScalePCY;                      // +0x1e0
    float JoystickTranslateY;                    // +0x1e4
    float JoystickMagBelow;                      // +0x1e8
    float AverageJoystickMagAbove;               // +0x1ec
    float AverageDrift;                          // +0x1f0
    float MaxPower;                              // +0x1f4
    float RestSpeed;                             // +0x1f8
    long  Rounds;                                // +0x1fc
    CTavernGameShoveHaPennyDef& operator=(const CTavernGameShoveHaPennyDef& other);
};

CTavernGameShoveHaPennyDef& CTavernGameShoveHaPennyDef::operator=(const CTavernGameShoveHaPennyDef& other)
{
    CTavernGameDef::operator=(other);
    Regions = other.Regions;
    BoardDefIndex = other.BoardDefIndex;
    ArrowDefIndex = other.ArrowDefIndex;
    PositionCoinSpeedXbox = other.PositionCoinSpeedXbox;
    PositionCoinSpeedPC = other.PositionCoinSpeedPC;
    PositionCoinMinimum = other.PositionCoinMinimum;
    PositionCoinMaximum = other.PositionCoinMaximum;
    PositionCoinDepth = other.PositionCoinDepth;
    ImpulseScale = other.ImpulseScale;
    TargetDistanceScale = other.TargetDistanceScale;
    JoystickScaleXboxX = other.JoystickScaleXboxX;
    JoystickScaleXboxY = other.JoystickScaleXboxY;
    JoystickScalePCX = other.JoystickScalePCX;
    JoystickScalePCY = other.JoystickScalePCY;
    JoystickTranslateY = other.JoystickTranslateY;
    JoystickMagBelow = other.JoystickMagBelow;
    AverageJoystickMagAbove = other.AverageJoystickMagAbove;
    AverageDrift = other.AverageDrift;
    MaxPower = other.MaxPower;
    RestSpeed = other.RestSpeed;
    Rounds = other.Rounds;
    return *this;
}