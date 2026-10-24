// CPersistContext::TransferVectorOfSubComponents<NSpeechGainManager::CDialogueLayerDef>(std::vector<NSpeechGainManager::CDialogueLayerDef>& value) -- binary persist of a vector of dialogue-layer sub-defs: the element count, then each element's own virtual Transfer
// Leaf: TransferVectorOfSubComponents
// Retail: Fable.exe (TLC, VC7.1) @ 0x00472A8D, 214 bytes, `ret 4` (a /Os module: unaligned entry, ebp frame, `leave`).
// Callees: CPersistContext::CheckCRC 0x00404500 (with the pooled empty key 0x0122D70E), CDataOutputStream::WriteLong
//   0x00993EB0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, vector<CDialogueLayerDef>::resize(n) 0x00472B63
//   (the next function), and the element's virtual Transfer(CPersistContext&) (vtable slot 18, +0x48).
// Shape: the model of the landed 0x007D2740 (CShoveHaPennyRegionDef) under the size pragmas: a switch with only
//   `case 2` (binary load: the inlined by-value ReadLong into the frame word, resize, transfer each element) and a
//   `default` (write size(), transfer each element).  The 0x40-byte element makes size() a `sar 6`; `this` is kept in
//   the frame (`[ebp-8]`) because ebx doubles as the element offset in the save loop.  CPersistContext offsets from Ego_r.pdb.
#pragma optimize("y", off)
#pragma optimize("s", on)
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

namespace NSpeechGainManager {
class CDialogueLayerDef {                                   // 0x40 bytes
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Transfer(CPersistContext& context);   // slot 18
    unsigned char _body[0x40 - 4];
};
}

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long index) { return _M_start[index]; }
    void resize(long count);               // 0x00472B63 for this T
};

struct CPersistContext {
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

template void CPersistContext::TransferVectorOfSubComponents<NSpeechGainManager::CDialogueLayerDef>(vector<NSpeechGainManager::CDialogueLayerDef>& value);