// CChunkedFileRawDataChunk_00c21ff0::WriteToFile(void*) -- builds a 12-byte visitor (vtable 0x012B3848, result 0, the file) on the stack, hands it to this's virtual slot 10 and returns the visitor's result; EH-framed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21FF0, oracle length 79 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,0Ch ; mov edx,[file] ; xor eax,eax ; mov dword ptr [esp],offset vtable ; mov [esp+4],eax ; mov [esp+8],edx ; lea edx,[esp] ; mov [esp+14h],eax ; mov eax,[ecx] ; push edx ; call [eax+0x28] ; mov ecx,[esp+0Ch] ; mov eax,[esp+4] ; mov fs:[0],ecx ; add esp,18h ; ret 4
// A stack visitor with an inline constructor (vtable, result zeroed, the file pointer -- the zero
// is shared with the EH state 0 store) is passed to the object's virtual slot 10; its result
// field is returned. The visitor's destructor is inline and empty, so nothing is called on the
// normal path, but it is non-trivial, which is what registers EH state 0 (the funclet calls the
// out-of-line copy). Built under /EHsc. The manifest label names the method; the class is named
// by address. One of 3 bodies of this shape.
struct CFileVisitor {                      // vtable 0x012B3848: 12-byte visitor over the file
    virtual void v00();
    long m_result;                         // +4
    void* m_pFile;                         // +8
    CFileVisitor(void* file) : m_result(0), m_pFile(file) { }
    ~CFileVisitor() { }
};

struct CChunkedFileRawDataChunk_00c21ff0 {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void Visit(CFileVisitor* visitor);   // slot 10
    long WriteToFile_00c21ff0(void* file);
};

long CChunkedFileRawDataChunk_00c21ff0::WriteToFile_00c21ff0(void* file)
{
    CFileVisitor visitor(file);
    Visit(&visitor);
    return visitor.m_result;
}
