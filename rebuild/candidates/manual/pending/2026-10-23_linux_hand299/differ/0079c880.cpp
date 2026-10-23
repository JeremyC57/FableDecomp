// Grade: DIFFER(933v933): frame slots only -- retail homes the parent-thing local at +0x10 and the CIntelligentPointer<CParticleEmitter> temporary at +0x14; ours puts the parent at +0x14 and packs the temporary over the dead effect id at +0x0c
// CTCBulletTime::FrameUpdate() -- while running (not paused): count down; recreate the "SLOWTIME" screen particle effect whenever it has gone; when the count reaches zero play the "END" effect, deactivate and remove the bullet-time component; every frame keep the radial blur centred on the target and drop the target once the blur has faded out
// Leaf: FrameUpdate
// Retail 0x0079C880, 933 bytes, `ret`. Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem
// 0x00A01B90 / dtor 0x00A01C10, CDefClassBase::GetSubDef 0x00669580 and CCRC::CalcAndLookup 0x00669D30 (same-TU out-only
// stand-ins), CCharString 0x0099EBF0 / dtor 0x0099EAE0, GFAYToVY 0x009E1E40 / GFAXToVX 0x009E1E10 (__stdcall),
// CParticleEmitterManager::Get 0x00AEAAF0 / Create2DEmitter 0x00AEB790, CThing::RemoveTC 0x004C9840, CTCBase::GetWorld
// 0x00686A70, CThing::GetPosition 0x004C73D0, RadialBlurSetCenterWorldPos 0x006B8310, the radial-blur state getter
// 0x006B8350 (by hidden pointer; strength +0x24). Globals: screen centre doubles 0x013961F0 / 0x013961E8, 0.5f 0x0122F59C,
// 0.0f 0x0122DEDC; CIntelligentPointer vtables 0x0125A26C (emitter) / 0x01238C6C (thing).
// Layout: CTCBulletTime FramesLeft +0x0c, PDef +0x20, Emitter +0x30, Target +0x44, RadialBlurKey +0x4c, Active +0x54,
// Paused +0x55; CParticleEmitter flag bytes +0x40 (bit 5 finished) / +0x44 (bit 4).
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() : Up(0.0f, 0.0f, 1.0f), Forward(0.0f, 1.0f, 0.0f) {}
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
float __stdcall GFAXToVX(float x);                      // 0x009E1E10
float __stdcall GFAYToVY(float y);                      // 0x009E1E40
extern double g_ScreenCentreX;                          // 0x013961E8
extern double g_ScreenCentreY;                          // 0x013961F0
namespace NParticleEngine {
class CParticleEmitter {
public:
    unsigned char _pad00[0x40];
    unsigned short Flags40 : 5;                         // +0x40
    unsigned short Dead : 1;                            // +0x40 bit 5
    unsigned short Flags40High : 10;
    unsigned char _pad42[2];
    unsigned char Flags44;                              // +0x44 (bit 4: persistent)
    bool IsFinished() const { return Dead; }
    void SetPersistent() { Flags44 |= 0x10; }
};
}
using NParticleEngine::CParticleEmitter;
class CParticleEmitterManager {
public:
    static CParticleEmitterManager* Get();              // 0x00AEAAF0
    CParticleEmitter* Create2DEmitter(long effect, const C2DVector* position, long layer, bool start,
                                      const CRightHandedSet& axes);   // 0x00AEB790
};
struct CSpecialEffectsDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    bool CalcAndLookup(const CCharString& name, long* value);   // 0x00669D30
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
long Lookup_CSpecialEffectsDef_CalcAndLookup(const void* def, const CCharString& name);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::CalcAndLookup(const CCharString& name, long* value)
{
    long found = Lookup_CSpecialEffectsDef_CalcAndLookup(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee
struct CBulletTimeDef {
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580 (CDefClassBase)
};
#pragma auto_inline(off)
__declspec(noinline) bool CBulletTimeDef::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                             // +0x91 bit 0
    unsigned char _flags91 : 7;
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void RemoveTC(long type);                           // 0x004C9840
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void* GetPItem() const;                             // 0x00A01B50
    void SetPItem(void* item);                          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CRadialBlurState {
    unsigned char _pad00[0x24];
    float         Strength;                             // +0x24
    unsigned char _pad28[0x8];
};
struct CEnvironmentSpellEffects {
    void RadialBlurSetCenterWorldPos(void* key, const C3DVector& pos);   // 0x006B8310
    CRadialBlurState GetRadialBlurState(void* key);     // 0x006B8350
};
struct CEnvironment {
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffects* SpellEffects;             // +0x1964
};
struct CWorld {
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                         // +0x18
};
struct CTCBulletTime {
    void*                                 __vftable;    // +0x00
    CThing*                               ParentThing;  // +0x04
    unsigned char                         _pad08[0x4];
    long                                  FramesLeft;   // +0x0c
    unsigned char                         _pad10[0x20 - 0x10];
    CBulletTimeDef*                       PDef;         // +0x20
    unsigned char                         _pad24[0x30 - 0x24];
    CIntelligentPointer<CParticleEmitter> Emitter;      // +0x30
    unsigned char                         _pad38[0x44 - 0x38];
    CIntelligentPointer<CThing>           Target;       // +0x44
    void*                                 RadialBlurKey;// +0x4c
    unsigned char                         _pad50[0x4];
    bool                                  Active;       // +0x54
    bool                                  Paused;       // +0x55
    CWorld* GetWorld();                                 // 0x00686A70
    void FrameUpdate();
};

void CTCBulletTime::FrameUpdate()
{
    CThing* parent = ParentThing;
    if (!Paused) {
        --FramesLeft;
        if (!Emitter.GetPItem() || Emitter.GetPItem()->IsFinished()) {
            CDefPointer<CSpecialEffectsDef> effects;
            long effect = 0;
            if (PDef->GetSubDef(&effects) && effects->CalcAndLookup("SLOWTIME", &effect)) {
                CParticleEmitter* emitter = CParticleEmitterManager::Get()->Create2DEmitter(effect,
                    &C2DVector(GFAXToVX((float)g_ScreenCentreX) * 0.5f, GFAYToVY((float)g_ScreenCentreY) * 0.5f), 0x52, true,
                    CRightHandedSet());
                Emitter = CIntelligentPointer<CParticleEmitter>(emitter);
                if (Emitter.GetPItem() && !Emitter.GetPItem()->IsFinished())
                    Emitter.GetPItem()->SetPersistent();
            }
        }
        if (FramesLeft == 0) {
            CDefPointer<CSpecialEffectsDef> effects;
            long effect = 0;
            if (PDef->GetSubDef(&effects) && effects->CalcAndLookup("END", &effect)) {
                CParticleEmitter* emitter = CParticleEmitterManager::Get()->Create2DEmitter(effect,
                    &C2DVector(GFAXToVX((float)g_ScreenCentreX) * 0.5f, GFAYToVY((float)g_ScreenCentreY) * 0.5f), 0x52, true,
                    CRightHandedSet());
                if (emitter && !emitter->IsFinished())
                    emitter->SetPersistent();
            }
            Active = false;
            parent->RemoveTC(0x92);
        }
    }
    if (Target.GetPItem() && !Target.GetPItem()->Dead) {
        CEnvironmentSpellEffects* spellEffects = GetWorld()->PEnvironment->SpellEffects;
        spellEffects->RadialBlurSetCenterWorldPos(RadialBlurKey, Target.GetPItem()->GetPosition());
        if (spellEffects->GetRadialBlurState(RadialBlurKey).Strength == 0.0f)
            Target = CIntelligentPointer<CThing>(0);
    }
}
