// CMorphableNode_00c185b0::CMorphableNode(mode) -- EH-framed constructor of the 8-byte node holder (vtable 0x012B2DEC over a base with an out-of-line destructor @ 0x00C17AD0, state 0): the +4 pointer takes either a 0x28-byte tree node (mode != 0; @ 0x00C194F0 with 0, state 1) or a 0x10-byte flat node (@ 0x00C32BA0 with 0, state 2), both null-checked `new`; a null result raises the "Error: EMEM" warning (PKCTRMorphable.cpp line 12; state 3 covers the string, the trace temporary lands in the dead argument slot)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C185B0, oracle length 278 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+1Ch] ; xor ebx,ebx ; cmp eax,ebx ; mov [esp+14h],ebx ; mov dword ptr [esi],12B2DECh ; mov [esi+4],ebx ; je flat ; push 28h ; call operator new ; add esp,4 ; mov [esp+1Ch],eax ; cmp eax,ebx ; mov byte ptr [esp+14h],1 ; je null ; push ebx ; mov ecx,eax ; call CTreeNode::CTreeNode ; jmp got ; flat: push 10h ; call operator new ; add esp,4 ; mov [esp+1Ch],eax ; cmp eax,ebx ; mov byte ptr [esp+14h],2 ; je null ; push ebx ; mov ecx,eax ; call CFlatNode::CFlatNode ; jmp got ; null: xor eax,eax ; got: cmp eax,ebx ; mov byte ptr [esp+14h],bl ; mov [esi+4],eax ; jne done ; ... (warning) ... done: ... mov eax,esi ; ... ret 4
// The classes are named by address.
void* __cdecl operator new(unsigned int size);
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c185b0)(const char* text);
extern PFNLOGSINK_00c185b0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c185b0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c185b0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c185b0();                       // 0x00C0BA40
    CString_00c185b0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c185b0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c185b0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte (here the argument slot)
    CTrace_00c185b0();                         // 0x00C16750
};

struct CNodeBase_00c185b0 { };             // the common type of both node kinds

struct CTreeNode_00c185b0 : CNodeBase_00c185b0 {   // 0x28 bytes
    unsigned char _body[0x28];
    CTreeNode_00c185b0(long mode);         // 0x00C194F0
};

struct CFlatNode_00c185b0 : CNodeBase_00c185b0 {   // 0x10 bytes
    unsigned char _body[0x10];
    CFlatNode_00c185b0(long mode);         // 0x00C32BA0
};

struct CHolderBase_00c185b0 {              // polymorphic base: its vtable store is dead and dropped
    virtual void v00();
    CHolderBase_00c185b0() { }
    ~CHolderBase_00c185b0();               // 0x00C17AD0
};

struct CMorphableNode_00c185b0 : CHolderBase_00c185b0 {   // 8 bytes
    CNodeBase_00c185b0* m_pNode;           // +4
    CMorphableNode_00c185b0(long mode);
};

CMorphableNode_00c185b0::CMorphableNode_00c185b0(long mode)
    : m_pNode(0)
{
    m_pNode = mode ? (CNodeBase_00c185b0*)new CTreeNode_00c185b0(0) : (CNodeBase_00c185b0*)new CFlatNode_00c185b0(0);
    if (m_pNode == 0) {
        {
            CString_00c185b0 line;
            line.Append(".\\PKCTRMorphable.cpp")->Append("(")->AppendNumber(12)->Append(") : ")->Append("Error: EMEM")->Append("\n");
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CTrace_00c185b0();
    }
}
