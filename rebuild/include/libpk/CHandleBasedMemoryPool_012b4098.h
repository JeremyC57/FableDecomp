// CHandleBasedMemoryPool_012b4098 -- libpk's own build of the engine's handle-based memory pool (vtable 0x012B4098; owner
//   decision 2026-10-31: the class keeps the engine name plus its vtable address, since libpk has no PDB or RTTI of its own).
// Identity: the Ego_r.pdb CHandleBasedMemoryPool name set (Defragment, InternalSetResource, InternalGetResource, MoveArea, ...)
//   matches this copy's function order and arities at 0x00C27E60..0x00C28C20; this copy has no critical section and no
//   shutdown / enable flags, so engine/CHandleBasedMemoryPool.h (the engine layout, sizeof 0x464) does not fit it.
// Layout: the landed Allocate / MoveArea / Free / Defragment model: Buffer +4, PoolSize +0xc, AllocatedSize +0x10, LockCount
//   +0x18, free lists +0x3c, the handle table at +0x43c, a VC7.1 Dinkumware vector<CHeader*> (allocator word +0x43c, _Myfirst
//   +0x440), FirstUnusedHandle +0x44c.
// Containers: libpk was built against the VC7.1 Dinkumware library, not the engine's STLport 4.6, so the handle vector is the
//   Dinkumware subset below: operator[] = `*(begin() + _Pos)` over the one-pointer _Ptrit iterator class, which VC7.1 keeps as
//   a dead `lea` after the slot load (InternalSetResource 0x00C28540, InternalGetResource 0x00C28560). Include this header in
//   a translation unit that uses no other std:: container header.
// CHeader is the pool's 32-byte area header (Ego_r: CHandleBasedMemoryPool::CHeader). The landed rows bind it at global scope
//   (`PAUCHeader@@`), as engine/CHandleBasedMemoryPool.h forward-declares it, so it stays there.
#ifndef FABLE_LIBPK_CHANDLEBASEDMEMORYPOOL_012B4098_H
#define FABLE_LIBPK_CHANDLEBASEDMEMORYPOOL_012B4098_H

struct CHeader {                                // CHandleBasedMemoryPool_012b4098::CHeader, 32 bytes in front of every area
    void*          Resource;                    // +0x00
    long           Handle;                      // +0x04
    long           Size;                        // +0x08
    short          Padding;                     // +0x0c
    unsigned char  Status;                      // +0x0e ('N' free, 'T' taken)
    unsigned char  Locked;                      // +0x0f
    CHeader**      RefAvailable;                // +0x10
    CHeader*       NextAvailable;               // +0x14
    CHeader*       PrevContiguous;              // +0x18
    CHeader*       NextContiguous;              // +0x1c
};

#ifdef FABLE_RETAIL_LIBPK_HANDLEINFO_DINKUMWARE
// Original VC7.1 standard header, supplied by the RTM compiler distribution.
#include <vector>
#else
namespace std {
template<class _Ty> class allocator { };

template<class _Ty> class _Ptrit {              // VC7.1 Dinkumware vector iterator: a class over one pointer
public:
    _Ptrit() { }
    explicit _Ptrit(_Ty* _Ptr) : current(_Ptr) { }
    _Ty& operator*() const { return (*current); }
    _Ptrit& operator+=(int _Off) { current += _Off; return (*this); }
    _Ptrit operator+(int _Off) const { _Ptrit _Tmp = *this; return (_Tmp += _Off); }
    _Ty* current;
};

template<class _Ty, class _Ax = allocator<_Ty> > class vector {
public:
    typedef _Ptrit<_Ty> iterator;
    iterator begin() { return (iterator(_Myfirst)); }
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    _Ty& operator[](unsigned int _Pos) { return (*(begin() + _Pos)); }
    _Ax  _Alval;                                // +0x00 (an empty allocator padded to a word)
    _Ty* _Myfirst;                              // +0x04
    _Ty* _Mylast;                               // +0x08
    _Ty* _Myend;                                // +0x0c
};
}

#endif // existing compact declaration branch

class CHandleBasedMemoryPool_012b4098 {
public:
#ifdef FABLE_RETAIL_LIBPK_HANDLEINFO_DINKUMWARE
    // Retail opt-in (Codex pass22): original both-PDB nested types plus native
    // pool allocation/free and C298D0's four-byte non-scalar element operations.
    // Existing owner-approved libpk identity retained; default branch untouched.
    class CHeader {
    public:
        CHeader();
        unsigned char* GetDataPointer();
        bool IsAllocated() const;
        long GetPaddedSize() const;
        void* Resource;
        long Handle;
        long Size;
        short Padding;
        unsigned char Status;
        unsigned char Locked;
        CHeader** RefAvailable;
        CHeader* NextAvailable;
        CHeader* PrevContiguous;
        CHeader* NextContiguous;
    };
    class CHandleInfo {
    public:
        CHandleInfo();
        union { CHeader* Header; long NextUnusedHandle; };
    };
#endif
    virtual void MoveResource(void* resource, long handle, void* from, void* to, unsigned long size);   // slot 0
    virtual int  CanMoveResource(void* resource, long handle);                                        // slot 1
    virtual void ValidateResource(void* resource);                                                    // slot 2
#ifdef FABLE_RETAIL_LIBPK_STREAM_BUFFER_PASS22
    void* GetPointerFromHandle(long handle); // Existing binding and native00C28380.
#endif
    void*                 Buffer;               // +0x04
    void*                 UnalignedBuffer;      // +0x08
    long                  PoolSize;             // +0x0c
    long                  AllocatedSize;        // +0x10
    long                  NumAllocations;       // +0x14
    long                  LockCount;            // +0x18
    long                  LargestFreeArea;      // +0x1c
    unsigned char         _pad20[0x34 - 0x20];
    void*                 m_defaultHandler;     // +0x34 embedded default handler (one vptr)
    void*                 FailureHandler;       // +0x38
    CHeader*              FreeLists[256];       // +0x3c
#ifdef FABLE_RETAIL_LIBPK_HANDLEINFO_DINKUMWARE
    std::vector<CHandleInfo> Handles;
#else
    std::vector<CHeader*> Handles;              // +0x43c
#endif
    long                  FirstUnusedHandle;    // +0x44c
protected:
    void  InternalSetResource(long handle, void* resource);   // 0x00C28540
    void* InternalGetResource(long handle);                   // 0x00C28560
};

#endif
