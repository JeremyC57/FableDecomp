// CVertexBufferWin32::DoSizeof  @ 0x00a0aaa0
// mov eax,[ecx+0x3a3c]; add eax,0x204; ret
class CVertexBufferWin32 { public:
    char pad[0x3a3c];
    unsigned long baseSize;
    unsigned long DoSizeof();
};

unsigned long CVertexBufferWin32::DoSizeof()
{
    return this->baseSize + 0x204;
}