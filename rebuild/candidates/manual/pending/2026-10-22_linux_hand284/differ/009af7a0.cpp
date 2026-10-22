// CDefinitionManager::SaveBinaryDefinitions(CDataOutputStream&) -- save the def string table, write the format flag, dependency CRC, string-table CRC and def count, then for every instantiated def write its indices and name CRC and pack its header plus persisted body into 32 KB compressed chunks (offset table per chunk), close the last chunk, append the end marker to the sorted offset map, trim and write both arrays, and release every def
// Grade: DIFFER(1007v1007) -- same length and call order; register allocation differs (retail keeps this in ebx, the stream in ebp and edi = 0 across the clears; VC swaps ebx/ebp and caches &CompressedOffsets in edi) with the frame-slot knock-on. Tried: inline STLport-style erase/copy for both arrays, same-TU InsertGrow stand-ins, the 0x2c-byte persist context, a separate string-table CRC global
// Leaf: SaveBinaryDefinitions
#include <new>
#include <string.h>
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14
void* GrowArrayStorage(unsigned long bytes);
struct SScalarTag { SScalarTag() { } };
struct CKeyPairCompareLess { unsigned char _empty; };
struct SCRCPair {
    unsigned long Key;
    unsigned long Value;
    SCRCPair(unsigned long key, unsigned long value) : Key(key), Value(value) {}
};
void __fastcall SortCRCPairs(SCRCPair* first, SCRCPair* last, CKeyPairCompareLess compare);   // 0x009B85A0
struct CUShortArray {                                  // CArray<unsigned short>
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    CUShortArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CUShortArray() { if (_M_start) free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    void InsertGrow(unsigned short* where, const unsigned short& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x005135D0
    void push_back(const unsigned short& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) unsigned short(value);
            ++_M_finish;
        }
        else
            InsertGrow(_M_finish, value, SScalarTag(), 1, true);
    }
};
#pragma auto_inline(off)
void CUShortArray::InsertGrow(unsigned short* where, const unsigned short& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    unsigned long oldSize = size();
    unsigned long newCapacity = oldSize + (oldSize > count ? oldSize : count);
    unsigned short* p = (unsigned short*)GrowArrayStorage(newCapacity * sizeof(unsigned short));
    unsigned short* out = p;
    for (unsigned short* it = _M_start; it != where; ++it)
        *out++ = *it;
    for (unsigned long i = 0; i < count; ++i)
        *out++ = value;
    if (_M_start)
        free(_M_start);
    _M_start = p;
    _M_finish = out;
    _M_end_of_storage = p + newCapacity;
}
#pragma auto_inline(on)
struct CByteArray {                                    // CArray<unsigned char>
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    static unsigned char* copy(unsigned char* first, unsigned char* last, unsigned char* result)
    {
        if (last == first)
            return result;
        long n = last - first;
        memmove(result, first, n);
        return result + n;
    }
    void erase(unsigned char* first, unsigned char* last) { _M_finish = copy(last, _M_finish, first); }
    void clear() { erase(_M_start, _M_finish); }
    const void* GetData() const { return size() > 0 ? (const void*)_M_start : (const void*)this; }
    void DeallocateUnusedMemory();                     // 0x009B5DB0
};
struct CCRCVectorMap {                                 // CVectorMap<unsigned long, unsigned long, CKeyPairCompareLess>
    SCRCPair*           _M_start;                      // +0x00
    SCRCPair*           _M_finish;                     // +0x04
    SCRCPair*           _M_end_of_storage;             // +0x08
    CKeyPairCompareLess Compare;                       // +0x0C
    bool                NeedsSort;                     // +0x0D
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    static SCRCPair* copy(SCRCPair* first, SCRCPair* last, SCRCPair* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    void erase(SCRCPair* first, SCRCPair* last) { _M_finish = copy(last, _M_finish, first); }
    void clear() { erase(_M_start, _M_finish); }
    void InsertGrow(SCRCPair* where, const SCRCPair& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x009B6300
    void push_back(const SCRCPair& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SCRCPair(value);
            ++_M_finish;
        }
        else
            InsertGrow(_M_finish, value, SScalarTag(), 1, true);
    }
    void Sort()
    {
        if (NeedsSort) {
            SortCRCPairs(_M_start, _M_finish, Compare);
            NeedsSort = false;
        }
    }
    const void* GetData() const { return size() > 0 ? (const void*)_M_start : (const void*)this; }
    void DeallocateUnusedMemory();                     // 0x009B7B10
};
#pragma auto_inline(off)
void CCRCVectorMap::InsertGrow(SCRCPair* where, const SCRCPair& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    unsigned long oldSize = size();
    unsigned long newCapacity = oldSize + (oldSize > count ? oldSize : count);
    SCRCPair* p = (SCRCPair*)GrowArrayStorage(newCapacity * sizeof(SCRCPair));
    SCRCPair* out = p;
    for (SCRCPair* it = _M_start; it != where; ++it)
        *out++ = *it;
    for (unsigned long i = 0; i < count; ++i)
        *out++ = value;
    if (_M_start)
        free(_M_start);
    _M_start = p;
    _M_finish = out;
    _M_end_of_storage = p + newCapacity;
}
#pragma auto_inline(on)

class CDataOutputStream {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long length);  // slot 4
    void WriteBool(bool value);                         // 0x00993E30
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteLong(long value);                         // 0x00993EB0
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    unsigned char _body[0x14];
    CMemoryDataOutputStream();                          // 0x00996AF0
    ~CMemoryDataOutputStream();                         // 0x00996B30
    long GetSize() const;                               // 0x00994B40
    const void* PeekData() const;                       // 0x00995E70
    void Write(const void* data, long length);          // 0x00996CC0
};
class CPersistContext {
public:
    unsigned char _body[0x2c];
    CPersistContext(CDataOutputStream& stream, bool safe);   // 0x009BAA50
};
class CDefClassBase {
public:
    virtual void v00();
    virtual void DeleteThis();                          // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Persist(CPersistContext& context);     // slot 18
    virtual void v19(); virtual void v20();
    virtual void SaveHeader(CDataOutputStream& stream); // slot 21
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDefPointer {
    CDefClassBase* Data;
    CDefClassBase* operator->() const { return Data; }
    __forceinline CDefPointer& operator=(int)
    {
        if (Data) {
            Data->Release();
            Data = 0;
        }
        Data = 0;
        return *this;
    }
};
struct CInstantiatedDefInfo {
    long          GlobalIndex;                          // +0x00
    long          ClassIndex;                           // +0x04
    unsigned long NameCRC;                              // +0x08
    CDefPointer   Def;                                  // +0x0c
};
struct CInstantiatedDefArray {
    CInstantiatedDefInfo** _M_start;
    CInstantiatedDefInfo** _M_finish;
    CInstantiatedDefInfo** _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CInstantiatedDefInfo* operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CDefStringTable {
    unsigned char _pad[0x2c];
    void Save(long flags);                              // 0x009D4CE0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828
extern unsigned long g_DefStringTableCRC;              // 0x013CA854 (the table's CRC member)
struct CMemoryAllocator {
    unsigned long GetHeaderOverhead();                  // 0x009F2450
    unsigned long GetWastedSpace();                     // 0x009F2550
};
extern CMemoryAllocator* g_pMemoryAllocator;           // 0x013CAA90

class CDefinitionManager {
public:
    unsigned char         _pad000[0x88];
    CByteArray            CompressedDefData;            // +0x88
    CCRCVectorMap         CompressedOffsets;            // +0x94
    CInstantiatedDefArray InstantiatedDefs;             // +0xa4
    void*                 ErrorCallback;                // +0xb0
    void*                 CurrentFilename;              // +0xb4
    void*                 CompiledFileName;             // +0xb8
    unsigned long         DependencyCheckCRC;           // +0xbc
    bool                  UseSafeBinaryFormat;          // +0xc0
    void CompressAndAddChunk(CMemoryDataOutputStream& chunk, CUShortArray& offsets, long firstDef);   // 0x009AE390
    void SaveBinaryDefinitions(CDataOutputStream& stream);
};

void CDefinitionManager::SaveBinaryDefinitions(CDataOutputStream& stream)
{
    g_DefStringTable.Save(0);
    stream.WriteBool(UseSafeBinaryFormat);
    stream.WriteULong(DependencyCheckCRC);
    stream.WriteULong(g_DefStringTableCRC);
    unsigned long count = InstantiatedDefs.size();
    stream.WriteULong(count);
    CMemoryDataOutputStream chunk;
    CUShortArray offsets;
    CompressedDefData.clear();
    CompressedOffsets.clear();
    long firstDef = 0;
    unsigned long i;
    for (i = 0; i < count; ++i) {
        CInstantiatedDefInfo* info = InstantiatedDefs[i];
        stream.WriteLong(info->GlobalIndex);
        stream.WriteLong(info->ClassIndex);
        stream.WriteULong(info->NameCRC);
        CMemoryDataOutputStream defData;
        info->Def->SaveHeader(defData);
        CMemoryDataOutputStream body;
        CPersistContext context(body, UseSafeBinaryFormat);
        info->Def->Persist(context);
        defData.Write(body.PeekData(), body.GetSize());
        unsigned long pending = chunk.GetSize() + offsets.size() * sizeof(unsigned short);
        if (pending > 0 && defData.GetSize() + pending + 2 > 0x8000) {
            CompressAndAddChunk(chunk, offsets, firstDef);
            firstDef = i;
        }
        offsets.push_back((unsigned short)chunk.GetSize());
        chunk.Write(defData.PeekData(), defData.GetSize());
    }
    CompressAndAddChunk(chunk, offsets, firstDef);
    CompressedOffsets.push_back(SCRCPair(count, CompressedDefData.size()));
    CompressedOffsets.NeedsSort = true;
    CompressedOffsets.Sort();
    CompressedDefData.DeallocateUnusedMemory();
    CompressedOffsets.DeallocateUnusedMemory();
    stream.WriteULong(CompressedOffsets.size());
    stream.Write(CompressedOffsets.GetData(), CompressedOffsets.size() * sizeof(SCRCPair));
    stream.WriteLong(CompressedDefData.size());
    stream.Write(CompressedDefData.GetData(), CompressedDefData.size());
    if (g_pMemoryAllocator) {
        g_pMemoryAllocator->GetHeaderOverhead();
        g_pMemoryAllocator->GetWastedSpace();
    }
    for (i = 0; i < count; ++i)
        InstantiatedDefs[i]->Def = 0;
    if (g_pMemoryAllocator) {
        g_pMemoryAllocator->GetHeaderOverhead();
        g_pMemoryAllocator->GetWastedSpace();
    }
}
