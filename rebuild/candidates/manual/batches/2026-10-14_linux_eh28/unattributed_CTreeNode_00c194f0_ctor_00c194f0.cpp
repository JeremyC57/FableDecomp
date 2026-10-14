// CTreeNode_00c194f0::CTreeNode(sentinel) -- EH-framed constructor of the 0x28-byte red-black tree node shared by the module's containers (vtable 0x012B2F0C over a base with an out-of-line destructor @ 0x00C17AD0, state 0): the root pointer at +4 takes the shared sentinel, +8 clears the owned one, +0x20 points at the embedded link at +0xc and +0x24 clears; without a shared sentinel a 4-byte sentinel (vtable 0x012B2F60) is allocated (null-checked `new`), its absence raises the "Error: EMEM" warning (PKCTRRedblackTree.cpp line 35; state 1 covers the string) and the owned sentinel becomes the root; the link is then self-linked (+0x10 and the link's own word point at +0xc) and +0x14..+0x1c cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C194F0, oracle length 268 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+20h] ; xor ebx,ebx ; cmp eax,ebx ; lea edi,[esi+0Ch] ; mov [esp+18h],ebx ; mov dword ptr [esi],12B2F0Ch ; mov [esi+4],eax ; mov [esi+8],ebx ; mov [esi+20h],edi ; mov [esi+24h],ebx ; jne link ; push 4 ; call operator new ; add esp,4 ; cmp eax,ebx ; je null ; mov dword ptr [eax],12B2F60h ; jmp got ; null: xor eax,eax ; got: cmp eax,ebx ; mov [esi+8],eax ; jne own ; mov eax,[g_pchNil] ; mov [esp+20h],eax ; mov edx,offset ".\PKCTRRedblackTree.cpp" ; lea ecx,[esp+20h] ; mov byte ptr [esp+18h],1 ; call Append ; ... mov edx,23h ; ... call AppendNumber ; ... call Append ("Error: EMEM") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; cmp eax,ebx ; je skip ; mov ecx,[esp+20h] ; call eax ; skip: lea ecx,[esp+20h] ; mov byte ptr [esp+18h],bl ; call ~CString ; lea ecx,[esp+20h] ; call CTrace::CTrace ; own: mov ecx,[esi+8] ; mov [esi+4],ecx ; link: ... mov [esi+10h],edi ; mov [edi],edi ; mov [esi+14h],ebx ; mov [esi+18h],ebx ; mov [esi+1Ch],ebx ; ... mov eax,esi ; ... ret 4
// The node is the one the landed containers build (mode 0) and the 0x00C185B0 holder allocates. The classes are named by address.
void* __cdecl operator new(unsigned int size);
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c194f0)(const char* text);
extern PFNLOGSINK_00c194f0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CString_00c194f0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c194f0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c194f0();                       // 0x00C0BA40
    CString_00c194f0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c194f0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c194f0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte (here the argument slot)
    CTrace_00c194f0();                         // 0x00C16750
};

struct CSentinel_00c194f0 {                // 4 bytes: vtable 0x012B2F60
    virtual void v00();
};

struct CLink_00c194f0 {                    // the embedded link at +0xc
    CLink_00c194f0* m_pNext;
};

struct CTreeNodeBase_00c194f0 {            // polymorphic base: its vtable store is dead and dropped
    virtual void v00();
    CTreeNodeBase_00c194f0() { }
    ~CTreeNodeBase_00c194f0();             // 0x00C17AD0
};

struct CTreeNode_00c194f0 : CTreeNodeBase_00c194f0 {   // 0x28 bytes
    CSentinel_00c194f0* m_pRoot;           // +4
    CSentinel_00c194f0* m_pOwned;          // +8
    CLink_00c194f0 m_link;                 // +0xc
    CLink_00c194f0* m_pHead;               // +0x10
    long m_14;
    long m_18;
    long m_1c;
    CLink_00c194f0* m_pTail;               // +0x20
    long m_24;
    CTreeNode_00c194f0(CSentinel_00c194f0* shared);
};

CTreeNode_00c194f0::CTreeNode_00c194f0(CSentinel_00c194f0* shared)
    : m_pRoot(shared), m_pOwned(0), m_pTail(&m_link), m_24(0)
{
    if (shared == 0) {
        m_pOwned = new CSentinel_00c194f0;
        if (m_pOwned == 0) {
            {
                CString_00c194f0 line;
                line.Append(".\\PKCTRRedblackTree.cpp")->Append("(")->AppendNumber(35)->Append(") : ")->Append("Error: EMEM")->Append("\n");
                if (g_pfnLogSink_0143bb2c)
                    g_pfnLogSink_0143bb2c(line.m_pchData);
            }
            CTrace_00c194f0();
        }
        m_pRoot = m_pOwned;
    }
    m_pHead = &m_link;
    m_link.m_pNext = &m_link;
    m_14 = 0;
    m_18 = 0;
    m_1c = 0;
}
