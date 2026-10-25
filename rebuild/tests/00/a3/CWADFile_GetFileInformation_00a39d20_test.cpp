// Behaviour test for CWADFile::GetFileInformation (0x00A39D20): the record comes from the entry's info buffer
// (ReadInfo(0, &info, sizeof(CFileInformation)) on UpdateData[Index]) and is returned by value.
#include <cstdio>
#include <string.h>

class CDateAndTime {
public:
    CDateAndTime() : Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0), Millisecond(0) {}
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
};
class CFileInformation {
public:
    CDateAndTime CreationTime;
    CDateAndTime LastAccessTime;
    CDateAndTime LastWriteTime;
    bool         ReadOnly;
};

static unsigned long g_lastSize = 0;
class CBankFileEntryUpdateData {
public:
    CFileInformation stored;
    void ReadInfo(unsigned long offset, void* dest, unsigned long size) const
    {
        g_lastSize = size;
        memcpy(dest, (const char*)&stored + offset, size);
    }
};

template <class T> class CArray {
public:
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};
class CBankFileAsync {
public:
    CArray<CBankFileEntryUpdateData*> UpdateData;
};

class CBase { public: virtual ~CBase() {} };
class CWADFile : public CBase {
public:
    CBankFileAsync* Bank;
    unsigned long   Index;
    long            FilePos;
    virtual CFileInformation GetFileInformation();
};

CFileInformation CWADFile::GetFileInformation()
{
    CFileInformation info;
    Bank->UpdateData[Index]->ReadInfo(0, &info, sizeof(info));
    return info;
}

int main()
{
    CBankFileEntryUpdateData e0, e1;
    e1.stored.CreationTime.Year = 2004; e1.stored.LastWriteTime.Second = 42; e1.stored.ReadOnly = true;
    e0.stored.ReadOnly = false;
    CBankFileEntryUpdateData* table[2] = { &e0, &e1 };
    CBankFileAsync bank; bank.UpdateData.m_pFirst = table; bank.UpdateData.m_pLast = table + 2; bank.UpdateData.m_pEnd = table + 2;
    CWADFile f; f.Bank = &bank; f.Index = 1; f.FilePos = 0;
    CFileInformation r = f.GetFileInformation();
    if (r.CreationTime.Year != 2004 || r.LastWriteTime.Second != 42 || !r.ReadOnly || g_lastSize != sizeof(CFileInformation))
        { std::printf("00a39d20_TEST FAIL\n"); return 1; }
    std::printf("00a39d20_TEST PASS\n");
    return 0;
}