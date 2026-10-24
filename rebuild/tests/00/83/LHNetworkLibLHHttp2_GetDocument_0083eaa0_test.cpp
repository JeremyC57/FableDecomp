// Behaviour test for 0x0083EAA0 LHNetworkLib::LHHttp2::GetDocument
#include <stdio.h>
#include <string.h>
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
    long GetDocument(char* buffer);
};
long LHHttp2::GetDocument(char* buffer)
{
    unsigned long offset = 0;
    for (LHHttpDocumentParts** it = DocumentParts.begin(); it != DocumentParts.end(); ++it) {
        LHHttpDocumentParts* part = *it;
        if (part) {
            memcpy(buffer + offset, part->Buffer, part->BufferLength);
            offset += part->BufferLength;
        }
    }
    return 1000;
}
int main()
{
    int ok = 1;
    char a[] = "Hello, ";
    char b[] = "world";
    LHHttpDocumentParts p1 = { a, 7, 0 };
    LHHttpDocumentParts p2 = { b, 5, 0 };
    LHHttpDocumentParts* parts[3];
    parts[0] = &p1; parts[1] = 0; parts[2] = &p2;
    LHHttp2 h;
    h.DocumentParts._M_start = parts; h.DocumentParts._M_finish = parts + 3; h.DocumentParts._M_end_of_storage = parts + 3;
    char out[32];
    memset(out, 0, sizeof(out));
    if (h.GetDocument(out) != 1000) ok = 0;
    if (strcmp(out, "Hello, world") != 0) ok = 0;
    printf(ok ? "0083eaa0_TEST PASS\n" : "0083eaa0_TEST FAIL\n");
    return ok ? 0 : 1;
}