// CTCHeroMorph::KillIdleParticles() -- kill and drop every idle particle emitter thing, popping them off the back of the IdleParticleEmitters list
// Leaf: KillIdleParticles
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071C1F0, length 104 bytes (+8 int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice per element), CThing::Kill(bool) @ 0x004C9B80, the element's
// virtual destructor (slot 0, flag 0), free @ 0x00BFEA14 (STLport malloc-allocator node).
// Shape: `while (!empty()) { if (<last>.GetPItem()) back().GetPItem()->Kill(true); pop_back(); }` over the inline STLport list:
// the test reads the last node's data directly (`mov ecx,[eax+4] ; add ecx,8`), the Kill goes through STLport back() =
// `*--end()` with an iterator temporary (`mov eax,[ecx+4] ; lea ecx,[eax+8]`); pop_back unlinks the node, destroys the CIntelligentPointer through its
// virtual destructor and frees the node.
// Layout: retail PMeshMorphSet +0x0c (CCountedPointer), ParticleEmitters +0x14, IdleParticleEmitters +0x18 (4-byte STLport lists).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

class CThing {
public:
    void Kill(bool b);                          // 0x004C9B80
};
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
};
template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CListIterator {       // STLport _List_iterator
    SListNode<T>* _M_node;
    CListIterator(SListNode<T>* n) : _M_node(n) {}
    CListIterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
    T& operator*() const { return _M_node->_M_data; }
};
template <class T> struct CRetailList {         // STLport list (4 bytes: the head node)
    SListNode<T>* _M_node;
    CListIterator<T> end() { return CListIterator<T>(_M_node); }
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& back() { return *(--end()); }
    void pop_back()
    {
        SListNode<T>* n = _M_node->_M_prev;
        SListNode<T>* next = n->_M_next;
        SListNode<T>* prev = n->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        n->_M_data.~T();
        free(n);
    }
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCHeroMorph : public CTCBase {
public:
    void* PMeshMorphSet[2];                     // +0x0c CCountedPointer<CMeshMorphSet>
    CRetailList<CIntelligentPointer<CThing> > ParticleEmitters;       // +0x14
    CRetailList<CIntelligentPointer<CThing> > IdleParticleEmitters;   // +0x18
    void KillIdleParticles();
};

void CTCHeroMorph::KillIdleParticles()
{
    while (!IdleParticleEmitters.empty()) {
        if (IdleParticleEmitters._M_node->_M_prev->_M_data.GetPItem())
            IdleParticleEmitters.back().GetPItem()->Kill(true);
        IdleParticleEmitters.pop_back();
    }
}
