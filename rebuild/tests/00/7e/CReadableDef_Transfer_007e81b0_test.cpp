// Behaviour test for CReadableDef::Transfer @ 0x007E81B0: CRC check, then load (mode 2) or save (mode 3) of the flag.
#include <cstdio>

struct CDataOutputStream { int writes; bool last; CDataOutputStream() : writes(0), last(false) {} void WriteBool(bool v) { ++writes; last = v; } };
struct CMemoryDataInputStream { bool next; int reads; CMemoryDataInputStream() : next(false), reads(0) {} };

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<bool> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value) { ++stream->reads; *value = stream->next; }
    static void TransferBinaryOut(CDataOutputStream* stream, const bool& value) { stream->WriteBool(value); }
};

struct CPersistContext {
    long m_mode; CMemoryDataInputStream* m_pLoadStream; CDataOutputStream* m_pSaveStream; int m_crcChecks;
    void CheckCRC(const char* name) { if (name[0] == 0) ++m_crcChecks; }
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2: CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value); break;
        case 3: CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value); break;
        }
    }
};

struct CReadableDef {
    bool UseInfoDisplay;
    void Transfer(CPersistContext& context) { context.Transfer("", UseInfoDisplay); }
};

int main()
{
    CMemoryDataInputStream in; in.next = true;
    CDataOutputStream out;
    CPersistContext c; c.m_pLoadStream = &in; c.m_pSaveStream = &out; c.m_crcChecks = 0;
    CReadableDef d; d.UseInfoDisplay = false;
    c.m_mode = 2; d.Transfer(c);
    if (!d.UseInfoDisplay || in.reads != 1 || out.writes != 0) { std::printf("007e81b0_TEST FAIL\n"); return 1; }
    c.m_mode = 3; d.Transfer(c);
    if (out.writes != 1 || !out.last || in.reads != 1) { std::printf("007e81b0_TEST FAIL\n"); return 1; }
    c.m_mode = 1; d.Transfer(c);
    if (out.writes != 1 || in.reads != 1 || c.m_crcChecks != 3) { std::printf("007e81b0_TEST FAIL\n"); return 1; }
    std::printf("007e81b0_TEST PASS\n");
    return 0;
}