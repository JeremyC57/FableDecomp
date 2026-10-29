// CThingCreatureBase::SetLastActionHandedness(long handedness) -- remember the handedness of the last combat action and the world frame it was set on
// Retail: Fable.exe (TLC, VC7.1) @ 0x006619F0, 33 bytes.
// Callees: CThing::GetWorld @ 0x004C7AB0 (reads the main game component global's +0x24; the manifest
//          "CViewBase::GetDisplayEngine" label is propagated), CWorld::GetWorldFrame @ 0x0049D870 (static: returns the
//          global @ 0x013B89BC; the "GFGetPreMainMemoryUsed" label is propagated).
// Layout: PDB CThingCreatureBase has PreviousActionHandedness (ECombatAnimationHandedness) +0x15c and
//   PreviousActionHandednessWF +0x160; retail stores them at +0x138/+0x13c (the PDB's debug vectors and wider STL
//   containers sit in front), so engine/CThingCreatureBase.h (PDB offsets) does not fit and a local model is used.
// Shape: `GetWorld()->GetWorldFrame()` with a STATIC GetWorldFrame: the object expression is still evaluated
//   (`call GetWorld`, result unused), then the static call -- the same pair as the landed 0x006929E0.
class CWorld { public:
    static long __fastcall GetFrame();        // 0x0049D870
};

class CThingCreatureBase { public:
    unsigned char _pad000[0x138];
    long PreviousActionHandedness;      // +0x138 ECombatAnimationHandedness
    long PreviousActionHandednessWF;    // +0x13c
    CWorld* GetWorld();                 // 0x004C7AB0 (CThing)
    void SetLastActionHandedness(long handedness);
};

void CThingCreatureBase::SetLastActionHandedness(long handedness)
{
    PreviousActionHandedness = handedness;
    PreviousActionHandednessWF = GetWorld()->GetFrame();
}