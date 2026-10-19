#include <stdio.h>
#ifndef _MSC_VER
#define FABLE_ENGINE_CBANKFILE_H
#endif
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

static CBankFileClearEntriesView* active;
static int bad,assignCalls,eraseCalls;
static CBankSymbolTree* erased[2];
static CBankSymbolTree::Node* roots[2];
CBankSymbolTree& CBankSymbolTree::operator=(const CBankSymbolTree& source) {
    ++assignCalls;
    if (this!=&active->OldSymbolIndices || &source!=&active->SymbolIndices || eraseCalls) bad=1;
    for (unsigned long i=0;i<active->UpdateData.size();++i) {
        CBankUpdateEntryView* e=active->UpdateData[i];
        if (e && (e->Exists || e->RequiresUpdate)) bad=1;
    }
    Count=source.Count;
    // The dependency clones the source root; preserve an independent sentinel here.
    Head->Parent=source.Head->Parent;
    return *this;
}
void CBankSymbolTree::Erase(Node* root) {
    if (eraseCalls>=2 || assignCalls!=1) {bad=1;return;}
    erased[eraseCalls]=this;roots[eraseCalls]=root;++eraseCalls;
    if (root!=Head->Parent) bad=1;
    if (this==&active->FilenameIndices && active->SymbolCRCIndices.Finish!=active->SymbolCRCIndices.Start) bad=1;
}
static void setup(CBankSymbolTree& t,CBankSymbolTree::Node& head,CBankSymbolTree::Node& root,unsigned long count) {
    t.Head=&head;t.Count=count;t.Comparator=19;
    head.Parent=count ? &root : 0;head.Left=count ? &root : &head;head.Right=count ? &root : &head;
    root.Parent=&head;root.Left=root.Right=0;
}
static int run(unsigned long symbolCount,unsigned long filenameCount,int noEntries) {
    CBankFileClearEntriesView bank={};active=&bank;
    CBankSymbolTree::Node symbolHead={},oldHead={},fileHead={},symbolRoot={},oldRoot={},fileRoot={};
    setup(bank.SymbolIndices,symbolHead,symbolRoot,symbolCount);
    setup(bank.OldSymbolIndices,oldHead,oldRoot,77);
    setup(bank.FilenameIndices,fileHead,fileRoot,filenameCount);
    CBankUpdateEntryView a={},b={};
    a.StateBlockCRC=31;a.InfoSize=13;a.Exists=true;a.RequiresUpdate=true;
    b.StateBlockCRC=41;b.InfoSize=29;b.Exists=true;b.RequiresUpdate=false;
    CBankUpdateEntryView* entries[4]={&a,0,&b,&a};
    bank.UpdateData.Start=entries;bank.UpdateData.Finish=entries+(noEntries ? 0 : 4);bank.UpdateData.End=entries+4;
    CBankSymbolPair pairs[3]={{1,11},{2,22},{3,33}};
    bank.SymbolCRCIndices.Start=pairs;bank.SymbolCRCIndices.Finish=pairs+(noEntries ? 0 : 3);bank.SymbolCRCIndices.End=pairs+3;
    bank.SymbolCRCIndices.Comparator=37;bank.SymbolCRCIndices.Dirty=true;
    bank.Size=123;bank.OpenFlags=321;bank.ChangeCount=73;bank.WarningFlag=true;
    bad=assignCalls=eraseCalls=0;
    bank.ClearEntries();
    if (bad || assignCalls!=1 || eraseCalls!=(int)((symbolCount!=0)+(filenameCount!=0))) return 0;
    int n=0;
    if (symbolCount && (erased[n]!=&bank.SymbolIndices || roots[n++]!=&symbolRoot)) return 0;
    if (filenameCount && (erased[n]!=&bank.FilenameIndices || roots[n++]!=&fileRoot)) return 0;
    if (!noEntries && (a.Exists || a.RequiresUpdate || b.Exists || b.RequiresUpdate)) return 0;
    if (noEntries && (!a.Exists || !a.RequiresUpdate || !b.Exists || b.RequiresUpdate)) return 0;
    if (a.StateBlockCRC!=31 || a.InfoSize!=13 || b.StateBlockCRC!=41 || b.InfoSize!=29) return 0;
    if (bank.SymbolIndices.Count || bank.FilenameIndices.Count || bank.OldSymbolIndices.Count!=symbolCount ||
        oldHead.Parent!=(symbolCount ? &symbolRoot : 0)) return 0;
    if (symbolHead.Parent || symbolHead.Left!=&symbolHead || symbolHead.Right!=&symbolHead ||
        fileHead.Parent || fileHead.Left!=&fileHead || fileHead.Right!=&fileHead) return 0;
    if (bank.SymbolCRCIndices.Finish!=pairs || bank.SymbolCRCIndices.Start!=pairs || bank.SymbolCRCIndices.End!=pairs+3 ||
        bank.SymbolCRCIndices.Comparator!=37 || !bank.SymbolCRCIndices.Dirty || pairs[0].Key!=1 || pairs[0].Value!=11 ||
        pairs[1].Key!=2 || pairs[1].Value!=22 || pairs[2].Key!=3 || pairs[2].Value!=33) return 0;
    return bank.Size==123 && bank.OpenFlags==321 && bank.ChangeCount==73 && bank.WarningFlag &&
        bank.SymbolIndices.Comparator==19 && bank.OldSymbolIndices.Comparator==19 && bank.FilenameIndices.Comparator==19 &&
        bank.UpdateData.Start==entries && bank.UpdateData.Finish==entries+(noEntries ? 0 : 4) && bank.UpdateData.End==entries+4;
}
int main() {
    if (!run(4,3,0) || !run(0,0,1) || !run(4,0,0) || !run(0,3,0)) {
        printf("009ccc00_TEST FAIL\n");return 1;
    }
    printf("009ccc00_TEST PASS\n");return 0;
}