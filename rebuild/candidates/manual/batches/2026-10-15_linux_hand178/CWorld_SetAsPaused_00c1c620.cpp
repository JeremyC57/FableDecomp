// CWorld::SetAsPaused(bool) -- third-party steering/navigation library world (0x00C0xxxx-0x00C4xxxx module, not the game's CWorld): pause or resume, notifying the library and refreshing
// Flags: /EHsc
// Retail: Fable.exe @ 0x00C1C620, 122 bytes, `ret 4`, EH frame (handler 0x0121248F).
// NOTE: this class is NOT the game's CWorld (engine/CWorld.h): it is the AI steering library's world object (its
// neighbours are the CWorldManager / CSteeringGroup code, e.g. 0x00C096E0); no RTTI. The paused flag sits at +0x18.
// Callees (manifest labels in this module are byte-twin noise): the scope guard constructor 0x00C0FFC0 (stores the
// guarded object -- the static at 0x0143BAFC in .bss -- calls 0x00C1C830 and records a timer import value) and its
// destructor 0x00C0FFE0 (labels "CTCHeroExperience::AddSuccessfulBlockParry" / "EventQueue_ProcessWithTimeout"),
// the library's __stdcall state notification 0x00C335C0 (state 1 = paused, 4 = running; label "operator!=<char,char>"),
// and this->Refresh() 0x00C1C460 (label "ParticleMorphs_UpdateEditor").
// Shape: nothing happens when the state is unchanged; the guard lives across the store, the notification and the
// refresh; `Paused ? 1 : 4` compiles to neg/sbb/and/add.
struct CLibGuardTarget;
extern CLibGuardTarget g_LibWorldGuardTarget;   // 0x0143BAFC

struct CLibScopeGuard {
    CLibGuardTarget* Target;                    // +0x00
    unsigned long    StartTime;                 // +0x04
    CLibScopeGuard(CLibGuardTarget* target);    // 0x00C0FFC0
    ~CLibScopeGuard();                          // 0x00C0FFE0
};

bool __stdcall LibNotifyWorldState(long state, long arg);   // 0x00C335C0

struct CWorld {
    unsigned char _pad[0x18];
    bool          Paused;                       // +0x18
    void Refresh();                             // 0x00C1C460
    void SetAsPaused(bool paused);
};

void CWorld::SetAsPaused(bool paused)
{
    if (Paused != paused)
    {
        Paused = paused;
        CLibScopeGuard guard(&g_LibWorldGuardTarget);
        LibNotifyWorldState(Paused ? 1 : 4, 0);
        Refresh();
    }
}
