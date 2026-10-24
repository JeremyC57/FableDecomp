// CCountedPointer<CScopedMemoryHandle>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the scoped memory handle (return its handle to the pool) and free the object
// Leaf: ?DeleteData@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8D7C0, 46 bytes, `ret` (static `__fastcall`, data in ecx).
// Identity: Ego_r.pdb `?DeleteData@?$CCountedPointer@VCScopedMemoryHandle@@@@SIXPAX@Z` (public static __fastcall
//   void (void*)), BSim-matched (bsim_port_audit_final.tsv); referenced (absolute) at 0x00A8DA40 by the landed
//   CCountedPointer Reset 0x00A8D9F0, which stores it in the count block.
// Callees: CHandleBasedMemoryPool::Free(long) @ 0x00A02B30, operator delete @ 0x00BFE9BC.
// Shape: `delete (CScopedMemoryHandle*)data` -- the null test, the inline destructor (= Clear(): `if (Pool) { Pool->Free(
//   Handle); Pool = 0; Handle = -1; }`, its dead stores kept because the object escapes to operator delete), then delete.
class CHandleBasedMemoryPool {
public:
    void Free(long handle);                        // 0x00A02B30
};

class CScopedMemoryHandle {
public:
    CHandleBasedMemoryPool* Pool;                  // +0x00
    long                    Handle;                // +0x04
    void Clear()
    {
        if (Pool) {
            Pool->Free(Handle);
            Pool = 0;
            Handle = -1;
        }
    }
    ~CScopedMemoryHandle() { Clear(); }
};

template <class T>
class CCountedPointer {
public:
    T* Object;
    void* CountBlock;
    static void __fastcall DeleteData(void* data);
};

template <class T>
void __fastcall CCountedPointer<T>::DeleteData(void* data)
{
    delete (T*)data;
}

template void __fastcall CCountedPointer<CScopedMemoryHandle>::DeleteData(void* data);