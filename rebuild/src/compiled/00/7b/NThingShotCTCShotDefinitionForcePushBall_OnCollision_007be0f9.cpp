// NThingShot::CTCShotDefinitionForcePushBall::OnCollision(ECollideType, const C3DVector&, const C3DVector&, CThing*) -- with a live shooter: spawn the impact effect, force-push a game-object thing away from the shooter (adding the pushed TC and a hit-by-ability event if needed), mark it attacked, post the attack message, make a perceiving thing indirectly aware, then fade the shield effect and kill the ball; with no shooter fall back to the base collision
// Leaf: OnCollision
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x44 - 0x21] removed. Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: g_InvSqrtMantissaTable is GFastOneOverSqrtTable (?GFastOneOverSqrtTable@@3PAKA); ChatGPT pass 10 audit, raw Ego_r public.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Grade: RELOCATION_MATCH -- the inlined fast Normalise as `*this *= FastInvSqrt(X*X + Y*Y + Z*Z)` with an inline `operator*=(const float&)` (the landed 0x007AF0F0 / 0x008C0FB0 C3DVector model) gives retail's `fld st(2); fmul st(1)` component-first scaling; was one x87 instruction pair
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BE0F9, 743 bytes, `leave ; ret 10h` (optimize("y", off) + ("s", on) TU).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, the effect creator 0x006E0880 (__fastcall), CThing::GetPos
//   0x004C73D0, std::bitset::test 0x00410DE0, CThing::AddTC 0x004C9D60 ("CTCForcePushed" 0x0123C2AC), CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0, LowerBound 0x0040F020, CHitParameters 0x007A3BC0 / SetHitter 0x008685F0 / ~ 0x00789358, operator new
//   0x00BFEA1A, CEventHitBySpecialAbility 0x00811830, CTCEntityEvents::AddEvent 0x008113C0, CTCForcePushed::SetUpForcePush
//   0x007986A0, CThingCreatureBase::SetAttackedByCreature 0x006624F0, CGameDefinitionManager::Get 0x0044C6B0 (catalog
//   "GFGetPreMainMemoryUsed") / GetDef<CMessageEventDef> 0x007923C0 ("STANDARD_MESSAGE_EVENT_ATTACK" 0x0126E140), CTCBase::
//   GetWorld 0x00686A70, CMessageEventManager::AddMessage 0x00687730, CTCPerceiveThing::GetPerceivedThing 0x007446A0 (catalog
//   "_Cons_val<...>") / SetAsIndirectlyAwareOfPerceivedThing 0x007449E0, IS_THING_ALIVE 0x0040DCE0, CTCPhysicalShieldSpell::
//   FadeOutEffect 0x0072AC40, CThing::Kill 0x004C9B80, CTCShotDefinitionBase::OnCollision 0x007BF510.
//   Data: inverse-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: shot base PEffect +0x0c, PShooter +0x28, PShotDef +0x38 (impact effect +0x40); force-push ball push force +0xa4;
//   CThing presence bitset +0x20, interface map +0x44, type flags +0x6c; the world's message event manager +0x60.
// GetWorld (agentNF14 2026-11-11): the shot component now derives a local CTCBase (virtual dtor, ParentThing, the protected const
//   GetWorld returning CWorld&), so the call binds ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ.
// Shape: the no-shooter base call is the else arm (placed last); the shooter position is a named copy (user copy
//   constructor) before the subtraction; both CCharString names are block-scoped named locals (constructed before the
//   other arguments, destroyed right after the call); the message manager is a named local; GetDef has a same-TU stand-in.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
extern unsigned long GFastOneOverSqrtTable[128];          // 0x013CE558
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    FABLE_FORCEINLINE C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    FABLE_FORCEINLINE C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    FABLE_FORCEINLINE C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    FABLE_FORCEINLINE void operator*=(const float& s) { X *= s; Y *= s; Z *= s; }
    FABLE_FORCEINLINE void Normalise();
};
FABLE_FORCEINLINE float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
FABLE_FORCEINLINE void C3DVector::Normalise() { *this *= FastInvSqrt(X * X + Y * Y + Z * Z); }
FABLE_FORCEINLINE C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCBase;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    C3DVector const& GetPos() const;   // 0x004C73D0 (Ego_r.pdb ?GetPos@CThing@@QBEABVC3DVector@@XZ)
private: public:
    unsigned char _pad00[0x20];
    std::bitset<274> m_present;                            // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad54[0x18];
    unsigned long ThingTypeFlags;                       // +0x6c (bit 3: game object)
    bool HasTC(long type) { return m_present.test(type); }
    template <class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        return (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
    }
    CTCBase* AddTC(const CCharString& name, bool flag, void* params);   // 0x004C9D60
    void SetAttackedByCreature(CThing* attacker);       // 0x006624F0
    void Kill(bool flag);                               // 0x004C9B80
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);

class CHitParameters { public:                                 // 0x60 bytes
    unsigned char _pad[0x60];
    CHitParameters();                                   // 0x007A3BC0
    ~CHitParameters();                                  // 0x00789358
    void SetHitter(CThing* hitter);                     // 0x008685F0
};
namespace NEntityEvents {
class CEventBase { public: virtual ~CEventBase(); };
}
namespace NEntityEvents {
class CEventHitBySpecialAbility : public NEntityEvents::CEventBase { public:         // 0x18 bytes
    unsigned char _pad04[0x18 - 0x04];
    CEventHitBySpecialAbility(const CHitParameters& params, long ability);   // 0x00811830
};
}
class CTCEntityEvents { public: void AddEvent(NEntityEvents::CEventBase* event); };   // 0x008113C0
class CTCForcePushed { public: void SetUpForcePush(const C3DVector& dir, float force, CThing* pusher); };   // 0x007986A0
class CTCPerceiveThing { public:
    CThing* GetPerceivedThing();                        // 0x007446A0
    void SetAsIndirectlyAwareOfPerceivedThing();        // 0x007449E0
};
class CTCPhysicalShieldSpell { public: void FadeOutEffect(); };   // 0x0072AC40
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
};


struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    FABLE_FORCEINLINE void Release() { if (--RefCount == 0) Destroy(); }
};
class CMessageEventDef : public CDefBase { public: };

template <class T> class CDefPointer { public:
    T* Data;
    CDefPointer() : Data(0) { }
    FABLE_FORCEINLINE ~CDefPointer() { if (Data) Data->Release(); }
    T* operator->() const { return Data; }
};
class CThingShotDef { public:
    unsigned char _pad00[0x40];
    long          ImpactEffect;                         // +0x40
};
class CMessageEvent;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* thing1, CThing* thing2, long priority);   // 0x00687730
};
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* MessageEventManager;          // +0x60
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
    bool GetDef(const CCharString& name, CDefPointer<CMessageEventDef>* def);   // 0x007923C0
};
CMessageEventDef* FindRef_GetDef_0();
#pragma auto_inline(off)
bool CGameDefinitionManager::GetDef(const CCharString& name, CDefPointer<CMessageEventDef>* def)
{
    CMessageEventDef* found = FindRef_GetDef_0();
    if (!found)
        return false;
    def->Data = found;
    return true;
}
#pragma auto_inline(on)

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"   // CBaseIntelligentPointer 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (Ego_r.pdb)
#include "engine/ECollideType.h"
class CTCBase { public:                                 // the component base (members this row reads)
    virtual ~CTCBase();                                 // slot 0
    CThing*                 ParentThing;                // +0x04
protected:
    CWorld& GetWorld() const;                           // 0x00686A70 (Ego_r.pdb / FableWin.pdb IBE)
};
namespace NThingShot {

class CTCShotDefinitionBase : public CTCBase { public:
    unsigned char           _pad08[0x0c - 0x08];
    CBaseIntelligentPointer PEffect;                    // +0x0c
    unsigned char           _pad14[0x28 - 0x14];
    CBaseIntelligentPointer PShooter;                   // +0x28
    unsigned char           _pad30[0x38 - 0x30];
    CThingShotDef*          PShotDef;                   // +0x38
    virtual void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing);   // 0x007BF510 (Ego_r.pdb UAE)
};
struct CTCShotDefinitionForcePushBall : CTCShotDefinitionBase {
    unsigned char _pad3c[0xa4 - 0x3c];
    float         PushForce;                            // +0xa4
protected:
    virtual void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing);   // Ego_r.pdb MAE
};

void CTCShotDefinitionForcePushBall::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing)
{
    CThing* shooter = reinterpret_cast<CThing*>(PShooter.GetPItem());
    if (shooter) {
    if (thing) {
        if (PShotDef->ImpactEffect)
            CTCDParticleEmitter::Create(PShotDef->ImpactEffect, *(&position), false);
        if (thing->ThingTypeFlags & 8) {
            C3DVector shooterPos = ((CThing*)shooter)->GetPos();
            C3DVector away = shooterPos - ((CThing*)thing)->GetPos();
            away.Normalise();
            if (!thing->HasTC(0x8d)) {
                CTCForcePushed* pushed;
                {
                    CCharString name("CTCForcePushed");
                    pushed = (CTCForcePushed*)thing->AddTC(name, false, 0);
                }
                if (thing->HasTC(0x42)) {
                    CTCEntityEvents* events = thing->GetTC<CTCEntityEvents>(0x42);
                    CHitParameters hit;
                    hit.SetHitter(shooter);
                    events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, 1));
                }
                pushed->SetUpForcePush(-away, PushForce, shooter);
            }
            thing->SetAttackedByCreature(shooter);
            CDefPointer<CMessageEventDef> def;
            bool found;
            {
                CCharString name("STANDARD_MESSAGE_EVENT_ATTACK");
                found = CGameDefinitionManager::Get().GetDef(name, &def);
            }
            if (found) {
                CMessageEventManager* manager = GetWorld().MessageEventManager;
                manager->AddMessage(def.Data, &((CThing*)thing)->GetPos(), shooter, thing, 1);
            }
            if (thing->HasTC(0x34)) {
                CTCPerceiveThing* perceive = thing->GetTC<CTCPerceiveThing>(0x34);
                if (perceive->GetPerceivedThing() == shooter)
                    perceive->SetAsIndirectlyAwareOfPerceivedThing();
            }
        }
    }
    if (IS_THING_ALIVE(reinterpret_cast<CThing*>(PEffect.GetPItem())))
        ((CTCPhysicalShieldSpell*)reinterpret_cast<CThing*>(PEffect.GetPItem()))->FadeOutEffect();
    ParentThing->Kill(true);
    } else
        CTCShotDefinitionBase::OnCollision(type, position, normal, thing);
}

}