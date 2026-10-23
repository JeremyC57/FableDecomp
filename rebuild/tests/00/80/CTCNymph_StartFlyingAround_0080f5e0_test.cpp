// 0080f5e0 behaviour test: CTCNymph::StartFlyingAround (control flow + data of the final.cpp body)
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct CCharString { const char* Text; CCharString(const char* t, long = -1) : Text(t) {} ~CCharString() {} };
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
static int g_destroyed = 0;
template <class T> struct CDefPointer;
struct CDefClassBase {
    virtual void v00() {}
    virtual void Destroy() const { ++g_destroyed; }
    mutable long RefCount;
    const CDefClassBase* Sub;
    CDefClassBase() : RefCount(1), Sub(0) {}
    bool GetSubDef(CDefPointer<const struct CSpecialEffectsDef>* out) const;
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
static long g_effectId = -1;
struct CSpecialEffectsDef : CDefClassBase {
    bool GetEffectFromString(const CCharString& name, long* effect) const
    { if (g_effectId < 0 || strcmp(name.Text, "INLIMBO")) return false; *effect = g_effectId; return true; }
};
bool CDefClassBase::GetSubDef(CDefPointer<const CSpecialEffectsDef>* out) const
{ out->Object = (const CSpecialEffectsDef*)Sub; if (Sub) ++Sub->RefCount; return Sub != 0; }
struct CTCGraphicAppearance;
struct CTCDParticleEmitter;
struct CThing {
    virtual void SetVisible(bool visible) { Visible = visible ? 1 : 2; }
    int Visible;
    CTCGraphicAppearance* GraphicAppearance;
    CTCGraphicAppearance* GetGraphicAppearance() const { return GraphicAppearance; }
    CDefClassBase* PDef;
    unsigned char Flags91;
    unsigned char FlagsBC;
    int Killed;
    C3DVector Pos;
    CTCDParticleEmitter* Emitter;
    void Kill(bool removeNow) { Killed = removeNow ? 1 : 2; Flags91 |= 1; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    { if (interfaceType == 0x4b) { *ppOut = (T*)Emitter; return true; } return false; }
    const C3DVector& GetPosition() const { return Pos; }
};
static int g_hasDummy = 0;
static C3DVector g_dummy = { 1.0f, 2.0f, 9.0f };
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale)
    { if (!g_hasDummy || strcmp(name.Text, "DUMMY_CENTRE") || index || flags || scale != 1.0f || !orientation) return false; *position = g_dummy; return true; }
};
static CThing g_spawned;
static long g_createdEffect = -2;
static C3DVector g_createdPos;
struct CTCDParticleEmitter {
    CThing* AttachedTo; long Mode; const char* Bone;
    static CThing* __fastcall Create(long effect, const C3DVector& position, bool attached)
    { g_createdEffect = effect; g_createdPos = position; return attached ? &g_spawned : 0; }
    void AttachToThing(CThing* thing, long mode, const CCharString& bone, long, float) { AttachedTo = thing; Mode = mode; Bone = bone.Text; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
    bool IsAlive() const { return GetPItem() && !(GetPItem()->Flags91 & 1); }
};
long g_FramesPerSecond = 10;
struct CNymphDef { float FadeOutTime; };
static int g_picked = 0;
struct CTCNymph {
    CThing* ParentThing;
    const CNymphDef* PDef;
    C3DVector PointToFlyAround;
    float EffectHeightOffset;
    C3DVector InitialPos;
    bool FlyingAround;
    CIntelligentPointer<CThing> PEffect;
    bool HasBeenInLimbo;
    long FadeCounter;
    long TotalFadeTime;
    CTCNymph() : PEffect(0) {}
    void PickNewTargetPos() { ++g_picked; }
    void StartFlyingAround(long fadeTime);
};
void CTCNymph::StartFlyingAround(long fadeTime)
{
    long effect = 0;
    if (FlyingAround)
        return;
    CDefPointer<const CSpecialEffectsDef> effects;
    effect = 0;
    const CDefClassBase* thingDef = ParentThing->PDef;
    thingDef->GetSubDef(&effects) && effects->GetEffectFromString("INLIMBO", &effect);
    {
        CRightHandedSet orientation;
        bool notFound = !ParentThing->GetGraphicAppearance()->GetDummyObject("DUMMY_CENTRE", 0, 0, &InitialPos, &orientation, 1.0f);
        if (notFound)
            InitialPos = ParentThing->GetPosition();
    }
    PointToFlyAround = InitialPos;
    EffectHeightOffset = InitialPos.Z - ParentThing->GetPosition().Z;
    if (PEffect.IsAlive()) {
        PEffect.GetPItem()->Kill(true);
        PEffect = CIntelligentPointer<CThing>(0);
    }
    PEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, PointToFlyAround, true));
    if (PEffect.IsAlive()) {
        CTCDParticleEmitter* emitter = 0;
        PEffect.GetPItem()->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(ParentThing, 0xa, "DUMMY_CENTRE", 0, 0.0f);
        FlyingAround = true;
        if (fadeTime == -1)
            fadeTime = (long)(g_FramesPerSecond * PDef->FadeOutTime);
        FadeCounter = TotalFadeTime = fadeTime;
        ParentThing->FlagsBC &= 0xfa;
        if (FadeCounter == 0) {
            PickNewTargetPos();
            ParentThing->SetVisible(false);
        }
    }
    HasBeenInLimbo = true;
}
int main()
{
    CSpecialEffectsDef fx; CDefClassBase thingDef; thingDef.Sub = &fx;
    CTCGraphicAppearance app; CTCDParticleEmitter emitter = { 0, 0, 0 };
    CThing parent; memset(&parent, 0, sizeof(int) * 0); parent.Visible = 0; parent.GraphicAppearance = &app; parent.PDef = &thingDef;
    parent.Flags91 = 0; parent.FlagsBC = 0xff; parent.Killed = 0; parent.Pos.X = 5; parent.Pos.Y = 6; parent.Pos.Z = 7; parent.Emitter = 0;
    g_spawned.Flags91 = 0; g_spawned.Emitter = &emitter; g_spawned.Killed = 0;
    CNymphDef def; def.FadeOutTime = 2.5f;
    CTCNymph n; n.ParentThing = &parent; n.PDef = &def;

    // already flying: nothing
    n.FlyingAround = true; n.HasBeenInLimbo = false;
    n.StartFlyingAround(-1);
    CHECK(!n.HasBeenInLimbo && g_createdEffect == -2);

    // dummy found, effect found, default fade time
    n.FlyingAround = false; g_hasDummy = 1; g_effectId = 33;
    n.StartFlyingAround(-1);
    CHECK(n.InitialPos.X == 1.0f && n.InitialPos.Z == 9.0f && n.PointToFlyAround.Y == 2.0f && n.EffectHeightOffset == 2.0f);
    CHECK(g_createdEffect == 33 && g_createdPos.Z == 9.0f && n.PEffect.GetPItem() == &g_spawned);
    CHECK(emitter.AttachedTo == &parent && emitter.Mode == 0xa && strcmp(emitter.Bone, "DUMMY_CENTRE") == 0);
    CHECK(n.FlyingAround && n.FadeCounter == 25 && n.TotalFadeTime == 25 && parent.FlagsBC == 0xfa);
    CHECK(g_picked == 0 && parent.Visible == 0 && n.HasBeenInLimbo && fx.RefCount == 1 && g_destroyed == 0);

    // again with an alive effect: old killed; no dummy -> thing position; fade 0 hides and picks
    n.FlyingAround = false; g_hasDummy = 0; g_effectId = -1; emitter.AttachedTo = 0;
    n.StartFlyingAround(0);
    CHECK(g_spawned.Killed == 1);
    CHECK(n.InitialPos.X == 5.0f && n.InitialPos.Z == 7.0f && n.EffectHeightOffset == 0.0f && g_createdEffect == 0);
    CHECK(emitter.AttachedTo == 0 && !n.FlyingAround && n.HasBeenInLimbo); // respawned effect is dead (Flags91)

    // no sub-def, alive respawn, explicit fade 0
    thingDef.Sub = 0; g_spawned.Flags91 = 0; g_spawned.Killed = 0; n.PEffect.SetPItem(0); n.FlyingAround = false;
    n.StartFlyingAround(0);
    CHECK(g_createdEffect == 0 && g_spawned.Killed == 0 && n.FlyingAround && n.FadeCounter == 0 && g_picked == 1 && parent.Visible == 2);

    if (!g_fail) printf("0080f5e0_TEST PASS\n");
    return g_fail;
}