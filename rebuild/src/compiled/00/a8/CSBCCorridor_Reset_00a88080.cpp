// CSBCCorridor::Reset() -- clears the Walls list: frees every node from head->next back to the head, then re-links the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A88080, length 50 bytes.
// Leaf: ?Reset@CSBCCorridor
// Retail bytes: push esi ; push edi ; mov edi,ecx ; mov eax,[edi+18h] ; mov esi,[eax] ; cmp esi,eax ; je E ; L: mov eax,esi ; mov esi,[esi] ; push eax ;
//               call free ; mov eax,[edi+18h] ; add esp,4 ; cmp esi,eax ; jne L ; E: mov eax,[edi+18h] ; mov [eax],eax ; mov edi,[edi+18h] ; mov [edi+4],edi ; ret
// Callee: free @ 0x00BFEA14. Virtual (`?Reset@CSBCCorridor@@UAEXXZ`).
// The PDB layout (engine/CSBCCorridor.h) puts Walls (list<CWall>) at +0x18 as a 12-byte debug list; retail TLC uses the
// 4-byte STLport list (the sentinel node pointer, no size word), so the members after it sit 8 bytes lower in retail
// and this file models the list itself. CWall is a POD (no destructor call per node). Shape: the inlined STLport
// _List_base::clear -- the cursor advances before the old node is freed, the head is re-read from the member on each
// iteration and for each of the two final stores.
#include "engine/CSBCCorridor.h"

extern "C" void __cdecl free(void* block);    // 0x00BFEA14

struct CWall {
    float m_start[2];
    float m_end[2];
};

struct CRetailWallList {                       // STLport list<CWall>: the sentinel node pointer
    struct node {
        node* next;                            // +0x00
        node* prev;                            // +0x04
        CWall value;                           // +0x08
    };
    node* m_pNode;
    void clear()
    {
        node* n = m_pNode->next;
        while (n != m_pNode) {
            node* victim = n;
            n = n->next;
            free(victim);
        }
        m_pNode->next = m_pNode;
        m_pNode->prev = m_pNode;
    }
};

struct CSBCCorridorRetail {                    // retail model of CSBCCorridor
    virtual void Reset();
    unsigned char   _base[0x14];               // +0x04 steering-behaviour base members
    CRetailWallList Walls;                     // +0x18
};
FABLE_STATIC_ASSERT(offsetof(CSBCCorridor, Walls) == 0x18);

void CSBCCorridorRetail::Reset()
{
    Walls.clear();
}