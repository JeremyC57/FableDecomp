#pragma optimize("s", on)
// NPlayerGui::CDrawDamage::Update() -- age every queued damage value by one tick and drop the ones whose countdown went negative
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644CE2, length 44 bytes.
// Callees: list<SDamageValue>::erase(iterator) @ 0x00655125 (the iterator by value, built in its argument slot `push ecx ; mov ecx,esp ; mov [ecx],eax`;
// the result iterator through a hidden pointer in the 4-byte local opened by the leading `push ecx`). PDB CDrawDamage: DamageValues (list, one sentinel
// pointer) at +0x2c, retail +0x28 (the CDrawBase vector is 4 bytes smaller in the shipped build). Shape: size TU; the two ways of advancing the node
// (`erase` result vs `next`) are tail-merged into one `mov eax,[eax]` because the erase result's first word is the node pointer; the sentinel is
// re-read from the member each iteration.
extern "C" void* __cdecl malloc(unsigned int size);

namespace NPlayerGui {

struct SDamageValue {
    long  m_frame;                                 // +0x00 (node +8)
    float m_value;                                 // +0x04
    unsigned char m_flag;                          // +0x08
};

struct CRetailDamageList {                         // VC6 Dinkumware list<SDamageValue>
    struct node { node* next; node* prev; long m_countdown; float m_value; unsigned char m_flag; };
    struct iterator {
        node* m_pNode;
        iterator(node* n) : m_pNode(n) { }
        iterator(const iterator& o) : m_pNode(o.m_pNode) { }
    };
    node* head;
    iterator erase(iterator where);                // 0x00655125
};

struct CDrawDamage {
    unsigned char     _pad[0x28];
    CRetailDamageList DamageValues;                // +0x28
    void Update();
};

void CDrawDamage::Update()
{
    CRetailDamageList::node* n = DamageValues.head->next;
    while (n != DamageValues.head) {
        if (--n->m_countdown < 0)
            n = DamageValues.erase(CRetailDamageList::iterator(n)).m_pNode;
        else
            n = n->next;
    }
}

}
