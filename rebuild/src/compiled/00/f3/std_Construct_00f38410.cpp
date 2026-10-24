// std::_Construct(CGPUScratchBufferTextureManager::CResourcePage*, const CGPUScratchBufferTextureManager::CResourcePage&) -- placement copy of a scratch-buffer page: pool, resource list (out-of-line CArray copy), next free address, free size, auto-restore flag
// Retail: Fable.exe @ 0x00F38410, 47 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CArray<long> copy ctor @ 0x00514160.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00f38410 {
struct CArrayLong {                                 // retail 12-byte STLport-vector body (long)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CArrayLong(const CArrayLong& other);                  // 0x00514160
};
struct CResourcePage {                           // CGPUScratchBufferTextureManager::CResourcePage (retail sizeof 0x1C; PDB 0x20 with a 16-byte CArray)
    long           ScratchPool;                  // +0x00
    CArrayLong     ResourceList;                 // +0x04 CArray<long>
    unsigned char* NextFreeBufferAddress;        // +0x10
    long           FreeSpaceSize;                // +0x14
    bool           EnableAutoRestore;            // +0x18
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CResourcePage* p, const CResourcePage& value)
{
    if (p != 0)
        new (p) CResourcePage(value);
}
}