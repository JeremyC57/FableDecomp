// Behaviour test for CMemoryDataOutputStream::`scalar deleting destructor' (0x009973C0): deleting through the root base
// frees the data buffer, destroys NewLine and runs the root destructor once.
#include <cstdio>
#include <stdlib.h>

static int g_strings = 0; static int g_roots = 0;
class CCharString {
public:
    void* PStringData;
    CCharString() : PStringData(0) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
template <class T> class CArray {
public:
    T* m_pData; T* m_pLast; T* m_pEnd;
    CArray() : m_pData(0), m_pLast(0), m_pEnd(0) {}
    ~CArray() { if (m_pData) free(m_pData); }
};
class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable() { ++g_roots; }
    virtual ~CBaseClassNonCopyable() { --g_roots; }
};
class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream() {}
};
class CDataOutputStream : public CDataStream {
public:
    CCharString NewLine;
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    virtual ~CMemoryDataOutputStream();
    CArray<char>  Data;
    unsigned long CurrentPosition;
};
CMemoryDataOutputStream::~CMemoryDataOutputStream()
{
}

int main()
{
    CMemoryDataOutputStream* s = new CMemoryDataOutputStream;
    s->Data.m_pData = (char*)malloc(32); s->Data.m_pLast = s->Data.m_pData + 3; s->Data.m_pEnd = s->Data.m_pData + 32;
    if (g_strings != 1 || g_roots != 1) { std::printf("009973c0_TEST FAIL\n"); return 1; }
    CBaseClassNonCopyable* base = s;
    delete base;
    if (g_strings != 0 || g_roots != 0) { std::printf("009973c0_TEST FAIL\n"); return 1; }
    std::printf("009973c0_TEST PASS\n");
    return 0;
}