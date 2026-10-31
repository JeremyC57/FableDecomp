// CPersistContext::CleanOutLine(long start, long end) -- blank characters [start, end) of the string being cleaned with spaces, keeping newlines
// Leaf: ?CleanOutLine@CPersistContext@@IAEXJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BB0D0, length 50 bytes (+ int3 padding); `ret 8`.
// Callers: ten (CPersistContext::Transfer* text-load rows, e.g. 0x00463543, 0x009BB419, 0x009BC3C8).
// Name: `?CleanOutLine@CPersistContext@@IAEXJJ@Z` -- Ego_r.pdb name set (protected, two longs; the only (long, long)
//   member of CPersistContext) + the catalog name + the PDB member PStringToClean (+0x10) the body reads. The landed
//   callers bind the address as `?CleanText@CPersistContext@@QAEXJJ@Z` (NAME_FIXES.tsv).
// Shape: the PStringToClean (+0x10) null test returns at once; the inline CCharString text accessor (PDB PeekData)
//   loads the empty literal into ecx BEFORE testing the data block and overwrites it with the block's text (preset +
//   if-assign; the ternary spelling of the accessor keeps [ecx+eax] even with the inline routine below);
//   the top-tested loop compares text[i] with '\n' and stores ' ' otherwise.
// Lever (chunk 604): retail addresses the character as [eax+ecx] (index register as base). Every spelling with the text
//   pointer as a local of CleanOutLine (direct text[i], i+text, i[text], integer address arithmetic, unsigned index,
//   char&, continue, parameter as index, unused-local sweep, /Oa, /G5-/G7, optimize pragmas) gives [ecx+eax]; passing the
//   text pointer into an inline blanking routine (an inlinee parameter) gives retail's operand order (RELOCATION_MATCH).
// Global: the empty text 0x0129AAF4 is the merged "" literal (no invented global name).
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes, class key V); no engine header exists, so a local model is used.
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CMemoryDataInputStream;
class CDataOutputStream;

class CCharStringData { public:
    char* PData;                                          // +0x00
};
class CCharString {
public:
    CCharStringData* PStringData;                         // +0x00
    const char* PeekData() const
    {
        const char* text = "";
        if (PStringData)
            text = PStringData->PData;
        return text;
    }
};

class CPersistContext {
public:
    CDefinitionManager*     PDefinitionManager;           // +0x00
    CSymbolMap*             PSymbolMap;                   // +0x04
    CStringParser*          PParser;                      // +0x08
    void (__fastcall*       ErrorCallback)(const CCharString&); // +0x0c
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
protected:
    void CleanOutLine(long start, long end);
};

// Blank text[start..end) with spaces, keeping newlines (descriptive inline; fully inlined, no symbol).
inline void BlankLineRange(char* text, long start, long end)
{
    for (long i = start; i < end; ++i)
        if (text[i] != '\n')
            text[i] = ' ';
}

void CPersistContext::CleanOutLine(long start, long end)
{
    if (PStringToClean)
        BlankLineRange((char*)PStringToClean->PeekData(), start, end);
}