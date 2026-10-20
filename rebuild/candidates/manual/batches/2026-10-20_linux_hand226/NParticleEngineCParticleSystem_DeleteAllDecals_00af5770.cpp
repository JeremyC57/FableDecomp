// NParticleEngine::CParticleSystem::DeleteAllDecals() -- delete every decal of the system: clear its primitives, destroy and pool-free the decal, unlink and free the list node, count it down; then empty the list and zero the decal count
// Leaf: DeleteAllDecals
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF5770, 253 bytes, `ret`. Called from CParticleSystem::Update (0x00AF59A0) right after
//   DeleteAllParticles 0x00AF56E0: the PDB pair is ?DeleteAllParticles / ?DeleteAllDecals@CParticleSystem@NParticleEngine@@QAEXXZ
//   (catalogued "CParticleSystem::DestroyDecals").
// Callees: CParticleSystem::ClearDecal(list<CDecal*>::iterator&) 0x00AF5430 (PDB), CDecal::operator delete 0x00AF8AD0 (PDB
//   ??3CDecal@NParticleEngine@@SIXPAX@Z, a static `__fastcall` pool free), free 0x00BFEA14 (STLport node / vector storage).
// Layout: CDecal (Ego_r.pdb 0x7C, vtable 0x0129F4FC): PrimIDList CArray<CArray<long>> +0x5C, ParentHandleList
//   CArray<CEnginePrimitiveHandle> +0x68 (12-byte STLport vectors; 8-byte polymorphic handles destroyed through their
//   virtual destructor); non-virtual ~CDecal inline. CParticleSystem: Decals list +0x20 (4-byte node pointer),
//   NoActiveDecals +0x4C.
// Shape: a stand-in ClearDecal body sits above the function under `auto_inline(off)` (see there); the PDB's
//   DeleteDecal(iterator&) is inline (ClearDecal, `delete *it`, erase, --NoActiveDecals, return the next
//   node); STLport `_Destroy(first, last)` takes its bounds by value (the end is loaded first); the list clear walks with `victim = n; n = n->next; free(victim)` re-reading the head after every free.
extern "C" void __cdecl free(void* block);   // 0x00BFEA14

namespace NParticleEngine {

template <class T> inline void _Destroy(T* first, T* last)   // STLport range destroy (bounds by value)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T> struct vector {             // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

class CEnginePrimitiveHandle {                 // 8 bytes, polymorphic
public:
    virtual ~CEnginePrimitiveHandle();
    void* Data;
};

class CDecal {                                 // 0x74 bytes in retail, vtable 0x0129F4FC
public:
    virtual void Delete();
    unsigned char                   _body04[0x58];
    vector<vector<long> >           PrimIDList;        // +0x5c CArray<CArray<long> >
    vector<CEnginePrimitiveHandle>  ParentHandleList;  // +0x68 CArray<CEnginePrimitiveHandle>
    ~CDecal() {}
    static void __fastcall operator delete(void* p);   // 0x00AF8AD0
};

template <class T> struct list {               // STLport list: the 4-byte node pointer
    struct _Node { _Node* _M_next; _Node* _M_prev; T _M_data; };
    struct iterator {
        _Node* _M_node;
        T& operator*() const { return _M_node->_M_data; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _Node* _M_node;
    iterator begin() const { iterator it; it._M_node = _M_node->_M_next; return it; }
    iterator end() const { iterator it; it._M_node = _M_node; return it; }
    iterator erase(iterator position)
    {
        _Node* next = position._M_node->_M_next;
        _Node* prev = position._M_node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(position._M_node);
        iterator it; it._M_node = next; return it;
    }
    void clear()
    {
        _Node* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _Node* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};

class CParticle;

class CParticleSystem {
public:
    virtual ~CParticleSystem();
    unsigned char           _pad04[0x18];
    list<CParticle*>        Particles;             // +0x1c
    list<CDecal*>           Decals;                // +0x20
    unsigned char           _pad24[0x28];
    long                    NoActiveDecals;        // +0x4c
    void ClearDecal(list<CDecal*>::iterator& it);  // 0x00AF5430
    list<CDecal*>::iterator DeleteDecal(list<CDecal*>::iterator& it)
    {
        ClearDecal(it);
        delete *it;
        list<CDecal*>::iterator next = Decals.erase(it);
        --NoActiveDecals;
        return next;
    }
    void DeleteAllDecals();
};

// Stand-in for the same-TU ClearDecal 0x00AF5430 (it sits just above this function in retail): VC7.1 uses the facts of a
// callee compiled earlier in the TU, here that ClearDecal only reads the iterator, so the node stays cached in ebx across the
// call. The real body releases the decal's primitives through *it; this stand-in keeps only that read and an opaque call.
void ReleaseDecalPrimitives(CParticleSystem* system, CDecal* decal);   // stand-in callee (not graded)
#pragma auto_inline(off)
void CParticleSystem::ClearDecal(list<CDecal*>::iterator& it)
{
    ReleaseDecalPrimitives(this, *it);
}
#pragma auto_inline(on)

void CParticleSystem::DeleteAllDecals()
{
    list<CDecal*>::iterator it = Decals.begin();
    while (it != Decals.end())
        it = DeleteDecal(it);
    Decals.clear();
    NoActiveDecals = 0;
}

} // namespace NParticleEngine
