// CAIStateGroup_WhisperPotionCombat::DisappearJump() -- jump into the sky, hide at the jump-to position for half a second, then land
// Leaf: DisappearJump
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096AF60; SW2 sweep (advisor/0096af60/probe/n3.cpp).
struct CThingCreatureBase;
struct C3DVector { float x, y, z; };
struct CThingGraphic {
    virtual void g00();
    virtual void g01();
    virtual void g02();
    virtual void g03();
    virtual void g04();
    virtual void g05();
    virtual void g06();
    virtual void g07();
    virtual void g08();
    virtual void g09();
    virtual void g10();
    virtual void g11();
    virtual void g12();
    virtual void g13();
    virtual void g14();
    virtual void g15();
    virtual void g16();
    virtual void g17();
    virtual void g18();
    virtual void g19();
    virtual void g20();
    virtual void g21();
    virtual void g22();
    virtual void g23();
    virtual void g24();
    virtual void g25();
    virtual void g26();
    virtual void g27();
    virtual void g28();
    virtual void g29();
    virtual void SetPosition(const C3DVector* pos);     // slot 30 (+0x78)
    virtual void g31();
    virtual void g32();
    virtual void g33();
    virtual void g34();
    virtual void g35();
    virtual void g36();
    virtual void g37();
    virtual void g38();
    virtual void g39();
    virtual void g40();
    virtual void g41();
    virtual void g42();
    virtual void SetFlags(long flags);          // slot 43 (+0xac)
    virtual void Restore();                     // slot 44 (+0xb0)
};
struct CThing {
    virtual void v00();
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
    virtual void SetActive(bool active);        // slot 28 (+0x70)
    unsigned char _pad04[0x60 - 0x04];
    CThingGraphic* PGraphic;                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {        // CIntelligentPointer<CThing> (vtable 0x01238C6C)
    virtual void v00();
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                         // 0x00693EF0
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x114 - 0xa8];
    CCombatActionBase(CThingCreatureBase* creature, CThing* target, long action, const CSoundPair& sound);   // 0x00857F30
    ~CCombatActionBase();                           // 0x0062E900
    void SetPDef(const CCharString& name);          // 0x008574D0
};
struct CCreatureAction_WhisperJumpIntoSky : CCombatActionBase {   // vtable 0x012969E4
    virtual void v00();
    __forceinline CCreatureAction_WhisperJumpIntoSky(CThingCreatureBase* creature, CThing* target)
        : CCombatActionBase(creature, target, 0x96, CSoundPair()) { SetPDef("JUMP_INTO_SKY"); }
};
struct CCreatureAction_WhisperLand : CCombatActionBase {   // ctor 0x008CCBD0
    CCreatureAction_WhisperLand(CThingCreatureBase* creature);
};
struct CThingCreature : CThing { };
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                      // +0x18
};
struct CWorld {
    static long GetWorldFrame();                    // 0x0049D870
};
extern long g_FramesPerSecond;                      // 0x01375550
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld() const;                  // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
    long AIAction_DoNothing();                  // 0x0097E200
};
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
struct CAIStateGroup_WhisperPotionCombat : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;           // +0x14
    vector<CIntelligentPointerThing> TrapThings;    // +0x1c
    long FrameLastRun;                          // +0x28
    C3DVector Jump_to_pos;                      // +0x2c
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    void DisappearJump();
    bool JumpIntoSky();
};

__forceinline bool CAIStateGroup_WhisperPotionCombat::JumpIntoSky()
{
    long result;
    do {
        if (PTarget.GetPItem() == 0 || (PTarget.GetPItem()->m_flags91 & 1))
            return false;
        CCreatureAction_WhisperJumpIntoSky action(GetCreature(), PTarget.GetPItem());
        result = AIAction_PerformActionAndWait(&action, false);
    } while (!IsStopping() && result != 2);
    if (IsStopping())
        return false;
    return true;
}

void CAIStateGroup_WhisperPotionCombat::DisappearJump()
{
    if (!JumpIntoSky()) {
        Terminate();
        return;
    }
    PBrain->PCreature->PGraphic->SetPosition(&Jump_to_pos);
    PBrain->PCreature->SetActive(false);
    PBrain->PCreature->PGraphic->SetFlags(0x20);
    long start = GetPWorld()->GetWorldFrame();
    long now;
    do {
        AIAction_DoNothing();
        now = GetPWorld()->GetWorldFrame();
    } while (!IsStopping() && (float)(now - start) < (float)g_FramesPerSecond * 0.5f);
    PBrain->PCreature->PGraphic->Restore();
    PBrain->PCreature->SetActive(true);
    AIAction_PerformActionAndWait(&CCreatureAction_WhisperLand(GetCreature()), false);
}