// CMurderTwistSheriff::CMurderTwistSheriff(CScriptOwner* parent, void* masterData, const CScriptThing& me) -- the sheriff's entity script in the murder-twist quest: entity-script base over the quest's interface and Me, then the scripted-thing handle, the attract timer, and the master data last
// Leaf: ??0CMurderTwistSheriff
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EAED90, length 97 bytes (ret 0Ch; factory 0x00EAED10 registered as
// "MurderTwistSheriff", vtable 0x012EFA0C).
// Label correction: the manifest's "CBriarRose::CBriarRose" is propagated; the factory name and Ego_r.pdb's
// CMurderTwistSheriff layout (MyEntityHandle +0x1c, AttractTimer +0x2c) identify the class.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, CBaseObject::CBaseObject @ 0x0099A380 (the
// handle's base), the timer service's virtual slot 87 (+0x15c) through the global 0x0143E8F8.
// Layout = Ego_r.pdb CMurderTwistSheriff (56 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
// PMasterData +0x18, MyEntityHandle +0x1c, AttractTimer +0x2c, NextLine +0x30 and four flags +0x34.. (left
// uninitialised by this constructor).
// Shape: as the landed CVtable_012efee8 constructor (0x00EB2000): Game and ParentClass are plain initialisers stored
// before their vtables, the handle's zero words follow its vtable through eax, the timer's constructor calls the
// service, PMasterData is a body assignment stored last.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CCountedPointerWords {                 // {Data, Info}: a member object so its zero stores follow the owner's vtable store
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass { public:            // vtable 0x01238C8C, 12 bytes (PImp_Data +4, PImp_Info +8)
    CCountedPointerWords PImp;                // +0x04
    CScriptThing() : CBaseClass(), PImp() { }
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
};
class CBaseObject { public:                          // CBaseIntelligentPointer root (vtable 0x0129A7D8, +4 nulled)
    void* m_pIntelligentPointer;              // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                // +0x08 Data, +0x0c Info
    virtual void v00();
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
};
struct CTimerManager {                        // the game-script timer service behind the global at 0x0143E8F8
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual long NewTimer();                  // slot 87 (+0x15c)
};
extern CTimerManager* g_pScriptTimerManager;  // 0x0143E8F8
class CTimer { public:                               // 4 bytes
    long TimerIndex;                          // +0x00
    CTimer() : TimerIndex(g_pScriptTimerManager->NewTimer()) { }
};
struct CScriptOwner {                         // the quest script that owns this entity script
    unsigned char _pad00[0x40];
    void*         m_pInterface;               // +0x40 CGameScriptInterfaceBase*
};
class CEntityScriptBase { public:                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    void*        Game;                        // +0x04 (a plain initialiser: stored before the vtable)
    CScriptThing Me;                          // +0x08
    CEntityScriptBase(const CScriptThing& thing, void* game) : Game(game), Me(thing) { }
};

class CMurderTwistSheriff : public CEntityScriptBase {   // vtable 0x012EFA0C
public:
    CScriptOwner* ParentClass;                // +0x14 CV_MurderTwistFinalScript*
    void*         PMasterData;                // +0x18 CQ_SunnyvaleMasterData*
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    CTimer        AttractTimer;               // +0x2c
    unsigned long NextLine;                   // +0x30
    bool          HasGivenReminderToHero;     // +0x34
    bool          HasBeingRumbledCutscenePlayed;   // +0x35
    bool          HasExpressedSuspicionToHero;     // +0x36
    bool          HasSaidWellDoneToHero;      // +0x37
    virtual void v00();
    CMurderTwistSheriff(CScriptOwner* parent, void* masterData, const CScriptThing& me);
};

CMurderTwistSheriff::CMurderTwistSheriff(CScriptOwner* parent, void* masterData, const CScriptThing& me)
    : CEntityScriptBase(me, parent->m_pInterface), ParentClass(parent), MyEntityHandle(), AttractTimer()
{
    PMasterData = masterData;
}