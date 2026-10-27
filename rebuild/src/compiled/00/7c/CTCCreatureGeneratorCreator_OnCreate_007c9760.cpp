// CTCCreatureGeneratorCreator::OnCreate() -- make the parent thing level-persistent
// Leaf: ?OnCreate@CTCCreatureGeneratorCreator@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C9760, 11 bytes, plain `ret`.
// Identity: CTCCreatureGeneratorCreator vtable 0x0126EB3C slot 4 (RTTI .?AVCTCCreatureGeneratorCreator@@); Ego_r.pdb
//   `?OnCreate@CTCCreatureGeneratorCreator@@UAEXXZ` is in the class's name set.
// CTCBase virtual slots (Ego_r.pdb donor order, matched to the CTCBase vtable 0x0125BE0C): 2 OnRemove, 3 ConstructFromParams,
//   4 OnCreate, 5 InitialActivate, 6 OnInitialActivate, 7 RenderUpdate, 8 HasFrameUpdate, 9 FrameUpdate,
//   10 BulletTimeFrameUpdate, 11 DrawPreparePrimitivesForRendering, 12 Copy, 13 OnDie, 14 OnKill (the editor-only
//   EditFrameUpdate / EditorDrawPreparePrimitivesForRendering of the debug build are absent from retail).
// Layout: CThing as the landed CTCCreatureGenerator::OnCreate 0x00749EA0: the second flag bitfield byte at +0x92 (PDB
//   +0xa2), LevelPersistent = bit 4.
// Shape: the one-bit bitfield set compiles to `or byte ptr [eax+92h],10h`.
class CThing { public:
    unsigned char  _pad0[0x92];
    unsigned char  DeletedOnOwnerLevelOutOfScope : 1;       // +0x92 bit 0
    unsigned char  Drawable                      : 1;
    unsigned char  Dying                         : 1;
    unsigned char  ForceFrameUpdate              : 1;
    unsigned char  LevelPersistent               : 1;       // bit 4
    unsigned char  GameStatePersisted            : 1;
    unsigned char  PersistOnUnloadWhenInLimbo    : 1;
    unsigned char  PausedDuringCutscenes         : 1;
};

class CTCCreatureGeneratorCreator {
public:
    virtual void OnCreate();
    CThing* ParentThing;                        // +0x04 (CTCBase)
};

void CTCCreatureGeneratorCreator::OnCreate()
{
    ParentThing->LevelPersistent = true;
}