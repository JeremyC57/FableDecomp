// CVertexBufferWin32::DoUnlock() -- calls slot 14 of the COM-style (`__stdcall`) interface held at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2DC0, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; mov ecx, dword ptr [eax] ; push eax ; call dword ptr [ecx + 0x38] ; ret 
// Tiny-body shape `CallInterfaceSlot` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct IInterface {                       // COM-style: `this` is pushed as the first stack argument
    virtual long __stdcall Slot0();
    virtual long __stdcall Slot1();
    virtual long __stdcall Slot2();
    virtual long __stdcall Slot3();
    virtual long __stdcall Slot4();
    virtual long __stdcall Slot5();
    virtual long __stdcall Slot6();
    virtual long __stdcall Slot7();
    virtual long __stdcall Slot8();
    virtual long __stdcall Slot9();
    virtual long __stdcall Slot10();
    virtual long __stdcall Slot11();
    virtual long __stdcall Slot12();
    virtual long __stdcall Slot13();
    virtual long __stdcall Method();       // slot 14
};

class CVertexBufferWin32 { public:
    unsigned char _pad[0x4];
    IInterface* m_pInterface;              // +0x4
    void DoUnlock();
};

void CVertexBufferWin32::DoUnlock()
{
    m_pInterface->Method();
}