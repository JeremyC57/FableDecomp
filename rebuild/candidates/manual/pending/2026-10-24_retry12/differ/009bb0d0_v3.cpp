// CPersistContext::CleanOutLine(long start, long end) -- blank characters [start, end) of the string being cleaned with spaces, keeping newlines
// Grade: DIFFER(ours v retail) -- 50v50; only the SIB operand order differs: retail `cmp byte ptr [eax+ecx],0Ah` / `mov byte ptr [eax+ecx],20h` (index register as base), the lane `[ecx+eax]`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BB0D0, length 50 bytes (+ int3 padding).
// Callees: none. Globals: the empty CCharString text 0x0129AAF4.
// Shape: the PStringToClean (+0x10) null test returns at once; the inline CCharString text accessor loads the empty
// string pointer into ecx BEFORE testing the data block and overwrites it with `data->text` (the ternary); the top-tested
// `for` loop compares `text[i]` with '\n' and stores ' ' otherwise (`[eax+ecx]` addressing, eax = index); `ret 8`.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
struct CDefinitionManager;
struct CSymbolMap;
struct CStringParser;
struct CCharStringData {
    char* text;                                           // +0x00
};
extern char g_EmptyCharString[];                          // 0x0129AAF4 (the shared empty narrow string)
struct CCharString {
    CCharStringData* PStringData;                         // +0x00
    char* GetText() const
    {
        char* text = g_EmptyCharString;
        if (PStringData)
            text = PStringData->text;
        return text;
    }
};
struct CMemoryDataInputStream;
struct CDataOutputStream;

// CPersistContext (0x2c bytes, PDB layout Ego_r.pdb; no engine header, HAND_OWNED in engine/INDEX.tsv)
struct CPersistContext {
    CDefinitionManager*     PDefinitionManager;           // +0x00
    CSymbolMap*             PSymbolMap;                   // +0x04
    CStringParser*          PParser;                      // +0x08
    void (__cdecl*          ErrorCallback)(CCharString*); // +0x0c
    CCharString*            PStringToClean;               // +0x10
    bool                    UseDefaults;                  // +0x14
    bool                    UseCRCCheck;                  // +0x15
    bool                    SingleTransfer;               // +0x16
    bool                    Finished;                     // +0x17
    long                    Mode;                         // +0x18 EMode: 0 text load, 1 text save, 2 binary load, 3 binary save
    bool                    Binary;                       // +0x1c
    CCharString*            PStringToSaveTo;              // +0x20
    CMemoryDataInputStream* PLoadStream;                  // +0x24
    CDataOutputStream*      PSaveStream;                  // +0x28

    void CleanOutLine(long start, long end);
};

void CPersistContext::CleanOutLine(long start, long end)
{
    if (PStringToClean) {
        char* text = PStringToClean->GetText();
        for (long i = start; i < end; ++i) {
            if (text[i] == '\n')
                continue;
            text[i] = ' ';
        }
    }
}
