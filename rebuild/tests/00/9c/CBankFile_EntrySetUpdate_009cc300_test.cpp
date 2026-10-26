#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CRuntimeData { unsigned long DataOffset; unsigned long DataSize; unsigned char DataType; bool Valid; unsigned char _pad[2]; };
struct CPackedUIntArray {
    unsigned long m_bitsPerValue; unsigned long* m_pData; unsigned long m_count; unsigned long m_capacity;
    unsigned long operator[](unsigned long index) const { return m_pData[index]; }
    bool IsPacked() const { return m_pData != 0; }
};
template <class T> class CArray { public:
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    long GetCount() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};
class CBankFileEntryUpdateData { public: unsigned char _pad[0x14]; bool m_update; };
struct CDiskFileCalls {
    unsigned long m_position; unsigned long m_readSize; void* m_readBuffer; int m_closed; int m_opened; unsigned long m_mode, m_share;
    virtual void Open(const void*, unsigned long mode, unsigned long share) { ++m_opened; m_mode = mode; m_share = share; }
    virtual void Close() { ++m_closed; }
    virtual void Read(void* buffer, unsigned long size, unsigned long) { m_readBuffer = buffer; m_readSize = size; }
    virtual void Seek(unsigned long position) { m_position = position; }
    virtual unsigned long GetPosition() { return m_position; }
};
class CBankFile { public:
    unsigned long Size;
    CArray<CRuntimeData> RuntimeData;
    CArray<CBankFileEntryUpdateData*> UpdateData;
    CPackedUIntArray PackedDataOffset, PackedDataSize, PackedDataType, PackedValid;
    CDiskFileCalls* BankFile_Data;
    void* Filename_PStringData;
    bool WrittenToFile;
    bool EntryExists(unsigned long index) const
    {
        if (index != 0 && index < Size) {
            if (PackedValid.IsPacked()) return PackedValid[index - 1] != 0;
            return RuntimeData[index].Valid;
        }
        return false;
    }
    unsigned long GetEntryDataType(unsigned long index) const { if (PackedDataType.IsPacked()) return PackedDataType[index - 1]; return RuntimeData[index].DataType; }
    unsigned long GetEntryDataSize(unsigned long index) const { if (PackedDataSize.IsPacked()) return PackedDataSize[index - 1]; return RuntimeData[index].DataSize; }
    unsigned long GetEntryDataOffset(unsigned long index) const { if (PackedDataOffset.IsPacked()) return PackedDataOffset[index - 1]; return RuntimeData[index].DataOffset; }
    void ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size)
    {
        unsigned long dataSize = GetEntryDataSize(index); (void)dataSize;
        unsigned long dataOffset = GetEntryDataOffset(index);
        BankFile_Data->Seek(dataOffset + offset);
        BankFile_Data->Read(buffer, size, 0);
    }
    bool EntrySetUpdate(unsigned long index, bool update)
    {
        if (index < (unsigned long)RuntimeData.GetCount() && RuntimeData[index].Valid) {
            CBankFileEntryUpdateData* data = UpdateData[index];
            bool old = data->m_update; data->m_update = update; return old;
        }
        return false;
    }
    long GetMaxEntryIndex() const { long i; for (i = Size - 1; i >= 0; --i) { if (EntryExists(i)) break; } return i; }
    unsigned long CountEntries() const { unsigned long c = 0; for (unsigned long i = 0; i < Size; ++i) if (EntryExists(i)) ++c; return c; }
    void MakeWritable()
    {
        if (!WrittenToFile) {
            unsigned long position = BankFile_Data->GetPosition();
            BankFile_Data->Close();
            BankFile_Data->Open(&Filename_PStringData, 2, 4);
            BankFile_Data->Seek(position);
            WrittenToFile = true;
        }
    }
};
static CRuntimeData g_rt[4] = { {0,0,0,false,{0,0}}, {100,10,1,true,{0,0}}, {200,20,2,false,{0,0}}, {300,30,3,true,{0,0}} };
static unsigned long g_pvalid[3] = {1, 0, 0};     // packed validity for entries 1..3
static unsigned long g_ptype[3] = {7, 8, 9};
static unsigned long g_psize[3] = {11, 12, 13};
static unsigned long g_poff[3] = {1000, 2000, 3000};
static CBankFile MakeBank(bool packed)
{
    CBankFile b; std::memset(&b, 0, sizeof b);
    b.Size = 4; b.RuntimeData.m_pFirst = g_rt; b.RuntimeData.m_pLast = g_rt + 4; b.RuntimeData.m_pEnd = g_rt + 4;
    if (packed) { b.PackedValid.m_pData = g_pvalid; b.PackedDataType.m_pData = g_ptype; b.PackedDataSize.m_pData = g_psize; b.PackedDataOffset.m_pData = g_poff; }
    return b;
}

static bool Check()
{
    CBankFileEntryUpdateData u[4]; std::memset(u, 0, sizeof u); CBankFileEntryUpdateData* ups[4] = {&u[0], &u[1], &u[2], &u[3]};
    CBankFile b = MakeBank(false); b.UpdateData.m_pFirst = ups; b.UpdateData.m_pLast = ups + 4;
    if (b.EntrySetUpdate(4, true)) return false;                 // out of range
    if (b.EntrySetUpdate(2, true) || u[2].m_update) return false; // invalid record untouched
    if (b.EntrySetUpdate(1, true) || !u[1].m_update) return false;
    if (!b.EntrySetUpdate(1, false) || u[1].m_update) return false;  // old value returned
    return true;
}
int main()
{
    if (!Check()) { std::printf("009cc300_TEST FAIL\n"); return 1; }
    std::printf("009cc300_TEST PASS\n");
    return 0;
}