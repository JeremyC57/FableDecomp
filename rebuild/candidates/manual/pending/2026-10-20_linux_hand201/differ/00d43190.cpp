// CQ_AwakeningTheOracleScript::CPretendGuard::Main() -- register an is-alive functor over Me, run a frame, then keep trying (a frame at a time) to start scripting the pretend guard through a fresh entity handle, and idle until the script ends
// Grade: DIFFER(233v226) -- loop layout only: retail enters the final `while (!IsDone()) NewScriptFrame();` by jumping to its bottom test (2-byte `mov edi,edi` alignment pad before the body); the lane copies the IsDone test above the body (while, for(;;)+break and an explicit goto-to-test do/while all compile the same)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D43190, length 226 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb `?Main@CPretendGuard@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ`; vtable 0x012CE330 slot 1.
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (the functor's member; the functor's inline dtor
// re-stores its vtable 0x012C2FE8 first), the entity script's functor registration 0x00F35B10 and IsDone 0x00F35B30
// (both on `this`, labels propagated as in the landed CNOVI_CreatedBeetle::Main 0x00DB80C0), the script interface
// (+4) slots 7 NewScriptFrame (+0x1C) and 8 StartScriptingEntity(const CScriptThing&,
// CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority) (+0x20), CBaseObject::CBaseObject 0x0099A380, the
// handle's IsScripting 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0 (as the landed CFH_BriarRose::Main 0x00D37330).
// Layout: Ego_r.pdb CPretendGuard (28 bytes) = CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass
// (CQ_AwakeningTheOracleScript*) +0x14, PMasterData +0x18. The handle (vtable 0x0127094C, 0x10 bytes) is
// CBaseObject + PImp {Data +8, Info +0xC}.
// Shape: the functor is a scoped frame object; the handle's inline constructor stores its vtable before the PImp
// member's zero words; `&Me` stays in edi; both waits are rotated loops (`while (!Start...) { frame; if (IsDone())
// return; }` then `while (!IsDone()) frame;`), the early return sharing the handle-destructor exit.
struct CScriptThing {                                   // 12 bytes
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing();                                    // 0x004AA840
};
struct CScriptThingFunctor_00ef7ab0 {                   // vtable 0x012C2FE8: an is-alive functor holding a script thing
    virtual void v00();
    CScriptThing m_thing;                               // +0x04
    CScriptThingFunctor_00ef7ab0(const CScriptThing& thing) : m_thing(thing) { }
    ~CScriptThingFunctor_00ef7ab0() { }
};

struct CBaseObject {                                    // CBaseIntelligentPointer root (vtable 0x0129A7D8, +4 nulled)
    virtual void v00();
    void* m_pIntelligentPointer;                        // +0x04
    CBaseObject();                                      // 0x0099A380
};
struct CCountedPointerWords {                           // {Data, Info}: a member object so its zero stores follow the owner's vtable store
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                          // +0x08 Data, +0x0c Info
    virtual void v00();
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };

struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority);   // slot 8 (+0x20)
};

namespace NScript {
struct CQ_SunnyvaleMasterData;
struct CQ_AwakeningTheOracleScript {
    struct CPretendGuard {
        void*                        __vftable;         // +0x00
        CGameScriptInterface*        Game;              // +0x04
        CScriptThing                 Me;                // +0x08
        CQ_AwakeningTheOracleScript* ParentClass;       // +0x14
        CQ_SunnyvaleMasterData*      PMasterData;       // +0x18
        void RegisterFunctor(CScriptThingFunctor_00ef7ab0& functor);   // 0x00F35B10
        bool IsDone();                                  // 0x00F35B30
        void Main();
    };
};
}

void NScript::CQ_AwakeningTheOracleScript::CPretendGuard::Main()
{
    {
        CScriptThingFunctor_00ef7ab0 functor(Me);
        RegisterFunctor(functor);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    while (!IsDone())
        Game->NewScriptFrame();
}
