// CSBCCorridor::Reset() -- clears the Walls list: frees every node from head->next back to the head, then re-links the head to itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A88080, length 50 bytes.
// Leaf: ?Reset@CSBCCorridor@@UAEXXZ
// Retail bytes: push esi ; push edi ; mov edi,ecx ; mov eax,[edi+18h] ; mov esi,[eax] ; cmp esi,eax ; je E ; L: mov eax,esi ; mov esi,[esi] ; push eax ;
//               call free ; mov eax,[edi+18h] ; add esp,4 ; cmp esi,eax ; jne L ; E: mov eax,[edi+18h] ; mov [eax],eax ; mov edi,[edi+18h] ; mov [edi+4],edi ; ret
// Callee: free @ 0x00BFEA14. Virtual (`?Reset@CSBCCorridor@@UAEXXZ`).
// The PDB layout (engine/CSBCCorridor.h) puts Walls (list<CWall>) at +0x18 as a 12-byte debug list; retail TLC uses the
// 4-byte STLport list (the sentinel node pointer, no size word), so the members after it sit 8 bytes lower in retail
// and this file models the class locally (the header's later offsets disagree, so it is not included). CWall is the
// 24-byte POD of Ego_r.pdb (Point1, Point2, ToInternal: three C2DVectors; list<CWall>::push_back 0x00A88B10 copies six
// dwords), so a node is 0x20 bytes; no destructor call per node. Shape: the inlined STLport
// _List_base::clear -- the cursor advances before the old node is freed, the head is re-read from the member on each
// iteration and for each of the two final stores.
extern "C" void __cdecl free(void* block);    // 0x00BFEA14

class C2DVector {
public:
    float X, Y;
};

class CWall {                                  // 24 bytes (Ego_r.pdb)
public:
    C2DVector Point1;                          // +0x00
    C2DVector Point2;                          // +0x08
    C2DVector ToInternal;                      // +0x10
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

class CSBCCorridor {                           // retail layout (STLport list)
public:
    virtual void Reset();
    unsigned char   _base[0x14];               // +0x04 steering-behaviour base members
    CRetailWallList Walls;                     // +0x18
    C2DVector       CurrentEndPoint;           // +0x1C (PDB +0x24)
    float           BestLeft;                  // +0x24 (PDB +0x2C)
    float           BestRight;                 // +0x28 (PDB +0x30)
};

void CSBCCorridor::Reset()
{
    Walls.clear();
}