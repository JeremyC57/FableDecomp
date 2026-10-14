// static CRetailTree @ 0x013BAFB4 atexit destructor -- clear (recursive Erase @ 0x008B1D80 from the root, header relinked to itself, size zeroed) then free the header node (free @ 0x00BFEA14)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122A100, oracle length 84 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[size] ; test eax,eax ; je F ; mov eax,[header] ; mov ecx,[eax+4] ; push ecx ; mov ecx,offset tree ; call Erase ; mov eax,[header] ; mov [eax+8],eax ; mov edx,[header] ; mov dword ptr [edx+4],0 ; mov eax,[header] ; mov [eax+0Ch],eax ; mov dword ptr [size],0 ; F: mov eax,[header] ; test eax,eax ; je R ; push eax ; call free ; pop ecx ; R: ret
// Compiler-generated dynamic atexit destructor (`??__F`) of a static STLport-style tree object at
// 0x013BAFB4 (header pointer, then the node count): the inlined destructor clears the tree when it
// holds nodes -- the recursive node release @ 0x008B1D80 from the root, the header's left,
// parent and right links reset (each through a fresh load of the header pointer, since the
// stores may alias the static) and the count zeroed -- then returns the header node to the
// allocator, which frees it when non-null (free @ 0x00BFEA14). The manifest label is a
// descriptive name for the static; the element type is irrelevant here. One of 15
// bodies of this shape.
extern "C" void __cdecl free(void* block);

struct SNode {
    long m_color;                          // +0
    SNode* m_parent;                       // +4
    SNode* m_left;                         // +8
    SNode* m_right;                        // +0xC
};

struct CRetailTree {
    SNode* m_header;                       // +0
    long m_size;                           // +4
    void Erase(SNode* root);               // 0x008B1D80 recursive node release
    void Clear()
    {
        if (m_size != 0) {
            Erase(m_header->m_parent);
            m_header->m_left = m_header;
            m_header->m_parent = 0;
            m_header->m_right = m_header;
            m_size = 0;
        }
    }
    ~CRetailTree()
    {
        Clear();
        if (m_header)
            free(m_header);
    }
};

static CRetailTree g_Cleanup_LTextTreeWalker_3_0122a100;             // 0x013BAFB4