#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/009c82c0.cpp) ----
// NGameText::CDataBank::NotifyBankSize(unsigned long noEntries, const CArray<CBankFileDataTypeCount>& counts) -- for a read-only bank, size the entry table to the bank and pre-reserve the text and group entry buffers from the per-type entry counts
// Leaf: ?NotifyBankSize@CDataBank@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C82C0, 122 bytes (+6 int3 padding), `ret 8`.
// Name: FableWin.pdb ?NotifyBankSize@CDataBank@NGameText@@MAEXKABV?$CArray@VCBankFileDataTypeCount@@@@@Z (protected virtual,
// catalog vfunc_10); same shape as the landed CMeshDataBank::NotifyBankSize 0x00A26660.
// Callees: CArray<CDataBankEntry*>::resize(unsigned long, const T&) @ 0x009CAEB0 (fill value = a zero pointer in the frame),
// CArray<CDataBankTextEntry>::reserve @ 0x009CB1F0 (8-byte elements), CArray<CDataBankGroupEntry>::reserve @ 0x009CB2A0.
// Layout: ReadOnly +0x8D (CBankFile); Ego_r.pdb CDataBank members are 0x74 above retail with 16-byte CArrays: retail
// Entries +0x1E0, TextEntryBuffer +0x1EC, GroupEntryBuffer +0x1F8 (12-byte retail CArrays; the ctor 0x009C85A0 zeroes them).
// Shape: everything under the ReadOnly test; the count walk is an index loop over 8-byte {type, count} records strength-reduced
// to a pointer and a down-counter; type 0 adds to the text total (ebp), type 1 to the group total (ebx, which is also the zero).
namespace NGameText {
class CDataBankEntry;
class CDataBankTextEntry;
class CDataBankGroupEntry;
}

template<class T> struct CArray {               // retail CArray: 12 bytes
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T& operator[](unsigned long i) const { return *(First + i); }
    void resize(unsigned long size, const T& value);
    void reserve(unsigned long size);
};
struct CBankFileDataTypeCount {                 // 8 bytes
    long          Type;
    unsigned long Count;
};

namespace NGameText {
class CDataBank {
public:
    // +0x000 vftable (CBankFile), from the virtual below
    unsigned char _bankFile[0x88];              // +0x004 CBankFile
    bool          RetailMode;                   // +0x08c
    bool          ReadOnly;                     // +0x08d
    unsigned char _pad08e[0x1e0 - 0x8e];
    CArray<CDataBankEntry*>     Entries;            // +0x1e0
    CArray<CDataBankTextEntry>  TextEntryBuffer;    // +0x1ec
    CArray<CDataBankGroupEntry> GroupEntryBuffer;   // +0x1f8
protected:
    virtual void NotifyBankSize(unsigned long noEntries, const CArray<CBankFileDataTypeCount>& counts);
};

void CDataBank::NotifyBankSize(unsigned long noEntries, const CArray<CBankFileDataTypeCount>& counts)
{
    if (ReadOnly) {
        Entries.resize(noEntries, 0);
        unsigned long textCount = 0;
        unsigned long groupCount = 0;
        for (unsigned long i = 0; i < counts.Size(); ++i) {
            long type = counts[i].Type;
            unsigned long count = counts[i].Count;
            if (type == 0)
                textCount += count;
            else if (type == 1)
                groupCount += count;
        }
        TextEntryBuffer.reserve(textCount);
        GroupEntryBuffer.reserve(groupCount);
    }
}
} // namespace NGameText

// ---- stubs for the external callees ----

static const void* g_resizeThis = 0;
static unsigned long g_resizeSize = 0;
static int g_resizeNull = 0;
static const void* g_reserveThis[2];
static unsigned long g_reserveSize[2];
static int g_reserves = 0;
template<class T> void CArray<T>::resize(unsigned long size, const T& value) { g_resizeThis = this; g_resizeSize = size; g_resizeNull = (value == 0); }
template<class T> void CArray<T>::reserve(unsigned long size) { g_reserveThis[g_reserves] = this; g_reserveSize[g_reserves] = size; ++g_reserves; }
struct STestBank : NGameText::CDataBank {
    void Call(unsigned long n, const CArray<CBankFileDataTypeCount>& c) { NotifyBankSize(n, c); }
};

static void run()
{

    CBankFileDataTypeCount records[5];
    records[0].Type = 0; records[0].Count = 5;
    records[1].Type = 1; records[1].Count = 3;
    records[2].Type = 0; records[2].Count = 2;
    records[3].Type = 2; records[3].Count = 9;     // another type: ignored
    records[4].Type = 1; records[4].Count = 1;
    CArray<CBankFileDataTypeCount> counts;
    counts.First = records; counts.Last = records + 5; counts.End = records + 5;
    STestBank bank;
    bank.ReadOnly = false;
    bank.Call(40, counts);                         // a writable bank sizes nothing
    CHECK(g_resizeThis == 0 && g_reserves == 0);
    bank.ReadOnly = true;
    bank.Call(40, counts);
    CHECK(g_resizeThis == &bank.Entries && g_resizeSize == 40 && g_resizeNull == 1);
    CHECK(g_reserves == 2);
    CHECK(g_reserveThis[0] == &bank.TextEntryBuffer && g_reserveSize[0] == 7);
    CHECK(g_reserveThis[1] == &bank.GroupEntryBuffer && g_reserveSize[1] == 4);
}

int main()
{
    run();
    if (!g_ok) { std::printf("009c82c0_TEST FAIL\n"); return 1; }
    std::printf("009c82c0_TEST PASS\n");
    return 0;
}