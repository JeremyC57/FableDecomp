// CTCAssassinRush::FrameUpdate() -- per frame: on release finish the current action, play the cast sound and start the trail; while the trail delay runs keep the radial blur centred above the rusher, otherwise rush on; end the slow-down, and once the trail delay is over leave the exit modes, kill the trails and fade the blur; remove the rush component when everything has run out, and cancel when scheduled
// Leaf: FrameUpdate
// Retail 0x007AB190, 620 bytes, `ret`. Callees: CThingCreatureBase::FinishCurrentAction 0x00662650, LowerBound
// 0x0040F020, CCharString 0x0099EBF0 / dtor 0x0099EAE0, CTCSound::PlaySound 0x0066BD00, CreateTrailingEffect
// 0x007A9FE0, CThing::GetPosition 0x004C73D0, CTCBase::GetWorld 0x00686A70, the spell-effect interpolator's
// RadialBlurSetCenterWorldPos 0x006B8310 and fade-out 0x006B82E0 (world +0x18 -> +0x1964), RushForwards 0x007AAC50,
// RemoveBulletTimeSlowdown 0x004C6140 (world +0x68), SetSuspendAnimation 0x00833A10, CIntelligentPointer::GetPItem
// 0x00A01B50, CThing::Kill 0x004C9B80, CThing::RemoveTC 0x004C9840, CancelMove 0x007AA480.
// Layout: Ego_r.pdb CTCAssassinRush less 4 past the 12-byte retail vector: PDef +0x14, AssassinTrail +0x18,
// AssassinDisplacementTrail +0x20, TrailDelay +0x28, StartTrailDelay +0x2c, JustReleased +0x42, SlowTimeLength
// +0x44, RadialBlurFadeKey +0x98, ScheduleForCancel +0x9e, RushDelayFrameCount +0xa0, CompletedExitModes +0xa4;
// retail def blur height +0x44, blur fade time +0x4c; CThing state byte +0x10 (1 = player), interface 0xA7's mode +0x10.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char State;                                // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
    void RemoveTC(long interfaceType);                  // 0x004C9840
    void FinishCurrentAction();                         // 0x00662650
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_stateFlags & 1))
            return true;
    }
    return false;
}
struct CTCSound {                                       // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CTCExitMode {                                    // interface 0xA7
    unsigned char _pad00[0xc];
    bool          Active;                               // +0x0c
    long          Mode;                                 // +0x10
};
struct CEnvironmentSpellEffects {
    void RadialBlurFadeOut(float time, void* key);      // 0x006B82E0
    void RadialBlurSetCenterWorldPos(void* key, const C3DVector& pos);   // 0x006B8310
};
class CEnvironment { public:
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffects* SpellEffects;             // +0x1964
};
class CBulletTimeManager { public:
    void RemoveBulletTimeSlowdown(unsigned long key);   // 0x004C6140
};
class CWorld { public:
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                         // +0x18
    unsigned char _pad1c[0x68 - 0x1c];
    CBulletTimeManager* PBulletTimeManager;             // +0x68
    void RemoveBulletTimeSlowdown(unsigned long key) { PBulletTimeManager->RemoveBulletTimeSlowdown(key); }
};
struct CAssassinRushDef {
    unsigned char _pad00[0x44];
    float         BlurHeight;                           // +0x44
    unsigned char _pad48[0x4c - 0x48];
    float         BlurFadeTime;                         // +0x4c
};
class CTCAssassinRush { public:
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[0x14 - 0x08];
    CAssassinRushDef*       PDef;                       // +0x14
    CBaseIntelligentPointer AssassinTrail;              // +0x18
    CBaseIntelligentPointer AssassinDisplacementTrail;  // +0x20
    long                    TrailDelay;                 // +0x28
    bool                    StartTrailDelay;            // +0x2c
    unsigned char           _pad2d[0x42 - 0x2d];
    bool                    JustReleased;               // +0x42
    long                    SlowTimeLength;             // +0x44
    unsigned char           _pad48[0x98 - 0x48];
    void*                   RadialBlurFadeKey;          // +0x98
    bool                    UseCollision;               // +0x9c
    bool                    IsZTargetting;              // +0x9d
    bool                    ScheduleForCancel;          // +0x9e
    bool                    FakeCollideWithSomething;   // +0x9f
    long                    RushDelayFrameCount;        // +0xa0
    bool                    CompletedExitModes;         // +0xa4
    CWorld* GetWorld();                                 // 0x00686A70
    void CreateTrailingEffect();                        // 0x007A9FE0
    void RushForwards();                                // 0x007AAC50
    void CancelMove();                                  // 0x007AA480
    void FrameUpdate();
};

void CTCAssassinRush::FrameUpdate()
{
    CThing* parent = ParentThing;
    if (JustReleased) {
        parent->FinishCurrentAction();
        CTCSound* sound;
        if (ParentThing->GetInterface(&sound, 0x10))
            sound->PlaySound("HERO_ABILITY_ASSASSIN_RUSH_CAST", 1, 0, 0, -1);
        JustReleased = false;
        CreateTrailingEffect();
    } else if (StartTrailDelay) {
        --TrailDelay;
        C3DVector pos = parent->GetPosition();
        CWorld* world = GetWorld();
        world->PEnvironment->SpellEffects->RadialBlurSetCenterWorldPos(RadialBlurFadeKey,
            C3DVector(pos.X, pos.Y, pos.Z + PDef->BlurHeight));
    } else {
        RushForwards();
    }
    if (SlowTimeLength > 0) {
        if (--SlowTimeLength <= 0)
            GetWorld()->RemoveBulletTimeSlowdown((unsigned long)this);
    }
    if (TrailDelay <= 0) {
        if (!CompletedExitModes) {
            CompletedExitModes = true;
            CTCExitMode* exitMode;
            if (parent->GetInterface(&exitMode, 0xa7)) {
                if (exitMode->Mode == 13) {
                    exitMode->Active = false;
                    exitMode->Mode = 0;
                }
            }
            if (parent->State == 1)
                ParentThing->SetSuspendAnimation(false, 0);
            else
                GetWorld()->RemoveBulletTimeSlowdown((unsigned long)this);
        }
        --RushDelayFrameCount;
        if (IS_THING_ALIVE(AssassinTrail))
            AssassinTrail.GetPItem()->Kill(true);
        if (IS_THING_ALIVE(AssassinDisplacementTrail))
            AssassinDisplacementTrail.GetPItem()->Kill(true);
        if (RadialBlurFadeKey) {
            CAssassinRushDef* def = PDef;
            CWorld* world = GetWorld();
            world->PEnvironment->SpellEffects->RadialBlurFadeOut(def->BlurFadeTime, RadialBlurFadeKey);
            RadialBlurFadeKey = 0;
        }
    }
    bool finished = parent->State == 1 || RushDelayFrameCount <= 0;
    if (SlowTimeLength <= 0 && TrailDelay <= 0 && finished)
        parent->RemoveTC(0xa9);
    if (ScheduleForCancel)
        CancelMove();
}