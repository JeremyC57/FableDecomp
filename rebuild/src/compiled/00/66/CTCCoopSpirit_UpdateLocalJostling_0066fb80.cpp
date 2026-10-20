// CTCCoopSpirit::UpdateLocalJostling() -- for every other live player hero (not the main player, not the spirit's own player) closer than half a unit, push the spirit away from it with an acceleration of 0.4 times the offset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066FB80, 261 bytes (+ int3 padding), plain `ret`.
// Callees: CTCBase::GetPlayerManager @ 0x00686D40 (x2; manifest "CViewBase::GetDisplayEngine" is propagated),
//   CPlayerManager::GetMainPlayerNumber @ 0x00449700, CPlayerManager::GetPlayer(long) @ 0x004498C0, CPlayer::GetHeroThing
//   @ 0x00487DC0, CThing::GetPosition @ 0x004C73D0 (x2), C3DVector::GetLength @ 0x00A14480 (manifest "GetScaled" is
//   propagated), CTCPhysicsBase::ApplyAcceleration(const C3DVector&) @ 0x006B0210 (the spirit's physics TC at +0x60).
// Globals: the 1e-4f tolerance @ 0x0129BA3C (an extern float variable), 0.5f @ 0x0122F59C, 0.4f @ 0x0125A318.
// Layout: retail CThing player number (signed char) +0x90, Dead bit 0 of +0x91, PhysicsTC +0x60.
// Shape: a counted for loop over the four player slots (ebx); the offset is a named C3DVector built by `spirit - hero`
//   (the ctor arguments evaluated right to left: Z first, stored X/Y/Z); the length a named float kept on the FPU;
//   `len > eps` (`fcom ; test ah,41h ; jne` to a pop) then `len < 0.5` (`fcomp ; test ah,5 ; jp`); the push is a named
//   `offset * 0.4f` vector; the spirit thing is a named local for the subtraction (held in ebp across the hero call;
//   reading ParentThing inline gives up ebp and moves the loop counter into it), ParentThing is re-read for the physics TC.
extern float g_epsilon_0129ba3c;                // 0x0129BA3C (1e-4f)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    float GetLength() const;                    // 0x00A14480
};
struct CTCPhysicsBase {
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char   _pad64[0x90 - 0x64];
    signed char     PlayerNumber;               // +0x90
    unsigned char   Flags91;                    // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CPlayer {
public:
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    long GetMainPlayerNumber();                 // 0x00449700
    CPlayer* GetPlayer(long number);            // 0x004498C0
};
struct CTCCoopSpirit {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    CPlayerManager* GetPlayerManager();         // 0x00686D40 (CTCBase)
    void UpdateLocalJostling();
};

void CTCCoopSpirit::UpdateLocalJostling()
{
    for (long i = 0; i < 4; ++i) {
        if (GetPlayerManager()->GetMainPlayerNumber() == i)
            continue;
        if (ParentThing->PlayerNumber == i)
            continue;
        CThing* hero = GetPlayerManager()->GetPlayer(i)->GetHeroThing();
        if (!hero || hero->IsDead())
            continue;
        CThing* spirit = ParentThing;
        C3DVector offset = spirit->GetPosition() - hero->GetPosition();
        float length = offset.GetLength();
        if (length > g_epsilon_0129ba3c && length < 0.5f) {
            C3DVector push = offset * 0.4f;
            ParentThing->PhysicsTC->ApplyAcceleration(push);
        }
    }
}