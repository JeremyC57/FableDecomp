// NParticleEngine::CPSCUpdateNormal::KillAllDecalEmitters() -- kill every still-alive decal emitter this update component spawned, then empty the decal-emitter list
// Leaf: KillAllDecalEmitters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0C730, length 121 bytes (padded to 128).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice per element), CParticleEmitter::Kill @ 0x00AEE8C0, the
//          intelligent pointer's virtual destructor (slot 0, flag 0), free @ 0x00BFEA14 (node deallocation).
// Layout: PDB CPSCUpdateNormal::DecalEmitterList (list<CIntelligentPointer<CParticleEmitter>>) at +0x9c, which in retail is an
// STLport list: one head-node pointer; a node is {next, prev, CIntelligentPointer (vptr, item)}.
// Shape: `for (it = begin(); it != end(); ++it) if (it->Get()) it->Get()->Kill();` with end() re-read every pass, then
// the inlined STLport list::clear(): the cursor advances before the element's explicit virtual destructor call
// (`push 0 ; call [eax]`) and the node is freed; the head is re-linked to itself.
extern "C" void __cdecl free(void* p);          // 0x00BFEA14

namespace NParticleEngine {
class CParticleSystemCost { public:
    float PolyCount;                        // +0x00
    float PixelCount;                       // +0x04
    float LightCount;                       // +0x08
    float RenderCost;                       // +0x0c
    float SetupCost;                        // +0x10
    float LightCost;                        // +0x14
};
class CParticleEmitter { public:
    unsigned char  _pad00[0x40];
    unsigned short Priority : 3;            // +0x40 bits 0-2
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;                // bit 5
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;         // +0x41 bit 0
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    void Kill();                                            // 0x00AEE8C0
    void GetEmitterCost(CParticleSystemCost& cost);         // 0x00AED020
};
}
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    void* m_pItem;                          // +0x04
    void* GetPItem() const;                 // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual ~CIntelligentPointer();
    T* Get() const { return (T*)GetPItem(); }
};
template <class T> struct SListNode {       // STLport _List_node
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CRetailList {     // STLport list (4 bytes: the head node)
    SListNode<T>* _M_data;
    struct iterator {
        SListNode<T>* _M_node;
        iterator(SListNode<T>* n) : _M_node(n) {}
        T* operator->() const { return &_M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    iterator begin() const { return iterator(_M_data->_M_next); }
    iterator end() const { return iterator(_M_data); }
    void clear()
    {
        SListNode<T>* cur = _M_data->_M_next;
        while (cur != _M_data) {
            SListNode<T>* tmp = cur;
            cur = cur->_M_next;
            tmp->_M_data.~T();
            free(tmp);
        }
        _M_data->_M_next = _M_data;
        _M_data->_M_prev = _M_data;
    }
};
typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> CEmitterPointer;

namespace NParticleEngine {
class CPSCUpdateNormal { public:
    unsigned char                _pad00[0x9c];
    CRetailList<CEmitterPointer> DecalEmitterList;     // +0x9c
    void KillAllDecalEmitters();
};

void CPSCUpdateNormal::KillAllDecalEmitters()
{
    for (CRetailList<CEmitterPointer>::iterator it = DecalEmitterList.begin(); it != DecalEmitterList.end(); ++it)
    {
        if (it->Get())
            it->Get()->Kill();
    }
    DecalEmitterList.clear();
}
}