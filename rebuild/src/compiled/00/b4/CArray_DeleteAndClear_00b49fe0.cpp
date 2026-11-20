// CArray::DeleteAndClear() -- (CArray<CEngineLightingManager::CManagerLocalLight*>; descriptive name) delete every queued local light through its class heap, then empty the array
// Leaf: ?DeleteAndClear@?$CArray@PAUCManagerLocalLight@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B49FE0, 121 bytes. __fastcall (this = the array), plain `ret`.
// Identity: no PDB name (neither FableWin.pdb nor Ego_r.pdb lists a CArray<T*> member beyond ctor/dtor/[]/push_back/
//   DeallocateMemory/Find/At), no caller in the image (an unreferenced COMDAT): the same loop + clear sits inline in the
//   landed CEngineLightingManager::FrameEnd 0x00B47980 for LocalLightList. Element: CEngineLightingManager::
//   CManagerLocalLight, whose static `__fastcall` operator delete (`??3CManagerLocalLight@CEngineLightingManager@@SIXPAX@Z`)
//   is the inline CDynamicObjectHeap::Free on the local-light heap 0x01436FB0 (the landed FrameEnd / AddLightPrimitive
//   0x00B47FD0 model). The catalog label "RefCountedObjectVector_Clear" is descriptive.
// Callees: CDynamicObjectHeapBase::RefCountedRelease 0x00F36CA0 (the heap's last free), memmove 0x00BFEAE6 (STLport
//   __copy_trivial in clear()).
// Shape: `for (i = 0; i < size(); ++i) delete (*this)[i]; clear();` -- the trivially destructible element's delete is
//   the bare class operator delete (no null test); clear() is erase(begin(), end()) through __copy_trivial (the
//   `cmp eax,eax` of finish against itself).
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

// Naming (NF183, 2026-11-20, q4505): the local CDynamicObjectHeapBase / CDynamicObjectHeap model moved onto the shared
//   engine/CDynamicObjectHeap.h (inline Free through the NF183 opt-in FABLE_RETAIL_CDYNAMICOBJECTHEAP_FREE_INLINE); the heap is its PDB
//   static LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight (Ego_d.pdb public; Ego_r.pdb + FableWin.pdb dynamic initializer, the
//   landed initializer 0x01224730), was the descriptive g_LocalLightHeap.
#define FABLE_RETAIL_CDYNAMICOBJECTHEAP_FREE_INLINE
#include "engine/CDynamicObjectHeap.h"

extern CDynamicObjectHeap<72, 16> LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight;   // 0x01436FB0

class CEngineLightingManager {
public:
    struct CManagerLocalLight {
        static void __fastcall operator delete(void* object) { LOCAL_HEAP_CEngineLightingManager__ManagerLocalLight.Free(object); }
    };
};

template <class T> class CArray {                       // STLport vector<T> (12 bytes)
public:
    T* _M_start;                                        // +0x00
    T* _M_finish;                                       // +0x04
    T* _M_end_of_storage;                               // +0x08
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
    void clear()
    {
        T* i = (T*)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
    void DeleteAndClear()
    {
        for (unsigned int i = 0; i < size(); ++i)
            delete (*this)[i];
        clear();
    }
};

template class CArray<CEngineLightingManager::CManagerLocalLight*>;