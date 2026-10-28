// CPersistContext::TransferVectorOfSubComponents<CUIStateDef>(vector<CUIStateDef>& value) -- binary persist of a vector of NUISystem::CUIStateDef sub-components: the element count, then each element's own virtual Transfer; a binary load resizes with a default-constructed element first
// Leaf: TransferVectorOfSubComponents
// Retail: Fable.exe (TLC, VC7.1) @ 0x00632E00, 263 bytes, `ret 4`. Called from 0x00631CFC.
// Label correction: catalogued as TransferVectorOfSubComponents<CEngineLocalDetailObjectDef> (propagated); the element is
//   0x7C bytes (reciprocal multiply 0x84210843 / `sar 6` = divide by 124, stride 0x7C) and the resize temporary is built by
//   0x00630350, which stores vtable 0x0125871C = NUISystem::CUIStateDef (RTTI): this is the CUIStateDef instantiation.
// Callees: CPersistContext::CheckCRC 0x00404500 (pooled empty key 0x0122D70E), CDataOutputStream::WriteLong 0x00993EB0,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, NUISystem::CUIStateDef::CUIStateDef() 0x00630350 (catalogued
//   CUIStateDef), its destructor 0x00624DC0 (catalogued "~CCompressedMemoryDataInputStream"), vector<CUIStateDef>::resize(n,
//   const T&) 0x00632F10, the element's virtual Transfer(CPersistContext&) (slot 18, +0x48).
// Shape: the landed 0x0067D6B0 template with the STLport `resize(n, T())` of the binary-load arm: the temporary is built by
//   the out-of-line constructor (its returned `this` is the pushed reference) in the 0x7C bytes above the ReadLong word and
//   destroyed after the call. A switch with only `case 2` and a `default` gives `sub eax,2 ; je`.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CPersistContext;
class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
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

class CUIStateDef {                                       // NUISystem::CUIStateDef, 0x7C bytes, vtable 0x0125871C
public:
    CUIStateDef();                                        // 0x00630350
    ~CUIStateDef();                                       // 0x00624DC0
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Transfer(CPersistContext& context);   // slot 18
    unsigned char _body[0x7C - 4];
};

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long index) { return _M_start[index]; }
    void resize(long count, const T& x);   // 0x00632F10 for this T
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
        value.resize(count, T());
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

template void CPersistContext::TransferVectorOfSubComponents<CUIStateDef>(vector<CUIStateDef>& value);