// CTCAtmosPlayer::OnCreate() -- when an atmos def index was set up front, apply it (create and start the emitter)
// Leaf: ?OnCreate@CTCAtmosPlayer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3690, 14 bytes, plain `ret`.
// Identity: CTCAtmosPlayer vtable 0x01270484 slot 4 (RTTI .?AVCTCAtmosPlayer@@); FableWin.pdb
//   `?OnCreate@CTCAtmosPlayer@@UAEXXZ` is in the class's name set; the landed callee names this caller already.
// CTCBase virtual slots (Ego_r.pdb donor order, matched to the CTCBase vtable 0x0125BE0C): 2 OnRemove, 3 ConstructFromParams,
//   4 OnCreate, 5 InitialActivate, 6 OnInitialActivate, 7 RenderUpdate, 8 HasFrameUpdate, 9 FrameUpdate,
//   10 BulletTimeFrameUpdate, 11 DrawPreparePrimitivesForRendering, 12 Copy, 13 OnDie, 14 OnKill (the editor-only
//   EditFrameUpdate / EditorDrawPreparePrimitivesForRendering of the debug build are absent from retail).
// Callees: CTCAtmosPlayer::SetAtmosDefIndex(long) @ 0x007E3610 (landed, FableWin.pdb `?SetAtmosDefIndex@CTCAtmosPlayer@@QAEXJ@Z`).
// Layout: Ego_r.pdb CTCAtmosPlayer (0x14): PSoundEmitter +0x0c, AtmosDefIndex +0x10 (as the landed callee).
// Shape: the index is loaded once and pushed; the callee takes a stack argument, so no tail jump.
class CThing;
class CASoundEmitter;

class CTCAtmosPlayer {
public:
    virtual void OnCreate();
    CThing*         ParentThing;                // +0x04 (CTCBase)
    bool            ToBeRemoved;                // +0x08
    bool            IsInGlobalUpdateTCs;        // +0x09
    CASoundEmitter* PSoundEmitter;              // +0x0c
    long            AtmosDefIndex;              // +0x10
    void SetAtmosDefIndex(long index);          // 0x007E3610
};

void CTCAtmosPlayer::OnCreate()
{
    if (AtmosDefIndex)
        SetAtmosDefIndex(AtmosDefIndex);
}