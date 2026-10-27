// COracleMinigameDef::operator=(const COracleMinigameDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4COracleMinigameDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E197A, 99 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of COracleMinigameDef::Copy 0x004E1975; the 13 member operations read from
//   these bytes (byte/dword copies) are Ego_r.pdb COracleMinigameDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M036, representative 0x0045426C): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const COracleMinigameDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
    unsigned char _base[0x25];
};
class COracleMinigameDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    float                     WaitBetweenRoundsSeconds;          // +0x28
    float                     PauseBetweenOutputSeconds;         // +0x2c
    float                     PauseBetweenOutputsLevelMultiplier; // +0x30
    long                      MaxLevel;                          // +0x34
    unsigned long             TintRed;                           // +0x38
    unsigned long             TintGreen;                         // +0x3c
    unsigned long             TintBlue;                          // +0x40
    unsigned long             TextLose;                          // +0x44
    unsigned long             TextWin;                           // +0x48
    unsigned long             TextWatch;                         // +0x4c
    unsigned long             TextRepeat;                        // +0x50
    unsigned long             TextOfferGame;                     // +0x54
    unsigned long             TextWonRound;                      // +0x58
};
void COracleMinigameDef::Copy(const CDefClassBase* other)
{
    *this = *(const COracleMinigameDef*)other;
}