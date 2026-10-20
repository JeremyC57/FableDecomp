// CQ_RansomVictimScript::CQ_RansomVictimScript(CScriptDataBase*, CGameScriptInterface&) -- quest-script ctor: script base, interface/data pair, own vtable, the enemy-name string, the NPC comment timer and five empty scripted-thing handles
// Leaf: ??0CQ_RansomVictimScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE0320, length 182 bytes, `ret 8`; vtable 0x012DCE70; built by the factory
// 0x00DEBC60 (registered as "S_QRV", `push 0BCh` = Ego_r.pdb 0xd0 - 0x14).
// Callees: CScriptBase() 0x00CB8110, CCharString() 0x0099E4B0, CBaseObject() 0x0099A380 (each handle's root), and
// g_pGameScriptInterface (0x0143E8F8) RegisterTimer (slot 87) for the inline CTimer ctor.
// Layout (Ego_r.pdb - 0x14): ActiveEnemyName +0x64, NPCCommentTimer +0x68, seh_Guard +0x6c, seh_Father +0x7c, seh_Victim
// +0x8c, seh_Brother +0x9c, seh_Ransommer +0xac (CScriptGameResourceObjectScriptedThingBase, vtable 0x0127094C, 16 bytes).
// Shape: the two argument words come from the inline `__declspec(novtable)` intermediate base and are stored before the
// derived vtable; the handles' inline ctors run on `lea edi,[esi+N]` and zero their counted pointers from one ebx.
struct CCharString {                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
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
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c) 0x006E79E0
};
struct CScriptDataBase;

struct CScriptBase {                                     // retail 0x40 bytes (PDB 0x54: debug containers), vtable 0x012C1648
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    CScriptBase();                                       // 0x00CB8110
};

struct __declspec(novtable) CQuestScriptBase : CScriptBase {   // the argument pair stored before the derived vtable
    CGameScriptInterface* GSI;                           // +0x40
    CScriptDataBase*      Data;                          // +0x44
    CQuestScriptBase(CScriptDataBase* data, CGameScriptInterface* gsi) : CScriptBase(), GSI(gsi), Data(data) {}
};

extern CGameScriptInterface* g_pGameScriptInterface;     // 0x0143E8F8

struct CTimer {                                          // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) {}
};

struct CQ_RansomVictimScript : CQuestScriptBase {        // vtable 0x012DCE70
    bool QuestStartScreened;                             // +0x48
    bool MissionSucceeded;                               // +0x49
    bool MissionFailed;                                  // +0x4a
    bool RescuedVictim;                                  // +0x4b
    bool RansommerDied;                                  // +0x4c
    bool RansomHelperStarted;                            // +0x4d
    bool RansomHelperSpoken;                             // +0x4e
    bool ExtraAssistanceGiven;                           // +0x4f
    bool RansomHelpersAttacked;                          // +0x50
    bool ToldGuardToWaitOutside;                         // +0x51
    bool EnteredNewRegion;                               // +0x52
    bool ThanksCutsceneStarting;                         // +0x53
    long HitChief;                                       // +0x54
    long HitEvilBrotherAndProtection;                    // +0x58
    bool SomeoneDealingWithBeingHit;                     // +0x5c
    long KidnappersKilled;                               // +0x60
    CCharString ActiveEnemyName;                         // +0x64
    CTimer NPCCommentTimer;                              // +0x68
    CScriptGameResourceObjectScriptedThingBase seh_Guard;      // +0x6c
    CScriptGameResourceObjectScriptedThingBase seh_Father;     // +0x7c
    CScriptGameResourceObjectScriptedThingBase seh_Victim;     // +0x8c
    CScriptGameResourceObjectScriptedThingBase seh_Brother;    // +0x9c
    CScriptGameResourceObjectScriptedThingBase seh_Ransommer;  // +0xac
    virtual void v00();
    CQ_RansomVictimScript(CScriptDataBase* data, CGameScriptInterface& gsi);
};

CQ_RansomVictimScript::CQ_RansomVictimScript(CScriptDataBase* data, CGameScriptInterface& gsi)
    : CQuestScriptBase(data, &gsi)
{
}