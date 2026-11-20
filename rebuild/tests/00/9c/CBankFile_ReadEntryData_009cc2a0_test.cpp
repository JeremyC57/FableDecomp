#include <cstdio>
#include <cstring>
// Behaviour test for 0x009CC2A0 CBankFile::ReadEntryData (links the row object).
// NF139 (2026-11-20, q6308): rewritten on the shared headers the row now uses (engine/CBankFile.h with the row's define set,
//   engine/CPackedUIntArray.h, engine/CDiskFileWin32.h); the checks and the PassPattern are unchanged. The bank is a zeroed 0x110-byte
//   block with RuntimeData (+0x20, STLport vector begin / end / capacity), PackedDataOffset (+0x38) / PackedDataSize (+0x48)
//   (PackedInts +0, Size +4) and BankFile_Data (+0x7C) written in place. CPackedUIntArray::operator[] (0x00A629F0) is a plain
//   indexing fixture; the disk file is a fake object with the CDiskFileWin32 slot order (Read slot 3, SetPosition slot 5).
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CPackedUIntArray.h"
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR
#define FABLE_RETAIL_CBANKFILE_PACKRUNTIMEDATA
#define FABLE_RETAIL_CBANKFILE_ENTRYDATA_INLINE
#define FABLE_RETAIL_CBANKFILE_READENTRYDATA
#include "engine/CBankFile.h"

unsigned long CPackedUIntArray::operator[](unsigned long index) const
{
    return PackedInts[index];
}

struct FakeDiskFile {
    unsigned long Position, ReadSize;
    void* ReadBuffer;
    void* Process;
    virtual void Slot0() {}
    virtual void Slot1() {}
    virtual void Slot2() {}
    virtual void Read(void* buffer, long size, void* process) { ReadBuffer = buffer; ReadSize = (unsigned long)size; Process = process; }
    virtual void Slot4() {}
    virtual void SetPosition(long position) { Position = (unsigned long)position; }
};

struct RuntimeRecord { unsigned long DataOffset, DataSize; unsigned char DataType; bool Valid; unsigned char Pad[2]; };
static RuntimeRecord g_rt[4] = { {0,0,0,false,{0,0}}, {100,10,1,true,{0,0}}, {200,20,2,false,{0,0}}, {300,30,3,true,{0,0}} };
static unsigned long g_psize[3] = {11, 12, 13};
static unsigned long g_poff[3] = {1000, 2000, 3000};
static double g_bank[2][(0x110 + 7) / 8];

static void Put(unsigned char* raw, unsigned long offset, const void* value) { std::memcpy(raw + offset, value, 4); }

static CBankFile& MakeBank(int slot, bool packed, FakeDiskFile* file)
{
    std::memset(g_bank[slot], 0, sizeof g_bank[slot]);
    unsigned char* raw = (unsigned char*)g_bank[slot];
    RuntimeRecord* first = g_rt; RuntimeRecord* last = g_rt + 4;
    unsigned long four = 3;
    Put(raw, 0x20, &first); Put(raw, 0x24, &last); Put(raw, 0x28, &last);
    if (packed) {
        unsigned long* off = g_poff; unsigned long* size = g_psize;
        Put(raw, 0x38, &off); Put(raw, 0x3C, &four);
        Put(raw, 0x48, &size); Put(raw, 0x4C, &four);
    }
    Put(raw, 0x7C, &file);
    return *(CBankFile*)raw;
}

static bool Check()
{
    char buf[8]; FakeDiskFile f; f.Position = 0; f.ReadSize = 0; f.ReadBuffer = 0; f.Process = &f;
    CBankFile& b = MakeBank(0, false, &f);
    b.ReadEntryData(2, 5, buf, 8);
    if (f.Position != 205 || f.ReadBuffer != buf || f.ReadSize != 8 || f.Process != 0) return false;
    CBankFile& p = MakeBank(1, true, &f);
    p.ReadEntryData(3, 1, buf, 2);
    return f.Position == 3001 && f.ReadSize == 2;
}
int main()
{
    if (sizeof(FakeDiskFile) != 0x14 || sizeof(RuntimeRecord) != 12) { std::printf("009cc2a0_TEST FAIL layout\n"); return 1; }
    if (!Check()) { std::printf("009cc2a0_TEST FAIL\n"); return 1; }
    std::printf("009cc2a0_TEST PASS\n");
    return 0;
}
