// Behaviour test for 009ccb20: src/009ccb20.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CBankFile::WritePadding(unsigned long count)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CBankFile::WritePadding(unsigned long count) -- make the bank file writable (reopen once), then write `count` zero bytes at the current position
// Leaf: WritePadding (Ego_r.pdb `?WritePadding@CBankFile@@AAEXK@Z`, private; the landed caller 0x009CD330 calls it WriteZeros)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CCB20, 156 bytes, __fastcall, ret 4.
// The catalog label (CBankFile::MakeWritable) names the inlined first half: the body of the landed
// CBankFile::MakeWritable() 0x009CBF90 is repeated here verbatim, followed by the zero fill. Callers:
// 0x009CCD9C, 0x009CE57F, 0x009CE960 and the write helper 0x009CD330 (its padding argument).
// Callees: CDiskFile virtual slots 7 (GetPosition), 2 (Close), 1 (Open), 5 (Seek), 4 (Write, +0x10);
// the byte-vector fill constructor 0x009D2A40 (labelled Vector_InitFill), free 0x00BFEA14.
// Layout: the retail CBankFile layout of the landed rows (engine/CBankFile.h is the PDB layout, 0x10 higher
// from +0x38 on): BankFile_Data +0x7C, Filename +0x84 (CWideString), WrittenToFile +0x8E.
// Shape: MakeWritable() inlined; the zero buffer is a local byte vector built by the out-of-line fill
// constructor (n, const value&, const allocator&) whose value and allocator temporaries are both homed
// in the dead `count` argument slot (`lea ecx,[esp+18h] ; push ecx ; lea edx,[esp+1Ch] ; push edx` both
// address it, the zero byte is stored after the pushes); its data pointer is the inline "first, or the
// array's own address when empty" accessor; the vector's inline destructor frees the storage.
// The two temporaries share the dead slot only with a same-TU noinline stand-in for the fill constructor (the
// const-ref taker; extern, VC7.1 gives them their own frame words, 26 lines), and the file pointer is a named local
// read before the accessor (retail loads it into ecx above the empty test).
#include <stdlib.h>

class CWideString;

struct CDiskFileCalls {
    virtual void v00();
    virtual void Open(const CWideString& filename, unsigned long mode, unsigned long share);   // slot 1 (+0x04)
    virtual void Close();                                                                       // slot 2 (+0x08)
    virtual void v03();
    virtual void Write(const void* data, unsigned long size, unsigned long flags);              // slot 4 (+0x10)
    virtual void Seek(unsigned long position);                                                  // slot 5 (+0x14)
    virtual void v06();
    virtual unsigned long GetPosition();                                                        // slot 7 (+0x1c)
};

struct CRetailAllocator { CRetailAllocator() {} };

struct CByteArray {                              // retail vector<unsigned char>: first / last / end of storage
    unsigned char* m_pFirst;
    unsigned char* m_pLast;
    unsigned char* m_pEnd;
    CByteArray(unsigned long count, const unsigned char& value, const CRetailAllocator& allocator = CRetailAllocator());   // 0x009D2A40
    ~CByteArray() { if (m_pFirst) free(m_pFirst); }
    unsigned char* GetData()
    {
        if (m_pLast - m_pFirst != 0)
            return m_pFirst;
        return reinterpret_cast<unsigned char*>(this);
    }
};

// Same-TU stand-in for the fill constructor 0x009D2A40 (not the retail body).
void FillBytes_009d2a40(CByteArray* array, unsigned long count, unsigned char value);
#pragma auto_inline(off)
__declspec(noinline) CByteArray::CByteArray(unsigned long count, const unsigned char& value, const CRetailAllocator& allocator)
{
    FillBytes_009d2a40(this, count, value);
}
#pragma auto_inline(on)
struct CBankFile_Retail {                        // the retail layout; engine/CBankFile.h (PDB) disagrees with the bytes
    void*                     __vftable;            // +0x00
    unsigned long             Size;                 // +0x04
    unsigned char             _arrays[0x70];        // +0x08 Symbols .. PackedValid
    unsigned long             OpenFlags;            // +0x78
    CDiskFileCalls*           BankFile_Data;        // +0x7c CCountedPointer<CDiskFileWin32>::Data
    void*                     BankFile_Info;        // +0x80
    void*                     Filename_PStringData; // +0x84 CWideString
    void*                     BankHandle_PStringData; // +0x88 CCharString
    bool                      RetailMode;           // +0x8c
    bool                      ReadOnly;             // +0x8d
    bool                      WrittenToFile;        // +0x8e
    bool                      Solid;                // +0x8f
    void MakeWritable()                             // 0x009CBF90 (landed)
    {
        if (!WrittenToFile) {
            unsigned long position = BankFile_Data->GetPosition();
            BankFile_Data->Close();
            BankFile_Data->Open(*(const CWideString*)&Filename_PStringData, 2, 4);
            BankFile_Data->Seek(position);
            WrittenToFile = true;
        }
    }
    void WritePadding(unsigned long count);
};

void CBankFile_Retail::WritePadding(unsigned long count)
{
    MakeWritable();
    if (count) {
        CByteArray zeros(count, 0);
        CDiskFileCalls* file = BankFile_Data;
        file->Write(zeros.GetData(), count, 0);
    }
}

// ---- stubs and driver ----
void CDiskFileCalls::v00() { } void CDiskFileCalls::v03() { } void CDiskFileCalls::v06() { }
void CDiskFileCalls::Open(const CWideString&, unsigned long, unsigned long) { CHECK(0); }
void CDiskFileCalls::Close() { CHECK(0); }
void CDiskFileCalls::Write(const void*, unsigned long, unsigned long) { CHECK(0); }
void CDiskFileCalls::Seek(unsigned long) { CHECK(0); }
unsigned long CDiskFileCalls::GetPosition() { CHECK(0); return 0; }
static int g_getPos = 0, g_close = 0, g_open = 0, g_seek = 0, g_write = 0, g_fills = 0;
static unsigned long g_seekPos = 0, g_openMode = 0, g_openShare = 0, g_writeSize = 0, g_writeFlags = 9;
static const CWideString* g_openName = 0; static bool g_allZero = false;
static unsigned char g_fillValue = 0xff;
void FillBytes_009d2a40(CByteArray* array, unsigned long count, unsigned char value)
{
    ++g_fills; g_fillValue = value;
    array->m_pFirst = (unsigned char*)malloc(count);
    for (unsigned long i = 0; i < count; ++i) array->m_pFirst[i] = value ^ 0;   // value written
    array->m_pLast = array->m_pEnd = array->m_pFirst + count;
}
struct CDiskFileTest : CDiskFileCalls {
    void Open(const CWideString& filename, unsigned long mode, unsigned long share) { ++g_open; g_openName = &filename; g_openMode = mode; g_openShare = share; }
    void Close() { ++g_close; }
    void Write(const void* data, unsigned long size, unsigned long flags)
    {
        ++g_write; g_writeSize = size; g_writeFlags = flags; g_allZero = true;
        for (unsigned long i = 0; i < size; ++i) if (((const unsigned char*)data)[i] != 0) g_allZero = false;
    }
    void Seek(unsigned long position) { ++g_seek; g_seekPos = position; }
    unsigned long GetPosition() { ++g_getPos; return 0x400; }
};
int main()
{
    static CDiskFileTest file; static CBankFile_Retail bank;
    bank.BankFile_Data = &file; bank.WrittenToFile = false;
    // 1) count 0: reopened for writing (position kept), nothing written
    bank.WritePadding(0);
    CHECK(g_getPos == 1 && g_close == 1 && g_open == 1 && g_seek == 1 && g_seekPos == 0x400);
    CHECK(g_openName == (const CWideString*)&bank.Filename_PStringData && g_openMode == 2 && g_openShare == 4);
    CHECK(bank.WrittenToFile == true && g_write == 0 && g_fills == 0);
    // 2) already writable: no reopen; 37 zero bytes written with flags 0
    bank.WritePadding(37);
    CHECK(g_open == 1 && g_close == 1 && g_fills == 1 && g_fillValue == 0);
    CHECK(g_write == 1 && g_writeSize == 37 && g_writeFlags == 0 && g_allZero);
    // 3) not yet writable with a count: reopen then write
    bank.WrittenToFile = false;
    bank.WritePadding(5);
    CHECK(g_open == 2 && g_write == 2 && g_writeSize == 5 && g_allZero && bank.WrittenToFile);
    // 4) the accessor: an empty array yields its own address
    {
        CByteArray empty(0, 0);
        CHECK(empty.GetData() == (unsigned char*)&empty);
    }
    if (fails == 0) printf("009ccb20_TEST PASS\n");
    return fails;
}
