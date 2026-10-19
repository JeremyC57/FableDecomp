// CBankFile::ClearEntries() -- invalidate entry state, snapshot symbol indices and clear current lookup tables
// Retail009CCC00,205 bytes to ret009CCCCC, followed by3 int3 bytes. Propagated CloseShaders
// label contradicts bank+2c UpdateData and the PDB ClearEntries workflow. Shared CBankFile
// donor layout has16-byte containers; use the established retail12-byte container layout.
// Tree assignment009D3420 and recursive tree Erase00579435.
#include "engine/CBankFile.h"
struct CBankUpdateEntryView {
    unsigned long StateBlockCRC, InfoSize;
    char* InfoBuffer;
    unsigned char Filenames[8];
    bool RequiresUpdate, Exists;
};
template<class T> struct CBankArray {
    T* Start; T* Finish; T* End;
    unsigned long size() const { return Finish-Start; }
    T& operator[](unsigned long i) { return *(Start+i); }
};
struct CBankSymbolTree {
    struct Node { unsigned char Color; unsigned char Pad[3]; Node* Parent; Node* Left; Node* Right; };
    Node* Head; unsigned long Count; unsigned char Comparator;
    CBankSymbolTree& operator=(const CBankSymbolTree&); // 009D3420
    void Erase(Node*); //00579435
    void Clear() {
        if (Count) {
            Erase(Head->Parent);
            Head->Left=Head; Head->Parent=0; Head->Right=Head; Count=0;
        }
    }
};
struct CBankSymbolPair { unsigned long Key, Value; };
struct CBankSymbolVector : CBankArray<CBankSymbolPair> {
    unsigned char Comparator; bool Dirty;
    static CBankSymbolPair* Copy(CBankSymbolPair* first, CBankSymbolPair* last, CBankSymbolPair* result) {
        for (long n=last-first;n>0;--n,++first,++result) *result=*first;
        return result;
    }
    void Clear() { Finish=Copy(Finish,Finish,Start); }
};
struct CDiskFileWin32Calls;
struct CProgressCallbackCalls;
struct CBankFileClearEntriesView {
    void*                   __vftable;                 // +0x00
    unsigned long           Size;                      // +0x04
    unsigned char           Symbols[0x0C];             // +0x08 CArray<CCharString>        (retail 12 bytes)
    unsigned char           Checksums[0x0C];           // +0x14 CArray<unsigned long>      (retail 12 bytes)
    unsigned char           RuntimeData[0x0C];         // +0x20 CArray<CRuntimeData>       (retail 12 bytes)
    CBankArray<CBankUpdateEntryView*> UpdateData;          // +0x2C CArray<CBankFileEntryUpdateData*> (retail 12 bytes)
    unsigned char           PackedDataOffset[0x10];    // +0x38 CPackedUIntArray   (PDB +0x48)
    unsigned char           PackedDataSize[0x10];      // +0x48 CPackedUIntArray   (PDB +0x58)
    unsigned char           PackedDataType[0x10];      // +0x58 CPackedUIntArray   (PDB +0x68)
    unsigned char           PackedValid[0x10];         // +0x68 CPackedUIntArray   (PDB +0x78)
    unsigned long           OpenFlags;                 // +0x78                    (PDB +0x88)
    CDiskFileWin32Calls*    BankFile_Data;             // +0x7C CCountedPointer<CDiskFileWin32>::Data (PDB +0x8C)
    void*                   BankFile_Info;             // +0x80
    void*                   Filename_PStringData;      // +0x84 CWideString
    void*                   BankHandle_PStringData;    // +0x88 CCharString
    bool                    RetailMode;                // +0x8C
    bool                    ReadOnly;                  // +0x8D
    bool                    WrittenToFile;             // +0x8E
    bool                    Solid;                     // +0x8F
    unsigned long           ChangeCount;               // +0x90
    bool                    FileValid;                 // +0x94
    bool                    UpdatingFlag;              // +0x95
    unsigned char           _pad_0x96[2];
    unsigned long           Alignment;                 // +0x98                    (PDB +0xA8)
    unsigned long           BankFormatVersion;         // +0x9C
    unsigned long           HeaderSize;                // +0xA0                    (PDB +0xB0)
    unsigned long           FileInformationSize;       // +0xA4                    (PDB +0xB4)
    unsigned long           FileSize;                  // +0xA8                    (PDB +0xB8)
    CBankSymbolTree SymbolIndices;       // +0xAC map<CCharString,ulong>      (retail 12 bytes)
    CBankSymbolTree OldSymbolIndices;    // +0xB8 map<CCharString,ulong>      (retail 12 bytes)
    CBankSymbolTree FilenameIndices;     // +0xC4 multimap<CCharString,ulong> (retail 12 bytes)
    CBankSymbolVector SymbolCRCIndices;    // +0xD0 CVectorMap<ulong,ulong>     (retail 16 bytes)
    unsigned char           BlockOrder[0x0C];          // +0xE0 CArray<unsigned long>       (retail 12 bytes)
    CProgressCallbackCalls* ProgressCallback;          // +0xEC                    (PDB +0x110)
    void*                   HeaderEnumerationType_PStringData; // +0xF0 CCharString
    bool                    WarningFlag;               // +0xF4                    (PDB +0x118)
    void ClearEntries();
};

void CBankFileClearEntriesView::ClearEntries() {
    for (unsigned long i=0;i<UpdateData.size();++i) {
        CBankUpdateEntryView* entry=UpdateData[i];
        if (entry) { entry->Exists=false; entry->RequiresUpdate=false; }
    }
    OldSymbolIndices=SymbolIndices;
    SymbolIndices.Clear();
    SymbolCRCIndices.Clear();
    FilenameIndices.Clear();
}