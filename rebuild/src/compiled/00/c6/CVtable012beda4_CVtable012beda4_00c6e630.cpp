// CSalRender::~CSalRender() -- the GameCODA 3D voice renderer base's destructor: the update lock (CSalMutex m_mutexUpdate +0x04, inline
//   ~CSalMutex: its own vtable 0x012B8FC8 restored, DeleteCriticalSection on +0x08), then the CCodaObjectBase root vtable 0x012B5868 restored
// Leaf: ??1CSalRender@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6E630, 34 bytes (+ int3 padding). Callee: KERNEL32!DeleteCriticalSection (IAT 0x0143FDE4).
// Option S unit (NF444): retail 0x00C6E630..0x00C6E6EB is the GameCODA SalRender.obj slice. This owner row holds the bodies in retail address
//   order; the thin rows 0x00C6E690 (ClearFilterHistory), 0x00C6E6A0 (the constructor) and 0x00C6E6D0 (??_GCSalRender, slot 0 of vtable
//   0x012BEDA4, emitted by this TU) include it and grade their own symbol from its object; so does 0x00C6E660 (?Initialize@CSalRender,
//   folded in by NF445: its only caller CSalSw3DVoice::Initialize 0x00C55680 is respelled in the same batch).
// Names (two sources each): (1) GameCODA 2.10 SDK CODAApi.lib Release/SalRender.obj defines ??1CSalRender@@UAE@XZ, ?Initialize,
//   ?ClearFilterHistory@CSalRender@@UAEXXZ (1 byte, `ret`), ??0CSalRender@@QAE@XZ and ??_GCSalRender@@UAEPAXI@Z, and
//   ??_7CSalRender@@6B@ = { ??_E, ClearFilterHistory, _purecall x3 }; (2) retail vtable 0x012BEDA4 = { 0x00C6E6D0, 0x00C6E690, _purecall
//   0x00BFEA08 x3 }, slot 1 inherited by the Pan / Disable vtables (0x012BED90 / 0x012BEDB8 and their SIMD subclasses), and the derived
//   constructors / destructors (0x00C6CA20, 0x00C6D9D0, 0x00C6E760, 0x00C70120, 0x00C70B00 / 0x00C6CAC0, 0x00C6DA30, 0x00C6E780,
//   0x00C701D0, 0x00C70BB0) call 0x00C6E6A0 / jump to 0x00C6E630, as the SDK's CSalRender subclasses do (NF441 CLASS_TABLE).
//   Was the stand-in CVtable_012beda4 (+ ?Hook@T@@QAEXXZ at 0x00C6E690, ?VecDel@T@@QAEPAXI@Z at 0x00C6E6D0).
// Layout: engine/CSalRender.h with FABLE_RETAIL_CSALRENDER_MEMBERS (retail 0x28 bytes: the SDK members over the 0x1C-byte retail CSalMutex
//   of the CSalOutputKS.h PASS23 layout), FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS (the inline CSalMutex constructor / destructor the
//   retail bodies expand) and FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR (the closing root vptr store).
// Flags: /Ob1 (catalog CompilerFlags). Retail ??_GCSalRender 0x00C6E6D0 CALLS 0x00C6E630 from the same object, and ??_GCSalRenderPanDelay
//   0x00C6D350 calls the 11-byte ~CSalRenderPanDelay 0x00C6CAC0 (vptr store + jmp) of its own slice: under the default /Ob2 VC7.1 expands
//   both (??_GCSalRender DIFFER 54v30); /Ob1 inlines only the inline-declared members (the CSalMutex bodies), as retail did.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRender.h"

CSalRender::~CSalRender()
{
}

// 0x00C6E660, 45 bytes: SetPosition (slot 4) with azimuth / elevation / distance / gain 0, zero macro-FX gains, no starting flag and
//   the update flag set. The two-float record is zeroed by immediate stores after the seven pushes; float zero arguments are `push 0`.
void CSalRender::Initialize()
{
    float afMacroFXGain[2];
    afMacroFXGain[0] = 0.0f;
    afMacroFXGain[1] = 0.0f;
    SetPosition(0.0f, 0.0f, 0.0f, 0.0f, afMacroFXGain, false, true);
}

// 0x00C6E690, 1 byte (`ret`): the base renderer keeps no filter history.
void CSalRender::ClearFilterHistory()
{
}

// 0x00C6E6A0, 38 bytes: vptr 0x012BEDA4, the inline CSalMutex constructor (vptr 0x012B8FC8, InitializeCriticalSection on +0x08 through
//   IAT 0x0143FEE4), m_fGain = 0 (+0x20) and m_bUpdate = false (+0x24) from one zero register.
CSalRender::CSalRender()
    : m_fGain(0.0f), m_bUpdate(false)
{
}
