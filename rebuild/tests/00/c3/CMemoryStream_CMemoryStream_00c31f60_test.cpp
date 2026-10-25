// Behaviour test for 00c31f60: CMemoryStream::CMemoryStream -- size query, buffer allocation, whole-data load; failures leave 0 remaining
#include <cstdio>
#include <cstring>
#include <new>
struct IRemoteSource {
    unsigned int m_size; int m_sizeOk; int m_loadOk; const char* m_data; int m_loads;
    virtual bool ReadSize(unsigned int* size) { if (!m_sizeOk) { *size = 99; return false; } *size = m_size; return true; }
    virtual ~IRemoteSource() { }
};
struct CRemoteLoader {
    IRemoteSource* m_pSource;
    bool LoadDataFromRemote(unsigned int offset, unsigned int size, void* dest)
    {
        ++m_pSource->m_loads;
        if (!m_pSource->m_loadOk) return false;
        std::memcpy(dest, m_pSource->m_data + offset, size);
        return true;
    }
};
struct IDataStream {
    virtual bool Read(void* dest, unsigned int size) = 0;
    virtual ~IDataStream() { }
};
struct CMemoryStream : IDataStream {
    char* m_pBuffer; char* m_pCursor; unsigned int m_remaining;
    CMemoryStream(IRemoteSource* source)
        : m_pBuffer(0), m_pCursor(0), m_remaining(0)
    {
        if (!source->ReadSize(&m_remaining)) {
            m_remaining = 0;
            return;
        }
        if (m_remaining) {
            m_pBuffer = (char*)::operator new(m_remaining);
            m_pCursor = m_pBuffer;
            CRemoteLoader loader;
            loader.m_pSource = source;
            if (!loader.LoadDataFromRemote(0, m_remaining, m_pBuffer))
                m_remaining = 0;
        }
    }
    virtual bool Read(void* dest, unsigned int size)
    {
        if (size > m_remaining) { m_remaining = 0; return false; }
        std::memcpy(dest, m_pCursor, size); m_remaining -= size; m_pCursor += size; return true;
    }
    virtual ~CMemoryStream() { if (m_pBuffer) { ::operator delete(m_pBuffer); m_pCursor = 0; m_pBuffer = 0; } }
};
int main()
{
    int ok = 1;
    IRemoteSource src; src.m_size = 5; src.m_sizeOk = 1; src.m_loadOk = 1; src.m_data = "hello"; src.m_loads = 0;
    {
        CMemoryStream s(&src);
        char out[6] = { 0 };
        ok = ok && s.m_remaining == 5 && s.m_pBuffer != 0 && s.m_pCursor == s.m_pBuffer && src.m_loads == 1;
        ok = ok && s.Read(out, 3) && std::memcmp(out, "hel", 3) == 0 && s.m_remaining == 2;
    }
    src.m_sizeOk = 0;
    { CMemoryStream s(&src); ok = ok && s.m_remaining == 0 && s.m_pBuffer == 0 && src.m_loads == 1; }
    src.m_sizeOk = 1; src.m_size = 0;
    { CMemoryStream s(&src); ok = ok && s.m_remaining == 0 && s.m_pBuffer == 0 && src.m_loads == 1; }
    src.m_size = 4; src.m_loadOk = 0;
    { CMemoryStream s(&src); ok = ok && s.m_remaining == 0 && s.m_pBuffer != 0 && src.m_loads == 2; }
    std::printf(ok ? "00c31f60_TEST PASS\n" : "00c31f60_TEST FAIL\n");
    return ok ? 0 : 1;
}