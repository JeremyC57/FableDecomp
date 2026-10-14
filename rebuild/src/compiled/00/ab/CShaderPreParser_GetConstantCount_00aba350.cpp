// CShaderPreParser::GetConstantCount() -- the element count of the 8-byte-element range at +0x30/+0x34
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABA350, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x34] ; sub eax, dword ptr [ecx + 0x30] ; sar eax, 3 ; ret 
// Tiny-body shape `GetCount` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct SElement { unsigned char _body[0x8]; };

struct CShaderPreParser {
    unsigned char _pad[0x30];
    SElement* m_first;                     // +0x30
    SElement* m_last;                      // +0x34
    long GetConstantCount();
};

long CShaderPreParser::GetConstantCount()
{
    return m_last - m_first;
}