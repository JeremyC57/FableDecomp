// Behaviour test for 0x0083EA70 LHNetworkLib::LHHttp2::GetDocumentSize
#include <stdio.h>
struct LHHttpDocumentParts { char* Buffer; unsigned long BufferLength; unsigned long CurrBufPos; };
struct LDocumentPartVector {
    LHHttpDocumentParts** _M_start;
    LHHttpDocumentParts** _M_finish;
    LHHttpDocumentParts** _M_end_of_storage;
    LHHttpDocumentParts** begin() { return _M_start; }
    LHHttpDocumentParts** end() { return _M_finish; }
};
struct LHHttp2 {
    LDocumentPartVector DocumentParts;
    unsigned long GetDocumentSize();
};
unsigned long LHHttp2::GetDocumentSize()
{
    unsigned long size = 0;
    for (LHHttpDocumentParts** it = DocumentParts.begin(); it != DocumentParts.end(); ++it) {
        if (*it)
            size += (*it)->BufferLength;
    }
    return size;
}
int main()
{
    int ok = 1;
    LHHttpDocumentParts p1 = { 0, 10, 0 };
    LHHttpDocumentParts p2 = { 0, 32, 0 };
    LHHttpDocumentParts* parts[3];
    parts[0] = &p1; parts[1] = 0; parts[2] = &p2;
    LHHttp2 h;
    h.DocumentParts._M_start = parts; h.DocumentParts._M_finish = parts + 3; h.DocumentParts._M_end_of_storage = parts + 3;
    if (h.GetDocumentSize() != 42) ok = 0;
    h.DocumentParts._M_finish = parts;
    if (h.GetDocumentSize() != 0) ok = 0;
    printf(ok ? "0083ea70_TEST PASS\n" : "0083ea70_TEST FAIL\n");
    return ok ? 0 : 1;
}