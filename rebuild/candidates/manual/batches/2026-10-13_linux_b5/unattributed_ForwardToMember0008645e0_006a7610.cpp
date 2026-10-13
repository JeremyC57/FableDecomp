// <unattributed>::ForwardToMember0_008645e0(void*) -- Member0->Shim_008645e0(arg, 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7610, oracle length 16 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov edx,[esp+4] ; mov ecx,[ecx] ; push 0 ; call Shim_008645e0 ; ret 4
// The callee @ 0x008645E0 is landed as a __fastcall(self, edx, stack) shim, which is why the stack
// argument is loaded into edx and a literal 0 is pushed: the object at +0x00 is the receiver.
struct CShimTarget_008645e0;
extern long __fastcall Shim_008645e0(CShimTarget_008645e0* self, void* argument, long flags);   // 0x008645E0

struct CForwarder_Retail {
    CShimTarget_008645e0* Target;           // +0x00
    long ForwardToMember0_008645e0(void* argument);
};

long CForwarder_Retail::ForwardToMember0_008645e0(void* argument)
{
    return Shim_008645e0(this->Target, argument, 0);
}
