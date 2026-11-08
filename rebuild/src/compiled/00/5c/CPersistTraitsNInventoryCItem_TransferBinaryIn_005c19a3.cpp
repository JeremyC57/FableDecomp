// CPersistTraits<NInventory::CItem>::TransferBinaryIn(CMemoryDataInputStream& stream, NInventory::CItem& value) -- binary read of one inventory item
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCMemoryDataInputStream@@AAVCItem@NInventory@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pBlock->Info are the header CCountedPointer's (same offsets).
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C19A3, 1165 bytes, EBP frame, `ret 8` (traits `this` unused); same TU as the
//   landed GFSerialiseVectorBinaryIn 0x005C2AD8 (size TU: `#pragma optimize("y", off)` + `("s", on)`).
// Ego_r.pdb / FableWin.pdb: this leaf; NInventory::CPerItemData (retail 0x24 bytes: STLport vector<long> 0xC, NumberOfKills +0xC,
//   TimeWielded +0x10, BestTrophyWitnessesAheadToDate +0x14, QuestPersistent +0x18, QuestPersistentRefCount +0x1C,
//   BestFishWeight +0x20 -- the inline default ctor leaves the fish weight unset); ??0CItem@NInventory@@QAE@JJPBVCPerItemData@1@J@Z.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CDataInputStream::ReadString(unsigned long) 0x00996BE0,
//   vector<long>::resize(n, const long&) 0x004428F0, CCharString::operator!=(const char*) 0x0099E960 ("" 0x0122D70E,
//   "NULL" 0x012393E4), CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   ~CCharString 0x0099EAE0, NInventory::CItem::CItem 0x005BEC40, CItem::operator= 0x005C0C24,
//   CCountedPointer<CPerItemData> release/dtor 0x005C04BF, free 0x00BFEA14 (the local vector's storage).
// Shape: a has-data byte, then (optionally) the per-item data block read into a frame-local CPerItemData whose address is
//   passed to the CItem ctor (pData declared first: its zero store leads the frame init); the augmentation index goes through
//   a named local so the call result is computed before the element address; the name/len/int temporaries live in the dead
//   stream-argument slot (stream is in ebx). Shared fable_string.h (retail CCharString) and engine/CGameDefinitionManager.h
//   (retail CDefinitionManager branch); the CMemoryDataInputStream / CDataInputStream::ReadString, CCountedPointer and
//   NInventory::CItem / CPerItemData models are row-local as in the landed same-TU siblings 0x005C2AD8 / 0x00614AF0
//   (engine/CItem.h flattens the counted pointer; the retail CDataInputStream branch has no ReadString; see NOTES.md).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CGameDefinitionManager.h"
#define CMemoryDataInputStream CMemoryDataInputStreamLayout
#include "engine/CMemoryDataInputStream.h"
#undef CMemoryDataInputStream
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void __cdecl free(void* p);
#pragma intrinsic(memcpy)

class CDataInputStream
{
public:
    CCharString ReadString(unsigned long length);      // 0x00996BE0
private:
    friend struct CMemoryDataInputStreamReader;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};
class CMemoryDataInputStream : public CDataInputStream, public CMemoryDataInputStreamLayout
{
};
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    static FABLE_FORCEINLINE bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    FABLE_FORCEINLINE void Read(void* buffer, long size)
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
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    template <class T> FABLE_FORCEINLINE T ReadType() { T v; Read(&v, sizeof(T)); return v; }
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    void resize(unsigned long n, const T& x);           // 0x004428F0
    T& operator[](unsigned long n) { return _M_start[n]; }
};
}
struct SCountBlock;
#include "engine/CCountedPointer.h"
namespace NInventory {
class CPerItemData { public:
    std::vector<long> Augmentations;                        // +0x00
    long NumberOfKills;                                     // +0x0C
    long TimeWielded;                                       // +0x10
    long BestTrophyWitnessesAheadToDate;                    // +0x14
    bool QuestPersistent;                                   // +0x18
    long QuestPersistentRefCount;                           // +0x1C
    float BestFishWeight;                                   // +0x20
    CPerItemData() : NumberOfKills(0), TimeWielded(0), BestTrophyWitnessesAheadToDate(0), QuestPersistent(true), QuestPersistentRefCount(-1) { }
};
class CItem { public:
    long ItemType;                                          // +0x00
    long NumberOfItems;                                     // +0x04
    CCountedPointer<CPerItemData> PPerItemData;             // +0x08
    long AverageBoughtPrice;                                // +0x10
    CItem(long type, long number, const CPerItemData* data, long price);   // 0x005BEC40
    CItem& operator=(const CItem& other);                   // 0x005C0C24
};
}
template <class T> class CPersistTraits;
template <> class CPersistTraits<NInventory::CItem> {
public:
    void TransferBinaryIn(CMemoryDataInputStream& stream, NInventory::CItem& value);
};

void CPersistTraits<NInventory::CItem>::TransferBinaryIn(CMemoryDataInputStream& inStream, NInventory::CItem& value)
{
    CMemoryDataInputStreamReader& stream = static_cast<CMemoryDataInputStreamReader&>(inStream);
    NInventory::CPerItemData* pData = 0;
    NInventory::CPerItemData data;
    if (stream.ReadType<unsigned char>()) {
        long count = stream.ReadType<long>();
        data.Augmentations.resize(count, 0);
        for (long i = 0; i < count; ++i) {
            long length = stream.ReadType<long>();
            CCharString name = stream.ReadString(length);
            if (name != "" && name != "NULL") {
                long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
                data.Augmentations[i] = index;
            }
        }
        data.NumberOfKills = stream.ReadType<long>();
        data.TimeWielded = stream.ReadType<long>();
        data.BestTrophyWitnessesAheadToDate = stream.ReadType<long>();
        data.QuestPersistent = stream.ReadType<unsigned char>() != 0;
        data.QuestPersistentRefCount = stream.ReadType<long>();
        pData = &data;
    }
    CCharString name = stream.ReadString(stream.ReadType<long>());
    long type = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    long number = stream.ReadType<long>();
    long price = stream.ReadType<long>();
    value = NInventory::CItem(type, number, pData, price);
}