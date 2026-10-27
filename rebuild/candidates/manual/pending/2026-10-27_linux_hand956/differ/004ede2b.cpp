// CPersistContext::TransferVectorOfSubComponents<CMultiStaticMeshEntryDef>(vector<CMultiStaticMeshEntryDef>& value) -- binary persist of a vector of CMultiStaticMeshEntryDef sub-components: the element count, then each element's own virtual Transfer; a binary load resizes first
// Grade: DIFFER(222v223) -- the inline ReadLong's 64-bit position check: retail keeps the low word in edx (lea edx,[eax+4]; xor eax,eax) and stores it as the new position, VC7.1 here allocates low=eax/high=edx (add eax,4; xor edx,edx) -- one byte longer jump layout; tried switch/if, direct Read, cast forms, signed size/index, pragma y/s on/off, QFE
// Leaf: ??$TransferVectorOfSubComponents@VCMultiStaticMeshEntryDef@@@CPersistContext@@QAEXAAV?$vector@VCMultiStaticMeshEntryDef@@V?$allocator@VCMultiStaticMeshEntryDef@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EDE2B, 223 bytes, thiscall, `ret 4` (frame-pointer size module).
// Identity (proven): CMultiStaticMeshDef::Transfer 0x004EDE1B passes Meshes (Ego_r.pdb vector<CMultiStaticMeshEntryDef>); the resize callee is the landed vector<CMultiStaticMeshEntryDef>::resize(unsigned int) 0x004EDF0A (0x38-byte stride).
// Callees: CPersistContext::CheckCRC 0x00404500 (pooled empty key 0x0122D70E), CDataOutputStream::WriteLong 0x00993EB0,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, vector<CMultiStaticMeshEntryDef>::resize(unsigned int) 0x004EDF0A, the element's
//   virtual Transfer(CPersistContext&) (slot 18, +0x48).
// Shape (family M102, representative 0x004ECF6E): the landed TransferVectorOfSubComponents template (0x00632E00) with STLport's
//   one-argument `resize(count)` in the load arm; `optimize("y", off)` + `("s", on)` (ebp frame, `push N ; pop ecx ; idiv`
//   size(), the count kept in the dead argument slot).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
class CPersistContext;
class CMemoryDataInputStream {
public:
    void* m_pVtable;                                      // +0x0
    unsigned long m_position;                             // +0x4
    long StreamSize;                                      // +0x8
    const unsigned char* m_pData;                         // +0xC current chunk read pointer
    long SourceChunkStreamPos;                            // +0x10
    long m_remaining;                                     // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, sizeof(value));
        return value;
    }
};
class CDataOutputStream {
public:
    void WriteLong(long value);                           // 0x00993EB0
};
class CMultiStaticMeshEntryDef {                                              // 0x38 bytes; Transfer is vtable slot 18
public:
    virtual void VirtualSlot00(); virtual void VirtualSlot01(); virtual void VirtualSlot02(); virtual void VirtualSlot03();
    virtual void VirtualSlot04(); virtual void VirtualSlot05(); virtual void VirtualSlot06(); virtual void VirtualSlot07();
    virtual void VirtualSlot08(); virtual void VirtualSlot09(); virtual void VirtualSlot10(); virtual void VirtualSlot11();
    virtual void VirtualSlot12(); virtual void VirtualSlot13(); virtual void VirtualSlot14(); virtual void VirtualSlot15();
    virtual void VirtualSlot16(); virtual void VirtualSlot17();
    virtual void Transfer(CPersistContext& context);      // slot 18
    unsigned char _body[0x38 - 4];
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
    void resize(unsigned int __new_size);                 // 0x004EDF0A
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CPersistContext {
public:
    template <class T> void TransferVectorOfSubComponents(std::vector<T>& value);
    void CheckCRC(const char* name);                      // 0x00404500
    unsigned char _pad00[0x18];
    long Mode;                                            // +0x18 (2 = binary load)
    unsigned char _pad1c[8];
    CMemoryDataInputStream* PLoadStream;                  // +0x24
    CDataOutputStream* PSaveStream;                       // +0x28
};
template <class T>
void CPersistContext::TransferVectorOfSubComponents(std::vector<T>& value)
{
    CheckCRC("");
    switch (Mode) {
    case 2: {
        long count = PLoadStream->ReadLong();
        value.resize(count);
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    default: {
        long count = value.size();
        PSaveStream->WriteLong(count);
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    }
}
template void CPersistContext::TransferVectorOfSubComponents<CMultiStaticMeshEntryDef>(std::vector<CMultiStaticMeshEntryDef>& value);
