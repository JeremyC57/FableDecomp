// Behaviour test for CBankFile::InternalClose (0x009CD850, catalogued as CDefStringTable::Destructor): every update
// entry is deleted (its info buffer freed first), the update/runtime arrays are detached and freed, the bank file
// counted pointer drops its reference (destroying the file on the last one), the trees are emptied only when they
// hold nodes, the CRC map is emptied and the size zeroed.
#include <cstdio>
#include <cstdlib>

static int g_entryDeletes = 0, g_infoDeletes = 0, g_frees = 0, g_fileDestroys = 0, g_treeErases = 0;

struct Entry {
    unsigned long InfoSize; char* InfoBuffer;
    ~Entry() { if (InfoBuffer) { ++g_infoDeletes; delete[] InfoBuffer; InfoBuffer = 0; } InfoSize = 0; ++g_entryDeletes; }
};
template <class T> struct Array {
    T* first; T* last; T* end_of_storage;
    void Deallocate()
    {
        if (end_of_storage - first > 0) {
            last = first;
            T* p = first; first = 0; last = 0; end_of_storage = 0;
            if (p) { ++g_frees; std::free(p); }
        }
    }
};
struct Info { long Count; void (*Destroy)(void*); void* Object; };
static void DestroyFile(void*) { ++g_fileDestroys; }
struct FilePtr {
    void* Data; Info* Info_;
    void Reset()
    {
        if (Info_) { --Info_->Count; if (Info_->Count == 0) { Info_->Destroy(Info_->Object); delete Info_; } }
        Info_ = 0; Data = 0;
    }
};
struct Tree {
    unsigned long Count;
    void Clear() { if (Count) { ++g_treeErases; Count = 0; } }
};
struct Bank {
    unsigned long Size;
    Array<Entry*> UpdateData;
    Array<int> RuntimeData;
    FilePtr BankFile;
    Tree SymbolIndices, OldSymbolIndices, FilenameIndices;
    int CRCCount;
    void InternalClose()
    {
        for (Entry** it = UpdateData.first; it != UpdateData.last; ++it) {
            Entry* entry = *it;
            if (entry) delete entry;
        }
        UpdateData.Deallocate();
        RuntimeData.Deallocate();
        BankFile.Reset();
        SymbolIndices.Clear();
        CRCCount = 0;
        OldSymbolIndices.Clear();
        FilenameIndices.Clear();
        Size = 0;
    }
};

int main()
{
    Bank b;
    b.Size = 100;
    Entry** e = (Entry**)std::malloc(4 * sizeof(Entry*));
    e[0] = new Entry; e[0]->InfoSize = 3; e[0]->InfoBuffer = new char[3];
    e[1] = 0;
    e[2] = new Entry; e[2]->InfoSize = 0; e[2]->InfoBuffer = 0;
    b.UpdateData.first = e; b.UpdateData.last = e + 3; b.UpdateData.end_of_storage = e + 4;
    b.RuntimeData.first = b.RuntimeData.last = b.RuntimeData.end_of_storage = 0;   // no capacity: nothing freed
    Info* info = new Info; info->Count = 1; info->Destroy = DestroyFile; info->Object = 0;
    b.BankFile.Data = (void*)&b; b.BankFile.Info_ = info;
    b.SymbolIndices.Count = 2; b.OldSymbolIndices.Count = 0; b.FilenameIndices.Count = 5; b.CRCCount = 7;
    b.InternalClose();
    if (g_entryDeletes != 2 || g_infoDeletes != 1 || g_frees != 1 || b.UpdateData.first != 0 || b.UpdateData.last != 0
        || g_fileDestroys != 1 || b.BankFile.Info_ != 0 || b.BankFile.Data != 0 || g_treeErases != 2
        || b.SymbolIndices.Count != 0 || b.FilenameIndices.Count != 0 || b.CRCCount != 0 || b.Size != 0) {
        std::printf("009cd850_TEST FAIL\n");
        return 1;
    }
    Info* shared = new Info; shared->Count = 2; shared->Destroy = DestroyFile; shared->Object = 0;
    b.BankFile.Info_ = shared;
    b.InternalClose();
    if (g_fileDestroys != 1 || shared->Count != 1 || g_frees != 1) { std::printf("009cd850_TEST FAIL\n"); return 1; }
    delete shared;
    std::printf("009cd850_TEST PASS\n");
    return 0;
}
