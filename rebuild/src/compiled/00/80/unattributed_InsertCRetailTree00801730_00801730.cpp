// CRetailTree_00801730::Insert(SIterator*, SNode*, SNode*, const SValue&, SNode*) -- STLport _M_insert: a 0x18-byte node (malloc @ 0x00BFEA0E) linked left or right of y, header root/leftmost/rightmost fixed, rebalance @ 0x0042971D, size incremented, the iterator written through the result pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00801730, oracle length 191 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; mov ebx,[y] ; push esi ; mov esi,[value] ; push edi ; mov edi,ecx ; cmp ebx,[edi] ; je L ; mov eax,[w] ; test eax,eax ; jne R ; mov eax,[x] ; test eax,eax ; jne L ; mov eax,[esi] ; cmp eax,[ebx+10h] ; jl L ; R: push 0x18 ; call malloc ; lea ecx,[eax+0x10] ; add esp,4 ; test ecx,ecx ; je S ; (two dwords copied) ; S: mov [ebx+0Ch],eax ; mov ecx,[edi] ; cmp ebx,[ecx+0Ch] ; mov esi,eax ; jne J ; mov [ecx+0Ch],eax ; jmp J ; L: push 0x18 ; call malloc ; lea ecx,[eax+0x10] ; add esp,4 ; test ecx,ecx ; je T ; (two dwords copied) ; T: mov [ebx+8],eax ; mov ecx,[edi] ; cmp ebx,ecx ; mov esi,eax ; jne U ; mov [ecx+4],eax ; mov ecx,[edi] ; mov [ecx+0Ch],eax ; jmp J ; U: cmp ebx,[ecx+8] ; jne J ; mov [ecx+8],eax ; J: mov [esi+4],ebx ; mov dword ptr [esi+8],0 ; mov dword ptr [esi+0Ch],0 ; mov edx,[edi] ; add edx,4 ; push edx ; push esi ; call Rebalance ; mov eax,[edi+4] ; add esp,8 ; inc eax ; mov [edi+4],eax ; mov eax,[result] ; pop edi ; mov [eax],esi ; pop esi ; pop ebx ; ret 14h
// STLport `_Rb_tree::_M_insert(x, y, v, w)`: the new node goes to the left of y when y is the
// header, or (with w null) when x is non-null or the key orders before y's; otherwise to the
// right. The node is malloc'd (0x18 bytes) and its 8-byte POD value placement-copied at
// +0x10 (hence the null test); the header's root/rightmost (y == header), leftmost or
// rightmost links are fixed, the node's parent/children set, the __cdecl rebalance @ 0x0042971D
// run with the root by reference, the count incremented and the iterator written through the
// explicit result pointer. Labels are propagated; names are descriptive. One of 15 bodies.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);

struct SValue {
    long m_key;
    long m_data;
};

struct SNode {
    long m_color;                          // +0
    SNode* m_parent;                       // +4
    SNode* m_left;                         // +8
    SNode* m_right;                        // +0xC
    SValue m_value;                        // +0x10
};

struct SIterator {
    SNode* m_node;
};

extern "C" void __cdecl Rebalance_0042971d(SNode* node, SNode*& root);   // 0x0042971D

struct CRetailTree_00801730 {
    SNode* m_header;                       // +0
    unsigned long m_size;                  // +4
    static SNode* CreateNode(const SValue& value)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        new (&node->m_value) SValue(value);
        return node;
    }
    SIterator* Insert_CRetailTree_00801730(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w);
};

SIterator* CRetailTree_00801730::Insert_CRetailTree_00801730(SIterator* result, SNode* x, SNode* y, const SValue& value, SNode* w)
{
    SNode* z;
    if (y == m_header || (w == 0 && (x != 0 || value.m_key < y->m_value.m_key))) {
        z = CreateNode(value);
        y->m_left = z;
        if (y == m_header) {
            m_header->m_parent = z;
            m_header->m_right = z;
        }
        else if (y == m_header->m_left)
            m_header->m_left = z;
    }
    else {
        z = CreateNode(value);
        y->m_right = z;
        if (y == m_header->m_right)
            m_header->m_right = z;
    }
    z->m_parent = y;
    z->m_left = 0;
    z->m_right = 0;
    Rebalance_0042971d(z, m_header->m_parent);
    ++m_size;
    result->m_node = z;
    return result;
}