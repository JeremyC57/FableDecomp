// CTavernGameCoinGolfDef::operator=(const CTavernGameCoinGolfDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CTavernGameCoinGolfDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1132, 99 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CTavernGameCoinGolfDef::Copy 0x004E112D; the 13 member operations read from
//   these bytes (byte/dword copies) are Ego_r.pdb CTavernGameCoinGolfDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M036, representative 0x0045426C): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CTavernGameCoinGolfDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
    unsigned char _base[0x25];
};
class CTavernGameCoinGolfDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    float                     TargetDistanceScale;               // +0x28
    float                     JoystickScaleXboxX;                // +0x2c
    float                     JoystickScaleXboxY;                // +0x30
    float                     JoystickScaleX;                    // +0x34
    float                     JoystickScaleY;                    // +0x38
    float                     JoystickMagBelow;                  // +0x3c
    float                     AverageJoystickMagAbove;           // +0x40
    float                     AverageDrift;                      // +0x44
    float                     MaxPower;                          // +0x48
    float                     RestSpeed;                         // +0x4c
    long                      Courses;                           // +0x50
    float                     GoalRadius;                        // +0x54
    long                      StartingCourse;                    // +0x58
};
void CTavernGameCoinGolfDef::Copy(const CDefClassBase* other)
{
    *this = *(const CTavernGameCoinGolfDef*)other;
}