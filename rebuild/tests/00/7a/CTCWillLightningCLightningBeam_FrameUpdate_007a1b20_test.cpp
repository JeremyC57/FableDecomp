// Behaviour test for 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate: the row's own function text (copied unchanged from
// src/007a1b20.cpp; its dead release assert compiles to nothing here) over a mock engine that logs every engine call.
// Checks: the clock advances by 1/FPS and the frame count by one; state 1 waits m_fDelay then goes to 2 and starts the home's looping
// sound; state 2 waits m_fActivationTime then goes to 3, resets the frame count to -1 and activates every child (or deactivates when
// home / target died and the beam is not temporary); state 3 deactivates when the target is underground / not lightningable / dead,
// a temporary beam deactivates after m_fLife, an alive player resets the electrocuted effect after a successful hit (adding the TC
// when missing), advances the decapitation clock and hits the target every m_OnhitEveryFrame frames: a decapitating hit (creature
// type 1 with a decapitation TC once the clock passed the time) or a plain hit on a flagged (type flags & 4) target, along the
// normalised target - player direction (+X when zero), lightning damage, no knock-back / dust, SuccessfulHit = !blocked && !negated,
// and a hit-by event; state 4 goes to 5 when all children are inactive and plays the end sound attached to a live target or
// unattached at the end position for a live non-temporary player; state 5 goes to 6 after m_fDeActivationTime. Afterwards the
// positions are updated and, unless inactive (0) or dead (6), LIGHTNING_START / LIGHTNING_END are fed to the particle emitter of a
// live particle and every child is frame-updated, dead (state 6) children erased.
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <list>
#include <vector>
static std::string gLog;
static void L(const std::string& s) { gLog += s; gLog += ";"; }
static std::string I2S(long v) { char b[32]; sprintf(b, "%ld", v); return b; }
static std::string F2S(float v) { char b[32]; sprintf(b, "%.3f", v); return b; }
#define FABLE_RETAIL_ASSERT(c) ((void)0)
const float DELTA_NOTIONAL_ZERO = 0.0001f;
struct C3DVector { float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    bool IsNonZero() const { return X * X + Y * Y + Z * Z > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO; }
    void Normalise() { float m = sqrtf(X * X + Y * Y + Z * Z); X /= m; Y /= m; Z /= m; }
    std::string S() const { return F2S(X) + "," + F2S(Y) + "," + F2S(Z); } };
struct CMainGameComponent { static long GetConstantFPS() { return 10; } };
enum { TCI_WILL_LIGHTNING = 1, TCI_ELECTROCUTED_EFFECT, TCI_DECAPITATION, TCI_ENTITY_EVENTS, TCI_SOUND_PLAYER, TCI_PARTICLE_EMITTER };
enum { DAMAGE_LIGHTNING = 2 }; enum { HERO_ABILITY_LIGHTNING_SPELL = 11 };
struct CTCBase { virtual ~CTCBase() { } };
struct CTCElectrocutedEffect : CTCBase { void Reset() { L("electro reset"); } };
struct CHitParameters;
namespace NEntityEvents { struct CEventHitBySpecialAbility { std::string D; CEventHitBySpecialAbility(const CHitParameters& h, int a); }; }
struct CTCEntityEvents : CTCBase { void AddEvent(NEntityEvents::CEventHitBySpecialAbility* e) { L("event " + e->D); delete e; } };
struct CTCSoundPlayer : CTCBase { std::string Who;
    void PlayAttachedCriteria(const std::string& s, bool, bool, bool, long) { L("attached " + Who + " " + s); }
    void PlayUnattachedCriteria(const std::string& s, const C3DVector& p, bool, bool) { L("unattached " + Who + " " + s + " @" + p.S()); } };
template<class K, class V> struct CParticleVectorMap { std::vector<std::pair<K, C3DVector> > V_; void Insert(const std::pair<K, C3DVector>& p) { V_.push_back(p); } };
namespace NParticleEngine { template<class A, class B> struct CParticleUpdateParameters { CParticleVectorMap<A, B> VectorParams; void Update() { L("params update"); } }; }
struct CTCDParticleEmitter : CTCBase { void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>& p) {
    std::string s = "emitter"; for (size_t i = 0; i < p.VectorParams.V_.size(); ++i) s += " " + I2S((long)p.VectorParams.V_[i].first) + "@" + p.VectorParams.V_[i].second.S(); L(s); } };
struct CCRC { static unsigned long Calc(unsigned long, const char* s, int n) { return (unsigned long)(strlen(s) * 100 + n); } };
struct CTCWillLightning;
struct CThing { std::string Name; bool Alive, Underground, Lightningable, Dead, HasDecap; int Type; unsigned long TypeFlags; C3DVector Pos;
    CTCBase* TCs[8];
    CThing(const char* n) : Name(n), Alive(true), Underground(false), Lightningable(true), Dead(false), HasDecap(false), Type(0), TypeFlags(0) { for (int i = 0; i < 8; ++i) TCs[i] = 0; }
    virtual ~CThing() { }
    template<class T> bool GetTC(int id, T** out) { if (!TCs[id]) return false; *out = (T*)TCs[id]; return true; }
    CTCBase* AddTC(const char* name, bool, int) { L(std::string("addtc ") + name); TCs[TCI_ELECTROCUTED_EFFECT] = new CTCElectrocutedEffect; return TCs[TCI_ELECTROCUTED_EFFECT]; }
    bool HasTC(int id) { return id == TCI_DECAPITATION && HasDecap; }
    int GetType() const { return Type; } unsigned long GetTypeFlags() const { return TypeFlags; }
    const C3DVector& GetPos() const { return Pos; } bool IsDead() const { return Dead; } };
struct CThingCreatureBase : CThing { CThingCreatureBase(const char* n) : CThing(n) { } };
struct CHitParameters { const CThing* Hitter; C3DVector At, Dir; bool Decap, MoveBack, Dust, Blocked, Negated; long Attr; float Damage;
    CHitParameters() : Hitter(0), Decap(false), MoveBack(true), Dust(true), Blocked(false), Negated(false), Attr(0), Damage(0) { }
    CHitParameters(int, const CThing* h, int, const C3DVector& at, const C3DVector& dir, bool, bool, bool, bool)
        : Hitter(h), At(at), Dir(dir), Decap(false), MoveBack(true), Dust(true), Blocked(false), Negated(false), Attr(0), Damage(0) { }
    void SetDecapitate(bool b) { Decap = b; } void AddDamageWithAttribute(long a, float d) { Attr = a; Damage += d; }
    void SetMoveBackOnHit(bool b) { MoveBack = b; } void SetCreateDustParticleEffectOnHit(bool b) { Dust = b; }
    bool IsBlocked() const { return Blocked; } bool IsHitNegated() const { return Negated; }
    void SetThingHitting(const CThing& t) { Hitter = &t; } };
NEntityEvents::CEventHitBySpecialAbility::CEventHitBySpecialAbility(const CHitParameters& h, int a)
    : D(std::string(h.Hitter ? h.Hitter->Name : "-") + " ability " + I2S(a) + (h.Decap ? " decap" : "")) { }
static bool gBlockNext;
struct CThingGameObject : CThing { CThingGameObject(const char* n) : CThing(n) { }
    void OnHit(CHitParameters& h) { L("onhit " + Name + " by " + (h.Hitter ? h.Hitter->Name : "-") + " at " + h.At.S() + " dir " + h.Dir.S()
        + (h.Decap ? " decap" : "") + " attr " + I2S(h.Attr) + " dmg " + F2S(h.Damage) + (h.MoveBack ? " moveback" : "") + (h.Dust ? " dust" : ""));
        h.Blocked = gBlockNext; } };
namespace NCreatureMode { bool IsUnderground(const CThing& t) { return t.Underground; } bool IsLightningable(const CThing& t) { return t.Lightningable; } }
template<class T> struct CIntelligentPointer { T* P; CIntelligentPointer() : P(0) { } CIntelligentPointer(T* p) : P(p) { } T* Get() const { return P; } T* operator->() const { return P; } };
#define IS_THING_ALIVE(x) ((x).Get() != 0 && (x).Get()->Alive)
template<class T> struct CCountedPointer { T* P; CCountedPointer(T* p) : P(p) { } T* operator->() const { return P; } };
struct CSpecialAbilitiesLightningSpellDef { std::string EndAttackeeSound; };
typedef int ELightningState;
struct CTCWillLightning : CTCBase {
    void StartLoopingSound() { L("looping sound"); }
    struct CLightningBeam {
        std::string Id; ELightningState m_lsState; float m_fTime, m_fTimeBase, m_fDelay, m_fActivationTime, m_fLife, m_fDecapitationClock,
        m_fDecapitationTime, m_fDamagePerHit, m_fDeActivationTime; long m_nFrameCount, m_OnhitEveryFrame; bool m_bTemporary, SuccessfulHit, ChildrenInactive;
        CIntelligentPointer<CThing> m_tcpHome, m_tcpTarget, m_tpParticleHandle; C3DVector m_vStartPos, m_vEndPos;
        std::list<CCountedPointer<CLightningBeam> > m_lChildren; const CSpecialAbilitiesLightningSpellDef* PDef;
        CLightningBeam(const char* id) : Id(id), m_lsState(1), m_fTime(0), m_fTimeBase(0), m_fDelay(0), m_fActivationTime(0), m_fLife(0),
            m_fDecapitationClock(0), m_fDecapitationTime(-1.0f), m_fDamagePerHit(0), m_fDeActivationTime(0), m_nFrameCount(0), m_OnhitEveryFrame(1),
            m_bTemporary(false), SuccessfulHit(false), ChildrenInactive(false), PDef(0) { }
        void Activate() { L("activate " + Id); }
        void Deactivate(CIntelligentPointer<CThingCreatureBase>) { L("deactivate " + Id); }
        void UpdatePositions() { L("positions " + Id); }
        bool AllChildrenInactive() { return ChildrenInactive; }
        void FrameUpdate(CIntelligentPointer<CThingCreatureBase> player, unsigned long* seed);
    };
};

// ---- the row (src/007a1b20.cpp), unchanged ----
void CTCWillLightning::CLightningBeam::FrameUpdate(CIntelligentPointer<CThingCreatureBase> player, unsigned long* seed)
{
    m_fTime += 1.0f / (float)CMainGameComponent::GetConstantFPS();
    ++m_nFrameCount;
    switch (m_lsState) {
    case 1:
        if (m_fTime - m_fTimeBase >= m_fDelay) {
            m_fTimeBase = m_fTime;
            m_lsState = (ELightningState)2;
            if (IS_THING_ALIVE(m_tcpHome)) {
                CTCWillLightning* will;
                if (m_tcpHome->GetTC(TCI_WILL_LIGHTNING, &will))
                    will->StartLoopingSound();
            }
        }
        break;
    case 2:
        if (m_bTemporary || (IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget))) {
            if (m_fTime - m_fTimeBase >= m_fActivationTime) {
                m_fTimeBase = m_fTime;
                m_lsState = (ELightningState)3;
                m_nFrameCount = -1;
                for (std::list<CCountedPointer<CLightningBeam> >::iterator it = m_lChildren.begin(); it != m_lChildren.end(); ++it) {
                    CCountedPointer<CLightningBeam> child = *it;
                    child->Activate();
                }
            }
        } else
            Deactivate(player);
        break;
    case 3:
        if (!m_bTemporary && (!IS_THING_ALIVE(m_tcpHome) || !IS_THING_ALIVE(m_tcpTarget)
                || NCreatureMode::IsUnderground(*m_tcpTarget.Get()) || !NCreatureMode::IsLightningable(*m_tcpTarget.Get()))) {
            Deactivate(player);
        } else if (m_bTemporary) {
            if (m_fTime - m_fTimeBase >= m_fLife)
                Deactivate(player);
        } else if (IS_THING_ALIVE(player)) {
            if (SuccessfulHit) {
                SuccessfulHit = false;
                CTCElectrocutedEffect* effect;
                if (!m_tcpTarget->GetTC(TCI_ELECTROCUTED_EFFECT, &effect))
                    effect = (CTCElectrocutedEffect*)m_tcpTarget->AddTC("CTCElectrocutedEffect", true, 0);
                effect->Reset();
            }
            if (NCreatureMode::IsLightningable(*m_tcpTarget.Get())) {
                m_fDecapitationClock += 1.0f / (float)CMainGameComponent::GetConstantFPS();
                if (m_tcpTarget->HasTC(TCI_DECAPITATION) && m_tcpTarget->GetType() == 1
                        && m_fDecapitationClock >= m_fDecapitationTime && m_fDecapitationTime != -1.0f
                        && m_nFrameCount % m_OnhitEveryFrame == 0) {
                    C3DVector direction = m_tcpTarget->GetPos() - player->GetPos();
                    FABLE_RETAIL_ASSERT(direction.X == direction.X && direction.Y == direction.Y && direction.Z == direction.Z);
                    if (direction.IsNonZero())
                        direction.Normalise();
                    else
                        direction = C3DVector(1.0f, 0.0f, 0.0f);
                    CHitParameters hit(0, player.Get(), 0, m_tcpTarget->GetPos(), direction, false, false, false, false);
                    hit.SetDecapitate(true);
                    hit.AddDamageWithAttribute(DAMAGE_LIGHTNING, m_fDamagePerHit);
                    hit.SetMoveBackOnHit(false);
                    CThingGameObject* struck = static_cast<CThingGameObject*>(m_tcpTarget.Get());
                    hit.SetCreateDustParticleEffectOnHit(false);
                    struck->OnHit(hit);
                    SuccessfulHit = !hit.IsBlocked() && !hit.IsHitNegated();
                    if (IS_THING_ALIVE(m_tcpTarget)) {
                        CTCEntityEvents* events;
                        if (m_tcpTarget->GetTC(TCI_ENTITY_EVENTS, &events))
                            events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, HERO_ABILITY_LIGHTNING_SPELL));
                    }
                } else if ((m_tcpTarget->GetTypeFlags() & 4) && m_nFrameCount % m_OnhitEveryFrame == 0) {
                    C3DVector direction = m_tcpTarget->GetPos() - player->GetPos();
                    if (direction.IsNonZero())
                        direction.Normalise();
                    else
                        direction = C3DVector(1.0f, 0.0f, 0.0f);
                    CHitParameters hit(0, player.Get(), 0, m_tcpTarget->GetPos(), direction, false, false, false, false);
                    hit.AddDamageWithAttribute(DAMAGE_LIGHTNING, m_fDamagePerHit);
                    hit.SetMoveBackOnHit(false);
                    CThingGameObject* struck = static_cast<CThingGameObject*>(m_tcpTarget.Get());
                    hit.SetCreateDustParticleEffectOnHit(false);
                    struck->OnHit(hit);
                    SuccessfulHit = !hit.IsBlocked() && !hit.IsHitNegated();
                    if (IS_THING_ALIVE(m_tcpTarget)) {
                        CTCEntityEvents* events;
                        if (m_tcpTarget->GetTC(TCI_ENTITY_EVENTS, &events))
                            events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, HERO_ABILITY_LIGHTNING_SPELL));
                    }
                }
            }
        } else if (IS_THING_ALIVE(player)) {
            CTCEntityEvents* events;
            if (m_tcpTarget->GetTC(TCI_ENTITY_EVENTS, &events)) {
                CHitParameters hit;
                hit.SetThingHitting(*player.Get());
                events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, HERO_ABILITY_LIGHTNING_SPELL));
            }
        }
        break;
    case 4:
        if (AllChildrenInactive()) {
            m_fTimeBase = m_fTime;
            m_lsState = (ELightningState)5;
            if (IS_THING_ALIVE(m_tcpTarget)) {
                CTCSoundPlayer* sound;
                if (m_tcpTarget->GetTC(TCI_SOUND_PLAYER, &sound)) {
                    sound->PlayAttachedCriteria(PDef->EndAttackeeSound, true, false, false, -1);
                }
            } else if (IS_THING_ALIVE(player) && !m_bTemporary) {
                CTCSoundPlayer* sound;
                if (player->GetTC(TCI_SOUND_PLAYER, &sound)) {
                    sound->PlayUnattachedCriteria(PDef->EndAttackeeSound, m_vEndPos, false, false);
                }
            }
        }
        break;
    case 5:
        if (m_fTime - m_fTimeBase >= m_fDeActivationTime) {
            m_fTimeBase = m_fTime;
            m_lsState = (ELightningState)6;
        }
        break;
    }
    UpdatePositions();
    if (m_lsState != 0 && m_lsState != 6) {
        std::pair<unsigned long, C3DVector> start(CCRC::Calc(0, "LIGHTNING_START", 15), m_vStartPos);
        std::pair<unsigned long, C3DVector> end(CCRC::Calc(0, "LIGHTNING_END", 13), m_vEndPos);
        NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> params;
        params.VectorParams.Insert(start);
        params.VectorParams.Insert(end);
        params.Update();
        if (m_tpParticleHandle.Get() && !m_tpParticleHandle->IsDead()) {
            CTCDParticleEmitter* emitter = 0;
            m_tpParticleHandle->GetTC(TCI_PARTICLE_EMITTER, &emitter);
            emitter->SetEmitterUpdateParameters(params);
        }
        for (std::list<CCountedPointer<CLightningBeam> >::iterator it = m_lChildren.begin(); it != m_lChildren.end(); ) {
            CCountedPointer<CLightningBeam> child = *it;
            child->FrameUpdate(player, seed);
            if (child->m_lsState == 6)
                it = m_lChildren.erase(it);
            else
                ++it;
        }
    }
}

static int gFail;
static void Check(bool c, const char* what) { if (!c) { printf("FAIL: %s\n", what); ++gFail; } }
static void Expect(const char* what, const char* log) { if (gLog != log) { printf("FAIL: %s\n  got %s\n", what, gLog.c_str()); ++gFail; } gLog = ""; }
int main()
{
    CThingCreatureBase hero("hero"); hero.Pos = C3DVector(1, 2, 3);
    CTCSoundPlayer heroSound; heroSound.Who = "hero"; hero.TCs[TCI_SOUND_PLAYER] = &heroSound;
    CThing home("home"); CTCWillLightning will; home.TCs[TCI_WILL_LIGHTNING] = &will;
    CThingGameObject target("target"); target.Pos = C3DVector(4, 6, 3);
    CTCEntityEvents events; target.TCs[TCI_ENTITY_EVENTS] = &events;
    CTCSoundPlayer targetSound; targetSound.Who = "target"; target.TCs[TCI_SOUND_PLAYER] = &targetSound;
    CThing particle("particle"); CTCDParticleEmitter emitter; particle.TCs[TCI_PARTICLE_EMITTER] = &emitter;
    CSpecialAbilitiesLightningSpellDef def; def.EndAttackeeSound = "END_SOUND";
    CIntelligentPointer<CThingCreatureBase> player(&hero);
    unsigned long seed = 0;
    CTCWillLightning::CLightningBeam b("b"), c1("c1"), c2("c2");
    b.m_tcpHome.P = &home; b.m_tcpTarget.P = &target; b.m_tpParticleHandle.P = &particle; b.PDef = &def;
    b.m_vStartPos = C3DVector(0, 0, 1); b.m_vEndPos = C3DVector(0, 0, 2);
    c1.m_lsState = 5; c1.m_fDeActivationTime = 0.0f;           // goes dead this frame -> erased
    c2.m_lsState = 1; c2.m_fDelay = 100.0f;                    // stays
    b.m_lChildren.push_back(CCountedPointer<CTCWillLightning::CLightningBeam>(&c1));
    b.m_lChildren.push_back(CCountedPointer<CTCWillLightning::CLightningBeam>(&c2));
    // state 1 -> 2
    b.m_fDelay = 0.05f; b.FrameUpdate(player, &seed);
    Check(b.m_lsState == 2 && b.m_nFrameCount == 1 && fabsf(b.m_fTime - 0.1f) < 1e-6f && b.m_fTimeBase == b.m_fTime, "state 1 -> 2");
    Check(b.m_lChildren.size() == 1 && b.m_lChildren.front().P == &c2 && c1.m_lsState == 6, "dead child erased");
    Expect("state 1", "looping sound;positions b;params update;emitter 1515@0.000,0.000,1.000 1313@0.000,0.000,2.000;positions c1;positions c2;params update;");
    // state 2 waits, then -> 3
    b.m_fActivationTime = 0.15f; b.FrameUpdate(player, &seed);
    Check(b.m_lsState == 2, "state 2 waits");
    gLog = "";
    b.FrameUpdate(player, &seed);
    Check(b.m_lsState == 3 && b.m_nFrameCount == -1, "state 2 -> 3, frame count reset");
    Expect("state 2", "activate c2;positions b;params update;emitter 1515@0.000,0.000,1.000 1313@0.000,0.000,2.000;positions c2;params update;");
    b.m_lChildren.clear();
    // state 3: decapitating hit (successful previous hit resets the electrocuted effect, adding it)
    target.Type = 1; target.HasDecap = true; b.m_fDecapitationTime = 0.05f; b.m_fDamagePerHit = 7.0f; b.SuccessfulHit = true; b.m_nFrameCount = 1;
    b.m_OnhitEveryFrame = 2; particle.Dead = true;
    b.FrameUpdate(player, &seed);
    Check(b.SuccessfulHit && b.m_lsState == 3 && fabsf(b.m_fDecapitationClock - 0.1f) < 1e-6f, "decap hit successful");
    Expect("decap", "addtc CTCElectrocutedEffect;electro reset;onhit target by hero at 4.000,6.000,3.000 dir 0.600,0.800,0.000 decap attr 2 dmg 7.000;event hero ability 11 decap;positions b;params update;");
    // not on a hit frame: nothing
    b.FrameUpdate(player, &seed);
    Expect("off frame", "electro reset;positions b;params update;");
    // plain hit on a flagged target, zero direction -> +X, blocked -> not successful
    target.HasDecap = false; target.TypeFlags = 4; target.Pos = hero.Pos; gBlockNext = true; b.SuccessfulHit = false;
    b.FrameUpdate(player, &seed);
    Check(!b.SuccessfulHit, "blocked hit not successful");
    Expect("plain", "onhit target by hero at 1.000,2.000,3.000 dir 1.000,0.000,0.000 attr 2 dmg 7.000;event hero ability 11;positions b;params update;");
    // decap clock below the time: plain hit path when flagged
    gBlockNext = false; target.TypeFlags = 0; target.HasDecap = true; b.m_fDecapitationClock = -10.0f; b.m_nFrameCount = 1;
    b.FrameUpdate(player, &seed);
    Expect("clock not reached, not flagged", "positions b;params update;");
    // dead player: hit-by event only through the else-if? (the else-if repeats IS_THING_ALIVE(player): never taken) -> nothing
    hero.Alive = false; b.FrameUpdate(player, &seed); hero.Alive = true;
    Expect("dead player", "positions b;params update;");
    // underground target -> deactivate
    target.Underground = true; b.FrameUpdate(player, &seed); target.Underground = false;
    Expect("underground", "deactivate b;positions b;params update;");
    // not lightningable -> deactivate
    target.Lightningable = false; b.FrameUpdate(player, &seed); target.Lightningable = true;
    Expect("not lightningable", "deactivate b;positions b;params update;");
    // temporary beam: life
    b.m_bTemporary = true; b.m_fLife = 1000.0f; b.FrameUpdate(player, &seed);
    Expect("temporary alive", "positions b;params update;");
    b.m_fLife = 0.0f; b.FrameUpdate(player, &seed); b.m_bTemporary = false;
    Expect("temporary expired", "deactivate b;positions b;params update;");
    // state 4: waits for children, then end sound attached to the live target
    b.m_lsState = 4; b.FrameUpdate(player, &seed);
    Check(b.m_lsState == 4, "state 4 waits");
    gLog = "";
    b.ChildrenInactive = true; b.FrameUpdate(player, &seed);
    Check(b.m_lsState == 5 && b.m_fTimeBase == b.m_fTime, "state 4 -> 5");
    Expect("state 4 attached", "attached target END_SOUND;positions b;params update;");
    target.Alive = false; b.m_lsState = 4; b.FrameUpdate(player, &seed);
    Expect("state 4 unattached", "unattached hero END_SOUND @0.000,0.000,2.000;positions b;params update;");
    b.m_lsState = 4; b.m_bTemporary = true; b.FrameUpdate(player, &seed); b.m_bTemporary = false; target.Alive = true;
    Expect("state 4 temporary: no sound", "positions b;params update;");
    // state 5 -> 6: no particle update, no children
    b.m_fDeActivationTime = 1000.0f; b.FrameUpdate(player, &seed);
    Check(b.m_lsState == 5, "state 5 waits");
    gLog = "";
    b.m_fDeActivationTime = 0.0f; particle.Dead = false; b.FrameUpdate(player, &seed);
    Check(b.m_lsState == 6, "state 5 -> 6");
    Expect("state 5", "positions b;");
    // state 2, home dead and not temporary -> deactivate
    b.m_lsState = 2; home.Alive = false; b.FrameUpdate(player, &seed);
    Expect("state 2 home dead", "deactivate b;positions b;params update;emitter 1515@0.000,0.000,1.000 1313@0.000,0.000,2.000;");
    if (!gFail) printf("007a1b20_TEST PASS\n");
    return gFail;
}