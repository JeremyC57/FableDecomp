// Grade: DIFFER(921v925) -- 275 difflines, coarse pass: the stack homes of the effects def pointer and the effect types are swapped (retail: single-effect branch def at [esp+14h] and type in the dead endEffectName slot; blended branch def in that slot and types at [esp+10h]/[esp+14h]; declaration order tried), which shifts every LowerBound key and CCharString temporary; the effect parameter loads before GetPosition in retail; the intelligent-pointer assignment in the blended branch calls the out-of-line CIntelligentPointer<CThingCreatureBase>::operator= 0x004A9760 where the model inlines it.
// CTCPhysicalShieldSpell::PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& effectName, const CCharString& endEffectName, CThingCreatureBase* creature, long attachType) -- spawn the named special effect at the creature (or, with an end effect, a blended effect between the two weighted by where the creature's value sits in its range), keep it in *effect when asked, and attach its emitter to the creature
// Leaf: PlayEffect
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079DEB0, 925 bytes, `ret 14h`.
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData { CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing;
class CTCDParticleEmitter {
public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    void SetAlwaysUpdate(bool on);              // 0x006E0590
    static CThing* __fastcall Create(long type, long endType, float blend, const C3DVector& position, bool flag);   // 0x006E11F0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0 dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
class CThingCreatureBase : public CThing {
public:
    float GetRangeMax();                        // 0x00662600
    float GetRangeMin();                        // 0x006625E0
    float GetRangeValue();                      // 0x00661AA0
};
CThing* __fastcall CreateParticleEffect(long type, const C3DVector& position);   // 0x0072AB70

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item);               // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& thing);   // 0x004CC340
bool __fastcall IS_THING_ALIVE(CThing* const& thing);                   // 0x0040DCE0

struct CDefVirtual {
    virtual ~CDefVirtual();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual { long m_refCount; };
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)

struct CTCPhysicalShieldSpell {
    void*          __vftable;                   // +0x00
    CThing*        ParentThing;                 // +0x04
    unsigned char  _pad08[4];
    CDefClassBase* PDef;                        // +0x0c
    void PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& effectName, const CCharString& endEffectName,
                    CThingCreatureBase* creature, long attachType);
};

void CTCPhysicalShieldSpell::PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& effectName,
                                        const CCharString& endEffectName, CThingCreatureBase* creature, long attachType)
{
    if (endEffectName == "") {
        long effectType = 0;
        CDefPointer<CSpecialEffectsDef> effects;
        if (PDef->GetSubDef(effects) && effects.m_p->GetEffectFromString(effectName, effectType)) {
            CThing* thing = CreateParticleEffect(effectType, creature->GetPosition());
            CTCDParticleEmitter* emitter;
            if (effect == 0) {
                if (thing && !(thing->m_flags91 & 1)) {
                    thing->GetInterface(&emitter, 0x4b);
                    emitter->AttachToThing(creature, attachType, "", 0, 0.0f);
                }
            } else {
                *effect = CIntelligentPointer<CThing>(thing);
                if (IS_THING_ALIVE(*effect)) {
                    effect->GetPItem()->GetInterface(&emitter, 0x4b);
                    emitter->AttachToThing(creature, attachType, "", 0, 0.0f);
                }
            }
        }
        if (effects.m_p) {
            if (--effects.m_p->m_refCount == 0)
                effects.m_p->Destroy();
        }
        return;
    }
    CDefPointer<CSpecialEffectsDef> effects;
    long startType = 0;
    long endType = 0;
    if (PDef->GetSubDef(effects) && effects.m_p->GetEffectFromString(effectName, startType) &&
        effects.m_p->GetEffectFromString(endEffectName, endType)) {
        float range = creature->GetRangeMax() - creature->GetRangeMin();
        float blend = 0.0f;
        if (range > 0.0f)
            blend = (creature->GetRangeValue() - creature->GetRangeMin()) / range;
        if (effect == 0) {
            CThing* thing = CTCDParticleEmitter::Create(startType, endType, blend, creature->GetPosition(), false);
            if (IS_THING_ALIVE(thing)) {
                CTCDParticleEmitter* emitter = 0;
                thing->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(creature, attachType, "", 0, 0.0f);
                emitter->SetAlwaysUpdate(true);
            }
        } else {
            *effect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(startType, endType, blend, creature->GetPosition(), false));
            if (IS_THING_ALIVE(*effect)) {
                CTCDParticleEmitter* emitter = 0;
                effect->GetPItem()->GetInterface(&emitter, 0x4b);
                emitter->AttachToThing(creature, attachType, "", 0, 0.0f);
                emitter->SetAlwaysUpdate(true);
            }
        }
    }
    if (effects.m_p) {
        if (--effects.m_p->m_refCount == 0)
            effects.m_p->Destroy();
    }
}
