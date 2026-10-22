// CPersistContext::TransferVectorOfSubComponents<CHeroSoulsRoundDef>(const char* name, vector<CHeroSoulsRoundDef>& value) -- persist a vector of 0x38-byte CHeroSoulsRoundDef sub-component defs: CRC-check the key, then by mode text load (through the transferable-vector wrapper) / binary load (count, resize, each element's Transfer) / binary save (count, each element) / per-element pass
// Leaf: ??$TransferVectorOfSubComponents@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F351F0, 364 bytes incl. the 5-entry switch table that follows the code
// (entry 1 is 0: no text-save arm), `ret 8`. PDB: ??$TransferVectorOfSubComponents@VCHeroSoulsRoundDef@@@CPersistContext@@QAEXPBDAAV?$vector@...
// Called once, from 0x00F332F4, with the key "HeroSoulsRounds" (0x012FB9E8) and the owner's vector at +0x1050. The manifest label
// `CPersistContext::Transfer<signed_char>` is propagated.
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistContext::TransferVectorOfSubComponentsLoadText 0x009BC120 (takes the
// {vtable 0x01301C7C, vector*} CPersistContext_TransferableVectorOfSubComponents<CHeroSoulsRoundDef> wrapper), CDataOutputStream::WriteLong
// 0x00993EB0, CMemoryDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, vector<CHeroSoulsRoundDef>::resize(n) 0x00F353F0 (one argument,
// out of line -- no default-constructed temporary in this TU), the element's virtual Transfer(CPersistContext&) (slot 18, +0x48).
// Element size 0x38 (0x92492493 / add / sar 5 in the count division); Ego_r.pdb gives 60: its vector<CHeroSoulsWaveDef> Waves is 12 bytes in retail.
// Shape: the landed CPersistContext::Transfer<CCreatureEntry> (0x00F274D0) closed-enum switch (`default: __assume(0)`, cases in
// source order 4,3,2,0) with the inline ReadLong; only the binary-load resize differs (the one-argument resize).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CPersistContext;
struct CMemoryDataInputStream {
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
struct CDataOutputStream {
    void WriteLong(long value);            // 0x00993EB0
};

class CHeroSoulsRoundDef {                                    // 0x38 bytes, CDefClassBase-derived sub-component def
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Transfer(CPersistContext& context);      // slot 18
    unsigned char _body[0x38 - 4];
};

template <class T> struct vector {                       // STLport, 12 bytes in retail
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long index) { return _M_start[index]; }
    void resize(long count);                              // 0x00F353F0 for this T
};

struct CPersistContext_TransferableVectorOfSubComponentsGenericType {
    virtual long Size();
};
template <class T> struct CPersistContext_TransferableVectorOfSubComponents : CPersistContext_TransferableVectorOfSubComponentsGenericType {   // vtable 0x01301C7C
    vector<T>* TransVector;
    CPersistContext_TransferableVectorOfSubComponents(vector<T>& v) : TransVector(&v) {}
    virtual long Size();
};

struct CPersistContext {
    unsigned char           _pad00[0x14];
    bool                    UseDefaults;          // +0x14
    unsigned char           _pad15[3];
    long                    Mode;                 // +0x18 (0 text load, 1 text save, 2 binary load, 3 binary save, 4 reset)
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;           // +0x24
    CDataOutputStream*      PSaveStream;           // +0x28
    void CheckCRC(const char* name);              // 0x00404500
    void TransferVectorOfSubComponentsLoadText(const char* name, CPersistContext_TransferableVectorOfSubComponentsGenericType& item);   // 0x009BC120
    template <class T> void TransferVectorOfSubComponents(const char* name, vector<T>& value);
};

template <class T>
void CPersistContext::TransferVectorOfSubComponents(const char* name, vector<T>& value)
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
        value.resize(count);
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    case 0: {
        CPersistContext_TransferableVectorOfSubComponents<T> item(value);
        TransferVectorOfSubComponentsLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

template void CPersistContext::TransferVectorOfSubComponents<CHeroSoulsRoundDef>(const char* name, vector<CHeroSoulsRoundDef>& value);