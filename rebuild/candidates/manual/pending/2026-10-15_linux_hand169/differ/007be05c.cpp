// CTCShotDefinitionDrainLife::SetSoulEffect(long effectType) -- create the soul effect thing at the shot's position; when it is alive keep it in PSoulEffect and attach its particle emitter to the shot's "weapon_pos_a" helper
// Leaf: SetSoulEffect
// Grade: DIFFER(154v157) -- frame slots only: retail homes the created effect thing in the dead argument slot [ebp+8] and the emitter out pointer at [ebp-4] (string [ebp-0Ch], temporary pointer [ebp-8]); ours swaps effect/emitter ([ebp-4] / [ebp+8]); named CCharString local and nested blocks tried; next: lane gap (dead-argument-slot homing of a named local)
// Retail: Fable.exe 0x007BE05C, 155 bytes (`ret 4`; size TU with frame: optimize("y", off) + ("s", on)).
// Callees: CThing::GetPosition @ 0x004C73D0, the __fastcall effect factory Create(long, const C3DVector*, bool) @ 0x006E0880,
// IS_THING_ALIVE<CThing*> @ 0x0040DCE0 (out-of-line `__fastcall`, pointer by reference), CIntelligentPointer<CThing>
// (CThing*) ctor @ 0x004A96F0 (manifest "CIntelligentPointer<CThing const>", byte twin), CIntelligentPointer::operator=
// @ 0x004A9760 (manifest "<CThingCreatureBase>", byte twin), ~CBaseIntelligentPointer @ 0x00A01C10,
// CThing::GetTCDriver<CTCDParticleEmitter>(CTCDParticleEmitter**) @ 0x004ACB50 (manifest "<CTCDExperienceOrb>", byte
// twin), CCharString(const char*, long = -1) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) @ 0x006E0BE0.
// Literal "weapon_pos_a" @ 0x0124374C. Same family as the landed CFirePoint::SetAsBurning 0x00780540.
// Layout: retail CTCShotDefinitionDrainLife: PSoulEffect (CIntelligentPointer<CThing>) +0xAC (PDB 0xB0).
// Shape: Create's bool is pushed before the position call; the created thing is homed in the dead argument slot for
// IS_THING_ALIVE; the pointer assignment goes through an out-of-line ctor/operator= temporary.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector { float x, y, z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CThing;
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CThing {
    const C3DVector& GetPosition();                     // 0x004C73D0
    bool GetTCDriver(CTCDParticleEmitter** out);        // 0x004ACB50
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
bool __fastcall IS_THING_ALIVE(CThing* const& thing);  // 0x0040DCE0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>
    CIntelligentPointer(CThing* item);                  // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
struct CTCShotDefinitionDrainLife {
    void*               __vftable;                      // +0x00
    CThing*             ParentThing;                    // +0x04 (CTCBase)
    unsigned char       _pad08[0xac - 0x08];
    CIntelligentPointer PSoulEffect;                    // +0xac
    void SetSoulEffect(long effectType);
};

void CTCShotDefinitionDrainLife::SetSoulEffect(long effectType)
{
    if (effectType) {
        CThing* effect = CreateEffectThing(effectType, &ParentThing->GetPosition(), false);
        if (IS_THING_ALIVE(effect)) {
            PSoulEffect = CIntelligentPointer(effect);
            {
                CTCDParticleEmitter* emitter = 0;
                effect->GetTCDriver(&emitter);
                {
                    CCharString helperName("weapon_pos_a");
                    emitter->AttachToThing(ParentThing, 0x3c, helperName, 0, 0.0f);
                }
            }
        }
    }
}
