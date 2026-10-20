// CWB_ScaredVillager::Main() -- the White Balverine (Knothole Glade) villager script: keep the villager scripted while conscious, run one frame, then hold an empty scripted-thing handle until the thread is told to stop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E16A80, length 199 bytes; vtable 0x012E0B28 slot 1 (Main), factory "WB_ScaredVillager" (0x00E16990,
// Ego_r.pdb 28 bytes). The catalog label CQ_HangingTreeEvilScript::BanditYells is propagated.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840, CEntityScriptBase::SetPredicate 0x00F35B10
// and CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30 (both on `this`; PDB names, landed callers also spell them
// RegisterFilter / IsDone), CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430, operator delete 0x00BFE9BC, and the
// script's game interface (+4) NewScriptFrame (slot 7).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18.
// Shape: the CIsThingConscious predicate (vtable 0x012C2FE8, IsAlive && !IsUnconscious) is a scoped frame object, its vtable
// kept in edi for the ctor and the inline dtor store; the scripted-thing handle's inline ctor (CBaseObject call, own
// vtable, zeroed counted pointer) and implicit dtor (release, CScriptGameResourceObjectBase vtable, ~CBaseObject) bracket
// the rotated wait loop.
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

struct CCPPointerInfo {                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CScriptThing {                                    // 12 bytes, vtable 0x01238C8C
    unsigned char _body[0xc];
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    ~CScriptThing();                                     // 0x004AA840
};

struct CIsThingConscious {                               // NScript predicate, vtable 0x012C2FE8: Thing alive and not unconscious
    CScriptThing Thing;                                  // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) {}
    ~CIsThingConscious() {}
    virtual bool operator()() const;                     // slot 0 (0x00CDDAB0)
};

struct CBaseObject {                                     // vtable 0x0129A7D8 {vptr, intelligent-pointer link}
    void* IntelligentPointer;                            // +0x04
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};

struct CScriptGameResourceObjectBase : CBaseObject {     // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};

struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // 16 bytes, vtable 0x0127094C
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    virtual void v01();                                  // slot 1 (0x007E72C0)
};
struct CGameScriptInterface {                           // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c) 0x006E7410
};

struct CQ_WhiteBalverineKnotholeGladeScript;
struct CQ_SunnyvaleMasterData;

struct CEntityScriptBase {                               // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    void SetPredicate(CIsThingConscious& predicate);     // 0x00F35B10
    bool IsActiveThreadTerminating() const;              // 0x00F35B30 (PDB: const)
};

struct CWB_ScaredVillager : CEntityScriptBase {
    CQ_WhiteBalverineKnotholeGladeScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*               PMasterData;   // +0x18

    void Main();
};

void CWB_ScaredVillager::Main()
{
    {
        CIsThingConscious predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}