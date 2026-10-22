// Grade: DIFFER(572v607) -- the ClearParticle (0x00AF5090) twin: retail's 8-byte aligned ebp frame, unrotated loops with the group index/offset in frame slots and the materialised handle test (setne) are not reproduced
// NParticleEngine::CParticleSystem::ClearDecal(const list<CDecal*>::iterator& decal) -- remove a decal's engine primitives: for each parent group handle (stopping at the first empty one) remove every non-negative primitive id recorded for that group from the engine, then clear the group handle and its id list; finally empty both lists
// Leaf: ClearDecal
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF5430, 597 bytes (+ int3 padding), `ret 4`, 8-byte aligned frame.
// Callees: CBaseClass() 0x0099A2D0 / ~CBaseClass() 0x0099A2E0 (the local handle's root), the primitive's virtual slots 3
//   (+0x0c, last handle released) and 1 (+0x04, destroy), the engine's virtual slot 30 (+0x78, RemovePrimitive), the
//   STLport __copy<CEnginePrimitiveHandle*> 0x00440260 (__fastcall, label "_Move"), memmove 0x00BFEAE6, the
//   vector<CArray<long> >::erase(first, last) 0x00AF7910 (label "Vector_CopyAndDeleteOld" propagated).
// Layout: Ego_r.pdb CParticleSystem: PEngine +0x08. CDecal (retail = PDB minus 4 from the 12-byte CArrays):
//   PrimIDList +0x5c (CArray<CArray<long> >), ParentHandleList +0x68 (CArray<CEnginePrimitiveHandle>, 8-byte handles:
//   vtable 0x0122F598 + counted primitive). The handle model is the landed CPSCLight destructor 0x00B1AEB0 one.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
struct CBaseClass {
    CBaseClass();                                      // 0x0099A2D0
    virtual void v00();
    ~CBaseClass();                                     // 0x0099A2E0
};
class CEnginePrimitiveHandle;
struct CEngineInternalPrimitiveBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0x0c)
    long m_handleCount;                                // +0x04
};
struct CIVCountedPointer {
    CEngineInternalPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) {}
    long GetRefCount() const { return Object ? Object->m_handleCount : 0; }
    __forceinline void Release()
    {
        CEngineInternalPrimitiveBase* object = Object;
        if (object) {
            if (--object->m_handleCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->m_handleCount;
        return *this;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};
class CEnginePrimitiveHandle : public CBaseClass {    // vtable 0x0122F598
public:
    CIVCountedPointer Data;                            // +0x04
    CEnginePrimitiveHandle() {}
    virtual void v00();
    bool IsValid() const { return Data.Object != 0; }
    __forceinline void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other)
    {
        Clear();
        Data = other.Data.Object;
        return *this;
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};
struct random_access_iterator_tag {};
CEnginePrimitiveHandle* __fastcall __copy(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CEnginePrimitiveHandle* result, const random_access_iterator_tag&, int*);   // 0x00440260
template <class T> struct CVector {                    // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long i) { return _M_start[i]; }
    T* erase(T* first, T* last);                       // out of line for CArray<long> elements: 0x00AF7910
    __forceinline T* erase_inline(T* first, T* last)
    {
        T* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        for (T* p = i; p != _M_finish; ++p)
            p->~T();
        _M_finish = i;
        return first;
    }
};
template <> struct CVector<long> {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    long& operator[](unsigned long i) { return _M_start[i]; }
    __forceinline void clear()
    {
        long* first = _M_start;
        long* last = _M_finish;
        long* i;
        if (last == _M_finish)
            i = first;
        else {
            unsigned int n = (unsigned int)((char*)_M_finish - (char*)last);
            memmove(first, last, n);
            i = (long*)((char*)first + n);
        }
        _M_finish = i;
    }
};
struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void RemovePrimitive(const CEnginePrimitiveHandle& group, long primitiveId);   // slot 30 (+0x78)
};
namespace NParticleEngine {
struct CDecal {
    unsigned char                   _state00[0x5c];
    CVector<CVector<long> >         PrimIDList;        // +0x5c
    CVector<CEnginePrimitiveHandle> ParentHandleList;  // +0x68
    __forceinline bool GetPrimitiveID(unsigned long group, unsigned long index, long& id)
    {
        if (group < PrimIDList.size() && index < PrimIDList[group].size()) {
            id = PrimIDList[group][index];
            return id >= 0;
        }
        return false;
    }
};
struct SDecalNode {
    SDecalNode* _M_next;
    SDecalNode* _M_prev;
    CDecal*     _M_data;
};
struct CDecalIterator { SDecalNode* _M_node; };
class CParticleSystem {
public:
    void*     __vftable;                               // +0x00
    void*     PParentEmitter;                          // +0x04
    CIEngine* PEngine;                                 // +0x08
    void ClearDecal(const CDecalIterator& decal);
};

void CParticleSystem::ClearDecal(const CDecalIterator& it)
{
    CDecal* decal = it._M_node->_M_data;
    CEnginePrimitiveHandle handle;
    for (unsigned long group = 0; group < decal->ParentHandleList.size(); ++group) {
        handle = decal->ParentHandleList[group];
        if (!handle.IsValid())
            break;
        if (PEngine && handle.IsValid()) {
            long id;
            for (unsigned long index = 0; decal->GetPrimitiveID(group, index, id); ++index)
                PEngine->RemovePrimitive(handle, id);
        }
        if (group < decal->ParentHandleList.size()) {
            decal->ParentHandleList[group].Clear();
            decal->PrimIDList[group].clear();
        }
    }
    decal->ParentHandleList.erase_inline(decal->ParentHandleList._M_start, decal->ParentHandleList._M_finish);
    decal->PrimIDList.erase(decal->PrimIDList._M_start, decal->PrimIDList._M_finish);
}
}
