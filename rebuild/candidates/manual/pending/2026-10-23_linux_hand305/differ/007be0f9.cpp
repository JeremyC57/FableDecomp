// Grade: DIFFER(743v743) -- NEAR-MISS, one instruction pair: in the inlined fast Normalise retail scales X as `fld st(2) ; fmul st(1)` (component first, then the inverse root), ours `fld st(0) ; fmul st(3)` (root duplicated first); `X *= r`, `X = r * X` and `X = x * r` (local copies) all give ours. The table operand is a relocation.
// NThingShot::CTCShotDefinitionForcePushBall::OnCollision(ECollideType, const C3DVector&, const C3DVector&, CThing*) -- with a live shooter: spawn the impact effect, force-push a game-object thing away from the shooter (adding the pushed TC and a hit-by-ability event if needed), mark it attacked, post the attack message, make a perceiving thing indirectly aware, then fade the shield effect and kill the ball; with no shooter fall back to the base collision
// Leaf: OnCollision
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BE0F9, 743 bytes, `leave ; ret 10h` (optimize("y", off) + ("s", on) TU).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, the effect creator 0x006E0880 (__fastcall), CThing::GetPosition
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
// Shape: the no-shooter base call is the else arm (placed last); the shooter position is a named copy (user copy
//   constructor) before the subtraction; both CCharString names are block-scoped named locals (constructed before the
//   other arguments, destroyed right after the call); the message manager is a named local; GetDef has a same-TU stand-in.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern unsigned long g_InvSqrtMantissaTable[128];          // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    __forceinline C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
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
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void SetAttackedByCreature(CThing* attacker);       // 0x006624F0
    void Kill(bool flag);                               // 0x004C9B80
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
struct CTCForcePushed { void SetUpForcePush(const C3DVector& dir, float force, CThing* pusher); };   // 0x007986A0
struct CTCPerceiveThing {
    CThing* GetPerceivedThing();                        // 0x007446A0
    void SetAsIndirectlyAwareOfPerceivedThing();        // 0x007449E0
};
struct CTCPhysicalShieldSpell { void FadeOutEffect(); };   // 0x0072AC40
CThing* __fastcall CreateThingEffect(long def, const C3DVector* pos, bool flag);   // 0x006E0880


struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    __forceinline void Release() { if (--RefCount == 0) Destroy(); }
};
struct CMessageEventDef : CDefBase { };

template <class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) { }
    __forceinline ~CDefPointer() { if (Data) Data->Release(); }
    T* operator->() const { return Data; }
};
struct CThingShotDef {
    unsigned char _pad00[0x40];
    long          ImpactEffect;                         // +0x40
};
struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* thing1, CThing* thing2, long priority);   // 0x00687730
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* MessageEventManager;          // +0x60
};
struct CGameDefinitionManager {
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

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};

namespace NThingShot {

struct CTCShotDefinitionBase {
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[0x0c - 0x08];
    CBaseIntelligentPointer PEffect;                    // +0x0c
    unsigned char           _pad14[0x28 - 0x14];
    CBaseIntelligentPointer PShooter;                   // +0x28
    unsigned char           _pad30[0x38 - 0x30];
    CThingShotDef*          PShotDef;                   // +0x38
    CWorld* GetWorld();                                 // 0x00686A70
    void OnCollision(long type, const C3DVector& position, const C3DVector& normal, CThing* thing);   // 0x007BF510
};
struct CTCShotDefinitionForcePushBall : CTCShotDefinitionBase {
    unsigned char _pad3c[0xa4 - 0x3c];
    float         PushForce;                            // +0xa4
    void OnCollision(long type, const C3DVector& position, const C3DVector& normal, CThing* thing);
};

void CTCShotDefinitionForcePushBall::OnCollision(long type, const C3DVector& position, const C3DVector& normal, CThing* thing)
{
    CThing* shooter = PShooter.GetPItem();
    if (shooter) {
    if (thing) {
        if (PShotDef->ImpactEffect)
            CreateThingEffect(PShotDef->ImpactEffect, &position, false);
        if (thing->ThingTypeFlags & 8) {
            C3DVector shooterPos = shooter->GetPosition();
            C3DVector away = shooterPos - thing->GetPosition();
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
                CMessageEventManager* manager = GetWorld()->MessageEventManager;
                manager->AddMessage(def.Data, &thing->GetPosition(), shooter, thing, 1);
            }
            if (thing->HasTC(0x34)) {
                CTCPerceiveThing* perceive = thing->GetTC<CTCPerceiveThing>(0x34);
                if (perceive->GetPerceivedThing() == shooter)
                    perceive->SetAsIndirectlyAwareOfPerceivedThing();
            }
        }
    }
    if (IS_THING_ALIVE(PEffect.GetPItem()))
        ((CTCPhysicalShieldSpell*)PEffect.GetPItem())->FadeOutEffect();
    ParentThing->Kill(true);
    } else
        CTCShotDefinitionBase::OnCollision(type, position, normal, thing);
}

}
