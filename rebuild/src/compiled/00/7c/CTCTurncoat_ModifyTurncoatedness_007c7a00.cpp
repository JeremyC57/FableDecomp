// CTCTurncoat::ModifyTurncoatedness(float) -- while below its resistance, add to the creature's turncoatedness (clamped to [0, resistance], restarting the recovery delay); on reaching the resistance play the activate effect, attach the turncoated effect, kill the stage effects and play the turncoat chant
// Leaf: ?ModifyTurncoatedness@CTCTurncoat
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C7A00, 572 bytes (+ int3 padding), `ret 4`.
// Callees: CThing::GetPos() const 0x004C73D0; CreateParticleEffect(long, const C3DVector*) 0x0072AB70 (`__fastcall`);
//   CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / ~CBaseIntelligentPointer 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem
//   0x00A01B90; IS_THING_ALIVE<CIntelligentPointer<...> > 0x004CC340 (`__fastcall`, out of line); LowerBound 0x0040F020
//   (interfaces 0x4B particle emitter, 0x10 sound player); CTCDParticleEmitter::AttachToThing 0x006E0BE0; CThing::Kill(bool)
//   0x004C9B80; vector<CIntelligentPointer<CThing> >::erase(first, last) 0x004AC0F0; CTCSoundPlayer::PlaySound 0x0066BD00;
//   CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0.
// Layout (retail CTCTurncoat = Ego_r.pdb, the stage-effect vector 12 bytes, as the landed ctor 0x007C78C0):
//   CurrentTurncoatedness +0x10, PDef +0x14, TurncoatedEffect +0x18, TimeDelayBeforeResistanceRecoveryFrames +0x30,
//   CurrentRecoveryTimer +0x34, TurncoatStageEffects +0x3c, CreatureTurncoatResistance +0x48; CTurncoatDef TurncoatedEffect
//   +0x28, TurncoatActivateEffect +0x2c.
class C3DVector { public: float X, Y, Z; };
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector* pos);   // 0x0072AB70

class CBaseObject;
struct CBaseIntelligentPointer {                        // 8 bytes: vptr + item
    virtual void v00();
    void* m_pData;
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThing* item);                  // 0x004A96F0
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer* p);   // 0x004CC340
inline bool IsThingAlive(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}
struct CEffectVector {                                  // vector<CIntelligentPointer<CThing> > (12 bytes)
    CIntelligentPointer* _M_start;
    CIntelligentPointer* _M_finish;
    CIntelligentPointer* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    CIntelligentPointer* erase(CIntelligentPointer* first, CIntelligentPointer* last);   // 0x004AC0F0
    void clear() { erase(_M_start, _M_finish); }
};

class CTCDParticleEmitter { public:                            // interface 0x4B
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CTCSoundPlayer { public:                                 // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};

class CTurncoatDef { public:
    unsigned char _pad0[0x28];
    long TurncoatedEffect;                              // +0x28
    long TurncoatActivateEffect;                        // +0x2c
};
struct CDefPointer {                                    // CDefPointer<const CTurncoatDef>
    CTurncoatDef* Object;
    CTurncoatDef* operator->() const { return Object; }
};

class CTCTurncoat { public:
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
    void ModifyTurncoatedness(float amount);
};

void CTCTurncoat::ModifyTurncoatedness(float amount)
{
    if (CurrentTurncoatedness >= CreatureTurncoatResistance)
        return;
    CurrentRecoveryTimer = TimeDelayBeforeResistanceRecoveryFrames;
    CurrentTurncoatedness += amount;
    if (CurrentTurncoatedness > CreatureTurncoatResistance)
        CurrentTurncoatedness = CreatureTurncoatResistance;
    else if (CurrentTurncoatedness < 0.0f)
        CurrentTurncoatedness = 0.0f;
    if (CurrentTurncoatedness >= CreatureTurncoatResistance) {
        if (PDef->TurncoatActivateEffect)
            CreateParticleEffect(PDef->TurncoatActivateEffect, &ParentThing->GetPos());
        if (!IsThingAlive(TurncoatedEffect)) {
            long effect = PDef->TurncoatedEffect;
            if (effect) {
                {
                    CIntelligentPointer created(CreateParticleEffect(effect, &ParentThing->GetPos()));
                    TurncoatedEffect.SetPItem(((CThing*)created.GetPItem()));
                }
                if (IS_THING_ALIVE(&TurncoatedEffect)) {
                    CTCDParticleEmitter* emitter;
                    ((CThing*)TurncoatedEffect.GetPItem())->GetInterface(&emitter, 0x4b);
                    emitter->AttachToThing(*ParentThing, 8, "", 0, 0.0f);
                }
            }
        }
        for (long i = 0; i < TurncoatStageEffects.size(); ++i) {
            if (IsThingAlive(TurncoatStageEffects._M_start[i])) {
                CIntelligentPointer* start = TurncoatStageEffects._M_start;
                ((CThing*)start[i].GetPItem())->Kill(true);
            }
        }
        TurncoatStageEffects.clear();
        CTCSoundPlayer* sound;
        if (ParentThing->GetInterface(&sound, 0x10)) {
            CCharString chant("HERO_ABILITY_TURNCOAT_CHANT");
            sound->PlaySound(chant, 1, 0, 0, -1);
        }
    }
}