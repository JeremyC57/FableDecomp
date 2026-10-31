// CGetBestAttackerForProng::operator()(CThing* candidate, const CProngInfo& info) -- score a candidate attacker for a combat-ring prong (alignment of its offset with the prong direction plus its weapon preference) and remember the best one
// Grade: DIFFER(370v375) 53 difflines: offset scaled straight off the GetProngDirection return temp (distance * dir), candidate position copied by value, one target GetPosition for the flat delta; residue offset components computed Z-first (ctor args right-to-left; retail X-first), prong pointer not cached in ebp, frame 0x28 v 0x2c (retry wave 2 sec2b)
// Was: DIFFER(376v375) -- first-pass model: frame 0x34 vs 0x2C and register roles differ (retail holds the prong pointer in ebp and the candidate in edi, keeps the prong key in a frame word, builds the offset from the by-value direction); the radius fallback and range test already line up
// Leaf: ??R
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F2DE0, length 373 bytes (`ret 8`).
// Callees: CThingCreatureBase::GetCombatTypeDef @ 0x00666D10, the ring's prong distance @ 0x006E8850 and prong direction
//   (C3DVector by value) @ 0x006E7FD0 (manifest labels propagated), CThing::GetPosition @ 0x004C73D0, the normalised
//   alignment helper @ 0x006E7F00 (`__fastcall`), GetAttackerProngSelectionCurrentWeaponBaseCompareValue(CThing*, bool)
//   @ 0x006E9F60 (`__fastcall`); constants 0.0001f @ 0x0129BA3C, 0.5f @ 0x0122F59C.
// Layout: the functor holds the prong (+0: {direction key, ..., index +8}), the ring (+4), the ring geometry (+8), the
//   target (+0xC), the best score (+0x14) and the best attacker (+0x18); CCombatTypeDef radius +0x80, sizes +0x6C/+0x70,
//   prong count +0x48; the info's override flag +0xC and offset +0x14/+0x18.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
inline C3DVector operator*(float f, const C3DVector& v) { return C3DVector(f * v.X, f * v.Y, f * v.Z); }
struct CCombatTypeDef {
    unsigned char _pad00[0x48];
    long          ProngCount;                           // +0x48
    unsigned char _pad4c[0x20];
    float         SizeA;                                // +0x6C
    float         SizeB;                                // +0x70
    unsigned char _pad74[0xc];
    float         Radius;                               // +0x80
};
struct CThingPhysics_Retail { unsigned char _pad[0x34]; float Radius; };
struct CThing {
    unsigned char         _pad00[0x60];
    CThingPhysics_Retail* m_pPhysics;                   // +0x60
    const CCombatTypeDef* GetCombatTypeDef();           // 0x00666D10
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct SProng { long DirectionKey; long _pad04; long Index; };
struct CCombatRing { float GetProngDistance(long index); };              // 0x006E8850
struct CCombatRingGeometry { C3DVector GetProngDirection(long key); };    // 0x006E7FD0
struct CProngInfo {
    unsigned char _pad00[0xc];
    bool          UseOffset;                            // +0x0C
    unsigned char _pad0d[7];
    float         OffsetX;                              // +0x14
    float         OffsetY;                              // +0x18
};
float __fastcall GetAlignment(const C3DVector& from, const C3DVector& to, const C3DVector& offset, const C3DVector& delta);   // 0x006E7F00
float __fastcall GetAttackerProngSelectionCurrentWeaponBaseCompareValue(CThing* attacker, bool atRange);    // 0x006E9F60
struct CGetBestAttackerForProng {
    SProng*              Prong;                         // +0x00
    CCombatRing*         Ring;                          // +0x04
    CCombatRingGeometry* Geometry;                      // +0x08
    CThing*              Target;                        // +0x0C
    long                 _pad10;
    float                BestScore;                     // +0x14
    CThing*              Best;                          // +0x18
    void operator()(CThing* candidate, const CProngInfo& info);
};

void CGetBestAttackerForProng::operator()(CThing* candidate, const CProngInfo& info)
{
    float distance = Target->m_pPhysics->Radius;
    bool atRange = false;
    const CCombatTypeDef* def = candidate->GetCombatTypeDef();
    float radius = def->Radius;
    if (radius < 0.0001f)
        radius = (def->SizeB + def->SizeA) * 0.5f;
    distance += radius;
    if (Prong->Index >= Target->GetCombatTypeDef()->ProngCount) {
        atRange = true;
        distance = Ring->GetProngDistance(Prong->Index);
    }
    C3DVector offset = distance * Geometry->GetProngDirection(Prong->DirectionKey);
    Target->GetPosition();
    C3DVector candidatePosition = candidate->GetPosition();
    const C3DVector& targetPosition = Target->GetPosition();
    C3DVector delta(candidatePosition.X - targetPosition.X, candidatePosition.Y - targetPosition.Y, 0.0f);
    if (info.UseOffset)
        delta = C3DVector(info.OffsetX, info.OffsetY, 0.0f);
    float score = GetAlignment(Target->GetPosition(), candidate->GetPosition(), offset, delta)
                + GetAttackerProngSelectionCurrentWeaponBaseCompareValue(candidate, atRange);
    if (score > BestScore) {
        Best = candidate;
        BestScore = score;
    }
}
