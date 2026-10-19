// CBankFile::PackRuntimeData() -- retain packed entry arrays only when they consume less memory than runtime records.
// Retail: 0x009CD740, 270 bytes. Leaf: PackRuntimeData
// Shared CBankFile.h uses donor 16-byte CArrays; retail uses 12-byte arrays.
// Thus RuntimeData is +20, UpdateData +2c and packed arrays +38/+48/+58/+68,
// rather than header +28/+38/+48/+58/+68/+78. Local typed model is required.
// PDB CRuntimeData: DataOffset +0, DataSize +4, DataType +8, Valid +9, stride 12.
// Calls: CPackedUIntArray::Initialise 00A62D80, GetMemoryUsage 00A62A60,
// Clear 00A629D0, CArray<CRuntimeData>::ClearAndFree 009D3D90.
struct CRuntimeData {
    unsigned long DataOffset, DataSize;
    unsigned char DataType;
    bool Valid;
};
template<class T> struct CArray {
    T* First; T* Last; T* Limit;
    unsigned long size() const { return Last-First; }
    unsigned long capacity() const { return Limit-First; }
    void ClearAndFree();
};
struct CPackedUIntArray {
    unsigned long* PackedInts;
    unsigned long Size;
    unsigned char Bits;
    unsigned long Bias;
    void Initialise(const void*, unsigned long bits, long stride, unsigned long count); // 00A62D80
    long GetMemoryUsage() const; // 00A62A60
    void Clear(); // 00A629D0
};
struct CBankFile {
    void* VTable;
    unsigned long Size;
    CArray<void*> Symbols;
    CArray<unsigned long> Checksums;
    CArray<CRuntimeData> RuntimeData;
    CArray<void*> UpdateData;
    CPackedUIntArray PackedDataOffset, PackedDataSize, PackedDataType, PackedValid;
    void PackRuntimeData();
};
void CBankFile::PackRuntimeData()
{
    unsigned long count=RuntimeData.size();
    if (count>1) {
        CRuntimeData* records=RuntimeData.First+1;
        --count;
        PackedDataOffset.Initialise(&records->DataOffset,32,sizeof(CRuntimeData),count);
        PackedDataSize.Initialise(&records->DataSize,32,sizeof(CRuntimeData),count);
        PackedDataType.Initialise(&records->DataType,8,sizeof(CRuntimeData),count);
        PackedValid.Initialise(&records->Valid,8,sizeof(CRuntimeData),count);
        long unpackedBytes=(RuntimeData.capacity()+1)*sizeof(CRuntimeData);
        long packedBytes=PackedDataOffset.GetMemoryUsage()+PackedDataSize.GetMemoryUsage()+PackedDataType.GetMemoryUsage()+PackedValid.GetMemoryUsage();
        if (packedBytes<unpackedBytes) RuntimeData.ClearAndFree();
        else {
            PackedDataOffset.Clear(); PackedDataSize.Clear(); PackedDataType.Clear(); PackedValid.Clear();
        }
    }
}