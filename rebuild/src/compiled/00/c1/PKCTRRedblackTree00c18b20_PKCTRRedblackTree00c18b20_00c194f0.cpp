// PKCTRRedblackTree_00c18b20::PKCTRRedblackTree_00c18b20(PKCTRAllocator* allocator) -- EH-framed constructor of the libpk
// audio library's 0x28-byte red-black tree (vtable 0x012B2F0C over a base with an out-of-line destructor @ 0x00C17AD0,
// state 0): the allocator at +4 takes the shared one, +8 clears the owned one, the root (+0x20) points at the embedded
// nil sentinel node (+0xc) and the count (+0x24) clears; without a shared allocator a 4-byte one (vtable 0x012B2F60) is
// allocated (null-checked `new`), its absence raises the "Error: EMEM" warning (PKCTRRedblackTree.cpp line 35; state 1
// covers the string) and the owned allocator becomes the allocator; the sentinel's left/right then point at itself and
// its parent, colour and key clear.
// Leaf: ??0PKCTRRedblackTree_00c18b20@@
// Renamed (rename1, 2026-10-24) from _global::RedblackTree_NodeCtor (symbol CTreeNode_00c194f0::CTreeNode_00c194f0) @
//   00/c1/unattributed_CTreeNode00c194f0ctor_00c194f0.cpp: it is not a node constructor but the constructor of the tree
//   object itself -- its warning path names ".\PKCTRRedblackTree.cpp" (line 35), and the layout it builds is exactly the
//   one the landed PKCTRRedblackTree_00c18b20::InsertFixup 0x00C18B20 works on (vtable 0x012B2F0C; allocator +4, owned
//   allocator +8, nil sentinel node {Left, Right, Parent, Colour, Key} +0xc, Root +0x20, Count +0x24). libpk is built
//   without RTTI and no PDB name set holds a PKCTR symbol, so the class keeps the address-suffixed name of that row.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C194F0, oracle length 268 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+20h] ; xor ebx,ebx ; cmp eax,ebx ; lea edi,[esi+0Ch] ; mov [esp+18h],ebx ; mov dword ptr [esi],12B2F0Ch ; mov [esi+4],eax ; mov [esi+8],ebx ; mov [esi+20h],edi ; mov [esi+24h],ebx ; jne link ; push 4 ; call operator new ; add esp,4 ; cmp eax,ebx ; je null ; mov dword ptr [eax],12B2F60h ; jmp got ; null: xor eax,eax ; got: cmp eax,ebx ; mov [esi+8],eax ; jne own ; mov eax,[g_pchNil] ; mov [esp+20h],eax ; mov edx,offset ".\PKCTRRedblackTree.cpp" ; lea ecx,[esp+20h] ; mov byte ptr [esp+18h],1 ; call Append ; ... mov edx,23h ; ... call AppendNumber ; ... call Append ("Error: EMEM") ; ... call Append ("\n") ; mov eax,[g_pfnLogSink] ; cmp eax,ebx ; je skip ; mov ecx,[esp+20h] ; call eax ; skip: lea ecx,[esp+20h] ; mov byte ptr [esp+18h],bl ; call ~CString ; lea ecx,[esp+20h] ; call CTrace::CTrace ; own: mov ecx,[esi+8] ; mov [esi+4],ecx ; link: ... mov [esi+10h],edi ; mov [edi],edi ; mov [esi+14h],ebx ; mov [esi+18h],ebx ; mov [esi+1Ch],ebx ; ... mov eax,esi ; ... ret 4
// The tree is the one the landed containers build (mode 0) and the 0x00C185B0 holder allocates.
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

struct PKCTRAllocator_00c18b20 {           // 4 bytes: vtable 0x012B2F60
    virtual void v00();
};

struct PKCTRRedblackNode_00c18b20 {        // 0x14 bytes
    PKCTRRedblackNode_00c18b20* Left;      // +0x00
    PKCTRRedblackNode_00c18b20* Right;     // +0x04
    PKCTRRedblackNode_00c18b20* Parent;    // +0x08
    long                        Colour;    // +0x0c
    void*                       Key;       // +0x10
};

struct PKCTRRedblackTreeBase_00c18b20 {    // polymorphic base: its vtable store is dead and dropped
    virtual void v00();
    PKCTRRedblackTreeBase_00c18b20() { }
    ~PKCTRRedblackTreeBase_00c18b20();     // 0x00C17AD0
};

struct PKCTRRedblackTree_00c18b20 : PKCTRRedblackTreeBase_00c18b20 {   // 0x28 bytes
    PKCTRAllocator_00c18b20*    PAllocator;        // +0x04
    PKCTRAllocator_00c18b20*    POwnedAllocator;   // +0x08
    PKCTRRedblackNode_00c18b20  Sentinel;          // +0x0c
    PKCTRRedblackNode_00c18b20* Root;              // +0x20
    long                        Count;             // +0x24
    PKCTRRedblackTree_00c18b20(PKCTRAllocator_00c18b20* shared);
};

PKCTRRedblackTree_00c18b20::PKCTRRedblackTree_00c18b20(PKCTRAllocator_00c18b20* shared)
    : PAllocator(shared), POwnedAllocator(0), Root(&Sentinel), Count(0)
{
    if (shared == 0) {
        POwnedAllocator = new PKCTRAllocator_00c18b20;
        if (POwnedAllocator == 0) {
            {
                CString_00c194f0 line;
                line.Append(".\\PKCTRRedblackTree.cpp")->Append("(")->AppendNumber(35)->Append(") : ")->Append("Error: EMEM")->Append("\n");
                if (g_pfnLogSink_0143bb2c)
                    g_pfnLogSink_0143bb2c(line.m_pchData);
            }
            CTrace_00c194f0();
        }
        PAllocator = POwnedAllocator;
    }
    Sentinel.Right = &Sentinel;
    Sentinel.Left = &Sentinel;
    Sentinel.Parent = 0;
    Sentinel.Colour = 0;
    Sentinel.Key = 0;
}