// CAIStateGroup_BriarRoseWillUse::Update() -- use Briar Rose's will on a live enemy: force-push it when close, shoot far into it otherwise; then schedule the next use
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956FE0, length 389 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10, Terminate @ 0x008FCF40 (manifest "CXMVPlaybackInfo"),
//          CDefClassBase::GetSubDef<CBriarRoseDef> @ 0x008E7790 (landed; same-TU stand-in below), the def's slot 1 release,
//          CThing::GetPos @ 0x004C73D0, CCreatureAction_BriarRoseForcePush(creature, target) @ 0x009572A0 (vtable 0x01294CD4),
//          CCreatureAction_BriarRoseShotFarInto(creature, target) @ 0x00957170 (landed), AIAction_PerformActionAndWait @ 0x0097DB70,
//          the combat action base dtor @ 0x0062E900, GetPWorld @ 0x008FCFC0, the static frame counter @ 0x0049D870, __ftol2.
// Globals: frames per second @ 0x01375550 (int 15).
// Shape: IS_THING_ALIVE on PEnemy (two GetPItem); the creature def is add-ref'd by hand around GetSubDef and the same
//          register then holds the sub-def (released by hand after Terminate, no null test); the push threshold and the
//          use interval are copied to float locals; when the enemy's +0x6C bit 3 is set, the XY distance is compared
//          inline against threshold^2 and the two action calls share their hoisted `push 0 ; push target`; each arm
//          reads the creature through an inline GetCreature() accessor (a plain `Brain()->PCreature` gets hoisted above
//          the branch).
// Layout: engine/CAIStateGroup_BriarRoseWillUse.h (PEnemy +0x14, NextWillUseFrame +0x1C) behind engine/CAIStateGroupBase.h;
//          creature PDef +0x70; CBriarRoseDef (PDB): SecondsBetweenWillUse +0x48, DistanceForcePushThreshold +0x50.
#include "engine/CAIStateGroup_BriarRoseWillUse.h"

class CThing;
class C3DVector { public: float x, y, z; };
class C2DVector { public:
    float x, y;
    float GetLengthSquared() const { return x * x + y * y; }
};

class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};
class CBriarRoseDef : public CDefPointeeBase { public:
    unsigned char _08[0x48 - 0x08];
    float SecondsBetweenWillUse;                // +0x48
    float SecondsBetweenTeleportAway;           // +0x4c
    float DistanceForcePushThreshold;           // +0x50
};
struct SDefOut { CDefPointeeBase* Object; };   // the GetSubDef out slot (plain)
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(SDefOut* out);               // 0x008E7790 GetSubDef<CBriarRoseDef> (stand-in body below)
    CDefPointeeBase* FindSubDef(const char* name);   // the index lookup + manager fetch (add-refs its result)
};
#pragma auto_inline(off)
// Same-TU stand-in for CDefClassBase::GetSubDef<CBriarRoseDef> (0x008E7790, landed; retail's LTCG saw its body): it only
// stores the add-ref'd sub-def through the out slot, so the slot's address does not escape.
__declspec(noinline) bool CDefClassBase::GetSubDef(SDefOut* out)
{
    CDefPointeeBase* sub = FindSubDef("CBriarRoseDef");
    if (!sub)
        return false;
    out->Object = sub;
    return true;
}
#pragma auto_inline(on)

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThing { public:
    unsigned char _00[0x6c];
    unsigned char m_flags6C;                    // +0x6c bit 3 tested
    unsigned char _6d[0x70 - 0x6d];
    CDefClassBase* PDef;                        // +0x70
    unsigned char _74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

namespace N_0062e900 { struct CCreatureActionBase {
    unsigned char _body[0x114];
    ~CCreatureActionBase();                     // 0x0062E900
}; } using N_0062e900::CCreatureActionBase;
class CCreatureAction_BriarRoseForcePush : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PTarget;            // +0x114
    CCreatureAction_BriarRoseForcePush(CThing* creature, CThing* target);   // 0x009572A0
};
class CCreatureAction_BriarRoseShotFarInto : public CCreatureActionBase { public:
    CCreatureAction_BriarRoseShotFarInto(CThing* creature, CThing* target);   // 0x00957170
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
extern long g_FramesPerSecond;                  // 0x01375550

struct CAIStateGroupBriarRoseWillUseMethods : CAIStateGroup_BriarRoseWillUse {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointer<CThing>& Enemy() { return *(CIntelligentPointer<CThing>*)PEnemy; }
    void Terminate();                           // 0x008FCF40
    CWorld* GetPWorld();                   // 0x008FCFC0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    void Update();
};

void CAIStateGroupBriarRoseWillUseMethods::Update()
{
    if (!IS_THING_ALIVE(Enemy())) {
        Terminate();
        return;
    }
    CDefPointeeBase* def = Brain()->PCreature->PDef;
    SDefOut out;
    out.Object = 0;
    if (def)
        ++def->m_refCount;
    ((CDefClassBase*)def)->GetSubDef(&out);
    if (def) {
        if (--def->m_refCount == 0)
            def->Delete();
    }
    def = out.Object;
    float threshold = ((CBriarRoseDef*)def)->DistanceForcePushThreshold;
    float interval = ((CBriarRoseDef*)def)->SecondsBetweenWillUse;
    if (Enemy().GetPItem()->m_flags6C & 8) {
        CThing* enemy = Enemy().GetPItem();
        CThing* creature = Brain()->PCreature;
        CThing* target = Enemy().GetPItem();
        const C3DVector& enemyPos = creature->GetPos();
        const C3DVector& pos = target->GetPos();
        C2DVector delta;
        delta.x = pos.x - enemyPos.x;
        delta.y = pos.y - enemyPos.y;
        if (delta.GetLengthSquared() < threshold * threshold)
            AIAction_PerformActionAndWait(&CCreatureAction_BriarRoseForcePush(GetCreature(), enemy), false);
        else
            AIAction_PerformActionAndWait(&CCreatureAction_BriarRoseShotFarInto(GetCreature(), enemy), false);
    }
    {
        CWorld* world = GetPWorld();
        long fps = g_FramesPerSecond;
        NextWillUseFrame = (long)((float)world->GetFrame() + (float)fps * interval);
    }
    Terminate();
    if (--def->m_refCount == 0)
        def->Delete();
}