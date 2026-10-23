// Grade: DIFFER(727v732): effects def pointer not kept in ebp (retail caches it across CalcAndLookup/Create2DEmitter and uses ebp/ebx for the filter vector bases); frame slots differ
// CTCBulletTime::UnPause() -- resume a paused bullet time: restart the looping sound, recreate the "SLOWTIME" screen particle effect if it has gone, fade the screen filter back in (the hero bypasses it) and let anything the hero carries bypass the filter too
// Leaf: UnPause
// Retail 0x0079D7E0, 732 bytes, `ret`. Callees: CDefStringTable::GetString 0x009D49B0 (g_DefStringTable 0x013CA828, by hidden
// pointer), CTCBulletTime::PlaySound 0x0079CE80, CCharString 0x0099EBF0 / dtor 0x0099EAE0, CBaseIntelligentPointer ctor
// 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CDefClassBase::GetSubDef 0x00669580, CCRC
// CalcAndLookup 0x00669D30, GFAYToVY 0x009E1E40 / GFAXToVX 0x009E1E10, CParticleEmitterManager::Get 0x00AEAAF0 /
// Create2DEmitter 0x00AEB790, CIntelligentPointer<T>(T*) out of line 0x00655318, IS_THING_ALIVE 0x0079DC20,
// vector<CScreenFilterBypassThing>::push_back 0x006A3C70 / dtor 0x006A3770, CTCBase::GetWorld 0x00686A70,
// CEnvironmentSpellEffects::ScreenFilterFadeTo 0x006B85D0, CVectorMap LowerBound 0x0040F020,
// CTCCarrying::SetAllCarriedThingsAsBypassFilter 0x006A2520.
// Layout: engine/CTCBulletTime.h; def LoopSound +0xcc, ScreenFilter{Alpha,Saturation,Contrast,Brightness,TintR,TintG,TintB}
// 12-byte vectors from +0x130; CParticleEmitter flag bytes +0x40 (bit 5 dead) / +0x44 (bit 4 persistent).
#include "engine/CTCBulletTime.h"
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
struct CRGBAFloat {
    float R, G, B, A;
    CRGBAFloat(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
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
struct CFloatArray {                                    // 12-byte vector<float>
    float* Start;
    float* Finish;
    float* EndOfStorage;
    const float& operator[](long i) const { return Start[i]; }
};
struct CSpecialAbilitiesBulletTimeDef {
    unsigned char _pad00[0xcc];
    long          LoopSound;                            // +0xcc CDefString
    unsigned char _padd0[0x130 - 0xd0];
    CFloatArray   ScreenFilterAlpha;                    // +0x130
    CFloatArray   ScreenFilterSaturation;               // +0x13c
    CFloatArray   ScreenFilterContrast;                 // +0x148
    CFloatArray   ScreenFilterBrightness;               // +0x154
    CFloatArray   ScreenFilterTintR;                    // +0x160
    CFloatArray   ScreenFilterTintG;                    // +0x16c
    CFloatArray   ScreenFilterTintB;                    // +0x178
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);   // 0x00669580 (CDefClassBase)
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
    bool IsThingAlive() const;                          // IS_THING_ALIVE 0x0079DC20
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.GetPItem(); SetPItem(item); return *this; }
};
template<> CIntelligentPointer<CParticleEmitter>::CIntelligentPointer(CParticleEmitter* item);   // 0x00655318 (out of line)
struct CThing;
struct CScreenFilterBypassThing {                       // 12 bytes
    CIntelligentPointer<CThing> Thing;
    bool                        Bypass;
    CScreenFilterBypassThing(CThing* thing, bool bypass) : Thing(thing), Bypass(bypass) {}
};
struct CScreenFilterBypassThings {                      // vector<CScreenFilterBypassThing>
    CScreenFilterBypassThing* Start;
    CScreenFilterBypassThing* Finish;
    CScreenFilterBypassThing* EndOfStorage;
    CScreenFilterBypassThings() : Start(0), Finish(0), EndOfStorage(0) {}
    ~CScreenFilterBypassThings();                       // 0x006A3770
    void push_back(const CScreenFilterBypassThing& x);  // 0x006A3C70
};
struct CTCCarrying {
    void SetAllCarriedThingsAsBypassFilter(void* key);  // 0x006A2520
};
struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
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
struct CEnvironmentSpellEffects {
    void* ScreenFilterFadeTo(long mode, long priority, float alpha, float saturation, float contrast, float brightness,
                             const CRGBAFloat& tint, const CScreenFilterBypassThings* bypass);   // 0x006B85D0
};
struct CEnvironment {
    unsigned char _pad00[0x1964];
    CEnvironmentSpellEffects* SpellEffects;             // +0x1964
};
struct CWorld {
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                         // +0x18
};

struct CTCBulletTimeImpl : CTCBulletTime {
    CThing* GetParentThing() const { return (CThing*)f4; }
    CSpecialAbilitiesBulletTimeDef* GetDef() const { return (CSpecialAbilitiesBulletTimeDef*)PDef_Object; }
    CIntelligentPointer<CParticleEmitter>& LoopingEffect()
    {
        return *(CIntelligentPointer<CParticleEmitter>*)m_tpLoopingEffect;
    }
    CWorld* GetWorld();                                 // 0x00686A70
    unsigned long PlaySound(const CCharString& name, bool loop);   // 0x0079CE80
    void UnPause();
};

void CTCBulletTimeImpl::UnPause()
{
    if (Pausable && Paused) {
        Paused = false;
        {
            CCharString sound = g_DefStringTable.GetString(GetDef()->LoopSound);
            LoopSound = PlaySound(sound, true);
        }
        if (!LoopingEffect().GetPItem() || LoopingEffect().GetPItem()->IsFinished()) {
            CDefPointer<CSpecialEffectsDef> effects;
            long effect = 0;
            if (GetDef()->GetSubDef(&effects) && effects->CalcAndLookup("SLOWTIME", &effect)) {
                CParticleEmitter* emitter = CParticleEmitterManager::Get()->Create2DEmitter(effect,
                    &C2DVector(GFAXToVX((float)g_ScreenCentreX) * 0.5f, GFAYToVY((float)g_ScreenCentreY) * 0.5f), 0x52, true,
                    CRightHandedSet());
                {
                    CIntelligentPointer<CParticleEmitter> effectPointer(emitter);
                    LoopingEffect() = effectPointer;
                }
                if (LoopingEffect().IsThingAlive())
                    LoopingEffect().GetPItem()->SetPersistent();
            }
        }
        CScreenFilterBypassThings bypass;
        bypass.push_back(CScreenFilterBypassThing(GetParentThing(), true));
        CEnvironmentSpellEffects* spellEffects = GetWorld()->PEnvironment->SpellEffects;
        CSpecialAbilitiesBulletTimeDef* def = GetDef();
        void* key = spellEffects->ScreenFilterFadeTo(0, 2, def->ScreenFilterAlpha[m_nLevel], def->ScreenFilterSaturation[m_nLevel],
            def->ScreenFilterContrast[m_nLevel], def->ScreenFilterBrightness[m_nLevel],
            CRGBAFloat(def->ScreenFilterTintR[m_nLevel], def->ScreenFilterTintG[m_nLevel], def->ScreenFilterTintB[m_nLevel], 1.0f), &bypass);
        ScreenFilterKey = key;
        CTCCarrying* carrying;
        if (GetParentThing()->GetInterface(&carrying, 0x46))
            carrying->SetAllCarriedThingsAsBypassFilter(key);
    }
}
