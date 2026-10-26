// CTCStealth::UpdateHearability() -- unless sneaking (or without a stealth def), a moving owner can be heard from the def's base distance scaled by its pace (running / walking / creeping multiplier) and the hearability modifiers; a SIMPLE_NOISE of that radius is made at the owner and handed the owner
// Leaf: UpdateHearability
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007479B0, 331 bytes (plain `ret`).
// Callees: CThing::GetPosition 0x004C73D0 (x3); the owner's virtual speed getters slot 80 (+0x140, running) and slot 79
//          (+0x13c, walking), each taking the position; CTCStealth::GetHearabilityModifier 0x007475C0 (name descriptive; manifest
//          "CTCHearabilityManager::GetSoundRadiusWithModifiers"); CCharString(const char*, long) 0x0099EBF0 ("SIMPLE_NOISE"
//          0x01268A5C) / ~CCharString 0x0099EAE0; CTCBase::GetDefinitions 0x006869D0; CDefinitionManager::
//          GetDefGlobalIndexFromName 0x009AD410; CThingNoise::Create(long, const C3DVector&, float) 0x008385B0 (__fastcall);
//          CThingNoise::PassToNoiseComponent(CThing*) 0x008386E0. Global: g_epsilon 1e-4f 0x0129BA3C.
// Layout: Ego_r.pdb CTCStealth (retail = PDB here): ParentThing +0x04, Sneaking +0x0f, DistanceCanBeHeardFrom +0x14, PDef +0x1c;
//          CStealthDef (descriptive): base hearing distance +0x28, running / walking / creeping multipliers +0x2c/+0x30/+0x34;
//          retail CThing PhysicsTC +0x60 (Position +0x0c, OldPosition +0x18), dead flag +0x91 bit 0.
// Shape: the frame movement's squared length is summed Z, Y, X (statement order, as the landed UpdateEffects 0x007C2480); each
//          pace limit is a named float (`r * r` from the x87 stack); the noise def name is an unnamed temporary.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
extern const float g_epsilon_0129ba3c;          // 0x0129BA3C (1e-4f)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CTCPhysicsBase { public:
    unsigned char _pad00[0x0c];
    C3DVector     Position;                     // +0x0c
    C3DVector     OldPosition;                  // +0x18
};
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6)
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78();
    virtual float GetWalkSpeed(const C3DVector& pos);   // slot 79 (+0x13c)
    virtual float GetRunSpeed(const C3DVector& pos);    // slot 80 (+0x140)
    unsigned char _pad04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CThingNoise : public CThing { public:
    static CThingNoise* __fastcall Create(long defIndex, const C3DVector& pos, float radius);   // 0x008385B0
    void PassToNoiseComponent(CThing* thing);   // 0x008386E0
};
class CStealthDef { public:
    unsigned char _pad00[0x28];
    float BaseHearingDistance;                  // +0x28
    float RunningMultiplier;                    // +0x2c
    float WalkingMultiplier;                    // +0x30
    float CreepingMultiplier;                   // +0x34
};
class CTCStealth { public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[4];
    bool Viewable;                              // +0x0c
    bool Smellable;                             // +0x0d
    bool Hearable;                              // +0x0e
    bool Sneaking;                              // +0x0f
    bool SneakingToggleSet;                     // +0x10
    float DistanceCanBeHeardFrom;               // +0x14
    float MaxSneakingSpeed;                     // +0x18
    CStealthDef* PDef;                          // +0x1c CDefPointer<CStealthDef const>
    CDefinitionManager* GetDefinitions() const; // 0x006869D0 (CTCBase)
    float GetHearabilityModifier();             // 0x007475C0
    void UpdateHearability();
};

void CTCStealth::UpdateHearability()
{
    DistanceCanBeHeardFrom = 0.0f;
    if (Sneaking || !PDef)
        return;
    CThing* owner = ParentThing;
    CTCPhysicsBase* physics = owner->PhysicsTC;
    C3DVector movement(physics->Position.X - physics->OldPosition.X, physics->Position.Y - physics->OldPosition.Y,
                       physics->Position.Z - physics->OldPosition.Z);
    float speedSquared = movement.Z * movement.Z;
    speedSquared += movement.Y * movement.Y;
    speedSquared += movement.X * movement.X;
    if (speedSquared > g_epsilon_0129ba3c) {
        DistanceCanBeHeardFrom = PDef->BaseHearingDistance;
        float multiplier;
        float runSpeed = owner->GetRunSpeed(owner->GetPosition()) + g_epsilon_0129ba3c;
        if (speedSquared > runSpeed * runSpeed)
            multiplier = PDef->RunningMultiplier;
        else {
            float walkSpeed = owner->GetWalkSpeed(owner->GetPosition()) + g_epsilon_0129ba3c;
            if (speedSquared > walkSpeed * walkSpeed)
                multiplier = PDef->WalkingMultiplier;
            else
                multiplier = PDef->CreepingMultiplier;
        }
        DistanceCanBeHeardFrom *= multiplier;
        DistanceCanBeHeardFrom *= GetHearabilityModifier();
        long defIndex;
        {
            CCharString noiseName("SIMPLE_NOISE");
            defIndex = GetDefinitions()->GetDefGlobalIndexFromName(noiseName);
        }
        CThingNoise* noise = CThingNoise::Create(defIndex, ParentThing->GetPosition(), DistanceCanBeHeardFrom);
        if (noise && !(noise->m_flags91 & 1))
            noise->PassToNoiseComponent(ParentThing);
    }
}