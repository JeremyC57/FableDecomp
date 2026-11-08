// CTCWillLightning::CLightningBeam::FrameUpdate(CIntelligentPointer<CThingCreatureBase> player, unsigned long* seed) -- one frame of a will-lightning beam: count the frame and advance its clock, then by state: 1 waits m_fDelay (state 2, the home's will-lightning looping sound); 2 waits m_fActivationTime while home and target live (state 3, frame count -1, activate the children) or deactivates; 3 deactivates a dead / underground / unlightningable target (or a temporary beam after m_fLife), else re-arms the target's electrocuted effect after a successful hit and every m_OnhitEveryFrame frames hits a lightningable target (a decapitating hit once the decapitation clock passes m_fDecapitationTime on a decapitatable creature, otherwise a plain hit on a game object) with m_fDamagePerHit, records whether it landed and posts a hit-by-lightning event; 4 waits for every child (state 5, the def's end-attackee sound on the target or at the end point through the player); 5 waits m_fDeActivationTime (state 6). Then update the positions, feed LIGHTNING_START / LIGHTNING_END to the particle emitter, and frame-update the children, erasing the finished ones
// Grade: DIFFER(2744v2840) -- md=562: local-model draft (no shared CTCWillLightning / CThing headers fit); residues: ebx=0x10 hoisted in retail, HasTC result kept in a frame byte, GetTC compare against an immediate, the Normalise x87 order (test ah,5; jp), and the child ~CCountedPointer out of line (0x007A3200) in the first loop
// Next idea: move to shared headers (engine/CThing.h retail branch, CTCWillLightning) and model CLightningBeam from the PDB layout; then iterate the per-state blocks
// Leaf: ?FrameUpdate@CLightningBeam@CTCWillLightning@@QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@PAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A1B20, 2840 bytes (+ the 5-entry state jump table 0x007A2624), thiscall, `ret 0Ch`, frame 0xbc.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?FrameUpdate@CLightningBeam@CTCWillLightning@@QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@PAK@Z
//   (the PDB 0x018273D2 / 0x026B0EF0 entries); the body recurses into itself for each child (0x007A25BC).
// Draft model (DIFFER oracle, not a landing candidate): no shared header carries CLightningBeam, CThing's slot-65 OnHit, the
//   9-argument CHitParameters ctor or CParticleUpdateParameters, so this draft keeps local models in the style of the landed siblings
//   (Deactivate 0x0079FE90, IsTargetValid 0x0079FD90, CTCElectrocuteOnTouch::OnHit 0x007E2600). Layout: Ego_r.pdb CLightningBeam with
//   the 4-byte STLport list (m_bTemporary +0x50 .. SuccessfulHit +0x78, sizeof 0x7c); the def's EndAttackeeSound is +0x11c (InitSound
//   +0x110 / LoopingSound +0x114 per the landed CTCWillLightning ctor / StartLoopingSound rows, Ego_r order).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
class CBaseObject;
class CTCBase;
class CTCParamsBase;
namespace NHitLocations { class CHitLocation; }

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                                  // 0x00A01B50
    void SetPItem(CBaseObject* item);                               // 0x00A01B90
    void* PData;
};
class CThing;
class CThingCreatureBase;
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return Get(); }
};
template<class T> bool __fastcall IS_THING_ALIVE(const T& pointer);   // 0x004CC340 <CThing> / 0x006961F0 <CThingCreatureBase>

struct CVectorMapEntry { ETCInterfaceType first; CTCBase* second; };
struct CInterfaceMap {
    CVectorMapEntry* _M_start; CVectorMapEntry* _M_finish; CVectorMapEntry* _M_end_of_storage; bool Compare; bool Dirty;
    CVectorMapEntry* LowerBound(const ETCInterfaceType& key);                     // 0x0040F020
    CVectorMapEntry* end() { return _M_finish; }
};
class CHitParameters {
public:
    unsigned char _pad00[0x1b];
    bool Decapitate;                                                // +0x1b
    unsigned char _pad1c[0x22 - 0x1c];
    bool MoveBack;                                                  // +0x22
    bool CreateParticleEffectOnHit;                                 // +0x23
    bool CreateDustParticleEffectOnHit;                             // +0x24
    bool GuaranteeHit;                                              // +0x25
    bool Blocked;                                                   // +0x26
    bool HitNegated;                                                // +0x27
    unsigned char _pad28[0x60 - 0x28];
    CHitParameters();                                               // 0x007A3BC0
    CHitParameters(NHitLocations::CHitLocation* location, CThing* hitting, CThing* weapon, const C3DVector& position,
                   const C3DVector& direction, bool knockDown, bool blockable, bool flourish, bool blockCounter);   // 0x00789238
    ~CHitParameters();                                              // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);      // 0x00868CA0 (EDamageAttribute)
    void SetThingHitting(CThing& thing);                            // 0x008685F0
    void SetDecapitate(bool value) { Decapitate = value; }
    void SetMoveBackOnHit(bool value) { MoveBack = value; }
    void SetCreateDustParticleEffectOnHit(bool value) { CreateDustParticleEffectOnHit = value; }
    bool IsBlocked() const { return Blocked; }
    bool IsHitNegated() const { return HitNegated; }
};
class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void OnHit(CHitParameters& hit);                        // slot 65 (+0x104)
    unsigned char _pad04[0x10 - 0x04];
    unsigned char Type;                                             // +0x10 CClass::Type
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                               // +0x20 std::bitset<274>
    CInterfaceMap Components;                                       // +0x44
    unsigned char _pad54[0x6c - 0x54];
    unsigned long ThingTypeFlags;                                   // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                                         // +0x91 bit 0
    unsigned long GetType() const { return Type; }
    bool IsDead() const { return Dead != 0; }
    bool HasTC(ETCInterfaceType type) const { return (UsingComponents[type >> 5] >> (type & 31) & 1) != 0; }
    template<class T> __forceinline bool GetTC(ETCInterfaceType id, T** ptc) {
        if (HasTC(id)) {
            ETCInterfaceType type = id;
            CInterfaceMap* map = &Components;
            CVectorMapEntry* it = map->LowerBound(type);
            *ptc = (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    const C3DVector& GetPos() const;                                // 0x004C73D0
};
class CThingCreatureBase : public CThing { };
namespace NCreatureMode {
    bool __fastcall IsUnderground(const CThing& thing);             // 0x0085F120
    bool __fastcall IsLightningable(const CThing& thing);           // 0x0085FAA0
}
class CTCElectrocutedEffect { public: void Reset(); };              // 0x007CE7A0
namespace NEntityEvents {
    class CEventBase { public: virtual ~CEventBase(); };
    class CEventHitBySpecialAbility : public CEventBase {
    public:
        CEventHitBySpecialAbility(const CHitParameters& hit, long ability);   // 0x00811830 (EHeroAbility)
        unsigned char _pad04[0x18 - 0x04];
    };
}
class CTCEntityEvents { public: void AddEvent(NEntityEvents::CEventBase* event); };   // 0x008113C0
class CTCSoundPlayer { public:
    unsigned long PlayAttachedCriteria(const CCharString& name, bool a, bool b, bool c, long d);          // 0x0066BD00
    unsigned long PlayUnattachedCriteria(const CCharString& name, const C3DVector& pos, bool a, bool b);  // 0x0066BDF0
};
class CDefString { public: CCharString GetString() const; long TablePos; };   // GetString 0x00415DD0
class CSpecialAbilitiesLightningSpellDef { public:
    unsigned char _pad[0x11c];
    CDefString EndAttackeeSound;                                    // +0x11c
};
class CCRC { public: static unsigned long __fastcall Calc(unsigned long crc, const char* text, unsigned long length); };   // 0x004014A0
class CMainGameComponent { public: static long __fastcall GetConstantFPS() { return ConstantFPS; } static long ConstantFPS; };  // 0x01375550
namespace std {
    template<class A, class B> struct pair { A first; B second; pair(const A& a, const B& b) : first(a), second(b) { } };
}
struct CParticleVectorParams {                                       // CVectorMap<unsigned long,C3DVector,..> (STLport vector + Compare + Dirty)
    std::pair<unsigned long, C3DVector>* _M_start; std::pair<unsigned long, C3DVector>* _M_finish;
    std::pair<unsigned long, C3DVector>* _M_end_of_storage; bool Compare; bool Dirty;
    CParticleVectorParams() : _M_start(0), _M_finish(0), _M_end_of_storage(0), Dirty(false) { }
    void _M_insert_overflow(std::pair<unsigned long, C3DVector>* position, const std::pair<unsigned long, C3DVector>& x,
                            const bool& podType, unsigned long fillLength, bool atEnd);   // 0x0076ADA0
    void push_back(const std::pair<unsigned long, C3DVector>& x) {
        if (_M_finish != _M_end_of_storage) { if (_M_finish) *_M_finish = x; ++_M_finish; }
        else { bool podType; _M_insert_overflow(_M_finish, x, podType, 1, true); }
    }
    void Insert(const std::pair<unsigned long, C3DVector>& x) { push_back(x); Dirty = true; }
};
struct CParticleParamDescs {
    void* _M_start; void* _M_finish; void* _M_end_of_storage; bool Compare; bool Dirty;
    CParticleParamDescs() : _M_start(0), _M_finish(0), _M_end_of_storage(0), Dirty(false) { }
};
class CBaseClass { public: CBaseClass(); virtual ~CBaseClass(); };   // 0x0099A2D0 / 0x0099A2E0
namespace NParticleEngine {
    template<class K, class P> class CParticleUpdateParameters : public CBaseClass {
    public:
        CParticleUpdateParameters() { }
        virtual ~CParticleUpdateParameters();                       // 0x006E2500
        void Update();                                              // 0x0076C750
        CParticleVectorParams VectorParams;                         // +0x04
        CParticleParamDescs UpdateParams;                           // +0x14
    };
}
class CTCDParticleEmitter { public:
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>& params);   // 0x006E11A0
};
struct CCPPointerInfo { long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
void operator delete(void* p);                                      // 0x00BFE9BC
template<class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); operator delete(Info); } }
    T* operator->() const { return Data; }
};
class CTCWillLightning {
public:
    class CLightningBeam;
    void StartLoopingSound();                                       // 0x007A0400
};
struct CBeamNode { CBeamNode* _M_next; CBeamNode* _M_prev; CCountedPointer<CTCWillLightning::CLightningBeam> _M_data; };
struct CBeamIterator {
    CBeamNode* _M_node;
    CBeamIterator(CBeamNode* node) : _M_node(node) { }
    CBeamIterator(const CBeamIterator& other) : _M_node(other._M_node) { }
    bool operator!=(const CBeamIterator& other) const { return _M_node != other._M_node; }
    CBeamIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    CCountedPointer<CTCWillLightning::CLightningBeam>& operator*() const { return _M_node->_M_data; }
};
struct CBeamList {                                                  // std::list<CCountedPointer<CLightningBeam> > (STLport, 4 bytes)
    CBeamNode* _M_node;
    CBeamIterator begin() { return CBeamIterator(_M_node->_M_next); }
    CBeamIterator end() { return CBeamIterator(_M_node); }
    CBeamIterator erase(CBeamIterator position);                    // 0x007A38B0
};
class CTCWillLightning::CLightningBeam {
public:
    void Activate();                                                // 0x007A1380
    void Deactivate(CIntelligentPointer<CThingCreatureBase> player);   // 0x0079FE90
    bool AllBeamsInactive();                                        // 0x0079F730
    void UpdatePositions();                                         // 0x0079F970
    void FrameUpdate(CIntelligentPointer<CThingCreatureBase> player, unsigned long* seed);
    long m_lsState;                                                 // +0x00 ELightningState
    CIntelligentPointer<CThing> m_tpParticleHandle;                 // +0x04
    CIntelligentPointer<CThing> m_tcpHome;                          // +0x0c
    CIntelligentPointer<CThing> m_tcpTarget;                        // +0x14
    C3DVector m_vStartPos;                                          // +0x1c
    C3DVector m_vEndPos;                                            // +0x28
    C3DVector m_vDestination;                                       // +0x34
    float m_fDamagePerHit;                                          // +0x40
    float m_fTime;                                                  // +0x44
    float m_fTimeBase;                                              // +0x48
    CBeamList m_lChildren;                                          // +0x4c
    bool m_bTemporary;                                              // +0x50
    bool m_bMainBeam;                                               // +0x51
    bool m_bRightHand;                                              // +0x52
    float m_fActivationTime;                                        // +0x54
    float m_fDeActivationTime;                                      // +0x58
    float m_fLife;                                                  // +0x5c
    float m_fDecapitationTime;                                      // +0x60
    float m_fDecapitationClock;                                     // +0x64
    float m_fDelay;                                                 // +0x68
    long m_OnhitEveryFrame;                                         // +0x6c
    long m_nFrameCount;                                             // +0x70
    CSpecialAbilitiesLightningSpellDef* PDef;                       // +0x74 CDefPointer<const CSpecialAbilitiesLightningSpellDef>
    bool SuccessfulHit;                                             // +0x78
};
enum { LIGHTNING_STATE_NONE = 0, LIGHTNING_STATE_DELAY = 1, LIGHTNING_STATE_ACTIVATING = 2, LIGHTNING_STATE_ACTIVE = 3,
       LIGHTNING_STATE_WAITING_FOR_CHILDREN = 4, LIGHTNING_STATE_DEACTIVATING = 5, LIGHTNING_STATE_DEAD = 6 };   // descriptive
enum { THING_TYPE_CREATURE = 1, THING_TYPE_FLAG_GAME_OBJECT = 4, DAMAGE_ATTRIBUTE_LIGHTNING = 2, HERO_ABILITY_LIGHTNING_SPELL = 11 };   // descriptive

void CTCWillLightning::CLightningBeam::FrameUpdate(CIntelligentPointer<CThingCreatureBase> player, unsigned long* seed)
{
    ++m_nFrameCount;
    m_fTime += 1.0f / (float)CMainGameComponent::GetConstantFPS();
    switch (m_lsState) {
    case LIGHTNING_STATE_DELAY:
        if (m_fTime - m_fTimeBase >= m_fDelay) {
            m_fTimeBase = m_fTime;
            m_lsState = LIGHTNING_STATE_ACTIVATING;
            if (IS_THING_ALIVE(m_tcpHome)) {
                CTCWillLightning* will;
                if (m_tcpHome->GetTC(TCI_WILL_LIGHTNING, &will))
                    will->StartLoopingSound();
            }
        }
        break;
    case LIGHTNING_STATE_ACTIVATING:
        if (m_bTemporary || (IS_THING_ALIVE(m_tcpHome) && IS_THING_ALIVE(m_tcpTarget))) {
            if (m_fTime - m_fTimeBase >= m_fActivationTime) {
                m_fTimeBase = m_fTime;
                m_lsState = LIGHTNING_STATE_ACTIVE;
                m_nFrameCount = -1;
                for (CBeamIterator it = m_lChildren.begin(); it != m_lChildren.end(); ++it) {
                    CCountedPointer<CLightningBeam> child = *it;
                    child->Activate();
                }
            }
        } else
            Deactivate(player);
        break;
    case LIGHTNING_STATE_ACTIVE:
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
                    effect = (CTCElectrocutedEffect*)m_tcpTarget->AddTC(CCharString("CTCElectrocutedEffect"), true, 0);
                effect->Reset();
            }
            if (NCreatureMode::IsLightningable(*m_tcpTarget.Get())) {
                m_fDecapitationClock += 1.0f / (float)CMainGameComponent::GetConstantFPS();
                if (m_tcpTarget->HasTC(TCI_DECAPITATION) && m_tcpTarget->GetType() == THING_TYPE_CREATURE
                        && m_fDecapitationClock >= m_fDecapitationTime && m_fDecapitationTime != -1.0f
                        && m_nFrameCount % m_OnhitEveryFrame == 0) {
                    CThing* target = m_tcpTarget.Get();
                    C3DVector direction = target->GetPos() - player->GetPos();
                    if (direction.IsNonZero()) direction.Normalise();
                    else direction = C3DVector(1.0f, 0.0f, 0.0f);
                    CHitParameters hit(0, player.Get(), 0, m_tcpTarget->GetPos(), direction, false, false, false, false);
                    hit.SetDecapitate(true);
                    hit.AddDamageWithAttribute(DAMAGE_ATTRIBUTE_LIGHTNING, m_fDamagePerHit);
                    hit.SetMoveBackOnHit(false);
                    CThing* struck = m_tcpTarget.Get();
                    hit.SetCreateDustParticleEffectOnHit(false);
                    struck->OnHit(hit);
                    SuccessfulHit = !hit.IsBlocked() && !hit.IsHitNegated();
                    if (IS_THING_ALIVE(m_tcpTarget)) {
                        CTCEntityEvents* events;
                        if (m_tcpTarget->GetTC(TCI_ENTITY_EVENTS, &events))
                            events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, HERO_ABILITY_LIGHTNING_SPELL));
                    }
                } else if ((m_tcpTarget->ThingTypeFlags & THING_TYPE_FLAG_GAME_OBJECT) && m_nFrameCount % m_OnhitEveryFrame == 0) {
                    CThing* target = m_tcpTarget.Get();
                    C3DVector direction = target->GetPos() - player->GetPos();
                    if (direction.IsNonZero()) direction.Normalise();
                    else direction = C3DVector(1.0f, 0.0f, 0.0f);
                    CHitParameters hit(0, player.Get(), 0, m_tcpTarget->GetPos(), direction, false, false, false, false);
                    hit.AddDamageWithAttribute(DAMAGE_ATTRIBUTE_LIGHTNING, m_fDamagePerHit);
                    hit.SetMoveBackOnHit(false);
                    CThing* struck = m_tcpTarget.Get();
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
    case LIGHTNING_STATE_WAITING_FOR_CHILDREN:
        if (AllBeamsInactive()) {
            m_fTimeBase = m_fTime;
            m_lsState = LIGHTNING_STATE_DEACTIVATING;
            if (IS_THING_ALIVE(m_tcpTarget)) {
                CTCSoundPlayer* sound;
                if (m_tcpTarget->GetTC(TCI_SOUND_PLAYER, &sound))
                    sound->PlayAttachedCriteria(PDef->EndAttackeeSound.GetString(), true, false, false, -1);
            } else if (IS_THING_ALIVE(player) && !m_bTemporary) {
                CTCSoundPlayer* sound;
                if (player->GetTC(TCI_SOUND_PLAYER, &sound))
                    sound->PlayUnattachedCriteria(PDef->EndAttackeeSound.GetString(), m_vEndPos, false, false);
            }
        }
        break;
    case LIGHTNING_STATE_DEACTIVATING:
        if (m_fTime - m_fTimeBase >= m_fDeActivationTime) {
            m_fTimeBase = m_fTime;
            m_lsState = LIGHTNING_STATE_DEAD;
        }
        break;
    }
    UpdatePositions();
    if (m_lsState != LIGHTNING_STATE_NONE && m_lsState != LIGHTNING_STATE_DEAD) {
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
        for (CBeamIterator it = m_lChildren.begin(); it != m_lChildren.end(); ) {
            CCountedPointer<CLightningBeam> child = *it;
            child->FrameUpdate(player, seed);
            if (child->m_lsState == LIGHTNING_STATE_DEAD)
                it = m_lChildren.erase(it);
            else
                ++it;
        }
    }
}
