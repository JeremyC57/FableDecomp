// CBaseIntelligentPointer::CBaseIntelligentPointer() -- sets the vtable (0x012B51E8) and nulls the member at +4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36DA0, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, ecx ; mov dword ptr [eax], 0x12b51e8 ; mov dword ptr [eax + 4], 0 ; ret 
// Tiny-body shape `CtorVtableNull`: a polymorphic class with one pointer member, default-constructed
// to null (the vtable address is a relocation). The class comes from the manifest label when it is
// a plain class name; member names are descriptive.
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                         // +4
    CBaseIntelligentPointer();
};

CBaseIntelligentPointer::CBaseIntelligentPointer()
    : m_pItem(0)
{
}