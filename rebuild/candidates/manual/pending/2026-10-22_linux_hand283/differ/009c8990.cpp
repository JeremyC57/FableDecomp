// NGameText::CDataBank::GetAllTextEntriesForNarrator(const CCharString& narrator, std::set<unsigned long>& entries,
// Grade: DIFFER(604v604) -- frame 0x28 vs retail 0x24: the first insert's pair temp gets its own word above the second loop's speaker/value word (retail overlaps them); all code otherwise identical
//   unsigned long minIndex) const -- collect (from index minIndex up) every text entry spoken by the narrator (or, for
//   an empty narrator name, every entry with no narrator), walking plain text entries and the members of each group
// Leaf: GetAllTextEntriesForNarrator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8990, length 602 bytes, `ret 0Ch`. Mangled name (PDB):
//   ?GetAllTextEntriesForNarrator@CDataBank@NGameText@@QBEXABVCCharString@@AAV?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@K@Z
// Callees: _Rb_tree::_M_erase 0x004CF810 (inline clear()), ConvertNarratorNameToIndex 0x009C7EF0,
//   CBasicString<char>::operator==(const char*) 0x004115A0, GetEntryDataType 0x009CC210, GetPGroupEntry 0x009C8890,
//   set<unsigned long>::insert 0x00514C10 (pair by hidden pointer).
// Layout: CDataBank Entries (first/last/end) at retail +0x1e0 (the landed GetPGroupEntry 0x009C8890); a text entry's
//   narrator index at +4; CGroupEntry (CResource 0x1c bytes) Entries at +0x1c; STLport set: header node pointer +0,
//   node count +4; header node parent +4 / left +8 / right +0xc.
// Shape: the inline CCharString == "" with the strcmp null-block arm (landed 0x008EDA90); the group's counted pointer is a
//   full-expression temporary whose ReleaseTemporary dtor nulls it; the set takes converted (long -> unsigned long)
//   temporaries by reference.
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CULongSetNode {
    int            Color;
    CULongSetNode* Parent;                      // +0x04
    CULongSetNode* Left;                        // +0x08
    CULongSetNode* Right;                       // +0x0c
    unsigned long  Value;
};
struct CULongSet {                              // std::set<unsigned long> (STLport)
    CULongSetNode* Header;                      // +0x00
    unsigned long  NodeCount;                   // +0x04
    struct CInsertResult { CULongSetNode* It; bool Inserted; };
    void EraseSubtree(CULongSetNode* node);     // 0x004CF810
    CInsertResult insert(const unsigned long& value);   // 0x00514C10
    void Add(unsigned long value) { insert(value); }
    void clear()
    {
        if (NodeCount != 0) {
            EraseSubtree(Header->Parent);
            Header->Left = Header;
            Header->Parent = 0;
            Header->Right = Header;
            NodeCount = 0;
        }
    }
};
template<class T> struct CArrayNative {
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    T& operator[](unsigned long n) const { return First[n]; }
};
namespace NGameText {
struct CResourceBase {
    virtual void v00();
    virtual void Delete() const;                // slot 1
    unsigned long IVRefCount;                   // +0x04
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Delete(); }
};
struct CGroupEntry : CResourceBase {
    unsigned char      _pad08[0x1c - 0x08];
    CArrayNative<long> Entries;                 // +0x1c
};
template<class T> struct CIVCountedPointer {
    T* Object;
    ~CIVCountedPointer() { if (Object) { const_cast<CGroupEntry*>(Object)->ReleaseTemporary(); Object = 0; } }
};
struct CTextEntry {
    void* _vtbl;
    long  NarratorIndex;                        // +0x04
};
struct CDataBank {
    unsigned char             _pad000[0x1e0];
    CArrayNative<CTextEntry*> Entries;          // +0x1e0
    unsigned long GetEntryDataType(unsigned long index) const;      // 0x009CC210
    long ConvertNarratorNameToIndex(const CCharString& name) const; // 0x009C7EF0
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long index, bool allowPending) const;   // 0x009C8890
    CTextEntry* GetPTextEntry(unsigned long index) const
    {
        if (GetEntryDataType(index) == 0)
            return Entries[index];
        return 0;
    }
    void GetAllTextEntriesForNarrator(const CCharString& narrator, CULongSet& entries, unsigned long minIndex) const;
};

void CDataBank::GetAllTextEntriesForNarrator(const CCharString& narrator, CULongSet& entries, unsigned long minIndex) const
{
    entries.clear();
    long narratorIndex = ConvertNarratorNameToIndex(narrator);
    if (narratorIndex == -1 && !(narrator == ""))
        return;
    long count = Entries.Size();
    for (long i = 1; i < count; ++i) {
        if (GetEntryDataType(i) == 0) {
            CTextEntry* entry = GetPTextEntry(i);
            if (entry) {
                long speaker = entry->NarratorIndex;
                if ((speaker == narratorIndex || (narrator == "" && speaker == -1)) && (unsigned long)i >= minIndex)
                    entries.insert(i);
            }
        } else if (GetEntryDataType(i) == 1) {
            const CArrayNative<long>& members = GetPGroupEntry(i, true).Object->Entries;
            for (unsigned long j = 0; j < members.Size(); ++j) {
                CTextEntry* entry = GetPTextEntry(members[j]);
                if (entry) {
                    long speaker = entry->NarratorIndex;
                    if ((speaker == narratorIndex || (narrator == "" && speaker == -1))
                        && (unsigned long)members[j] >= minIndex)
                        entries.Add(members[j]);
                }
            }
        }
    }
}
}
