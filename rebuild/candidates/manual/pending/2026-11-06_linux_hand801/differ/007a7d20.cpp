// CTCBattleCharge::CreateHitEffect() -- the charge's impact: make a noise, gather the creatures in the radius of effect (plus the dragon in DragonCliff2, over the def's extra radius), hit each one not yet hit and not incapacitated with the level's damage (scaled down with distance unless it is the collided thing; enemies only for game objects), tell its entity events, then the radial blur at the impact point, the collision sound, the level's on-hit / displacement effects, the debris effect, and a rumble for the main player
// Grade: DIFFER(2504v2520) -- the frame is 0x140 v 0x148 (VC7.1 slot packing: the one-byte temporaries -- the CanBeHitByEpicSpell functor, the find tag, the && result -- share [esp+13h] in retail and sit apart here, and the first search's box / the dragon search's In3DRange swap places), the box values are stored as computed (retail computes all four in registers, then stores), and the inline sound-player lookup keeps begin in ebx with a copy in esi (ours begin in esi, end in ebx); 64 lines, 10 attempts
// Next idea: VC7.1 frame packing: a scope for the first search that keeps the box but not the coord (a6's block moved the frame 0x150 -> 0x140), then the begin / first copy of the expanded LowerBound (by-value std::lower_bound helpers did not give it, a5)
// Leaf: ?CreateHitEffect@CTCBattleCharge@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A7D20, 2520 bytes, `ret` (aligned frame `and esp,-8 ; sub esp,148h`); four-entry
//   SpellLevel switch table at 0x007A86E8.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, CThingCreatureBase::MakeNoise 0x00662110, CThing::GetPos 0x004C73D0,
//   _ftol2 0x00BFEA70, CTCBase::PeekThingSearchTools 0x00686D90 / GetWorldMap 0x00686D30 / GetWorld 0x00686A70,
//   CThingSearchTools::GetAllThingsInRoughArea instances 0x007A9300 (In3DRange, IsNot, CanBeHitByEpicSpell) and 0x007A9580
//   (In3DRange, IsNot, HasInstantiationName), CWorldMap::GetLoadedRegion 0x004FB150 / GetRegion 0x004FB490, CRegion copy
//   0x0051A900 / ~CRegion 0x0051A110, CBasicString<char>::operator== 0x004115A0, vector::_M_insert_overflow 0x007A91A0,
//   free 0x00BFEA14, NCreatureMode::IsIncapacitated 0x0085F0C0, std::find 0x006C4060, CIntelligentPointer 0x00A01B10 /
//   0x00A01B90 / 0x00A01B50 / 0x00A01C10, CDefString::operator== 0x005B3440 (def slot 10 GetInstantiationName),
//   CTCEnemy::AreThingsEnemies 0x0076D2F0, CHitParameters 0x00789238 / AddDamageWithAttribute 0x00868CA0 / ~ 0x00789358,
//   CThing::OnHit (slot 65), CVectorMap::LowerBound 0x0040F020, operator new 0x00BFEA1A,
//   NEntityEvents::CEventHitBySpecialAbility 0x00811830, CTCEntityEvents::AddEvent 0x008113C0,
//   CEnvironmentSpellEffectInterpolator::RadialBlurFadeTo 0x006B8730 / RadialBlurSetCenterWorldPos 0x006B8310 /
//   RadialBlurFadeOut 0x006B82E0, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00, CDefClassBase::GetSubDef 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CreateParticleEffect 0x0072AB70, CThing::GetPlayer 0x004C7A30,
//   CPlayer::IsMainPlayer 0x00487520, GFGetSystemManager 0x009A4EC0, CThing::GetJoystickDeviceNumber 0x004C7CA0,
//   CJoystick::AddRumble (slot 17).
// Layout: Ego_r.pdb CTCBattleCharge (the landed 0x007A8BE0 model); CSpecialAbilitiesBattleChargeDef (PDB less 4, then less 4
//   after each vector): BattleChargeDamage +0x58, BattleChargeRadialStartSeconds +0x68 .. BattleChargeRadialTwist +0x84,
//   DragonExtraGumphRadiusMultiplier +0x9C, DragonExtraGumphRadius +0xA0.
#include <string.h>
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

class CTCBase;
enum ETCInterfaceType { };
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

extern char g_EmptyString[];                                // 0x0129AAF4
extern const unsigned long g_CRCTable[256];                 // 0x0129A168
class CBasicStringChar { public:
    const char*   PData;                                    // +0x00
    unsigned long StringLength;                             // +0x04
    bool operator==(const char* text) const;                // 0x004115A0 ??8?$CBasicString@D@@QBE_NPBD@Z
};
class CCharString { public:
    CBasicStringChar* PStringData;                          // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetAsCharArray() const { return PStringData == 0 ? g_EmptyString : PStringData->PData; }
    bool operator==(const char* text) const { if (!PStringData) { if (strcmp(text, "") == 0) return true; return false; } return *PStringData == text; }
};
class CDefString { public:
    long TablePos;
    bool operator==(const char* text) const;                // 0x005B3440
};

__forceinline unsigned long CalcCRC(unsigned long seed, const char* data, unsigned long length)
{
    if (length == 0)
        return seed;
    for (unsigned long index = 0; index < length; ++index)
        seed = g_CRCTable[((const unsigned char*)data)[index] ^ (seed & 0xff)] ^ (seed >> 8);
    return seed;
}

class CDefBase { public:
    virtual ~CDefBase();                                    // slot 0
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct SEffectNode {                                        // STLport _Rb_tree_node<pair<const unsigned long, long> >
    long          Color;
    SEffectNode*  Parent;                                   // +0x04
    SEffectNode*  Left;                                     // +0x08
    SEffectNode*  Right;                                    // +0x0c
    unsigned long Key;                                      // +0x10
    long          Value;                                    // +0x14
};
struct CEffectMap {
    SEffectNode* Header;
    SEffectNode* end() const { return Header; }
    __forceinline SEffectNode* find(const unsigned long& key) const
    {
        SEffectNode* y = Header;
        SEffectNode* x = Header->Parent;
        while (x != 0) {
            if (!(x->Key < key)) {
                y = x;
                x = x->Left;
            } else
                x = x->Right;
        }
        if (y == Header || key < y->Key)
            y = Header;
        return y;
    }
};
class CSpecialEffectsDef : public CDefBase { public:
    unsigned char _pad08[0x28 - 0x08];
    CEffectMap    Effects;                                  // +0x28 map<unsigned long, long>
    bool GetEffectFromString(const CCharString& name, long& effect) const;   // 0x00669D30
    __forceinline bool GetEffectFromStringInline(const CCharString& name, long& effect) const
    {
        unsigned long len = name.GetLength();
        const char* str = name.GetAsCharArray();
        unsigned long key = CalcCRC(0, str, len);
        SEffectNode* it = Effects.find(key);
        if (it != Effects.end()) {
            effect = it->Value;
            return true;
        }
        return false;
    }
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30, another TU): a same-TU noinline body (EXTRA_RULES chunk 422) that writes
// the out value only, so VC7.1 knows the caller's locals are not captured (the landed 0x008DDAD0 model).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& effect) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    effect = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);       // stand-in callee
// Stand-in body (out of line in retail @ 0x00669580): it writes the out pointer only (the landed 0x008DDAD0 model).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CThingBaseDef : public CDefClassBase { public:
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09();
    virtual CDefString GetInstantiationName() const;        // slot 10
};
class CSpecialAbilitiesBattleChargeDef : public CDefClassBase { public:
    unsigned char      _pad08[0x58 - 0x08];
    std::vector<float> BattleChargeDamage;                  // +0x58
    float BattleChargeCombatMultiplierFactor;               // +0x64
    float BattleChargeRadialStartSeconds;                   // +0x68
    float BattleChargeRadialEndSeconds;                     // +0x6C
    float BattleChargeRadialThreshold;                      // +0x70
    float BattleChargeRadialLength;                         // +0x74
    float BattleChargeRadialIntensity;                      // +0x78
    float BattleChargeRadialFade;                           // +0x7C
    float BattleChargeRadialRange;                          // +0x80
    float BattleChargeRadialTwist;                          // +0x84
    float PoseDelaySecs;                                    // +0x88
    float ChargeDelay;                                      // +0x8C
    std::vector<long>  NumPeopleHit;                        // +0x90
    float DragonExtraGumphRadiusMultiplier;                 // +0x9C
    float DragonExtraGumphRadius;                           // +0xA0
};

namespace NHitLocations { class CHitLocation; }
class CThing;
class CHitParameters { public:                              // 0x60 bytes in retail
    unsigned char _pad00[0x24];
    bool CreateDustParticleEffectOnHit;                     // +0x24
    unsigned char _pad25[0x60 - 0x25];
    CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon,
                   const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable,
                   bool flourish, bool blockCounter);       // 0x00789238
    ~CHitParameters();                                      // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);   // 0x00868CA0 (EDamageAttribute)
    void SetCreateDustParticleEffectOnHit(bool value) { CreateDustParticleEffectOnHit = value; }
};
class CEventBase { public:
    virtual ~CEventBase();
};
namespace NEntityEvents {
class CEventHitBySpecialAbility : public CEventBase { public:   // 0x18 bytes
    unsigned char _pad04[0x18 - 0x04];
    CEventHitBySpecialAbility(const CHitParameters& params, long ability);   // 0x00811830
};
}
class CTCEntityEvents { public:                             // interface 0x42
    void AddEvent(CEventBase* event);                       // 0x008113C0
};
class CTCSoundPlayer { public:                              // interface 0x10
    unsigned long PlayAttachedCriteria(const CCharString& criteria, bool a, bool b, bool c, long d);   // 0x0066BD00
};
class CPlayer { public:
    bool IsMainPlayer() const;                              // 0x00487520
};

// STLport __lower_bound shape (first / last by value, the distance, the halving loop), expanded in place here
// (0x007A833F); the other interface lookups of this row call CVectorMap::LowerBound 0x0040F020.
template <class I> __forceinline long DistanceInline(I first, I last) { return last - first; }
template <class I, class K, class D> __forceinline I LowerBoundAux(I first, I last, const K& key, D*)
{
    D len = DistanceInline(first, last);
    while (len > 0) {
        D half = len / 2;
        I middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}
template <class I, class K> __forceinline I LowerBoundInline(I first, I last, const K& key)
{
    return LowerBoundAux(first, last, key, (long*)0);
}
class CBaseObject { public:
    virtual ~CBaseObject();
};
class CThing : public CBaseObject { public:
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
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
    virtual void OnHit(CHitParameters& hit);                // slot 65 (+0x104)
    unsigned char  _pad04[0x10 - 0x04];
    unsigned char  Type;                                    // +0x10 CClass::Type
    unsigned char  _pad11[0x20 - 0x11];
    unsigned long  UsingComponents[9];                      // +0x20 bitset<274>
    CInterfaceMap  Components;                              // +0x44
    unsigned char  _pad54[0x6c - 0x54];
    unsigned long  ThingTypeFlags;                          // +0x6C
    CThingBaseDef* PBaseDef;                                // +0x70 CDefPointer<const CThingBaseDef>
    unsigned char  _pad74[0x91 - 0x74];
    bool           Dead : 1;                                // +0x91 bit 0
    bool IsDead() const { return Dead; }
    const CThingBaseDef& PeekBaseDef() const { return *PBaseDef; }
    const C3DVector& GetPos() const;                        // 0x004C73D0
    CPlayer& GetPlayer();                                   // 0x004C7A30
    long GetJoystickDeviceNumber() const;                   // 0x004C7CA0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &Components;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        if (it == map->end() || type < it->first)
            it = map->end();
        return (T*)it->second;
    }
    template <class T> __forceinline T* GetTCInline(long type)
    {
        CInterfaceMap::value_type* it = LowerBoundInline(Components.begin(), Components.end(), type);
        if (it == Components.end() || type < it->first)
            it = Components.end();
        return (T*)it->second;
    }
};
class CThingPhysical : public CThing { };
class CThingCreatureBase : public CThingPhysical { public:
    void MakeNoise(const CCharString& noise);               // 0x00662110
};

class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
template <class T> __forceinline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    return p.Get() != 0 && !p.Get()->IsDead();
}

namespace std {
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                  const random_access_iterator_tag&);   // 0x006C4060
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return find(__first, __last, __val, random_access_iterator_tag());
}
}

namespace NCreatureMode { bool __fastcall IsIncapacitated(const CThing& thing); }   // 0x0085F0C0
class CTCEnemy { public:
    static bool __fastcall AreThingsEnemies(const CThing& a, const CThing& b);   // 0x0076D2F0
};
CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position);   // 0x0072AB70

#include "engine/C2DBoxI.h"
class CThingFilter_In3DRange { public:
    C3DVector Pos;
    float     Range;
    CThingFilter_In3DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
};
class CThingFilter_IsNot { public:
    const CThing* Thing;
    CThingFilter_IsNot(const CThing* thing) : Thing(thing) { }
};
struct CThingFilter_CanBeHitByEpicSpell { };
class CThingFilter_HasInstantiationName { public:
    const CCharString& InstantiationName;
    CThingFilter_HasInstantiationName(const CCharString& name) : InstantiationName(name) { }
};
template <class A, class B, class C> class CFilter_Bind3 { public:
    const A* F0;
    const B* F1;
    const C* F2;
    CFilter_Bind3(const A& a, const B& b, const C& c) : F0(&a), F1(&b), F2(&c) { }
};
class CThingSearchTools { public:
    template <class T, class Container, class Filter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const Filter& filter, Container& result) const;
};
CThing* GetPThingInArea_StandIn(const C2DBoxI& area, long index);   // stand-in callee
bool ThingFilter_StandIn(const void* filter, const CThing& thing);  // stand-in callee
// Stand-in body (the real instances 0x007A9300 / 0x007A9580 follow this row in the retail TU): a same-TU noinline body
// (EXTRA_RULES chunk 422) that writes only the result container, so VC7.1 knows the caller's vectors are not captured.
#pragma auto_inline(off)
template <class T, class Container, class Filter>
__declspec(noinline) void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const Filter& filter, Container& result) const
{
    for (long index = 0; ; ++index) {
        CThing* thing = GetPThingInArea_StandIn(area, index);
        if (!thing)
            break;
        if ((thing->ThingTypeFlags & 3) == 3 && ThingFilter_StandIn(&filter, *thing))
            result.push_back(static_cast<T*>(thing));
    }
}
#pragma auto_inline(on)

class CRegion { public:
    unsigned char _pad00[0x18];
    CCharString   Name;                                     // +0x18
    unsigned char _pad1c[0x58 - 0x1c];
    CRegion(const CRegion& other);                          // 0x0051A900
    ~CRegion();                                             // 0x0051A110
    const CCharString& GetName() const { return Name; }
};
class CWorldMap { public:
    long GetLoadedRegion() const;                           // 0x004FB150
    CRegion& GetRegion(long region);                        // 0x004FB490
};
class CEnvironmentSpellEffectInterpolator;
class CEnvironment { public:
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffectInterpolator* SpellEffectInterpolator;   // +0x1964 scoped_ptr
    CEnvironmentSpellEffectInterpolator& GetSpellEffectInterpolator() { return *SpellEffectInterpolator; }
};
class CWorld { public:
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                             // +0x18 scoped_ptr
    CEnvironment& GetEnvironment() { return *PEnvironment; }
};
namespace NScreenEffect { enum ScreenEffectPriority { }; }
class CEnvironmentSpellEffectInterpolator { public:
    void* RadialBlurFadeTo(float time, NScreenEffect::ScreenEffectPriority priority, float threshold, float length,
                           float intensity, float fade, float range, float twist);   // 0x006B8730
    void RadialBlurSetCenterWorldPos(void* key, const C3DVector& pos);              // 0x006B8310
    void RadialBlurFadeOut(float time, void* key);                                  // 0x006B82E0
};
class CJoystick { public:
    class CInitRumble { public:
        CInitRumble(float maxLevel, float smoothness, float attackTime, float decayTime, bool cumulative)
            : MaxLevel(maxLevel), Smoothness(smoothness), AttackTime(attackTime), DecayTime(decayTime), Cumulative(cumulative) { }
        float MaxLevel;
        float Smoothness;
        float AttackTime;
        float DecayTime;
        bool  Cumulative;
    };
    virtual void JoystickVSlot00(); virtual void JoystickVSlot01(); virtual void JoystickVSlot02(); virtual void JoystickVSlot03();
    virtual void JoystickVSlot04(); virtual void JoystickVSlot05(); virtual void JoystickVSlot06(); virtual void JoystickVSlot07();
    virtual void JoystickVSlot08(); virtual void JoystickVSlot09(); virtual void JoystickVSlot10(); virtual void JoystickVSlot11();
    virtual void JoystickVSlot12(); virtual void JoystickVSlot13(); virtual void JoystickVSlot14(); virtual void JoystickVSlot15();
    virtual void JoystickVSlot16();
    virtual void AddRumble(long type, const CInitRumble& rumble, bool replace) = 0;   // slot 17 (+0x44)
};
template <class T> class CCountedPointer { public:
    T*    Object;
    long* RefCount;
    T* operator->() const { return Object; }
};
class CInputManager { public:
    unsigned char _pad00[0x2C];
    std::vector<CCountedPointer<CJoystick> > PJoysticks;    // +0x2C
};
class CSystemManager { public:
    unsigned char  _pad00[0x58];
    CInputManager* PInputManager;                           // +0x58 CCountedPointer
    CInputManager* GetInputManager() { return PInputManager; }
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0

class CBaseClass { public:
    virtual ~CBaseClass();
};
class CTCBase : public CBaseClass { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
protected:
    CWorld& GetWorld() const;                               // 0x00686A70
    CWorldMap& GetWorldMap();                               // 0x00686D30
    const CThingSearchTools& PeekThingSearchTools() const;  // 0x00686D90
};

#pragma pack(push, 1)
class CTCBattleCharge : public CTCBase { public:           // interface type 0xA8
    CIntelligentPointer<CThing>                   PTarget;                    // +0x0C
    CDefPointer<CSpecialAbilitiesBattleChargeDef> PDef;                       // +0x14
    C3DVector                                     PositionStartedFrom;        // +0x18
    CIntelligentPointer<CThing>                   CollidedThing;              // +0x24
    bool                                          StartedMoving;              // +0x2C
    bool                                          JustReleased;               // +0x2D
    bool                                          DirectionOverride;          // +0x2E
    C3DVector                                     DirectionOverrideVector;    // +0x2F
    unsigned char                                 _pad3b;                     // +0x3B
    float                                         DashDistance;               // +0x3C
    long                                          SpellLevel;                 // +0x40
    CIntelligentPointer<CThing>                   PItemRemoved;               // +0x44
    float                                         RadiusOfEffect;             // +0x4C
    CIntelligentPointer<CThing>                   ShieldEffect;               // +0x50
    CIntelligentPointer<CThing>                   LeftFeetEffect;             // +0x58
    CIntelligentPointer<CThing>                   RightFeetEffect;            // +0x60
    long                                          PoseDelay;                  // +0x68
    bool                                          StartPoseDelay;             // +0x6C
    bool                                          HitSomething;               // +0x6D
    unsigned char                                 _pad6e[2];                  // +0x6E
    std::vector<CIntelligentPointer<CThing> >     PeopleHit;                  // +0x70
    C3DVector                                     DisplacementVector;         // +0x7C
    bool                                          IsZTargetting;              // +0x88
private:
    void CreateHitEffect();
};
#pragma pack(pop)

void CTCBattleCharge::CreateHitEffect()
{
    CThing* parent = ParentThing;
    static_cast<CThingCreatureBase*>(parent)->MakeNoise("SIMPLE_NOISE");
    std::vector<CThingPhysical*> things;
    {
    const C3DVector& pos = parent->GetPos();
    C2DCoordI searchCentre((long)pos.X, (long)pos.Y);
    C2DBoxI area(searchCentre, (long)RadiusOfEffect);
    PeekThingSearchTools().GetAllThingsInRoughArea<CThingPhysical>(area,
        CFilter_Bind3<CThingFilter_In3DRange, CThingFilter_IsNot, CThingFilter_CanBeHitByEpicSpell>(
            CThingFilter_In3DRange(parent->GetPos(), RadiusOfEffect), CThingFilter_IsNot(parent),
            CThingFilter_CanBeHitByEpicSpell()),
        things);
    }
    float range = RadiusOfEffect;
    CRegion region = GetWorldMap().GetRegion(GetWorldMap().GetLoadedRegion());
    if (region.GetName() == "DragonCliff2") {
        std::vector<CThingPhysical*> dragons;
        range = PDef->DragonExtraGumphRadiusMultiplier * RadiusOfEffect + PDef->DragonExtraGumphRadius;
        const C3DVector& dragonPos = parent->GetPos();
        C2DCoordI dragonCentre((long)dragonPos.X, (long)dragonPos.Y);
        C2DBoxI dragonArea(dragonCentre, (long)range);
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingPhysical>(dragonArea,
            CFilter_Bind3<CThingFilter_In3DRange, CThingFilter_IsNot, CThingFilter_HasInstantiationName>(
                CThingFilter_In3DRange(parent->GetPos(), range), CThingFilter_IsNot(parent),
                CThingFilter_HasInstantiationName("CREATURE_DRAGON_01")),
            dragons);
        for (unsigned long d = 0; d < dragons.size(); ++d)
            things.push_back(dragons[d]);
    }
    for (unsigned long i = 0; i < things.size(); ++i) {
        float damage = PDef->BattleChargeDamage[SpellLevel];
        CThingPhysical* thing = things[i];
        if (thing == parent)
            continue;
        if (!NCreatureMode::IsIncapacitated(*thing)
            && std::find(PeopleHit.begin(), PeopleHit.end(), CIntelligentPointer<CThing>(thing)) == PeopleHit.end()) {
            const C3DVector& parentPos = parent->GetPos();
            const C3DVector& thingPos = thing->GetPos();
            C3DVector diff;
            diff.X = thingPos.X - parentPos.X;
            diff.Y = thingPos.Y - parentPos.Y;
            diff.Z = thingPos.Z - parentPos.Z;
            if (CollidedThing.Get() != thing) {
                float dist = diff.GetMagnitude();
                float radius = (thing->PeekBaseDef().GetInstantiationName() == "CREATURE_DRAGON_01") ? range : RadiusOfEffect;
                damage *= 1.0f - dist / radius;
            }
            if (thing->ThingTypeFlags & 8) {
                if (IS_THING_ALIVE(PTarget) && IsZTargetting) {
                    if (PTarget.Get() != thing && !CTCEnemy::AreThingsEnemies(*PTarget.Get(), *thing))
                        continue;
                } else if (!CTCEnemy::AreThingsEnemies(*parent, *thing))
                    continue;
            }
            CHitParameters hit(0, parent, 0, parent->GetPos(), diff, true, false, false, false);
            hit.AddDamageWithAttribute(0, damage);
            hit.SetCreateDustParticleEffectOnHit(false);
            thing->OnHit(hit);
            if (thing->HasTC(0x42)) {
                CTCEntityEvents* events = thing->GetTC<CTCEntityEvents>(0x42);
                events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 12));
            }
        }
    }
    C3DVector centre = parent->GetPos();
    if (IS_THING_ALIVE(CollidedThing) && (CollidedThing.Get()->ThingTypeFlags & 8))
        centre = CollidedThing.Get()->GetPos();
    CEnvironmentSpellEffectInterpolator& blur = GetWorld().GetEnvironment().GetSpellEffectInterpolator();
    void* key = blur.RadialBlurFadeTo(PDef->BattleChargeRadialStartSeconds, (NScreenEffect::ScreenEffectPriority)2,
                                      PDef->BattleChargeRadialThreshold, PDef->BattleChargeRadialLength,
                                      PDef->BattleChargeRadialIntensity, PDef->BattleChargeRadialFade,
                                      PDef->BattleChargeRadialRange, PDef->BattleChargeRadialTwist);
    blur.RadialBlurSetCenterWorldPos(key, centre);
    blur.RadialBlurFadeOut(PDef->BattleChargeRadialEndSeconds, key);
    if (parent->HasTC(0x10)) {
        CTCSoundPlayer* sound = parent->GetTCInline<CTCSoundPlayer>(0x10);
        sound->PlayAttachedCriteria("HERO_ABILITY_BATTLE_CHARGE_COLLISION", true, false, false, -1);
    }
    CDefPointer<CSpecialEffectsDef> effects;
    long effect = 0;
    if (PDef->GetSubDef(effects)) {
        switch (SpellLevel) {
        case 0:
            if (effects->GetEffectFromString("ONHIT_EFFECT_LEVEL0", effect))
                CreateParticleEffect(effect, centre);
            if (effects->GetEffectFromString("ONHIT_EFFECT_DISPLACEMENT_LEVEL0", effect))
                CreateParticleEffect(effect, centre);
            break;
        case 1:
            if (effects->GetEffectFromString("ONHIT_EFFECT_LEVEL1", effect))
                CreateParticleEffect(effect, centre);
            if (effects->GetEffectFromString("ONHIT_EFFECT_DISPLACEMENT_LEVEL1", effect))
                CreateParticleEffect(effect, centre);
            break;
        case 2:
            if (effects->GetEffectFromString("ONHIT_EFFECT_LEVEL2", effect))
                CreateParticleEffect(effect, centre);
            if (effects->GetEffectFromString("ONHIT_EFFECT_DISPLACEMENT_LEVEL2", effect))
                CreateParticleEffect(effect, centre);
            break;
        case 3:
            if (effects->GetEffectFromString("ONHIT_EFFECT_LEVEL3", effect))
                CreateParticleEffect(effect, centre);
            if (effects->GetEffectFromString("ONHIT_EFFECT_DISPLACEMENT_LEVEL3", effect))
                CreateParticleEffect(effect, centre);
            break;
        }
        if (effects->GetEffectFromStringInline("ONHIT_DEBRIS_EFFECT", effect))
            CreateParticleEffect(effect, centre);
    }
    if (ParentThing->Type == 2 && ParentThing->GetPlayer().IsMainPlayer()) {
        CThing* player = ParentThing;
        GFGetSystemManager()->GetInputManager()->PJoysticks[player->GetJoystickDeviceNumber()]->AddRumble(2,
            CJoystick::CInitRumble(1.0f, 0.0f, 0.0f, 0.4f, false), false);
    }
}
