// NGameText::CDataBank::CDataBank() -- construct the text bank: async bank-file base, resource-bank base, empty entry/text/group/narrator arrays; cap the bank's resident resource memory at 64KB with no unload delay
// Leaf: ??0CDataBank@NGameText@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C85A0, 141 bytes (+3 int3 padding), plain `ret`.
// Name: FableWin.pdb ??0CDataBank@NGameText@@QAE@XZ.
// Bases (RTTI; same as the landed CGraphicDataBank 0x009FEBC0): CBankFileAsync at +0 (ctor 0x009D5F80), CResourceBank at
// +0x164 (ctor 0x009FC5F0, landed); vtables CDataBank 0x0129B4D4 (primary) and 0x0129B528 (CResourceBank-in-CDataBank).
// Members (Ego_r.pdb names, retail 12-byte CArrays): Entries +0x1E0, TextEntryBuffer +0x1EC, GroupEntryBuffer +0x1F8,
// Narrators +0x204 (each zeroed by the inline CArray ctor), NarratorMap +0x210 (only its first word is written, and in the
// ctor body: modelled as a trivially-constructed header word cleared there -- see NOTES).
// Body: CResourceBank::GetResourceList() (inline) .SetMaximumMemory(0x10000) @ 0x009FC520 (catalog label AddToInterface is a
// byte-identical fold; PDB ?SetMaximumMemory@CResourceList@@QAEXK@Z sets MaximumMemory +0x28 and tail-calls MakeFreeSpace),
// then .SetResourceUnloadDelay(0) inline (UnloadDelay = list +0x34 = this +0x19C).
// Shape: the arrays are zeroed by their inline member ctors but the map word in the body -- that split is what puts
// `xor edi,edi` between the two vtable stores (all-inline ctors: after both; all in the body: the stores sink below the push).
// edi first holds the CResourceBank subobject for its ctor and secondary vtable store, then the shared zero.
class CCharString;

class CBankFileAsync { public:                                 // 0x164 bytes
    CBankFileAsync();                                   // 0x009D5F80
    virtual ~CBankFileAsync();
    unsigned char BankFileState[0x160];
};

class CResourceList { public:                                  // 0x3C bytes (vptr, CResource Head, counters)
    virtual ~CResourceList();
    unsigned char  Head[0x1c];                          // +0x04
    long           ResourceCount;                       // +0x20
    long           AllocatedMemory;                     // +0x24
    long           MaximumMemory;                       // +0x28
    unsigned long  CurrentFrame;                        // +0x2c
    unsigned long  DebugStatsFrame;                     // +0x30
    unsigned long  UnloadDelay;                         // +0x34
    unsigned long  UnloadedThisFrame;                   // +0x38
    void SetMaximumMemory(unsigned long maximum);       // 0x009FC520
    void SetResourceUnloadDelay(unsigned long delay) { UnloadDelay = delay; }
};

class CResourceBank { public:                                  // 0x7C bytes
    CResourceBank();                                    // 0x009FC5F0
    virtual ~CResourceBank();
    CResourceList ResourceList;                         // +0x04
    CResourceList LoadingQueue;                         // +0x40
    CResourceList& GetResourceList() { return ResourceList; }
};

template<class T> class CArray { public:                       // retail CArray: 12 bytes
    T* First;
    T* Last;
    T* End;
    CArray() : First(0), Last(0), End(0) {}
};
struct CNarratorMap {                                   // retail map header word
    void* Header;
};

namespace NGameText {
class CDataBankEntry;
class CDataBankTextEntry;
class CDataBankGroupEntry;

class CDataBank : public CBankFileAsync, public CResourceBank {
public:
    CDataBank();
    virtual ~CDataBank();
    CArray<CDataBankEntry*>     Entries;                // +0x1e0
    CArray<CDataBankTextEntry>  TextEntryBuffer;        // +0x1ec
    CArray<CDataBankGroupEntry> GroupEntryBuffer;       // +0x1f8
    CArray<CCharString>         Narrators;              // +0x204
    CNarratorMap                NarratorMap;            // +0x210
};

CDataBank::CDataBank()
{
    NarratorMap.Header = 0;
    GetResourceList().SetMaximumMemory(0x10000);
    GetResourceList().SetResourceUnloadDelay(0);
}
} // namespace NGameText