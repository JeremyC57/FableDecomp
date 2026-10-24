// Behaviour test for 0x0083EA10 LHNetworkLib::LHHttpDocumentParts::~LHHttpDocumentParts
#include <stdio.h>
namespace LHNetworkLib {
struct LHHttpDocumentParts {
    char*         Buffer;
    unsigned long BufferLength;
    unsigned long CurrBufPos;
    ~LHHttpDocumentParts();
};
LHHttpDocumentParts::~LHHttpDocumentParts()
{
    if (Buffer) {
        delete[] Buffer;
        Buffer = 0;
    }
    BufferLength = 0;
}
}
int main()
{
    using LHNetworkLib::LHHttpDocumentParts;
    int ok = 1;
    LHHttpDocumentParts* a = new LHHttpDocumentParts;
    a->Buffer = new char[16]; a->BufferLength = 16; a->CurrBufPos = 7;
    a->~LHHttpDocumentParts();
    if (a->Buffer != 0 || a->BufferLength != 0 || a->CurrBufPos != 7) ok = 0;
    a->Buffer = 0; a->BufferLength = 5;
    a->~LHHttpDocumentParts();
    if (a->Buffer != 0 || a->BufferLength != 0) ok = 0;
    operator delete(a);
    printf(ok ? "0083ea10_TEST PASS\n" : "0083ea10_TEST FAIL\n");
    return ok ? 0 : 1;
}