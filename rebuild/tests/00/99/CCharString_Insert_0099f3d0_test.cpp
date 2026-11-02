// test: CCharString::Insert(long, char) -- un-share the text block (copy-on-write), then insert the one-character string at the position
#include <stdio.h>
#include <string.h>
class CCharStringData { public: char Data[64]; long NoRefs; };
class CCharString { public:
    CCharStringData* PStringData;
    void MakeOwnCopyOfDataOrEmptyString() {
        if (!PStringData) { PStringData = new CCharStringData; PStringData->Data[0] = 0; PStringData->NoRefs = 1; }
        else if (PStringData->NoRefs > 1) { CCharStringData* d = new CCharStringData; strcpy(d->Data, PStringData->Data); d->NoRefs = 1; --PStringData->NoRefs; PStringData = d; }
    }
    static void InsertText(char* data, unsigned long position, const char* text) {
        size_t n = strlen(text); memmove(data + position + n, data + position, strlen(data + position) + 1); memcpy(data + position, text, n);
    }
    void Insert(long position, char c) {
        MakeOwnCopyOfDataOrEmptyString();
        { char text[2]; text[0] = c; text[1] = 0; InsertText(PStringData->Data, position, text); }
    }
};
int main() {
    CCharStringData shared; strcpy(shared.Data, "Fble"); shared.NoRefs = 2;
    CCharString s = { &shared };
    s.Insert(1, 'a');
    if (strcmp(s.PStringData->Data, "Fable") != 0 || strcmp(shared.Data, "Fble") != 0 || shared.NoRefs != 1) { printf("0099f3d0_TEST FAIL\n"); return 1; }
    CCharString e = { 0 };
    e.Insert(0, 'X');
    if (strcmp(e.PStringData->Data, "X") != 0) { printf("0099f3d0_TEST FAIL\n"); return 1; }
    delete s.PStringData; delete e.PStringData;
    printf("0099f3d0_TEST PASS\n");
    return 0;
}