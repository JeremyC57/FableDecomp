// Behaviour test for 0x0083EB00 LHNetworkLib::LHHttp2::ResetDocumentParts
#include <stdio.h>
#include <string.h>
static int g_destroyed = 0;
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
struct LHHttpDocumentParts {
    char* Buffer; unsigned long BufferLength; unsigned long CurrBufPos;
    ~LHHttpDocumentParts()
    {
        ++g_destroyed;
        if (Buffer) {
            delete[] Buffer;
            Buffer = 0;
        }
        BufferLength = 0;
    }
};
struct LDocumentPartVector {
    LHHttpDocumentParts** _M_start;
    LHHttpDocumentParts** _M_finish;
    LHHttpDocumentParts** _M_end_of_storage;
    LHHttpDocumentParts** begin() { return _M_start; }
    LHHttpDocumentParts** end() { return _M_finish; }
    void clear()
    {
        LHHttpDocumentParts** i = (LHHttpDocumentParts**)CopyTrivial(end(), _M_finish, begin());
        _M_finish = i;
    }
};
struct LHHttp2 {
    LDocumentPartVector DocumentParts;
    void ResetDocumentParts();
};
void LHHttp2::ResetDocumentParts()
{
    for (LHHttpDocumentParts** it = DocumentParts.begin(); it != DocumentParts.end(); ++it)
        delete *it;
    DocumentParts.clear();
}
int main()
{
    int ok = 1;
    LHHttpDocumentParts* parts[3];
    parts[0] = new LHHttpDocumentParts; parts[0]->Buffer = new char[4]; parts[0]->BufferLength = 4;
    parts[1] = 0;
    parts[2] = new LHHttpDocumentParts; parts[2]->Buffer = 0; parts[2]->BufferLength = 0;
    LHHttp2 h;
    h.DocumentParts._M_start = parts; h.DocumentParts._M_finish = parts + 3; h.DocumentParts._M_end_of_storage = parts + 3;
    h.ResetDocumentParts();
    if (g_destroyed != 2) ok = 0;
    if (h.DocumentParts._M_finish != h.DocumentParts._M_start) ok = 0;
    printf(ok ? "0083eb00_TEST PASS\n" : "0083eb00_TEST FAIL\n");
    return ok ? 0 : 1;
}