// CConsoleCommand::vfunc_5(void*) -- calls the `__fastcall` callback at +0x18 with the object at +0x14 and the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EFEB0, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov edx, dword ptr [esp + 4] ; mov eax, ecx ; mov ecx, dword ptr [eax + 0x14] ; call dword ptr [eax + 0x18] ; ret 4
// Tiny-body shape `InvokeCallback` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
// NF385: respelt from the vfunc_5 placeholder. Identity: retail vftable 0x0129C4E0 (RTTI CConsoleCommand<CConsole>) slot 5 = FableWin
// slot 5 ?CallCommandFunction@?$CConsoleCommand@VCConsole@@@@UBEXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// (FableWin lib_console2.obj; Ego_r.pdb carries no CConsoleCommand instance names). The callback is invoked with the command's object and the
// argument vector (passed by reference, so the same pointer in edx).
// Leaf: ?CallCommandFunction@?$CConsoleCommand@VCConsole@@@@UBEXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
class CConsole;
class CGenericVar;
namespace std { template <class T> class allocator; template <class T, class A> class vector; }
template <class T> struct CConsoleCommand {
    // +0x00 vfptr
    unsigned char _pad04[0x10];
    void* m_pObject;                        // +0x14
    void (__fastcall* m_pfnCallback)(void* object, void* argument);// +0x18
    virtual void CallCommandFunction(const std::vector<CGenericVar, std::allocator<CGenericVar> >& args) const;
};

template <> void CConsoleCommand<CConsole>::CallCommandFunction(const std::vector<CGenericVar, std::allocator<CGenericVar> >& args) const
{
    m_pfnCallback(m_pObject, (void*)&args);
}