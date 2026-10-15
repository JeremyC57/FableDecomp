// Grade: DIFFER(74v72) -- PDef (+0x0c) is kept in ebx (push/pop ebx) instead of reusing esi once this is dead; rest identical
// CTCDExperienceOrb::CombineWithExperienceOrb(CThing* otherOrb) -- absorb the other orb's points, play the combining effect, kill it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077A130, length 72 bytes (+8 int3).
// Callees: CTCDExperienceOrb::SetExperiencePoints(long) @ 0x00779E10, CThing::GetPosition @ 0x004C73D0, the __fastcall
// thing creator @ 0x006E0880 (def index in ecx, position in edx, one stack argument), CThing::Kill(bool) @ 0x004C9B80.
// Shape: the other thing's orb driver is the inlined GetTCDriver probe: bitset word +0x2c (CThing UsingComponents +0x20,
// word 3) bit 9 selects PTCDriver (+0x5c, retail offset) else null, read without a null test (`->ExperiencePoints`, +0x10).
// The effect is CExperienceDef::OrbCombiningParticleEffect (def +0x28) created at the parent thing's position.
struct C3DVector { float X, Y, Z; };

struct CTCDExperienceOrb;

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];       // +0x20 bitset<274>
    unsigned char _pad44[0x5c - 0x44];
    void* PTCDriver;                        // +0x5c
    const C3DVector& GetPosition() const;   // 0x004C73D0
    void Kill(bool checkLocked);            // 0x004C9B80
    CTCDExperienceOrb* GetTCDriverExperienceOrb() const
    {
        CTCDExperienceOrb* driver = 0;
        if (UsingComponents[3] & 0x200)
            driver = (CTCDExperienceOrb*)PTCDriver;
        return driver;
    }
};

CThing* __fastcall CreateThing(long defIndex, const C3DVector& position, long flags);   // 0x006E0880

struct CExperienceDef {
    unsigned char _pad00[0x28];
    long OrbCombiningParticleEffect;        // +0x28
};

template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};

struct CTCDExperienceOrb {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CDefPointer<CExperienceDef> PDef;       // +0x0c
    long ExperiencePoints;                  // +0x10
    long CombinationImmunityTimer;          // +0x14
    long LifeTimer;                         // +0x18
    bool DecayOverTime;                     // +0x1c
    bool PickedUp;                          // +0x1d
    void SetExperiencePoints(long points);  // 0x00779E10
    void CombineWithExperienceOrb(CThing* otherOrb);
};

void CTCDExperienceOrb::CombineWithExperienceOrb(CThing* otherOrb)
{
    SetExperiencePoints(otherOrb->GetTCDriverExperienceOrb()->ExperiencePoints + ExperiencePoints);
    CreateThing(PDef->OrbCombiningParticleEffect, ParentThing->GetPosition(), 0);
    otherOrb->Kill(true);
}
