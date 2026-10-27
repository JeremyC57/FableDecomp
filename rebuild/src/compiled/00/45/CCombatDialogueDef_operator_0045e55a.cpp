// CCombatDialogueDef::operator=(const CCombatDialogueDef& other) -- the implicit copy assignment: CParentDefClassBase's, then every member in order
// Leaf: ??4CCombatDialogueDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0045E55A, 159 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CCombatDialogueDef::Copy 0x0045E555 (catalogued CCombatDialogueDef::Copy); the member
//   copies (dword / byte at each offset) are Ego_r.pdb CCombatDialogueDef's members in order (20 members from +0x3c).
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0.
// Shape (family M033, representative 0x00453C58): compiler-generated `operator=`, emitted here through Copy's
//   `*this = *(const CCombatDialogueDef*)other;`; `optimize("s", on)` keeps every copy in eax/al (speed rotates eax/ecx/edx).
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
class CParentDefClassBase {
public:
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
    unsigned char _base[0x39];
};
class CCombatDialogueDef : public CParentDefClassBase {
public:
    void Copy(const CParentDefClassBase* other);
    float                 MinTimeBetweenSpeech;           // +0x3c
    float                 MinTimeBetweenSpeechRepeat;     // +0x40
    float                 MinDelayAfterBattleCry;         // +0x44
    long                  EnemySighted;                   // +0x48
    long                  RunForCover;                    // +0x4c
    long                  Hiding;                         // +0x50
    long                  Ambushing;                      // +0x54
    long                  WillSeenUsed;                   // +0x58
    long                  Threat;                         // +0x5c
    long                  Encourage;                      // +0x60
    long                  FoeKnockedDownByColleague;      // +0x64
    long                  MenLosing;                      // +0x68
    long                  EnemyKnockedDown;               // +0x6c
    long                  RangedCombatAiming;             // +0x70
    long                  RangedCombatAimingBlocked;      // +0x74
    long                  Injured;                        // +0x78
    long                  ReactionToDecapitation;         // +0x7c
    long                  ColleagueInjured;               // +0x80
    long                  Scared;                         // +0x84
    long                  Flee;                           // +0x88
};
void CCombatDialogueDef::Copy(const CParentDefClassBase* other)
{
    *this = *(const CCombatDialogueDef*)other;
}