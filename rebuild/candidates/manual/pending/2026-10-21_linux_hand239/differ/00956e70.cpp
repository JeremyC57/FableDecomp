// CAIStateGroup_BriarRose::Update() -- raise Briar Rose's physical shield, turn invulnerable towards the enemy and schedule the next run from the def's wait time
// Grade: DIFFER(363v363) -- register choice only: the sub-def holder lands in ebx (pushed with edi up front) where retail keeps it in edi and pushes ebx mid-function for the shield/creature; frame packing matches once GetSubDef has its same-TU stand-in
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00956E70, length 363 bytes.
// Callees: CAIStateGroup_BriarRose::IsStillRunnable @ 0x00956A70 (x2), Terminate @ 0x008FCF40 (manifest "CXMVPlaybackInfo"),
//          CDefClassBase::GetSubDef<CBriarRoseDef> @ 0x008E7790 (landed), the def's slot 1 release, GetPWorld @ 0x008FCFC0 and
//          the static frame counter @ 0x0049D870, __ftol2 @ 0x00BFEA70, CCharString(const char*, long) @ 0x0099EBF0
//          ("CTCPhysicalShieldSpell" @ 0x0123C418), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60,
//          ~CCharString @ 0x0099EAE0, CCreatureAction_BriarRoseIntoInvulnerable(creature, target) @ 0x00957210 (vtable 0x01294B8C; the manifest label ForcePush is wrong), CBaseIntelligentPointer
//          ::GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10, AIAction_PerformActionAndWait @ 0x0097DB70, the action base dtor @ 0x0062E900.
// Globals: frames per second @ 0x01375550 (int 15), 30.0f @ 0x01248018.
// Shape: the creature def is add-ref'd by hand around GetSubDef with the plain out slot zeroed between the load and
//          the null test; the sub-def is then held in a register by a releasing holder (released at the end without
//          a null test: it was dereferenced). NextRunFrame = now + fps * wait (the world fetched first, fps a named local
//          read before the static frame call, ftol); the shield TC gets mode 2 and a 30 s duration; the creature for
//          the action is a named local read before the enemy's GetPItem.
// Layout: engine/CAIStateGroup_BriarRose.h (PEnemy +0x14, NextRunFrame +0x1C) behind the engine/CAIStateGroupBase.h base;
//          creature PDef +0x70; CBriarRoseDef SecondsBetweenTeleportAway +0x4C (PDB); the shield TC's mode +0x10 and duration +0x1C.
#include "engine/CAIStateGroup_BriarRose.h"

extern "C" long __cdecl _ftol2(double);
struct CThing;
struct CTCParamsBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};
struct CBriarRoseDef : CDefPointeeBase {
    unsigned char _08[0x4c - 0x08];
    float SecondsBetweenTeleportAway;                             // +0x4c seconds between runs
};
struct SDefOut { CDefPointeeBase* Object; };   // the GetSubDef out slot (plain)
struct CBriarRoseDefHolder {                    // takes over the out slot's reference
    CBriarRoseDef* Object;
    CBriarRoseDefHolder(CDefPointeeBase* object) : Object((CBriarRoseDef*)object) { }
    ~CBriarRoseDefHolder()
    {
        if (--Object->m_refCount == 0)
            Object->Delete();
    }
    CBriarRoseDef* operator->() const { return Object; }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(SDefOut* out);               // 0x008E7790 GetSubDef<CBriarRoseDef> (stand-in body below)
    CDefPointeeBase* FindSubDef(const char* name);   // the index lookup + manager fetch (add-refs its result)
};
#pragma auto_inline(off)
// Same-TU stand-in for CDefClassBase::GetSubDef<CBriarRoseDef> (0x008E7790, landed; retail's LTCG saw its body): it only
// stores the add-ref'd sub-def through the out slot, so the slot's address does not escape and shares the frame.
__declspec(noinline) bool CDefClassBase::GetSubDef(SDefOut* out)
{
    CDefPointeeBase* sub = FindSubDef("CBriarRoseDef");
    if (!sub)
        return false;
    out->Object = sub;
    return true;
}
#pragma auto_inline(on)

struct CTCPhysicalShieldSpell {
    unsigned char _00[0x10];
    long Mode;                                  // +0x10
    unsigned char _14[0x08];
    long DurationFrames;                        // +0x1c
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CThing {
    unsigned char _00[0x70];
    CDefClassBase* PDef;                        // +0x70 CDefPointer<const CThingCreatureDef>::Object
    CTCPhysicalShieldSpell* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};

struct CCreatureActionBase {
    unsigned char _body[0x114];
    ~CCreatureActionBase();                     // 0x0062E900
};
struct CCreatureAction_BriarRoseIntoInvulnerable : CCreatureActionBase {
    CBaseIntelligentPointer PTarget;            // +0x114
    CCreatureAction_BriarRoseIntoInvulnerable(CThing* creature, CThing* target);   // 0x00957210 (vtable 0x01294B8C)
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CThingWorld {
    static long GetCurrentFrame();              // 0x0049D870
};
extern long g_FramesPerSecond;                  // 0x01375550

struct CAIStateGroupBriarRoseMethods : CAIStateGroup_BriarRose {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CBaseIntelligentPointer& Enemy() { return *(CBaseIntelligentPointer*)PEnemy; }
    bool IsStillRunnable();                     // 0x00956A70
    void Terminate();                           // 0x008FCF40
    CThingWorld* GetPWorld();                   // 0x008FCFC0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    void Update();
};

void CAIStateGroupBriarRoseMethods::Update()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CDefClassBase* creatureDef = Brain()->PCreature->PDef;
    SDefOut out;
    out.Object = 0;
    if (creatureDef)
        ++creatureDef->m_refCount;
    creatureDef->GetSubDef(&out);
    if (creatureDef) {
        if (--creatureDef->m_refCount == 0)
            creatureDef->Delete();
    }
    CBriarRoseDefHolder def(out.Object);
    float wait = def->SecondsBetweenTeleportAway;
    {
        CThingWorld* world = GetPWorld();
        long fps = g_FramesPerSecond;
        NextRunFrame = (long)((float)world->GetCurrentFrame() + (float)fps * wait);
    }
    CTCPhysicalShieldSpell* shield = Brain()->PCreature->AddTC("CTCPhysicalShieldSpell", true, 0);
    shield->Mode = 2;
    shield->DurationFrames = (long)((float)g_FramesPerSecond * 30.0f);
    {
        CThing* creature = Brain()->PCreature;
        AIAction_PerformActionAndWait(&CCreatureAction_BriarRoseIntoInvulnerable(creature, Enemy().GetPItem()), false);
    }
    if (IsStillRunnable()) {
        CThingWorld* world = GetPWorld();
        long fps = g_FramesPerSecond;
        NextRunFrame = (long)((float)world->GetCurrentFrame() + (float)fps * wait);
    }
    Terminate();
}
