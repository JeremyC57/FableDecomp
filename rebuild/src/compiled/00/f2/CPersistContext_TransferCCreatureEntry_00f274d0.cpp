// CPersistContext::Transfer<CCreatureEntry>(const char* name, vector<CCreatureEntry>& value) -- persist a vector of 0x34-byte sub-component records (the "Creatures" member of its owner): CRC-check the key, then by mode text load / binary load (count, resize, each element's Transfer) / binary save (count, each element) / per-element pass
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F274D0 (the 5-entry switch table at 0x00F27638 follows the code; entry 1
// is 0: there is no text-save arm), `ret 8`. Called from 0x00F274C1 with the key "Creatures" (0x012FB3FC) and the
// owner's vector at +0x2C.
// Callees: CPersistContext::CheckCRC 0x00404500, the sub-component text-load helper 0x009BC120 (takes the
// {vtable 0x012FB570, vector*} transferable-vector wrapper), CDataOutputStream::WriteLong 0x00993EB0,
// CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, the element's ctor 0x00F27650 / dtor 0x00F270D0 (vtable
// 0x012FB500, two CCharString members at +0x28/+0x2C; the script module has no RTTI, so the element class name is
// descriptive), vector<T>::resize(n, const T&) 0x00F27710, the element's virtual Transfer(CPersistContext&) (slot 18,
// +0x48). The manifest's "Transfer<signed_char>" label is propagated.
// Shape: the landed Transfer<CQuestActivityRecord> (0x00569F20) closed-enum switch (`default: __assume(0)`, cases in
// source order 4,3,2,0) with the landed TransferVectorOfSubComponents<CUIStateDef> (0x00632E00) arms: binary load reads
// the count inline, resizes with a default-constructed temporary and transfers each element; binary save writes the
// count first; mode 4 transfers each element only.
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

class CCreatureEntry {                                    // 0x34 bytes, vtable 0x012FB500
public:
    CCreatureEntry();                                     // 0x00F27650
    ~CCreatureEntry();                                    // 0x00F270D0
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Transfer(CPersistContext& context);      // slot 18
    unsigned char _body[0x34 - 4];
};

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long index) { return _M_start[index]; }
    void resize(long count, const T& x);                  // 0x00F27710 for this T
};

class CPersistContext_TransferableVectorGenericType { public:
    virtual long GetCount();
};
template <class T> struct CPersistContext_TransferableVector : CPersistContext_TransferableVectorGenericType {   // vtable 0x012FB570
    vector<T>* TransVector;
    CPersistContext_TransferableVector(vector<T>& v) : TransVector(&v) {}
    virtual long GetCount();
};

class CPersistContext { public:
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18 (0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset)
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    void CheckCRC(const char* name);              // 0x00404500
    void TransferSubComponentVectorLoadText(const char* name, CPersistContext_TransferableVectorGenericType& item);   // 0x009BC120
    template <class T> void Transfer(const char* name, vector<T>& value);
};

template <class T>
void CPersistContext::Transfer(const char* name, vector<T>& value)
{
    CheckCRC(name);
    switch (Mode) {
    case 4: {
        long count = value.size();
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    case 3: {
        long count = value.size();
        PSaveStream->WriteLong(count);
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    case 2: {
        long count = PLoadStream->ReadLong();
        value.resize(count, T());
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    case 0: {
        CPersistContext_TransferableVector<T> item(value);
        TransferSubComponentVectorLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::Transfer<CCreatureEntry>(const char* name, vector<CCreatureEntry>& value);