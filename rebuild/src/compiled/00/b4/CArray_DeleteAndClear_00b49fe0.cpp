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

struct CObjectEntry { CObjectEntry* Next; };

class CDynamicObjectHeapBase {
public:
    virtual ~CDynamicObjectHeapBase();
    long                    AllocCount;                 // +0x04
    long                    TotalAllocatorBlocks;       // +0x08
    bool                    RefCountReleaseEnabled;     // +0x0c
    CDynamicObjectHeapBase* HeapNext;                   // +0x10
    void RefCountedRelease();                           // 0x00F36CA0
};

template <int SIZE, int COUNT> class CDynamicObjectHeap : public CDynamicObjectHeapBase {
public:
    CObjectEntry* FreeList;                             // +0x14
    void Free(void* object)
    {
        --AllocCount;
        ((CObjectEntry*)object)->Next = FreeList;
        FreeList = (CObjectEntry*)object;
        if (AllocCount == 0 && RefCountReleaseEnabled)
            RefCountedRelease();
    }
};

extern CDynamicObjectHeap<72, 16> g_LocalLightHeap;     // 0x01436FB0

class CEngineLightingManager {
public:
    struct CManagerLocalLight {
        static void __fastcall operator delete(void* object) { g_LocalLightHeap.Free(object); }
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