// behaviour test for 0x009BAA30
#include <stdio.h>
#include <string.h>
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CCharString;
class CMemoryDataInputStream;
class CDataOutputStream;
class CPersistContext {
public:
    enum EMode { MODE_0, MODE_1, MODE_2 };
    CDefinitionManager*     PDefinitionManager;          // +0x00
    CSymbolMap*             PSymbolMap;                  // +0x04
    CStringParser*          PParser;                     // +0x08
    void (*ErrorCallback)(const CCharString&);           // +0x0c
    CCharString*            PStringToClean;              // +0x10
    bool                    UseDefaults;                 // +0x14
    bool                    UseCRCCheck;                 // +0x15
    bool                    SingleTransfer;              // +0x16
    bool                    Finished;                    // +0x17
    EMode                   Mode;                        // +0x18
    bool                    Binary;                      // +0x1c
    CCharString*            PStringToSaveTo;             // +0x20
    CMemoryDataInputStream* PLoadStream;                 // +0x24
    CDataOutputStream*      PSaveStream;                 // +0x28
    void Init(CMemoryDataInputStream& stream, bool);
};
void CPersistContext::Init(CMemoryDataInputStream& stream, bool)
{
    PLoadStream = &stream;
    Mode = MODE_2;
    PSaveStream = 0;
    PDefinitionManager = 0;
    UseCRCCheck = false;
}

int main()
{
    CPersistContext c; memset(&c, 0x5a, sizeof(c));
    CMemoryDataInputStream* stream = (CMemoryDataInputStream*)&c;
    c.Init(*stream, true);
    int ok = c.PLoadStream == stream && c.Mode == CPersistContext::MODE_2 && c.PSaveStream == 0
          && c.PDefinitionManager == 0 && c.UseCRCCheck == false
          && *(unsigned char*)&c.UseDefaults == 0x5a && *(unsigned char*)&c.Binary == 0x5a;
    printf(ok ? "009baa30_TEST PASS\n" : "009baa30_TEST FAIL\n");
    return ok ? 0 : 1;
}