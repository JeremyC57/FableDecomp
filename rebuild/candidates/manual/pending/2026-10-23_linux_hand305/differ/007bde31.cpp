// Grade: DIFFER(474v555) -- prologue, burn and hit-event blocks line up; retail re-uses the dead `thing` argument slot for the 0x42 key and the level def (ours keeps it, the slot being address-taken), keeps the power-up level in ebx (ours spills it and hoists a zero register), and gives each switch arm its own CCharString slot (+0x10/+0xc/+8/-4) where VC7.1 here tail-merges the four arms
// NThingShot::CTCShotDefinitionFireballSpell::OnCollision(ECollideType, const C3DVector&, const C3DVector&, CThing*) -- base collision, then set fire to a flammable thing, tell a live thing's entity events it was hit by the fireball, and play the explosion sound for the spell's power-up level
// Leaf: OnCollision
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BDE31, 555 bytes, `leave ; ret 10h` (optimize("y", off) + ("s", on) TU).
// Callees: CTCShotDefinitionBase::OnCollision 0x007BF510, std::bitset::test 0x00410DE0, LowerBound 0x0040F020,
//   CTCBurntEffect::Reset 0x007CDD10, CThing::AddTC 0x004C9D60, CCharString 0x0099EBF0 / ~ 0x0099EAE0, IS_THING_ALIVE
//   0x0040DCE0, CHitParameters 0x007A3BC0 / ~ 0x00789358, CHitParameters::SetHitter 0x008685F0 (catalog
//   "CVillageTaskCarryCrate::AssignCreatureToTask", propagated), operator new 0x00BFEA1A, CEventHitBySpecialAbility
//   0x00811830 (landed), CTCEntityEvents::AddEvent 0x008113C0, GetSubDef<CFireballSpellLevelDef> 0x007BE88C (landed),
//   CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00.
// Layout: shot base PShooter +0x28, PShotDef +0x38; CThing presence bitset +0x20, interface map +0x44, type flags +0x6c
//   (as the landed CTCMultiStrike::FrameUpdate 0x007CCE20); CFireballSpellLevelDef power-up level +0x28.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                                // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                            // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                       // +0x6c (bit 3: game object)
    bool HasTC(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    CTCBase* AddTC(const CCharString& name, bool flag, void* params);   // 0x004C9D60
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);

struct CHitParameters {                                 // 0x60 bytes
    unsigned char _pad[0x60];
    CHitParameters();                                   // 0x007A3BC0
    ~CHitParameters();                                  // 0x00789358
    void SetHitter(CThing* hitter);                     // 0x008685F0
};
struct CEventBase { virtual ~CEventBase(); };
namespace NEntityEvents {
struct CEventHitBySpecialAbility : CEventBase {         // 0x18 bytes
    unsigned char _pad04[0x18 - 0x04];
    CEventHitBySpecialAbility(const CHitParameters& params, long ability);   // 0x00811830
};
}
struct CTCEntityEvents { void AddEvent(CEventBase* event); };   // 0x008113C0
struct CTCBurntEffect { void Reset(); };                // 0x007CDD10
struct CTCSoundPlayer {
    void PlayAttachedCriteria(const CCharString& name, bool flag, long a, long b, long c);   // 0x0066BD00
    __forceinline void Play(const char* text)
    {
        CCharString name(text);
        PlayAttachedCriteria(name, true, 0, 0, -1);
    }
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    __forceinline void Release() { if (--RefCount == 0) Destroy(); }
};
struct CFireballSpellLevelDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    long PowerUpLevel;                                  // +0x28
};
template <class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) { }
    __forceinline ~CDefPointer() { if (Data) Data->Release(); }
    T* operator->() const { return Data; }
};
struct CThingShotDef {
    bool GetSubDef(CDefPointer<CFireballSpellLevelDef>* out);   // 0x007BE88C
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};

namespace NThingShot {

struct CTCShotDefinitionBase {
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[0x28 - 0x08];
    CBaseIntelligentPointer PShooter;                   // +0x28
    unsigned char           _pad30[0x38 - 0x30];
    CThingShotDef*          PShotDef;                   // +0x38
    void OnCollision(long type, const C3DVector& position, const C3DVector& normal, CThing* thing);   // 0x007BF510
};
struct CTCShotDefinitionFireballSpell : CTCShotDefinitionBase {
    void OnCollision(long type, const C3DVector& position, const C3DVector& normal, CThing* thing);
};

void CTCShotDefinitionFireballSpell::OnCollision(long type, const C3DVector& position, const C3DVector& normal, CThing* thing)
{
    CThing* hit = thing;
    CTCShotDefinitionBase::OnCollision(type, position, normal, hit);
    if (hit && (hit->ThingTypeFlags & 8)) {
        if (hit->HasTC(0x89))
            hit->GetTC<CTCBurntEffect>(0x89)->Reset();
        else {
            CCharString name("CTCBurntEffect");
            hit->AddTC(name, true, 0);
        }
    }
    if (IS_THING_ALIVE(thing) && hit->HasTC(0x42)) {
        CTCEntityEvents* events = hit->GetTC<CTCEntityEvents>(0x42);
        CHitParameters hit;
        hit.SetHitter(PShooter.GetPItem());
        events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 0x10));
    }
    CDefPointer<CFireballSpellLevelDef> levelDef;
    long level = 0;
    if (PShotDef->GetSubDef(&levelDef))
        level = levelDef->PowerUpLevel;
    CThing* parent = ParentThing;
    if (parent->HasTC(0x10)) {
        CTCSoundPlayer* sound = parent->GetTC<CTCSoundPlayer>(0x10);
        switch (level) {
        case 0: sound->Play("HERO_ABILITY_FIREBALL_EXPLOSION_NO_POWERUP"); break;
        case 1: sound->Play("HERO_ABILITY_FIREBALL_EXPLOSION_LEVEL_1"); break;
        case 2: sound->Play("HERO_ABILITY_FIREBALL_EXPLOSION_LEVEL_2"); break;
        case 3: sound->Play("HERO_ABILITY_FIREBALL_EXPLOSION_LEVEL_3"); break;
        }
    }
}

}
