// CV_BodyGuardScript::HenchmanCommentary() -- henchman chatter loop: wait for henchmen; clear the owner's comment flag once the commentary timer runs out; every second refresh HeroWieldingWeapon; reset the stood-still counter whenever the hero moves; every 5 seconds try an idle comment
// Grade: DIFFER(584v585) -- one missing instruction: retail opens with `mov dword ptr [esp+0xc],0` (a dead zero store to the first CTimer slot before RegisterTimer); everything else matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5E350, length 585 bytes, plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CTimer (inline ctor/dtor through g_pGameScriptInterface
//   0x0143E8F8 slots 87..90; out-of-line ~CTimer 0x00CD4470 at one return); CCounter::operator=(long) 0x00D90550;
//   CV_BodyGuardScript::MakeHenchmanPerformIdleComment 0x00E5E5A0; CScriptThing slot 6 GetPos (+0x18); GameCalls slots 7
//   NewScriptFrame, 70 GetHero, 195 IsPlayerWieldingWeapon. Epsilon 0.0001f 0x0129BA3C.
// Layout: Ego_r.pdb CV_BodyGuardScript minus 0x14 (retail CScriptBase): HeroWieldingWeapon +0x4d, HeroStoodStillCounter
//   +0x50, CommentaryTimer +0x5c, HenchmanList +0x64 (12-byte STLport vector). The owner (+0x44) is descriptive: a long
//   at +8 (a pending-comment flag).
// Shape: generated script code; two CTimer locals; the moved test is an inline epsilon C3DVector operator!= written with
//   `>` and `||` (`test ah,41h ; je`).
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    bool operator!=(const C3DVector& v) const
    {
        return fabs(X - v.X) > 0.0001f || fabs(Y - v.Y) > 0.0001f || fabs(Z - v.Z) > 0.0001f;
    }
};
struct CBaseClass { virtual void v00(); };
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    virtual void t001() const; virtual void t002() const; virtual void t003() const; virtual void t004() const; virtual void t005() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18) 0x004AA980
    void* PImpData;
    void* PImpInfo;
};
struct GameCalls {                                       // CGameScriptInterface, retail vtable 0x01260F0C
 virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
 virtual void NewScriptFrame(void) const; //slot7 0x006e7410
 virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
 virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
 virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
 virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
 virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
 virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
 virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
 virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
 virtual CScriptThing & GetHero(void); //slot70 0x00891ca0
 virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
 virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
 virtual long RegisterTimer(void); //slot87 0x006e79e0
 virtual void DeregisterTimer(long); //slot88 0x006e7690
 virtual void SetTimer(long, long); //slot89 0x006e76e0
 virtual long GetTimer(long) const; //slot90 0x006e7710
 virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
 virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
 virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
 virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
 virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
 virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
 virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146();
 virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
 virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162();
 virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170();
 virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178();
 virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186();
 virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194();
 virtual bool IsPlayerWieldingWeapon(void) const; //slot195 0x00897480
};
extern GameCalls* g_pGameScriptInterface;                // 0x0143E8F8
struct CTimer {                                          // a script timer: the interface's timer index
    long Index;
    CTimer() { Index = g_pGameScriptInterface->RegisterTimer(); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
};
struct CCounter {
    long Value;
    CCounter& operator=(long value);                     // 0x00D90550
};
struct CScriptThingVector {                              // STLport vector<CScriptThing>, 12 bytes
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};
struct CScriptBase {                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};
struct CV_BodyGuardOwner {
    unsigned char _pad00[8];
    long          PendingComment;                        // +0x08
};
struct CV_BodyGuardScript : CScriptBase {
    GameCalls*         Game;                             // +0x40
    CV_BodyGuardOwner* Owner;                            // +0x44
    unsigned char      _pad48[0x4d - 0x48];
    bool               HeroWieldingWeapon;               // +0x4d
    CCounter           HeroStoodStillCounter;            // +0x50
    CTimer             HenchmanLastIdleChatTimer;        // +0x54
    CTimer             HenchmanLastBoredChatTimer;       // +0x58
    CTimer             CommentaryTimer;                  // +0x5c
    CTimer             HenchmanFreakedOutThePlayerTimer; // +0x60
    CScriptThingVector HenchmanList;                     // +0x64
    bool MakeHenchmanPerformIdleComment();               // 0x00E5E5A0
    void HenchmanCommentary();
};

void CV_BodyGuardScript::HenchmanCommentary()
{
    CTimer idleTimer;
    CTimer weaponTimer;
    C3DVector lastHeroPos;
    while (!IsActiveThreadTerminating()) {
        while (HenchmanList.size() == 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (Owner->PendingComment && CommentaryTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            Owner->PendingComment = 0;
        }
        if (weaponTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            weaponTimer.SetTime(1);
            HeroWieldingWeapon = Game->IsPlayerWieldingWeapon();
        }
        if (lastHeroPos != Game->GetHero().GetPos()) {
            if (IsActiveThreadTerminating())
                return;
            HeroStoodStillCounter = 0;
        }
        lastHeroPos = Game->GetHero().GetPos();
        if (idleTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            if (MakeHenchmanPerformIdleComment()) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                idleTimer.SetTime(5);
            }
        }
        Game->NewScriptFrame();
    }
}
