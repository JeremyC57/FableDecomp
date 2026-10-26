// NParticleEngine::CPSCUpdateNormal::PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system, const CParticleUpdateParameters& params, CEnginePrimitiveHandle& handle) -- vtable slot 13: drop dead decal emitters from the list, initialise any new ones against the render engine, then start the frame and prepare the primitives of each live one
// Leaf: ?PreparePrimitives@CPSCUpdateNormal@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0C910, 174 bytes (+2 int3 padding).
// Name: FableWin.pdb ?PreparePrimitives@CPSCUpdateNormal@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z;
// slot 13 of the vtable 0x0129FC1C (rtti-vtables.tsv; five stack arguments, `ret 14h`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the intelligent pointer's virtual destructor (slot 0, flag 0),
// free @ 0x00BFEA14 (the list node), CParticleEmitter::Initialise(CIEngine*) @ 0x00AEE240,
// CParticleEmitter::StartFrame(float, CEnginePrimitivePrepareInfo*) @ 0x00AEEBA0,
// CParticleEmitter::PreparePrimitives(float, const CEnginePrimitivePrepareInfo*) @ 0x00AEE560 (all three landed with
// pointer parameters; StartFrame's is non-const, so the info is passed through a const_cast).
// Layout: the landed KillAllDecalEmitters 0x00B0C730: DecalEmitterList (STLport list head node) +0x9C, node {next,
// prev, CIntelligentPointer}; the emitter flags Dead (+0x40 bit 5) and Initialised (+0x41 bit 0) read through the
// PDB's inline IsDead() / IsInitialised(); CEnginePrimitivePrepareInfo::Engine at +0x34 (after the 0x34-byte
// CInterpolationInfo, as the landed CView3DBase::Render model).
// Shape: an erase loop `it = dead ? DecalEmitterList.erase(it) : (work, ++it)` with end() re-read every pass
// (`this` homed in the `push ecx` slot); the inlined STLport erase unlinks the node (next in ebp), runs the
// element's virtual destructor and frees the node. The distance float is forwarded as a dword (ebp). `ret 14h`.
extern "C" void __cdecl free(void* p);          // 0x00BFEA14

class CIEngine;
class CEnginePrimitiveHandle;
class CEnginePrimitivePrepareInfo { public:
    unsigned char _interpolate[0x34];       // CInterpolationInfo
    CIEngine*     Engine;                   // +0x34
};

namespace NParticleEngine {
template <class A, class B> class CParticleUpdateParameters;
class CParticleSystem;
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
    bool IsInitialised() const { return Initialised; }
    bool Initialise(CIEngine* engine);                                                   // 0x00AEE240
    void StartFrame(float interpolate, CEnginePrimitivePrepareInfo* info);               // 0x00AEEBA0
    void PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo* info);     // 0x00AEE560
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
    iterator erase(iterator position)
    {
        SListNode<T>* next = position._M_node->_M_next;
        SListNode<T>* prev = position._M_node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        position._M_node->_M_data.~T();
        free(position._M_node);
        return iterator(next);
    }
};
typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> CEmitterPointer;

namespace NParticleEngine {
class CPSCUpdateNormal { public:
    virtual void v00();
    unsigned char                _pad04[0x9c - 4];
    CRetailList<CEmitterPointer> DecalEmitterList;     // +0x9c
    virtual void PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                   CEnginePrimitiveHandle& handle);
};

void CPSCUpdateNormal::PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                         const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                         CEnginePrimitiveHandle& handle)
{
    CRetailList<CEmitterPointer>::iterator it = DecalEmitterList.begin();
    while (it != DecalEmitterList.end()) {
        if (it->Get()->IsDead()) {
            it = DecalEmitterList.erase(it);
        } else {
            if (!it->Get()->IsInitialised())
                it->Get()->Initialise(info.Engine);
            it->Get()->StartFrame(distance, const_cast<CEnginePrimitivePrepareInfo*>(&info));
            it->Get()->PreparePrimitives(distance, &info);
            ++it;
        }
    }
}
} // namespace NParticleEngine