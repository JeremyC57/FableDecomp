// behaviour test for 0x005C19A3 CPersistTraits<NInventory::CItem>::TransferBinaryIn
#include "fable_compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
template <class T> class CCountedPointer { public:
    T* m_pObject;
    SCountBlock* m_pBlock;
    ~CCountedPointer();                                     // 0x005C04BF
};
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

// ---- stubs ----
static int g_dtors = 0, g_frees_seen = 0, g_overflow = 0;
struct FakeStringData { char* PData; unsigned long StringLength; unsigned long DataLength : 31; bool UseFastExtend : 1; long NoRefs; };
static const char* Text(const CCharString& s) { return (*(FakeStringData* const*)&s)->PData; }
CCharString::CCharString(const char* text, long length)
{
    FakeStringData* d = new FakeStringData; d->StringLength = (unsigned long)length; d->PData = (char*)malloc(length + 1);
    memcpy(d->PData, text, length); d->PData[length] = 0; d->DataLength = length; d->UseFastExtend = false; d->NoRefs = 1;
    PStringData = (CCharStringData*)d;
}
CCharString::~CCharString() { ++g_dtors; }
bool CCharString::operator!=(const char* text) const { return strcmp(Text(*this), text) != 0; }
CCharString CDataInputStream::ReadString(unsigned long length)
{
    CMemoryDataInputStreamReader* r = (CMemoryDataInputStreamReader*)this;
    const char* p = (const char*)r->chunkBase;
    r->chunkBase += length; r->chunkLength -= length; r->position += length;
    return CCharString(p, (long)length);
}
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++g_overflow; memset(buffer, 0, size); }
static long g_mgr[0x40];
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return *(CGameDefinitionManager*)g_mgr; }
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) const
{
    const char* s = Text(name);
    if (!strcmp(s, "AUG_FIRE")) return 101;
    if (!strcmp(s, "SWORD")) return 555;
    return -7;
}
namespace std { template<> void vector<long>::resize(unsigned long n, const long& x)
{ _M_start = (long*)malloc(n * sizeof(long)); for (unsigned long i = 0; i < n; ++i) _M_start[i] = x; _M_finish = _M_end_of_storage = _M_start + n; } }
template<> CCountedPointer<NInventory::CPerItemData>::~CCountedPointer() { }
static long g_aug[4]; static long g_kills, g_wield, g_trophy, g_ref; static bool g_qp; static int g_hasdata;
NInventory::CItem::CItem(long type, long number, const CPerItemData* data, long price)
{
    ItemType = type; NumberOfItems = number; AverageBoughtPrice = price; PPerItemData.m_pObject = 0; PPerItemData.m_pBlock = 0;
    g_hasdata = data != 0;
    if (data) {
        for (int i = 0; i < 4 && data->Augmentations._M_start + i < data->Augmentations._M_finish; ++i) g_aug[i] = data->Augmentations._M_start[i];
        g_kills = data->NumberOfKills; g_wield = data->TimeWielded; g_trophy = data->BestTrophyWitnessesAheadToDate;
        g_qp = data->QuestPersistent; g_ref = data->QuestPersistentRefCount;
    }
}
NInventory::CItem& NInventory::CItem::operator=(const CItem& o)
{ ItemType = o.ItemType; NumberOfItems = o.NumberOfItems; AverageBoughtPrice = o.AverageBoughtPrice; return *this; }

static unsigned char buf[256]; static int blen;
static void put32(long v) { memcpy(buf + blen, &v, 4); blen += 4; }
static void put8(unsigned char v) { buf[blen++] = v; }
static void putstr(const char* s) { long n = (long)strlen(s); put32(n); memcpy(buf + blen, s, n); blen += n; }
static void setup(CMemoryDataInputStream& s)
{
    memset(&s, 0, sizeof(s));
    CMemoryDataInputStreamReader& r = (CMemoryDataInputStreamReader&)s;
    r.position = 0; r.chunkBase = (long)buf; r.chunkLength = blen;
}
int main()
{
    int ok = 1;
    CPersistTraits<NInventory::CItem> traits;
    // case 1: per-item data present, three augmentations ("AUG_FIRE", "NULL", "")
    blen = 0;
    put8(1); put32(3); putstr("AUG_FIRE"); putstr("NULL"); putstr("");
    put32(12); put32(340); put32(5); put8(0); put32(2);
    putstr("SWORD"); put32(3); put32(250);
    CMemoryDataInputStream s; setup(s);
    NInventory::CItem item(0, 0, 0, 0);
    traits.TransferBinaryIn(s, item);
    if (item.ItemType != 555 || item.NumberOfItems != 3 || item.AverageBoughtPrice != 250) ok = 0;
    if (!g_hasdata || g_aug[0] != 101 || g_aug[1] != 0 || g_aug[2] != 0) ok = 0;
    if (g_kills != 12 || g_wield != 340 || g_trophy != 5 || g_qp != false || g_ref != 2) ok = 0;
    if (((CMemoryDataInputStreamReader&)s).position != blen || g_overflow) ok = 0;
    // case 2: no per-item data
    blen = 0; put8(0); putstr("SWORD"); put32(1); put32(9);
    setup(s);
    traits.TransferBinaryIn(s, item);
    if (g_hasdata || item.ItemType != 555 || item.NumberOfItems != 1 || item.AverageBoughtPrice != 9) ok = 0;
    if (((CMemoryDataInputStreamReader&)s).position != blen) ok = 0;
    // case 3: short stream -> overflow reader called
    blen = 0; put8(1); setup(s); ((CMemoryDataInputStreamReader&)s).chunkLength = 1;
    traits.TransferBinaryIn(s, item);
    if (!g_overflow) ok = 0;
    printf("005c19a3_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}