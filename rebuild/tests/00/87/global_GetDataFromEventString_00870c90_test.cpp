// Behaviour test for 0x00870C90: GetDataFromEventString returns the index-th space-separated field after the
// event name, false when there are not enough fields.
#include <cstdio>
#include <cstring>

class CCharStringData { public: char* m_pData; unsigned long m_length; };

class CCharString
{
public:
    CCharStringData* PStringData;
    CCharString() : PStringData(0) { }
    CCharString(const char* s, unsigned long n) : PStringData(0) { Set(s, n); }
    CCharString(const CCharString& o) : PStringData(0) { if (o.PStringData) Set(o.PStringData->m_pData, o.PStringData->m_length); }
    ~CCharString() { Clear(); }
    void Clear()
    {
        if (PStringData) { delete[] PStringData->m_pData; delete PStringData; PStringData = 0; }
    }
    void Set(const char* s, unsigned long n)
    {
        CCharStringData* d = new CCharStringData;
        d->m_pData = new char[n + 1];
        std::memcpy(d->m_pData, s, n);
        d->m_pData[n] = 0;
        d->m_length = n;
        Clear();
        PStringData = d;
    }
    const CCharString& operator=(const CCharString& o)
    {
        if (this != &o) { if (o.PStringData) Set(o.PStringData->m_pData, o.PStringData->m_length); else Clear(); }
        return *this;
    }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    long FindCharPos(char c) const
    {
        for (unsigned long i = 0; i < GetLength(); ++i)
            if (PStringData->m_pData[i] == c) return (long)i;
        return -1;
    }
    CCharString GetSubString(unsigned long start, unsigned long length) const
    {
        unsigned long n = GetLength();
        if (start > n) start = n;
        if (length > n - start) length = n - start;
        return CCharString(PStringData ? PStringData->m_pData + start : "", length);
    }
    bool Equals(const char* s) const
    {
        unsigned long n = (unsigned long)std::strlen(s);
        return GetLength() == n && (n == 0 || std::memcmp(PStringData->m_pData, s, n) == 0);
    }
};

bool GetDataFromEventString(const CCharString& eventString, long index, CCharString& data)
{
    data = eventString;
    long start = eventString.FindCharPos(' ') + 1;
    if (start == 0)
        return false;
    data = eventString.GetSubString(start, eventString.GetLength());
    while (--index != -1)
    {
        start = data.FindCharPos(' ') + 1;
        if (start == 0)
            return false;
        data = data.GetSubString(start, data.GetLength());
    }
    long pos = data.FindCharPos(' ');
    data = data.GetSubString(0, pos == -1 ? data.GetLength() : pos);
    return true;
}

int main()
{
    CCharString ev("PLAY_SOUND alpha beta gamma", 27);
    CCharString out;
    int fails = 0;
    if (!GetDataFromEventString(ev, 0, out) || !out.Equals("alpha")) ++fails;
    if (!GetDataFromEventString(ev, 1, out) || !out.Equals("beta")) ++fails;
    if (!GetDataFromEventString(ev, 2, out) || !out.Equals("gamma")) ++fails;
    if (GetDataFromEventString(ev, 3, out)) ++fails;
    CCharString bare("NOARGS", 6);
    if (GetDataFromEventString(bare, 0, out)) ++fails;
    if (fails) { std::printf("00870c90_TEST FAIL\n"); return 1; }
    std::printf("00870c90_TEST PASS\n");
    return 0;
}