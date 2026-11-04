// CAmbientCube::UpdateLevel(float) -- advance the ambient cube's ramped level parameter (member at +0xE0, after the eight apexes) by one time step
// Leaf: ?UpdateLevel@CAmbientCube@@QAEXM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C45980, 11 bytes: `add ecx,0E0h ; jmp 0x00C0B7B0` (the float argument left in place).
// Library: libpk ambient sound (no PDB/RTTI): descriptive names. Owner: the CAmbientCube of the landed UpdateDistances 0x00C45810 /
//   SetCentre 0x00C458D0 / GetLevel 0x00C45940 (eight 0x1C-byte apexes = 0xE0 bytes, then the ramped level parameter that GetLevel
//   reads at +0xE0 / +0x110); the only caller 0x00C36430 passes `this` = owner +0x4C, the cube, exactly as the cube forwarders
//   0x00C361F0 / 0x00C36220 do. The catalog's garbled `_Cons_val<...CParticleMorphs...>` label and the caller's binding
//   ?SetScale@CParticleMorphs@@QAEXM@Z are propagated (CParticleMorphs is a Fable PDB class with no such member; NAME_FIXES.tsv PROPOSAL).
// Callees: CRampedParam::Update(float) 0x00C0B7B0 (the spelling the landed CAudioParamBlock::Update 0x00C0AF20 binds: time += dt,
//   compared with the ramp duration).
// Layout: as the landed GetLevel 0x00C45940 (CRampedParam 0x34 bytes, value +0, target +4, angle flag +0x30).
class CRampedParam {                            // 0x34 bytes
public:
    float m_value;                              // +0x00 current value
    float m_target;                             // +0x04 target value
    float m_rates[10];                          // +0x08 rates, time, duration, coefficients
    bool  m_bAngle;                             // +0x30 wrap the value into [-pi, pi)
    void Update(float dt);                      // 0x00C0B7B0
};

struct SAmbientApex {                           // 0x1C bytes: offset, position (two C3DVectors), minimum distance
    float m_offset[3];                          // +0x00
    float m_position[3];                        // +0x0c
    float m_minDistance;                        // +0x18
};

class CAmbientCube {
public:
    SAmbientApex m_apexes[8];                   // +0x00
    CRampedParam m_level;                       // +0xe0
    void UpdateLevel(float dt);
};

void CAmbientCube::UpdateLevel(float dt)
{
    m_level.Update(dt);
}