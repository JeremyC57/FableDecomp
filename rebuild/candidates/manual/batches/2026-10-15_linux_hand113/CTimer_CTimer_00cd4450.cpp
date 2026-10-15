// CTimer::CTimer() -- a script timer: its index is a new timer allocated by the thing manager
// Leaf: ??0CTimer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD4450, length 23 bytes (+ int3 padding). 67 callers.
// Callees: the thing manager global @ 0x0143E8F8, vtable slot 87 (+0x15c, long: a new timer index; the
// same slot as the landed CCrateTeamManager constructor 0x00CDB360). The sibling 0x00CD4470 frees it
// through slot 88.
// PDB CTimer = { long TimerIndex } (4 bytes).
// Shape: `this` kept in esi across the virtual call, the index stored from eax, `this` returned.
struct CThingManager_Calls {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086();
    virtual long CreateTimer();                    // slot 87 (+0x15c)
};
extern CThingManager_Calls* g_pThingManager_0143e8f8;   // 0x0143E8F8

struct CTimer {
    long TimerIndex;                               // +0x00
    CTimer();
};

CTimer::CTimer()
    : TimerIndex(g_pThingManager_0143e8f8->CreateTimer())
{
}
