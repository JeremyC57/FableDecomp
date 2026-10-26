// CTCShotDefinitionBase::SetTimeToDie(float) -- schedules the shot's death a number of seconds from the current world frame
// Leaf: SetTimeToDie
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BED90, 53 bytes (+11 int3 padding). Manifest label
// "CTCHeroExperience::AddSuccessfulBlockParry" is propagated from an unrelated body: the store is
// CTCShotDefinitionBase::DieOnFrame (+0x60 in retail, PDB +0x64 -- the HitThingsList vector ahead of it is 12
// bytes, not 16), the field IsToDie (0x007BED10) compares against GetWorldFrame(). The only caller is
// NThingShot::CTCShotDefinitionTearInGround vtable slot 41 (0x007BBE24), which passes a float constant.
// Callees: GetWorldFrame() @ 0x0049D870 (returns the global @ 0x013B89BC), __ftol2 @ 0x00BFEA70.
// Globals: frames-per-second long @ 0x01375550 (15).
//
// Shape: the frame-rate global is copied into a frame slot BEFORE the call (`mov eax,[g] ; mov [esp+8],eax`):
// the source reads it first, so its value must survive the call; then `fild frame ; fild fps ; fmul seconds ;
// faddp` and __ftol2 into DieOnFrame. Written `(long)(g_FramesPerSecond * seconds + GetWorldFrame())` VC7.1
// hoists the call and `fild`s the global afterwards; the rate has to be read into a named local first.

extern long g_FramesPerSecond;                  // 0x01375550
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                           // 0x0049D870

struct CTCShotDefinitionBase_Retail {
    void*         __vftable;                    // +0x00
    unsigned char _pad_0x04[0x5C];              // +0x04 CTCBase, effect/target pointers, HitThingsList, defs, ...
    long          DieOnFrame;                   // +0x60 (PDB +0x64)

    void SetTimeToDie(float seconds);
};

void CTCShotDefinitionBase_Retail::SetTimeToDie(float seconds)
{
    long framesPerSecond = g_FramesPerSecond;
    DieOnFrame = (long)(framesPerSecond * seconds + CWorld::GetFrame());
}