// Grade: DIFFER(741v744) -- flow/calls match; this edi v ebp and def ebp v esi register swap, missing fxch before the stage-count product, stage-effect base loaded before GetPosition in retail, loop alignment jmp on the other loop
// CTCTurncoat::FrameUpdate() -- for a turncoatable creature: while fully turncoated, count down the turncoat time and on expiry play the deactivate effect, reset the turncoatedness, kill the turncoated effect and play the release sound; otherwise let the turncoatedness recover and grow or shrink the stage effects to match it
// Leaf: ?FrameUpdate@CTCTurncoat
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C7C40, 744 bytes (+ int3 padding), plain `ret`, 8-aligned frame.
// Callees: CThing::GetPosition() const 0x004C73D0; CreateParticleEffect(long, const C3DVector*) 0x0072AB70 (`__fastcall`);
//   IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 (`__fastcall`, out of line); CThing::Kill(bool) 0x004C9B80;
//   LowerBound 0x0040F020 (interfaces 0x10 sound player, 0x4B particle emitter); CTCSoundPlayer::PlaySound 0x0066BD00;
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0; CBaseIntelligentPointer() 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem
//   0x00A01B90 / ~ 0x00A01C10; vector<CIntelligentPointer<CThing> >::push_back 0x004AD520; __ftol2 0x00BFEA70.
//   CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout (retail CTCTurncoat = Ego_r.pdb, the stage-effect vector 12 bytes): see ModifyTurncoatedness 0x007C7A00;
//   CTurncoatDef Turncoatable +0x25, TurncoatDeactivateEffect +0x30, TurncoatStageEffects +0x40.
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CBaseObject;
struct CCharString {
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
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
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
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector* pos);   // 0x0072AB70

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10 (slot 0: scalar deleting)
    void* m_pData;
    CBaseIntelligentPointer();                          // 0x00A01B10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // vtable 0x01238C6C
    CIntelligentPointer(CThing* item) { SetPItem((CBaseObject*)item); }
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer* p);   // 0x004CC340
inline bool IsThingAlive(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CEffectVector {                                  // vector<CIntelligentPointer<CThing> > (12 bytes)
    CIntelligentPointer* _M_start;
    CIntelligentPointer* _M_finish;
    CIntelligentPointer* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    void push_back(const CIntelligentPointer& item);    // 0x004AD520
    void pop_back() { --_M_finish; _M_finish->~CIntelligentPointer(); }
};
struct CTCDParticleEmitter {                            // interface 0x4B
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CTCSoundPlayer {                                 // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    const long& operator[](long i) const { return _M_start[i]; }
};
struct CTurncoatDef {
    unsigned char _pad0[0x25];
    bool Turncoatable;                                  // +0x25
    unsigned char _pad26[0x30 - 0x26];
    long TurncoatDeactivateEffect;                      // +0x30
    unsigned char _pad34[0x40 - 0x34];
    CLongVector TurncoatStageEffects;                   // +0x40
};
struct CDefPointer {
    CTurncoatDef* Object;
    CTurncoatDef* operator->() const { return Object; }
};

struct CTCTurncoat {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
    long WillTurncoatTimer;                             // +0x0c
    float CurrentTurncoatedness;                        // +0x10
    CDefPointer PDef;                                   // +0x14
    CIntelligentPointer TurncoatedEffect;               // +0x18
    long TurncoatTimeLast;                              // +0x20
    CIntelligentPointer TurncoatOriginator;             // +0x24
    bool TurncoatOverride;                              // +0x2c
    long TimeDelayBeforeResistanceRecoveryFrames;       // +0x30
    long CurrentRecoveryTimer;                          // +0x34
    float ResistanceRecoveryPerFrame;                   // +0x38
    CEffectVector TurncoatStageEffects;                 // +0x3c
    float CreatureTurncoatResistance;                   // +0x48
    void FrameUpdate();
};

void CTCTurncoat::FrameUpdate()
{
    if (!TurncoatOverride || !PDef->Turncoatable)
        return;
    CThing* parent = ParentThing;
    if (parent && !parent->IsDead() && CurrentTurncoatedness >= CreatureTurncoatResistance) {
        if (--WillTurncoatTimer > 0)
            return;
        CurrentRecoveryTimer = 0;
        WillTurncoatTimer = TurncoatTimeLast;
        if (PDef->TurncoatDeactivateEffect)
            CreateParticleEffect(PDef->TurncoatDeactivateEffect, &parent->GetPosition());
        CurrentTurncoatedness = 0.0f;
        if (IS_THING_ALIVE(&TurncoatedEffect))
            TurncoatedEffect.GetPItem()->Kill(true);
        CTCSoundPlayer* sound;
        if (ParentThing->GetInterface(&sound, 0x10))
            sound->PlaySound("HERO_ABILITY_TURNCOAT_RELEASE", 1, 0, 0, -1);
        return;
    }
    if (CurrentRecoveryTimer == 0) {
        CurrentTurncoatedness -= ResistanceRecoveryPerFrame;
        if (CurrentTurncoatedness < 0.0f)
            CurrentTurncoatedness = 0.0f;
    } else
        --CurrentRecoveryTimer;
    float numStages = (float)PDef->TurncoatStageEffects.size();
    float fraction = CurrentTurncoatedness / CreatureTurncoatResistance / (1.0f - 1.0f / numStages);
    long current = TurncoatStageEffects.size();
    if (fraction < 0.0f)
        fraction = 0.0f;
    else if (fraction > 1.0f)
        fraction = 1.0f;
    long wanted = (long)(fraction * numStages);
    if (wanted > current) {
        for (long i = current; i < wanted; ++i) {
            CThing* effect = CreateParticleEffect(PDef->TurncoatStageEffects[i], &parent->GetPosition());
            if (effect && !effect->IsDead()) {
                CTCDParticleEmitter* emitter;
                effect->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(parent, 8, "", 0, 0.0f);
            }
            TurncoatStageEffects.push_back(CIntelligentPointer(effect));
        }
    } else if (wanted < current) {
        for (long n = current - wanted; n > 0; --n) {
            CIntelligentPointer* last = TurncoatStageEffects._M_finish - 1;
            CThing* thing = last->GetPItem();
            if (thing && !thing->IsDead())
                last->GetPItem()->Kill(true);
            TurncoatStageEffects.pop_back();
        }
    }
}
