// NCameraModes::CScriptModuleSelfTerminateBase::CScriptModuleSelfTerminateBase(long duration) -- constructor: CScriptModuleBase, WFDuration = duration, EndFrame = current frame + WFDuration
// Leaf: ??0CScriptModuleSelfTerminateBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852490, length 38 bytes.
// Callees: NCameraModes::CScriptModuleBase::CScriptModuleBase() @ 0x005ED070, GFGetCurrentFrame @ 0x0049D870
// (`__fastcall`, no arguments; the manifest's "GFGetPreMainMemoryUsed" label is propagated).
// Vtable: 0x012759CC (NCameraModes::CScriptModuleSelfTerminateBase, rtti-vtables.tsv).
// Shape: WFDuration in the initialiser list (stored before the call), EndFrame assigned in the body from
// the frame plus the member re-read from memory (`add eax,[esi+4Ch]`, the call clobbering the argument).
// Layout: PDB WFDuration +0x4c, EndFrame +0x50 (sizeof 0x54) over the 0x4c-byte CScriptModuleBase;
// the landed IsFinished 0x00851E50 compares the frame with EndFrame. No engine header exists.
long __fastcall GFGetCurrentFrame();                 // 0x0049D870

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4c - 4];
    CScriptModuleBase();                             // 0x005ED070
};

namespace NCameraModes {

struct CScriptModuleSelfTerminateBase : CScriptModuleBase {   // vtable 0x012759CC
    long WFDuration;                                 // +0x4c
    long EndFrame;                                   // +0x50
    virtual void v00();
    CScriptModuleSelfTerminateBase(long duration);
};

CScriptModuleSelfTerminateBase::CScriptModuleSelfTerminateBase(long duration)
    : WFDuration(duration)
{
    EndFrame = GFGetCurrentFrame() + WFDuration;
}

}
