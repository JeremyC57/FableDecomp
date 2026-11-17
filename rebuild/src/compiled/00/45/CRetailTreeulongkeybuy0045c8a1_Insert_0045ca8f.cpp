// CRetailTree_ulongkey_buy0045c8a1::_Insert(trynode, wherenode, const value_type&, forceRight) -- link a new node (Buynode @ 0x0045C8A1) under wherenode, rebalance @ 0x0042971D, bump the count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045CA8F, oracle length 136 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; mov eax,[ebp+14h] ; push ebx ; mov ebx,[ebp+10h] ; push esi ; push edi ; mov edi,ecx ; cmp ebx,[edi] ; je L ; cmp dword ptr [ebp+18h],0 ; jne R ; cmp dword ptr [ebp+0Ch],0 ; jne L ; mov ecx,[eax] ; cmp ecx,[ebx+10h] ; jl L ; R: push eax ; mov ecx,edi ; call Buynode ; mov esi,eax ; mov [ebx+0Ch],esi ; mov eax,[edi] ; cmp ebx,[eax+0Ch] ; jne K ; mov [eax+0Ch],esi ; jmp K ; L: push eax ; mov ecx,edi ; call Buynode ; mov esi,eax ; mov [ebx+8],esi ; mov eax,[edi] ; cmp ebx,eax ; jne M ; mov [eax+4],esi ; mov eax,[edi] ; jmp (head->right = Z) ; M: cmp ebx,[eax+8] ; jne K ; mov [eax+8],esi ; K: and dword ptr [esi+8],0 ; and dword ptr [esi+0Ch],0 ; mov [esi+4],ebx ; mov eax,[edi] ; add eax,4 ; push eax ; push esi ; call Rebalance ; inc dword ptr [edi+4] ; mov eax,[ebp+8] ; pop ecx ; pop ecx ; pop edi ; mov [eax],esi ; pop esi ; pop ebx ; pop ebp ; ret 14h
// STLport 4.6 `_Rb_tree::_M_insert(x, y, v, w)` shape (the generated stand-in keeps its Dinkumware-style names): the new node goes LEFT of wherenode when
// wherenode is the head, or (unless the fourth argument forces the right side) when a try-node was
// supplied or the key is less than wherenode's; the head's parent/left/right links are fixed up
// for a new root, leftmost or rightmost node; the node is detached, parented, rebalanced by the
// __cdecl red-black fix-up std::_Rb_global<bool>::_Rebalance @ 0x0042971D (node, root&) and counted. Returned through the hidden
// iterator pointer (`ret 14h`: hidden + four arguments). Frame pointer, `and [..],0` and shared
// call blocks: this TU is `optimize("y",off)` + `("s",on)`. One of 9 bodies.
#pragma optimize("y", off)
#pragma optimize("s", on)

// NF50 (round 7): the rebalance call binds the STLport ?_Rebalance@?$_Rb_global@_N@std@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z
// of the shared stlport/fable_map.h (was a local `Rebalance_0042971d` stand-in); `node` now derives from the header's
// std::_Rb_tree_node_base. The row's own name and the Buynode stand-in are unchanged (instance not proven here; see NF50 NOTES).
#include "stlport/fable_map.h"

struct CRetailTree_ulongkey_buy0045c8a1 {
    struct node : std::_Rb_tree_node_base { unsigned long key; };   // links +0..+0xC from stlport/fable_map.h, key +0x10
    struct iterator { node* current; iterator(node* n) : current(n) { } };
    node* head;
    unsigned long size;
    node* Buynode(const unsigned long& value);      // 0x0045C8A1
    iterator Insert(node* trynode, node* wherenode, const unsigned long& value, long forceRight);
};


CRetailTree_ulongkey_buy0045c8a1::iterator CRetailTree_ulongkey_buy0045c8a1::Insert(node* trynode, node* wherenode, const unsigned long& value, long forceRight)
{
    node* newnode;
    if (wherenode == head || (!forceRight && (trynode != 0 || value < wherenode->key))) {
        newnode = Buynode(value);
        wherenode->_M_left = newnode;
        if (wherenode == head) {
            head->_M_parent = newnode;
            head->_M_right = newnode;
        }
        else if (wherenode == head->_M_left)
            head->_M_left = newnode;
    }
    else {
        newnode = Buynode(value);
        wherenode->_M_right = newnode;
        if (wherenode == head->_M_right)
            head->_M_right = newnode;
    }
    newnode->_M_left = 0;
    newnode->_M_right = 0;
    newnode->_M_parent = wherenode;
    std::_Rb_global<bool>::_Rebalance(newnode, head->_M_parent);
    ++size;
    return iterator(newnode);
}