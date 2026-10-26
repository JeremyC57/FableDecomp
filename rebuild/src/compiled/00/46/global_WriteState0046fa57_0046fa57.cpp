#pragma optimize("s",on)
// WriteState(CDataOutputStream*, STree*) -- writes the count at +4 and, for every node from the leftmost to the head, the two dword fields at +0x10/+0x14 through the stream writers @ 0x00993EB0; successor through the __cdecl helper @ 0x004292C0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046FA57, oracle length 61 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; push edi ; mov edi,edx ; push [edi+4] ; mov ebx,ecx ; call Write ; mov eax,[edi] ; mov esi,[eax+8] ; cmp esi,eax ; je E ; L: push [esi+10h] ; mov ecx,ebx ; call Write ; push [esi+14h] ; mov ecx,ebx ; call Write ; push esi ; call Next ; mov esi,eax ; cmp esi,[edi] ; pop ecx ; jne L ; E: pop edi ; pop esi ; pop ebx ; ret
// __fastcall serialiser of a tree-backed container: the element count at +4 is written first,
// then every node from the leftmost (+8 of the head) until the head is reached again writes its
// fields at +0x10 and +0x14 (both as dwords); the successor comes from the __cdecl
// helper @ 0x004292C0 (its argument popped into ecx: a size TU) and the head is re-read from the
// tree each iteration. Labels are propagated (`CDataOutputStream::WriteCBYTE`, `GetNoAllocatedAreas`);
// names are descriptive. One of 12 bodies of this shape.
struct SNode {
    unsigned char _pad[0x10];
    long m_10;                             // +0x10
    long m_14;                           // +0x14
};

struct STree {
    SNode* m_head;                         // +0 (leftmost at +8 of the head)
    long m_count;                          // +4
};

struct SHead {
    unsigned char _pad[8];
    SNode* m_leftmost;                     // +8
};

class CDataOutputStream { public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};

SNode* __cdecl TreeNext_004292c0(SNode* node);   // 0x004292C0

void __fastcall WriteState_0046fa57(CDataOutputStream* out, STree* tree)
{
    out->WriteSLONG(tree->m_count);
    for (SNode* node = ((SHead*)tree->m_head)->m_leftmost; node != (SNode*)tree->m_head; node = TreeNext_004292c0(node)) {
        out->WriteSLONG(node->m_10);
        out->WriteSLONG(node->m_14);
    }
}