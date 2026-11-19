// Grade: DIFFER(539v539) -- same size; the only difference is the slot of the sound-name CCharString conversion temporary: retail homes it in the sound def pointer's frame word (dead once the def is held in ebx, an LTCG effect), ours in the fire-name word
#include <math.h>
// CTCFlammable::FrameUpdate() -- lights burnt-through fire points, starts burning (fire sound + opinion source), then applies periodic fire damage until extinguished
class CCharString {
public:
    void* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefString {
public:
    operator CCharString() const;                       // 0x00415DD0
};
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
struct CSoundNameMap {
    const CDefString* Find(const CCharString& name) const;   // 0x0069D090
};
class CSoundDef : public CDefClassBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    CSoundNameMap Sounds;                               // +0x28
};
class CThingDef : public CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CSoundDef>* out) const;   // 0x0065F604
};
namespace NFire {
    class CFirePoint {
    public:
        CCharString HelperName;
        long HelperIndex;
        long EffectIndex;
        long Resistance;                                // +0x0C
        bool Burning;                                   // +0x10
        unsigned char _pad11[0x2c - 0x11];
        void SetAsBurning();                            // 0x00780540
    };
}
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 4];
    unsigned long PresentBits[9];                       // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char Flags6C;                              // +0x6C
    unsigned char _pad6d[0x70 - 0x6d];
    const CThingDef* PDef;                              // +0x70
    unsigned char _pad74[0xb4 - 0x74];
    float Health;                                       // +0xB4
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
class CThingCreatureBase : public CThing {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void TakeFireDamage(float damage, bool flag);   // slot 59 (+0xEC)
    bool SetCurrentAction(const class CCreatureActionBase& action);   // 0x006644F0
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    unsigned char _pad04[0xac - 4];
};
class CCreatureAction_StabbedToDeath : public CCreatureActionBase {
public:
    CCreatureAction_StabbedToDeath(CThingCreatureBase& creature, const CThing* killer);   // 0x008ED650
    ~CCreatureAction_StabbedToDeath();                  // 0x00693EF0
};
class COpinionReactionManager {
public:
    bool AttachOpinionSource(CThing& thing, const CCharString& name, class CTCOpinionSource** out) const;   // 0x00700780
};
class CWorld {
public:
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactions;         // +0x74
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
    CWorld* GetWorld();                                 // 0x00686A80
};
class CTCSoundEmitter : public CTCBase {
public:
    void PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
template<class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
class CFlammableDef {
public:
    unsigned char _pad00[0x2c];
    float MaxDamageCaused;                              // +0x2C
    float FireDamagePerCycle;                           // +0x30
    float FireDamagePeriod;                             // +0x34
};
extern const CCharString& g_OnFireOpinionSource;       // 0x013BADE0 (reference global)
extern long g_framesPerSecond;                          // 0x01375550
extern const float g_fEpsilon;                          // 0x0129BA3C
long __fastcall GFGetCurrentFrame();                    // 0x0049D870

// Same-TU stand-in for the out-of-line GetSubDef (retail's link-time codegen saw that the out pointer does not escape).
#pragma auto_inline(off)
bool CThingDef::GetSubDef(CDefPointer<const CSoundDef>* out) const
{
    out->Object = (const CSoundDef*)this;
    return true;
}
#pragma auto_inline(on)

class CTCFlammable : public CTCBase {
public:
    const CFlammableDef* PDef;                          // +0x0C
    vector<NFire::CFirePoint> FirePoints;               // +0x10
    bool Burning;                                       // +0x1C
    long FireDamageAfterFrame;                          // +0x20
    long FireResistance;                                // +0x24
    long FireStrength;                                  // +0x28
    float DamageCaused;                                 // +0x2C
    bool DamagePerPoint;                                // +0x30
    bool HasFirePoint;                                  // +0x31
    void ExtinguishFire();                              // 0x00780C80
    virtual void FrameUpdate();
};

void CTCFlammable::FrameUpdate()
{
    if (!PDef)
        return;
    if (HasFirePoint) {
        for (NFire::CFirePoint* point = FirePoints._M_start; point != FirePoints._M_finish; ++point) {
            if (point->Resistance <= 0) {
                if (!point->Burning)
                    point->SetAsBurning();
                if (point->Resistance <= 0)
                    FireResistance = -1;
            }
        }
    }
    if (FireResistance <= 0 && !Burning) {
        CThing* thing = Thing;
        Burning = true;
        CDefPointer<const CSoundDef> soundDef;
        if (thing->HasTC(0x10)) {
            CTCSoundEmitter* sound = (CTCSoundEmitter*)thing->GetTC(0x10);
            const CThingDef* def = thing->PDef;
            if (def->GetSubDef(&soundDef)) {
                const CDefString* name;
                {
                    CCharString fire("FIRE_DEFAULT");
                    name = soundDef->Sounds.Find(fire);
                }
                if (name)
                    sound->PlaySound(*name, 0, 0, 0, -1);
            }
        }
        CWorld* world = GetWorld();
        world->POpinionReactions->AttachOpinionSource(*Thing, g_OnFireOpinionSource, 0);
    }
    if (FireResistance <= 0 && !DamagePerPoint) {
        long frame = GFGetCurrentFrame();
        if (frame > FireDamageAfterFrame) {
            CThingCreatureBase* thing = (CThingCreatureBase*)Thing;
            if (thing->Flags6C & 4) {
                bool extinguished = false;
                if (fabs(PDef->FireDamagePerCycle + DamageCaused) >= PDef->MaxDamageCaused) {
                    ExtinguishFire();
                    extinguished = true;
                }
                DamageCaused += PDef->FireDamagePerCycle;
                thing->TakeFireDamage(PDef->FireDamagePerCycle, true);
                if (thing->Health < g_fEpsilon) {
                    if (thing->Flags6C & 8)
                        thing->SetCurrentAction(CCreatureAction_StabbedToDeath(*thing, 0));
                    if (!extinguished)
                        ExtinguishFire();
                }
            }
            FireDamageAfterFrame = (long)(g_framesPerSecond * PDef->FireDamagePeriod + frame);
        }
    }
}
