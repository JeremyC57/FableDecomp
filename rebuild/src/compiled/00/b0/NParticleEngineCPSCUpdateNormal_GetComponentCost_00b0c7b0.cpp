// NParticleEngine::CPSCUpdateNormal::GetComponentCost(CParticleSystemCost& cost, ...) -- add the cost of every live, initialised decal emitter this component spawned, counting each one's render/setup/light cost twice
// Leaf: GetComponentCost
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0C7B0, length 123 bytes (padded to 128). Virtual: slot 20 of
// NParticleEngine::CPSCUpdateNormal's vtable 0x0129FC1C (`ret 10h`: four stack arguments, only the first is used).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (up to four times per element), CParticleEmitter::GetEmitterCost
//          (CParticleSystemCost&) @ 0x00AED020.
// Layout: CParticleSystemCost (PDB): RenderCost +0xc, SetupCost +0x10, LightCost +0x14. CParticleEmitter flag word +0x40
// (unsigned short bit-fields): Dead bit 5, Initialised bit 8 (+0x41 bit 0). DecalEmitterList (STLport list) +0x9c.
// Shape: nested tests each re-fetching the item through the out-of-line GetPItem; the Dead bit is read through an inline
// bool accessor (`shr al,5 ; test al,1`; the raw bit-field folds to `test byte ptr [eax+40h],20h`); after GetEmitterCost the cost fields are
// doubled in LightCost, RenderCost, SetupCost order (`x *= 2.0f` -> `fadd st(0),st(0)`).
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
    bool IsDead() const { return Dead; }
    void Kill();                                            // 0x00AEE8C0
    void GetEmitterCost(CParticleSystemCost& cost);         // 0x00AED020
};
}
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual ~CIntelligentPointer();
    T* Get() const { return (T*)GetPItem(); }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
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
    void GetComponentCost(CParticleSystemCost& cost, long unused1, long unused2, long unused3);
};

void CPSCUpdateNormal::GetComponentCost(CParticleSystemCost& cost, long, long, long)
{
    for (CRetailList<CEmitterPointer>::iterator it = DecalEmitterList.begin(); it != DecalEmitterList.end(); ++it)
    {
        if (it->Get() && !it->Get()->IsDead() && it->Get()->Initialised)
        {
            it->Get()->GetEmitterCost(cost);
            cost.LightCost *= 2.0f;
            cost.RenderCost *= 2.0f;
            cost.SetupCost *= 2.0f;
        }
    }
}
}