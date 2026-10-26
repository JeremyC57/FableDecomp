// CArenaSpawn::Main() -- arena spawn point: its data string is the spawn-slot index; while the script runs, wait for the parent script to flag that slot, play the SUMMON_ARENA effect at the spawn (unless the arena is in state 3), clear the flag and wait a frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F1B690, length 361 bytes (plain `ret`; vtable slot 1 = Main of the entity
//          script contract).
// Callees: CScriptThing slot 3 GetDataString (CCharString by hidden pointer) and slot 6 GetPos, GFCharStringToInt
//          @ 0x0099E7F0 (__fastcall), ~CCharString @ 0x0099EAE0, CCharString(const char*, long = -1) @ 0x0099EBF0,
//          ~CScriptThing @ 0x004AA840, CEntityScriptBase::IsDone @ 0x00F35B30, the game interface's slots 7 NewScriptFrame,
//          87 RegisterTimer, 88 DeregisterTimer, 100 CreateEffect (cgsi_slots_aligned.tsv); data g_pGameScriptInterface
//          @ 0x0143E8F8; literals "SUMMON_ARENA" 0x012F9D00 and "" 0x0122D70E.
// Shape: the timer handle is registered on the global interface; the inner wait loop's IsDone exit and the ArenaState
//          branch's IsDone exit both `goto stop` (one shared DeregisterTimer tail, retail copy 2 with eax), while the
//          after-wait IsDone check deregisters and returns inline and is tail-merged with the loop fallthrough (copy 1,
//          edx). Explicit returns at all three sites (or break at the middle one) put the inner exit into copy 1 or swap
//          the copies' registers; this goto/return mix is the only one of 36 exit-form combinations that matches. The slot
//          index stays in ebp, the timer in ebx with a frame spill across CreateEffect (which loads its vtable into ebx);
//          the CScriptThing result is a named local (destroyed after the two string temporaries).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14; the parent script's arena state +0xa8
//          and per-slot spawn flags (bytes) at +0xe2 (descriptive names). engine/CScriptThing.h and
//          engine/CGameScriptInterface.h flatten the vptr into a data member, but this row calls their virtual slots
//          (GetDataString/GetPos, NewScriptFrame/RegisterTimer/DeregisterTimer/CreateEffect), so polymorphic local
//          declarations are used.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
class C3DVector { public: float x, y, z; };

class CCPPointerInfo;                                   // CCountedPointer's shared block
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
};
class CBaseClass { public:                                      // polymorphic root of the script handles
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    ~CScriptThing();                                     // 0x004AA840
    virtual void v01() const; virtual void v02() const;
    virtual CCharString GetDataString() const;           // slot 3 (+0x0c)
    virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
};
long __fastcall GFCharStringToInt(const CCharString& text);                                                   // 0x0099E7F0
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
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
    virtual long RegisterTimer();   // slot 87 (+0x15c)
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160)
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099();
    virtual CScriptThing CreateEffect(const CCharString& name, const C3DVector& pos, const CCharString& param, float scale, bool a, bool b) const;   // slot 100 (+0x190)
};

extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

struct CQ_ArenaScript {
    unsigned char _pad[0xa8];
    long ArenaState;                                     // +0xa8
    unsigned char _pad2[0xe2 - 0xac];
    bool SpawnFlags[1];                                  // +0xe2 [slot]
};

struct CArenaSpawn {
    virtual ~CArenaSpawn();
    virtual void Main();                                 // slot 1
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    CQ_ArenaScript* ParentClass;                         // +0x14
    bool IsDone();                                       // 0x00F35B30
};

void CArenaSpawn::Main()
{
    long slot = GFCharStringToInt(Me.GetDataString());
    long timer = g_pGameScriptInterface->RegisterTimer();
    while (!IsDone()) {
        while (!ParentClass->SpawnFlags[slot]) {
            Game->NewScriptFrame();
            if (IsDone()) goto stop;
        }
        if (IsDone()) {
            g_pGameScriptInterface->DeregisterTimer(timer);
            return;
        }
        if (ParentClass->ArenaState != 3) {
            if (IsDone()) goto stop;
            CScriptThing effect = Game->CreateEffect("SUMMON_ARENA", Me.GetPos(), "", 0.0f, false, false);
        }
        ParentClass->SpawnFlags[slot] = false;
        Game->NewScriptFrame();
    }
    g_pGameScriptInterface->DeregisterTimer(timer);
    return;
stop:
    g_pGameScriptInterface->DeregisterTimer(timer);
}