// CV_MurderTwistScript::MagicRuneProcess() -- once the quest passes state 1, every time the timer has run out while no region has unloaded, restart it and rumble each magic rune within 15 units (squared distance) of the hero
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA8C20, 0x221 bytes, plain `ret`.
// Callees: IsActiveThreadTerminating 0x00CB7940, CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   GetDistanceBetweenThings 0x00CBE512 (__fastcall, as in the landed CGuardInWoods / NecropolisGate code),
//   ~CScriptThing 0x004AA840; the timer runs through the global interface g_pGameScriptInterface 0x0143E8F8 (slots 87
//   RegisterTimer / 88 DeregisterTimer / 89 SetTimer / 90 GetTimer); Game slots 7 NewScriptFrame, 23 MsgOnRegionUnloaded,
//   70 GetHero, 106 CreateRumble; CScriptThing virtual slot 6 (+0x18) the position accessor. 15.0f is 0x0124E640.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, MurderTwistState +0x4c, MagicRuneRumbleList +0x54 (vector<CScriptThing>).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharStringData { public: const char* m_pText; unsigned long m_length; };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C
    virtual ~CScriptThing();                            // 0x004AA840
    virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual const C3DVector& GetPosition() const;       // slot 6 (+0x18)
    void* PImpData;
    void* PImpInfo;
};
float __fastcall GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE512
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
class CGameScriptInterface { public:                       // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame(void) const;   // slot 7 (+0x1c) 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual bool MsgOnRegionUnloaded(CCharString &) const;   // slot 23 (+0x5c) 0x00893470
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing & GetHero(void);   // slot 70 (+0x118) 0x00891CA0
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer(void);   // slot 87 (+0x15c) 0x006E79E0
    virtual void DeregisterTimer(long);   // slot 88 (+0x160) 0x006E7690
    virtual void SetTimer(long, long);   // slot 89 (+0x164) 0x006E76E0
    virtual long GetTimer(long) const;   // slot 90 (+0x168) 0x006E7710
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual CScriptThing CreateRumble(C3DVector const &, float, float, CCharString) const;   // slot 106 (+0x1a8) 0x0089FDA0
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) {}
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CV_MurderTwistScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* PMasterData;                                  // +0x44
    long MurderTwistItemsFound;                         // +0x48
    long MurderTwistState;                              // +0x4c
    long MurderTwistEvent;                              // +0x50
    std::vector<CScriptThing> MagicRuneRumbleList;      // +0x54
    void MagicRuneProcess();
};
} // namespace NScript
using NScript::CV_MurderTwistScript;

void NScript::CV_MurderTwistScript::MagicRuneProcess()
{
    while (!(MurderTwistState > 1)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CTimer timer;
    CCharString region;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->MsgOnRegionUnloaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (timer.GetTime() == 0) {
                if (IsActiveThreadTerminating())
                    return;
                timer.SetTime(1);
                for (unsigned int i = 0; i < MagicRuneRumbleList.size(); ++i) {
                    if (IsActiveThreadTerminating())
                        return;
                    CScriptThing* start = MagicRuneRumbleList._M_start;
                    if (GetDistanceBetweenThings(Game->GetHero(), start[i]) < 15.0f)
                        Game->CreateRumble(MagicRuneRumbleList[i].GetPosition(), 0.3f, 0.2f, "");
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}