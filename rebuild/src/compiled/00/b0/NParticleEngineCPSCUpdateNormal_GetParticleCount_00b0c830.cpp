// NParticleEngine::CPSCUpdateNormal::GetParticleCount() -- vtable slot 26: the number of live particles in the decal emitters this component spawned (dead or uninitialised emitters skipped)
// Leaf: ?GetParticleCount@CPSCUpdateNormal@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0C830, 97 bytes (+15 int3 padding).
// Name: FableWin.pdb ?GetParticleCount@CPSCUpdateNormal@NParticleEngine@@UAEJXZ; slot 26 of the vtable 0x0129FC1C (rtti-vtables.tsv;
// CPSCBase declares GetParticleCount then GetDecalCount, slots 26/27; the twin rows 0x00B0C830/0x00B0C8A0 differ only
// in the per-emitter callee).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (four times per element), CParticleEmitter::GetNoParticles
// @ 0x00AEDCA0 (landed under the global class name ::CParticleEmitter; kept so the address binds one symbol).
// Layout: the landed KillAllDecalEmitters 0x00B0C730: DecalEmitterList (STLport list head node) at +0x9C, node
// {next, prev, CIntelligentPointer (vptr, item)}; the emitter flags Dead (+0x40 bit 5), Initialised (+0x41 bit 0).
// Shape: `for (it = begin(); it != end(); ++it)` with end() re-read each pass (`cmp edi,[ebp+9Ch]`); each
// test re-fetches the item through the out-of-line GetPItem; the flags are read through the PDB's inline
// `bool IsDead() const` / `IsInitialised() const` (the bool return gives `mov al ; shr al,5 ; test al,1`; a direct
// bit-field test is `test byte ptr [eax+40h],20h`); the sum lives in ebx. Plain `ret`.
struct CParticleEmitter {                   // NParticleEngine::CParticleEmitter (landed callee spelling)
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
    bool IsDead() const { return Dead; }
    bool IsInitialised() const { return Initialised; }
    long GetNoParticles();                         // 0x00AEDCA0
};

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    void* m_pItem;                          // +0x04
    void* GetPItem() const;                 // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
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
};
typedef CIntelligentPointer<CParticleEmitter> CEmitterPointer;

namespace NParticleEngine {
struct CPSCUpdateNormal {
    virtual void v00();
    unsigned char                _pad04[0x9c - 4];
    CRetailList<CEmitterPointer> DecalEmitterList;     // +0x9c
    virtual long GetParticleCount();
};

long CPSCUpdateNormal::GetParticleCount()
{
    long count = 0;
    for (CRetailList<CEmitterPointer>::iterator it = DecalEmitterList.begin(); it != DecalEmitterList.end(); ++it)
    {
        if (it->Get() && !it->Get()->IsDead() && it->Get()->IsInitialised())
            count += it->Get()->GetNoParticles();
    }
    return count;
}
} // namespace NParticleEngine