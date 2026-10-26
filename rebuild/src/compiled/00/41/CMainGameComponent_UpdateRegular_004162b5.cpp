// CMainGameComponent::UpdateRegular() -- while the application is active: clear the frame counter global, time the component's Update (slot 5) into the update-time global, then time the Render (slot 7, skipped while the full-screen movie player reports it is drawing) into the render-time global
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004162B5, length 119 bytes.
// Callees: GFGetSystemManager @ 0x009A4EC0 (`__fastcall`), CSystemManager::IsApplicationActive @ 0x009A57B0,
//          GFGetTime @ 0x009E1BC0 (`__fastcall`, double), IsActive @ 0x009E9FB0 (`__fastcall` bool, the movie
//          player), the component's virtual slots 5 (+0x14) and 7 (+0x1C), and slot 59 (+0xEC) of the object at
//          (+0x28)->+0x2c.
// Globals: 0x013B89A8 (zeroed), 0x013B8690 (update time), 0x013B8698 (render time), all doubles but the first.
// Shape: size TU with a frame pointer (`optimize("s", on)` + `("y", off)`: `push ecx ; push ecx` for the 8-byte
// start time, `and [g],0`, `leave`); the start time is one double local reused for both measurements.
// Binding (sweep 3): CSystemManager is class-keyed, so the callee binds Ego_r ?GFGetSystemManager@@YIPAVCSystemManager@@XZ
//   (was ...PAUCSystemManager...). Same bytes.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CSystemManager {
public:
    bool IsApplicationActive();                    // 0x009A57B0
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
double __fastcall GFGetTime();                     // 0x009E1BC0
bool __fastcall IsActive();                        // 0x009E9FB0
struct CDisplayOwner {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058();
    virtual bool IsDrawingExclusively();           // slot 59 (+0xEC)
};
struct CDisplayHolder {
    unsigned char  _pad00[0x2c];
    CDisplayOwner* m_pOwner;                       // +0x2c
};
extern long   g_frameCounter_013b89a8;             // 0x013B89A8
extern double g_updateTime_013b8690;               // 0x013B8690
extern double g_renderTime_013b8698;               // 0x013B8698
struct CMainGameComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Update();                         // slot 5 (+0x14)
    virtual void v06();
    virtual void Render();                         // slot 7 (+0x1C)
    unsigned char   _pad04[0x24];
    CDisplayHolder* m_pDisplay;                    // +0x28
    void UpdateRegular();
};

void CMainGameComponent::UpdateRegular()
{
    if (GFGetSystemManager()->IsApplicationActive())
    {
        g_frameCounter_013b89a8 = 0;
        double start = GFGetTime();
        Update();
        g_updateTime_013b8690 = GFGetTime() - start;
        start = GFGetTime();
        if (!IsActive() || !m_pDisplay->m_pOwner->IsDrawingExclusively())
            Render();
        g_renderTime_013b8698 = GFGetTime() - start;
    }
}