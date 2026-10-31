// GFSerialiseVectorBinaryIn<CTCInventoryMap::CUIRegionInfo>(CMemoryDataInputStream& stream, std::vector<CTCInventoryMap::CUIRegionInfo>& out) -- read the element count (inline checked 4-byte read), resize the vector with zeroed region records, then read every record through CPersistTraits
// Grade: DIFFER(180v180) -- only the traits object's slot differs (`lea ecx,[esp+18h]` = the dead count word, retail
//   `lea ecx,[esp+1Ch]` = over the dead T() resize temporary). The zeroing CUIRegionInfo() ctor (Ego_r.pdb has
//   ??0CUIRegionInfo@CTCInventoryMap@@QAE@XZ) keeps retail's edi zero register; a POD element with a 0x28-byte
//   `T DefaultVal` traits DOES overlay the temporary (chunk 629 probe) but zero-fills it through ecx and opens edi inside
//   the count guard; a DefaultVal of the ctor type adds ten more stores.
// Leaf: ??$GFSerialiseVectorBinaryIn@VCUIRegionInfo@CTCInventoryMap@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@VCUIRegionInfo@CTCInventoryMap@@V?$allocator@VCUIRegionInfo@CTCInventoryMap@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00619560, 180 bytes (+12 int3), `ret` (__fastcall: stream in ecx, vector in edx).
// Caller: the CPersistContext transfer 0x006197D0 (CTCInventoryMap::UIRegionsInfo), whose binary-out arm is
//   GFSerialiseVectorBinaryOut<CUIRegionInfo> 0x00614700 (this chunk).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (count slow path), vector<CUIRegionInfo>::resize(n,
//   const T&) 0x006190C0, CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferBinaryIn 0x00614DA0 (its manifest label).
// Identity: FableWin.pdb ??$GFSerialiseVectorBinaryIn@VCUIRegionInfo@CTCInventoryMap@@@@YIX... (the template family; the 0x28
//   stride, the CTCInventoryMap caller 0x006197D0 and the bound resize 0x006190C0 fix T).
// Label correction: the catalog `GFSerialiseVectorBinaryIn<CBrainUpdateZone>` is propagated; the element is the 0x28-byte
//   CTCInventoryMap::CUIRegionInfo (ten longs, PDB layout as the landed traits 0x005FA610).
// Shape: the landed GFSerialiseVectorBinaryIn<unsigned long> 0x00412D20 template with a 40-byte element: the count word
//   at [esp+10h], the value-initialised `T()` resize temporary (ten zero stores from edi) above it, the block-scoped
//   traits object in the temporary's slot; the loop is strength-reduced (byte offset in edi, count down in ebx).
//   Speed TU, no pragma.
#include "engine/CMemoryDataInputStream.h"

class CTCInventoryMap { public:
    class CUIRegionInfo { public:                          // 0x28 bytes (Ego_r.pdb)
        long Region;            // +0x00
        long Shops;             // +0x04
        long BarberShops;       // +0x08
        long TitleShops;        // +0x0C
        long TatooShops;        // +0x10
        long ShopsForSale;      // +0x14
        long HousesForSale;     // +0x18
        long ShopsOwned;        // +0x1C
        long HousesOwned;       // +0x20
        long Taverns;           // +0x24
        CUIRegionInfo() : Region(0), Shops(0), BarberShops(0), TitleShops(0), TatooShops(0), ShopsForSale(0),
                          HousesForSale(0), ShopsOwned(0), HousesOwned(0), Taverns(0) { }
    };
};
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ((CDataInputStream*)this)->ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> >
class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    void resize(unsigned int newSize, const T& value);   // 0x006190C0 (out of line)
    void resize(unsigned int newSize) { resize(newSize, T()); }
};
}

template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x00614DA0
};

__forceinline void ReadCount(CMemoryDataInputStreamReader& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& inStream, std::vector<T>& out)
{
    CMemoryDataInputStreamReader& stream = static_cast<CMemoryDataInputStreamReader&>(inStream);
    long count;
    ReadCount(stream, &count);
    out.resize(count);
    {
        CPersistTraits<T> traits;
        for (long i = 0; i < count; ++i)
            traits.TransferBinaryIn(stream, out[i]);
    }
}
template void __fastcall GFSerialiseVectorBinaryIn<CTCInventoryMap::CUIRegionInfo>(CMemoryDataInputStream& inStream,
                                                                                   std::vector<CTCInventoryMap::CUIRegionInfo>& out);