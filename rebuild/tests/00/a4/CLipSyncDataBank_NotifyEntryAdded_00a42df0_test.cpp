// Behaviour test for 00a42df0: final.cpp copied verbatim, resize and the base NotifyEntryAdded stubbed so the growth
// request, the recorded length and the forwarded arguments are observable.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define _snprintf snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[4096];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, "; "); } }
// ---- source (verbatim) ----
// CLipSyncDataBank::NotifyEntryAdded(unsigned long, unsigned long index, const CArray<char>& data) -- grow the per-entry table (+0x164) to cover the new entry (zero-filled), record the entry's leading float (the sample length stored in the first four data bytes) and forward to CBankFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A42DF0, length 128 bytes.
// Leaf: ?NotifyEntryAdded@CLipSyncDataBank@@MAEXKKABV?$CArray@D@@@Z
// Callees: the entry vector's resize(size, value) @ 0x00A43F20 (as in the landed NotifyBankSize 0x00A42E70),
//          CBankFile::NotifyEntryAdded @ 0x009CBC20 (direct base call).
// Shape: size() is the signed `(last - first) >> 3` compared unsigned; the zero value is an 8-byte frame
// temporary stored after the pushes; the data pointer is the inline `size() ? first : (char*)this` and its
// first four bytes are memcpy'd (intrinsic dword copy) into a float local the compiler homes in the dead `index` slot;
// the memcpy form is what lets the x87 store of that float schedule between the index and id pushes.
#include <string.h>
template <class T> class CArray {
public:
    T* m_first;
    T* m_last;
    T* m_end;
    const T* GetData() const { return (m_last - m_first) ? m_first : (const T*)this; }
};
struct SLipSyncEntry {                             // two floats, zeroed by the inline constructor
    float m_0;
    float m_length;                                // +0x04
    SLipSyncEntry() : m_0(0.0f), m_length(0.0f) { }
};
struct CLipSyncEntryVector {
    SLipSyncEntry* m_first;
    SLipSyncEntry* m_last;
    SLipSyncEntry* m_end;
    unsigned long size() const { return m_last - m_first; }
    SLipSyncEntry* begin() { return m_first; }
    void resize(unsigned long size, const SLipSyncEntry& value);   // 0x00A43F20
};
class CBankFile {
public:
    virtual void v00();
protected:
    virtual void NotifyEntryAdded(unsigned long id, unsigned long index, const CArray<char>& data);   // 0x009CBC20
    unsigned char _pad04[0x160];
};
class CLipSyncDataBank : public CBankFile {
protected:
    CLipSyncEntryVector m_entries;                 // +0x164
    virtual void NotifyEntryAdded(unsigned long id, unsigned long index, const CArray<char>& data);
};

void CLipSyncDataBank::NotifyEntryAdded(unsigned long id, unsigned long index, const CArray<char>& data)
{
    if (index >= m_entries.size())
        m_entries.resize(index + 1, SLipSyncEntry());
    float length;
    memcpy(&length, data.GetData(), sizeof(float));
    (m_entries.begin() + index)->m_length = length;
    CBankFile::NotifyEntryAdded(id, index, data);
}

// ---- stubs ----
static char g_buf[256];
static SLipSyncEntry g_storage[16];
void CLipSyncEntryVector::resize(unsigned long size, const SLipSyncEntry& value)
{
    _snprintf(g_buf, sizeof(g_buf), "resize(%lu,{%g,%g})", size, value.m_0, value.m_length);
    note(g_buf);
    unsigned long old = m_last - m_first;
    for (unsigned long i = old; i < size; ++i) m_first[i] = value;
    m_last = m_first + size;
}
void CBankFile::v00() { }
void CBankFile::NotifyEntryAdded(unsigned long id, unsigned long index, const CArray<char>& data)
{
    _snprintf(g_buf, sizeof(g_buf), "CBankFile::NotifyEntryAdded(%lu,%lu,%lu bytes)", id, index, (unsigned long)(data.m_last - data.m_first));
    note(g_buf);
}
class CTestBank : public CLipSyncDataBank {
public:
    CTestBank() { m_entries.m_first = g_storage; m_entries.m_last = g_storage; m_entries.m_end = g_storage + 16; }
    void Add(unsigned long id, unsigned long index, const CArray<char>& data) { NotifyEntryAdded(id, index, data); }
    unsigned long Count() const { return m_entries.size(); }
    float Length(unsigned long i) const { return m_entries.m_first[i].m_length; }
};

int main()
{
    CTestBank bank;
    char bytes[8];
    float f = 1.5f;
    memcpy(bytes, &f, 4);
    CArray<char> data; data.m_first = bytes; data.m_last = bytes + 8; data.m_end = bytes + 8;
    for (int i = 0; i < 16; ++i) { g_storage[i].m_0 = 9.0f; g_storage[i].m_length = 9.0f; }

    // 1. index beyond the table: grow to index+1 with zeroed entries, record the length, forward
    g_log[0] = 0;
    bank.Add(7, 2, data);
    CHECKLOG("resize(3,{0,0}); CBankFile::NotifyEntryAdded(7,2,8 bytes); ");
    CHECK(bank.Count() == 3);
    CHECK(bank.Length(0) == 0.0f && bank.Length(1) == 0.0f && bank.Length(2) == 1.5f);
    CHECK(g_storage[2].m_0 == 0.0f);

    // 2. index inside the table: no resize, length overwritten
    f = -2.25f; memcpy(bytes, &f, 4);
    g_log[0] = 0;
    bank.Add(8, 1, data);
    CHECKLOG("CBankFile::NotifyEntryAdded(8,1,8 bytes); ");
    CHECK(bank.Count() == 3);
    CHECK(bank.Length(1) == -2.25f && bank.Length(2) == 1.5f);

    // 3. index == size: grows by one
    f = 4.0f; memcpy(bytes, &f, 4);
    g_log[0] = 0;
    bank.Add(9, 3, data);
    CHECKLOG("resize(4,{0,0}); CBankFile::NotifyEntryAdded(9,3,8 bytes); ");
    CHECK(bank.Count() == 4 && bank.Length(3) == 4.0f);

    // 4. empty data array: GetData() yields the array object itself (its first dword is m_first)
    CArray<char> empty; empty.m_first = bytes; empty.m_last = bytes; empty.m_end = bytes;
    g_log[0] = 0;
    bank.Add(10, 0, empty);
    CHECKLOG("CBankFile::NotifyEntryAdded(10,0,0 bytes); ");
    float expect; memcpy(&expect, &empty.m_first, 4);
    CHECK(memcmp(&g_storage[0].m_length, &expect, 4) == 0);

    if (fails == 0) printf("00a42df0_TEST PASS\n");
    else printf("00a42df0_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}