// Grade: DIFFER(872v878) -- same flow and callees; retail keeps the index in ebp and the list node in ebx (ours swaps them), so its ebp-based addressing is 6 bytes longer; the effects/particleType zero stores swap order; the CIntelligentPointer vptr store reads as 0 in our obj (reloc)
// CTCHealLife::FrameUpdate() -- makes the caster's noise, heals every living non-hostile creature in range (spawning its healing effect once and applying whole hit points as they accumulate) and removes the spell once the total is spent or the target is gone
#include <math.h>
class CCharString {
public:
    void* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* PItem;
};
class CThing;
class CThingCreatureBase;
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    CIntelligentPointer(T* item);                       // 0x004A96F0 (const CThing instance)
    CIntelligentPointer& operator=(const CIntelligentPointer<const CThing>& other);   // 0x004A9760
    T* Get() const { return (T*)GetPItem(); }
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<const CThing>& thing);   // 0x004CC340
class CTCDParticleEmitter {
public:
    void AttachToThing(CThing& thing, long bone, const CCharString& helper, long index, float time);   // 0x006E0BE0
    void EnterTimelineEnd();                            // 0x006E05F0
};
struct CTCPair { long Type; void* TC; };
struct CTCMap {
    CTCPair* _M_start;
    CTCPair* _M_finish;
    CTCPair* LowerBound(const long& key);               // 0x0040F020
    __forceinline void* Find(long type)
    {
        long key = type;
        CTCPair* it = LowerBound(key);
        if (it == _M_finish || it->Type > type)
            it = _M_finish;
        return it->TC;
    }
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long PresentBits[9];                       // +0x20
    CTCMap TCs;                                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                              // +0x91
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void RemoveTC(long type);                           // 0x004C9840
};
__forceinline bool IsAlive(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() && !(((CThing*)p.GetPItem())->Flags91 & 1);
}
class CThingCreatureBase : public CThing {
public:
    void MakeNoise(const CCharString& noise);           // 0x00662110
};
class CHealable {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b(); virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c(); virtual void v1d(); virtual void v1e(); virtual void v1f();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v2a(); virtual void v2b(); virtual void v2c(); virtual void v2d(); virtual void v2e(); virtual void v2f();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v3a();
    virtual void IncreaseHealth(float amount, bool showEffect);   // slot 59 (+0xEC)
};
bool __fastcall AreThingsEnemies(CThing* a, CThing* b);   // 0x0076D2F0
CThing* __fastcall CreateParticleEffect(long type, const C3DVector* pos);   // 0x0072AB70
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
};
template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CParticleEffectsDef : public CDefClassBase {
public:
    bool GetParticleType(const CCharString& name, long* type) const;   // 0x00669D30
};
template<class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
class CSpecialAbilitiesHealLifeDef : public CDefClassBase {
public:
    unsigned char _pad08[0x3c - 0x08];
    vector<float> HealthIncreasePerFrame;               // +0x3C
    vector<float> TotalHealthIncrease;                  // +0x48
    bool GetSubDef(CDefPointer<const CParticleEffectsDef>* out) const;   // 0x00669580
};
#pragma auto_inline(off)
bool CSpecialAbilitiesHealLifeDef::GetSubDef(CDefPointer<const CParticleEffectsDef>* out) const
{
    out->Object = (const CParticleEffectsDef*)this;
    return true;
}
#pragma auto_inline(on)
class CTCBase {
public:
    virtual void v00();
    CThingCreatureBase* Thing;                          // +0x04
    unsigned char _pad08[4];
};
struct CHealNode {
    CHealNode* _M_next;
    CHealNode* _M_prev;
    CIntelligentPointer<CThingCreatureBase> _M_data;
};
class CTCHealLife : public CTCBase {
public:
    CDefPointer<const CSpecialAbilitiesHealLifeDef> PDef;   // +0x0C
    float TotalHealthAccum;                             // +0x10
    unsigned long LoopingSound;                         // +0x14
    long SpellLevel;                                    // +0x18
    CIntelligentPointer<CThing> PItemRemoved;           // +0x1C
    CHealNode* ThingsToHeal;                            // +0x24 (list header node)
    vector< CIntelligentPointer<const CThing> > HealingEffects;   // +0x28
    vector<float> HealthIncreaseAccum;                  // +0x34
    CIntelligentPointer<CThingCreatureBase> PHealingTarget;   // +0x40
    CCharString GetHealingEffectName();                 // 0x007C3EB0
    void FrameUpdate();
};

void CTCHealLife::FrameUpdate()
{
    CThingCreatureBase* thing = Thing;
    {
        CCharString noise("SIMPLE_NOISE");
        thing->MakeNoise(noise);
    }
    CDefPointer<const CParticleEffectsDef> effects;
    long particleType = 0;
    CCharString effectName = GetHealingEffectName();
    long i = 0;
    CHealNode* end = ThingsToHeal;
    for (CHealNode* node = end->_M_next; node != end; node = node->_M_next) {
        CIntelligentPointer<CThingCreatureBase> target(node->_M_data);
        if (IsAlive(target) && IsAlive(PHealingTarget)) {
            if (!AreThingsEnemies(PHealingTarget.Get(), target.Get())) {
                if (!HealingEffects._M_start[i].GetPItem()
                    && PDef->GetSubDef(&effects) && effects->GetParticleType(effectName, &particleType)) {
                    CIntelligentPointer<const CThing> particle(CreateParticleEffect(particleType, &target.Get()->GetPosition()));
                    if (IS_THING_ALIVE(particle)) {
                        CThing* p = (CThing*)particle.GetPItem();
                        CTCDParticleEmitter* emitter;
                        if (p->HasTC(0x4b))
                            emitter = (CTCDParticleEmitter*)p->TCs.Find(0x4b);
                        CCharString helper("");
                        emitter->AttachToThing(*target.Get(), 0x28, helper, 0, 0.0f);
                    }
                    HealingEffects._M_start[i] = particle;
                }
                HealthIncreaseAccum._M_start[i] += PDef->HealthIncreasePerFrame._M_start[SpellLevel];
                if (HealthIncreaseAccum._M_start[i] > 1.0f) {
                    float whole = (float)floor(HealthIncreaseAccum._M_start[i]);
                    ((CHealable*)target.GetPItem())->IncreaseHealth(whole, true);
                    HealthIncreaseAccum._M_start[i] -= whole;
                }
            }
        } else if (IsAlive(HealingEffects._M_start[i])) {
            CTCDParticleEmitter* emitter = 0;
            CThing* p = (CThing*)HealingEffects._M_start[i].GetPItem();
            if (p->HasTC(0x4b))
                emitter = (CTCDParticleEmitter*)p->TCs.Find(0x4b);
            emitter->EnterTimelineEnd();
        }
        ++i;
    }
    TotalHealthAccum += PDef->HealthIncreasePerFrame._M_start[SpellLevel];
    if (TotalHealthAccum >= PDef->TotalHealthIncrease._M_start[SpellLevel] || !IsAlive(PHealingTarget))
        thing->RemoveTC(0x99);
}
