// CPersistContext::TransferVectorOfSubComponents<CBlurredAttitudesDef>(std::vector<CBlurredAttitudesDef>& value) -- binary persist of a vector of def sub-components: the element count, then each element's own virtual Transfer
// Leaf: TransferVectorOfSubComponents
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067D6B0, 232 bytes, `ret 4`. Called from COpinionReactionManagerDef::Transfer (0x00679740) for AttitudeBlur (retail +0x3C, PDB +0x40).
// Callees: CPersistContext::CheckCRC 0x00404500 (with the pooled empty key 0x0122D70E), CDataOutputStream::WriteLong
//   0x00993EB0 (catalogued "WriteCBYTE"), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   vector<CBlurredAttitudesDef>::resize(n) 0x0067D610, and the element's virtual Transfer(CPersistContext&) (vtable slot 18, +0x48).
// Shape: the catalogued template argument "CEngineLocalDetailObjectDef" is a propagated label; the element size (0x48,
//   the reciprocal-multiply divide of size()) and the caller pin the real vector. A switch with only `case 2` and a `default` (an `if` gives `cmp [ebx+18h],2`; the switch gives retail's `mov eax ; sub eax,2 ; je`) -- any mode but binary load writes: count
//   (size()), then each element; binary load reads the count with the inlined by-value ReadLong (its buffer is the
//   `push ecx` frame word; the fast path keeps the value in edi), resizes and transfers each element. Element accesses
//   re-read the vector's start each pass (the virtual call may move it). CPersistContext offsets from Ego_r.pdb.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CPersistContext;
class CMemoryDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
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
class CDataOutputStream { public:
    void WriteLong(long value);            // 0x00993EB0
};

class CBlurredAttitudesDef {                              // 0x48 bytes
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Transfer(CPersistContext& context);   // slot 18
    unsigned char _body[0x48 - 4];
};

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long index) { return _M_start[index]; }
    void resize(long count);               // 0x0067D610 for this T
};

class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    Mode;                 // +0x18 (2 = binary load)
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;          // +0x24
    CDataOutputStream*      PSaveStream;          // +0x28
    void CheckCRC(const char* name);              // 0x00404500
    template <class T> void TransferVectorOfSubComponents(vector<T>& value);
};

template <class T>
void CPersistContext::TransferVectorOfSubComponents(vector<T>& value)
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

template void CPersistContext::TransferVectorOfSubComponents<CBlurredAttitudesDef>(vector<CBlurredAttitudesDef>& value);