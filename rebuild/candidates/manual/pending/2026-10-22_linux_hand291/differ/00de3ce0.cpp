// Grade: DIFFER(676v676) -- scratch-register rotation only: the two inline ~CScriptThing 'delete Info' pushes use ecx/edx (retail eax/ecx) and RemoveThing's vtable eax (retail edx); 12 masked lines
// CNPCComment::Main() -- Ransom Victim quest NPC comments: alive filter on Me, find the victim's father's guard and the victim by script name, then each frame let each of them make their NPC comment once (when alive and within the quest's comment distance of Me); once both have spoken, remove Me
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE3CE0, 676 bytes (+ int3 padding); entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840 (also inlined at the two loop exits), ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC (the counted pointer's info block), SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CScriptThing::GetDataString
//   slot 3 (+0x0c, 0x004AA900) on Me, the implementation's IsAlive slot 75 (+0x12c),
//   CQ_RansomVictimScript::MakeNPCComment 0x00DE3F90 (`ret 0Ch`); GameCalls slots 7 NewScriptFrame, 72 GetThingWithScriptName
//   (by value), 108 RemoveThing. Literals "VictimsFatherGuard" 0x012DD060, "Victim" 0x012CA958.
// Layout: Ego_r.pdb CNPCComment (32 bytes): entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18,
//   VictimsFatherGuardSaidIt +0x1c, VictimSaidIt +0x1d. The comment distance is a long at +0x54 of the script def global
//   0x0143E90C (as in the landed CAttackingAssassin::Main 0x00E383A0).
// Shape: the mixed inline/out-of-line ~CScriptThing (GOTCHAS chunk 282/advisor b18): the dtor is __forceinline, the filter
//   class sits in an inline_depth(0) region so its member dtor stays a call, and the in-loop returns sit in an
//   inline_depth(0) region so they call 0x004AA840; the rotated while's exit expands both dtors.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC

struct CCPPointerInfo {                                // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
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
struct CBaseClass {
    CBaseClass();                                      // 0x0099A2D0
    virtual ~CBaseClass();                             // 0x0099A2E0
};
struct CScriptThingImpl {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022(); virtual void i023();
    virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029(); virtual void i030(); virtual void i031();
    virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037(); virtual void i038(); virtual void i039();
    virtual void i040(); virtual void i041(); virtual void i042(); virtual void i043(); virtual void i044(); virtual void i045(); virtual void i046(); virtual void i047();
    virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051(); virtual void i052(); virtual void i053(); virtual void i054(); virtual void i055();
    virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059(); virtual void i060(); virtual void i061(); virtual void i062(); virtual void i063();
    virtual void i064(); virtual void i065(); virtual void i066(); virtual void i067(); virtual void i068(); virtual void i069(); virtual void i070(); virtual void i071();
    virtual void i072(); virtual void i073(); virtual void i074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
struct CScriptThing : CBaseClass {                     // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImpl> PImp;            // +0x04
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    __forceinline ~CScriptThing() {}                   // out of line 0x004AA840
    virtual void s001(); virtual void s002();
    virtual CCharString GetDataString() const;         // slot 3 (+0x0c), 0x004AA900
    bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // 0x004AB130
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CGameScriptInterface {                          // vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) 0x008A7D60
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool b1, bool b2) const;   // slot 108 (+0x1b0) 0x008910D0
};

#pragma inline_depth(0)
struct CIsThingAlive {                                 // vtable 0x012C32F0: a predicate holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual void v00();
    ~CIsThingAlive() { }
};
#pragma inline_depth()

struct CEntityScriptBase {                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void SetPredicate(CIsThingAlive& predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CScriptDef {
    unsigned char _pad[0x54];
    long NPCCommentDistance;                           // +0x54
};
extern CScriptDef* g_pScriptDef;                       // 0x0143E90C
enum ENPCComment { NPC_COMMENT_0, NPC_COMMENT_1 };
struct CQ_RansomVictimScript {
    bool MakeNPCComment(const CCharString& name, const CScriptThing& speaker, ENPCComment comment);   // 0x00DE3F90
};
struct CNPCComment : CEntityScriptBase {
    CQ_RansomVictimScript* ParentClass;                // +0x14
    void*                  PMasterData;                // +0x18
    bool VictimsFatherGuardSaidIt;                     // +0x1c
    bool VictimSaidIt;                                 // +0x1d
    void Main();
};

void CNPCComment::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    long distance = g_pScriptDef->NPCCommentDistance;
    CScriptThing guard = Game->GetThingWithScriptName("VictimsFatherGuard");
    CScriptThing victim = Game->GetThingWithScriptName("Victim");
    while (!IsActiveThreadTerminating()) {
        if (!VictimsFatherGuardSaidIt && guard.IsAlive() && IsDistanceBetweenThingsUnder(Me, guard, (float)distance)) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating()) return;
#pragma inline_depth()
            if (ParentClass->MakeNPCComment(Me.GetDataString(), guard, NPC_COMMENT_1)) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating()) return;
#pragma inline_depth()
                VictimsFatherGuardSaidIt = true;
            }
        }
        if (!VictimSaidIt && victim.IsAlive() && IsDistanceBetweenThingsUnder(Me, victim, (float)distance)) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating()) return;
#pragma inline_depth()
            if (ParentClass->MakeNPCComment(Me.GetDataString(), victim, NPC_COMMENT_1)) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating()) return;
#pragma inline_depth()
                VictimSaidIt = true;
            }
        }
        if (VictimsFatherGuardSaidIt && VictimSaidIt) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating()) return;
            Game->RemoveThing(Me, false, true);
            return;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}
