// CConsoleCommand::vfunc_5(void*) -- calls the `__fastcall` callback at +0x18 with the object at +0x14 and the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EFEB0, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov edx, dword ptr [esp + 4] ; mov eax, ecx ; mov ecx, dword ptr [eax + 0x14] ; call dword ptr [eax + 0x18] ; ret 4
// Tiny-body shape `InvokeCallback` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CConsoleCommand {
    unsigned char _pad[0x14];
    void* m_pObject;                        // +0x14
    void (__fastcall* m_pfnCallback)(void* object, void* argument);// +0x18
    void vfunc_5(void* argument);
};

void CConsoleCommand::vfunc_5(void* argument)
{
    m_pfnCallback(m_pObject, argument);
}