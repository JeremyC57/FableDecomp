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

#include <stdio.h>
#include <string.h>
static CBankFile* activeBank;
static CRuntimeData* records;
static int initialises, usages, clears, runtimeClears, invalid;
static long sizes[4];
static unsigned long seen[4][5];
static bool growCapacity;
static int indexOf(const CPackedUIntArray* p) {
    return p==&activeBank->PackedDataOffset?0:p==&activeBank->PackedDataSize?1:
           p==&activeBank->PackedDataType?2:p==&activeBank->PackedValid?3:-1;
}
void CPackedUIntArray::Initialise(const void* data, unsigned long bits, long stride, unsigned long count) {
    int index=indexOf(this);
    if (index!=initialises || initialises>=4 || stride!=(long)sizeof(CRuntimeData) || count!=4 || bits!=(index<2?32:8)) { ++invalid; return; }
    const void* expected=index==0?(void*)&records[1].DataOffset:index==1?(void*)&records[1].DataSize:
        index==2?(void*)&records[1].DataType:(void*)&records[1].Valid;
    if (data!=expected) ++invalid;
    for (unsigned long i=0;i<count;++i) {
        const unsigned char* ptr=(const unsigned char*)data+i*stride;
        if (index<2) seen[index][i]=*(const unsigned long*)ptr;
        else seen[index][i]=*ptr;
    }
    ++initialises;
    if (growCapacity && initialises==4) activeBank->RuntimeData.Limit=records+10;
}
long CPackedUIntArray::GetMemoryUsage() const {
    int index=indexOf(this);
    if (index<0 || initialises!=4 || clears || runtimeClears) { ++invalid; return 0; }
    ++usages; return sizes[index];
}
void CPackedUIntArray::Clear() {
    if (indexOf(this)!=clears || usages!=4 || runtimeClears) ++invalid;
    ++clears;
}
template<class T> void CArray<T>::ClearAndFree() {
    if (this!=&activeBank->RuntimeData || usages!=4 || clears) ++invalid;
    ++runtimeClears; First=Last=Limit=0;
}
static void reset(CBankFile& b, CRuntimeData* values) {
    activeBank=&b; records=values;
    initialises=usages=clears=runtimeClears=invalid=0; growCapacity=false;
    b.RuntimeData.First=values; b.RuntimeData.Last=values+5; b.RuntimeData.Limit=values+7;
    sizes[0]=sizes[1]=sizes[2]=sizes[3]=16;
    for(int i=0;i<4;++i) for(int j=0;j<5;++j) seen[i][j]=0;
}
int main() {
    CBankFile bank; CRuntimeData values[12];
    for (int i=0;i<12;++i) { values[i].DataOffset=1000+i*50; values[i].DataSize=20+i;
        values[i].DataType=(unsigned char)(7+i); values[i].Valid=(i%2)!=0; }
    int ok=1;
    for (int n=0;n<2;++n) {
        reset(bank,values); bank.RuntimeData.Last=values+n;
        bank.PackRuntimeData(); ok=ok && initialises==0 && usages==0 && clears==0 && runtimeClears==0 && invalid==0;
    }
    reset(bank,values); bank.PackRuntimeData();
    ok=ok && initialises==4 && usages==4 && clears==0 && runtimeClears==1 && invalid==0 && bank.RuntimeData.First==0;
    for(int j=0;j<4;++j) ok=ok && seen[0][j]==values[j+1].DataOffset && seen[1][j]==values[j+1].DataSize
        && seen[2][j]==values[j+1].DataType && seen[3][j]==(unsigned long)values[j+1].Valid;
    // Equality is deliberately rejected; compare uses capacity, not the five live entries.
    for (int delta=-1;delta<=1;++delta) {
        reset(bank,values); sizes[0]=(8*(long)sizeof(CRuntimeData))-48+delta;
        bank.PackRuntimeData();
        ok=ok && invalid==0 && initialises==4 && usages==4;
        if (delta<0) ok=ok && runtimeClears==1 && clears==0;
        else ok=ok && runtimeClears==0 && clears==4 && bank.RuntimeData.First==values && bank.RuntimeData.Last==values+5;
    }
    // Capacity must be read after all four packing calls, since they may change the bank.
    reset(bank,values); growCapacity=true; sizes[0]=(9*(long)sizeof(CRuntimeData))-48;
    bank.PackRuntimeData(); ok=ok && invalid==0 && runtimeClears==1 && clears==0;
    printf("009cd740_TEST %s\n",ok?"PASS":"FAIL"); return ok?0:1;
}