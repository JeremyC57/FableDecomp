// Grade: DIFFER(421v425) -- retail zeroes the CCharString temp slot [ebp-8] before IsBinary (with esi=0 set first and this in ebx); ours has no such store and picks edi=this, ebx=0 after the test. Source of the early [ebp-8]=0 unknown.
// NInventory::CTCInventoryBase::OnSerialise(CPersistContext& context, bool inGameSave) -- binary contexts persist the
// current non-empty item stacks, the names of the selected items and the confiscated items; other contexts re-create the
// cached item / selection vectors and the confiscated list empty and transfer into them
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BFD5E, 423 bytes, `ret 8` (frame pointer, `leave`).
// Callees: CPersistContext::IsBinary 0x004AEF50, CTCBase::GetWorld 0x00686A80 (+0x103 a bool), GetAllNonEmptyInventoryItems
// 0x005BFA44, CPersistContext::TransferVector (item vector) 0x005C2B8B, GetAllSelectedItems 0x005BEAF5, vector<CCharString>::
// reserve 0x004BA7A0 / push_back 0x0044BFF0 / dtor 0x00414EA0, CTCBase::GetDefinitions 0x006869D0, CDefinitionManager::
// GetDefNameFromGlobalIndex 0x009ACCC0, CDefString::operator CCharString 0x00415DD0, ~CCharString 0x0099EAE0,
// CPersistContext::Transfer(vector<CCharString>) 0x0049B8D0, ~vector<CItem> 0x005C1E30, free 0x00BFEA14, operator new
// 0x00BFEA1A, the two counted-pointer resets 0x005C29CE / 0x005C1E91, vector<CItem>::erase(first, last) 0x005C1ECC.
// Layout: retail CTCInventoryBase cached items +0x78 (CCountedPointer<vector<CItem> >), cached selection +0x80
// (CCountedPointer<vector<CCharString> >), ConfiscatedItems +0xAC (vector<CItem>).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};
struct CDefString {
    long Index;
    CCharString ToCharString() const;                       // 0x00415DD0 (operator CCharString)
};
struct CDefinitionManager {
    CDefString GetDefNameFromGlobalIndex(long index);       // 0x009ACCC0
};
namespace NInventory { struct CItem { long DefIndex; long Count; }; }
struct CItemVector {                                        // vector<NInventory::CItem>
    NInventory::CItem* _M_start;
    NInventory::CItem* _M_finish;
    NInventory::CItem* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CItemVector();                                         // 0x005C1E30
    NInventory::CItem* erase(NInventory::CItem* first, NInventory::CItem* last);   // 0x005C1ECC
    void clear() { erase(_M_start, _M_finish); }
};
struct CLongVector {                                        // vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CLongVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    long& operator[](unsigned long n) { return _M_start[n]; }
};
struct CCharStringVector {                                  // vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CCharStringVector();                                   // 0x00414EA0
    void reserve(unsigned long n);                          // 0x004BA7A0
    void push_back(const CCharString& value);               // 0x0044BFF0
};
template <class T> struct CCountedPointer { T* Object; long* RefCount; };
struct CCountedItems : CCountedPointer<CItemVector> { void Reset(CItemVector* p); };              // 0x005C29CE
struct CCountedNames : CCountedPointer<CCharStringVector> { void Reset(CCharStringVector* p); };  // 0x005C1E91

struct CPersistContext {
    bool IsBinary() const;                                  // 0x004AEF50
    void TransferVector(const char* name, CItemVector* value);   // 0x005C2B8B
    void Transfer(const char* name, CCharStringVector* value);   // 0x0049B8D0
};
struct CWorld { unsigned char _pad[0x103]; bool AllItemsVisible; };   // +0x103

namespace NInventory {
class CTCInventoryBase {
public:
    void*         __vftable;                                // +0x00
    void*         ParentThing;                              // +0x04
    unsigned char _pad08[0x78 - 0x08];
    CCountedItems CachedItems;                              // +0x78
    CCountedNames CachedSelectedItems;                      // +0x80
    unsigned char _pad88[0xac - 0x88];
    CItemVector   ConfiscatedItems;                         // +0xAC
    CWorld* GetWorld() const;                               // 0x00686A80 (CTCBase)
    CDefinitionManager* GetDefinitions() const;             // 0x006869D0 (CTCBase)
    void GetAllNonEmptyInventoryItems(CItemVector& items, bool includeAll);   // 0x005BFA44
    void GetAllSelectedItems(CLongVector& items) const;     // 0x005BEAF5
    void OnSerialise(CPersistContext& context, bool inGameSave);
};

void CTCInventoryBase::OnSerialise(CPersistContext& context, bool inGameSave)
{
    if (context.IsBinary()) {
        CItemVector items;
        GetAllNonEmptyInventoryItems(items, GetWorld()->AllItemsVisible);
        context.TransferVector("InventoryItems", &items);
        CLongVector selected;
        GetAllSelectedItems(selected);
        unsigned long count = selected.size();
        CCharStringVector names;
        names.reserve(count);
        for (unsigned long i = 0; i < count; ++i)
            if (selected[i] > 0)
                names.push_back(GetDefinitions()->GetDefNameFromGlobalIndex(selected[i]).ToCharString());
        context.Transfer("SelectedInventoryItems", &names);
        context.TransferVector("ConfiscatedItems2", &ConfiscatedItems);
    } else {
        CachedItems.Reset(new CItemVector);
        context.TransferVector("InventoryItems", CachedItems.Object);
        CachedSelectedItems.Reset(new CCharStringVector);
        context.Transfer("SelectedInventoryItems", CachedSelectedItems.Object);
        ConfiscatedItems.clear();
        context.TransferVector("ConfiscatedItems2", &ConfiscatedItems);
    }
}
}
