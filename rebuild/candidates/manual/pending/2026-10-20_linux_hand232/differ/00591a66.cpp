// Grade: DIFFER(281v281) -- one instruction: the IsSaving test is `cmp al,bl` (against the shared zero register) where retail has `test al,al`; everything else matches. With the condition written inline (probe p26) the test is right but the vector's zero stores move above the IsSaving call. Next: an `unsigned char`/const bool flag, or a block-scoped vector.
// CTCInventory::OnSerialise(CPersistContext* context, bool inGameSave) -- serialises the base inventory, then the quick-access slots as def NAMES: saving converts each set slot's def index to its name, loading maps the names back to indices
// Leaf: ?OnSerialise@CTCInventory@
// Retail 0x00591A66, 281 bytes, `ret 8`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CTCInventoryBase::OnSerialise(CPersistContext*, bool) 0x005BFD5E, CPersistContext::IsSaving 0x004AEF50 (manifest
// "IsBinary"), vector<CCharString>::reserve(unsigned long) 0x004BA7A0, CTCBase::GetDefinitionManager 0x006869D0,
// CDefinitionManager::GetDefNameFromGlobalIndex(long) 0x009ACCC0 (CDefString by hidden pointer), the CDefString ->
// CCharString conversion 0x00415DD0 (a twin of the landed operator CCharString() 0x00415D70), vector<CCharString>::
// push_back 0x0044BFF0, ~CCharString 0x0099EAE0, CPersistContext::Transfer(const char*, vector<CCharString>*) 0x0049B8D0,
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) 0x009AD410, ~vector<CCharString> 0x00414EA0.
// Shape: the name vector is a zeroed local built after the IsSaving answer is taken into
// a bool (the end-of-storage alloc proxy first, then finish/start zeroed in the ctor body: retail's store order); saving reserves the slot count, pushes the name of every positive slot (the
// converted CCharString temporary homed in the dead `inGameSave` slot, the CDefString result in a frame word) and
// transfers; loading transfers first and writes each looked-up index into the slot (the slot address is formed before
// the lookup call).
// Layout (retail): QuickAccessItems vector<long> +0x148.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefString {
    void* m_pData;
    CDefString(const CDefString& other);
    operator CCharString() const;                      // 0x00415DD0
};
struct SAllocator { SAllocator() {} };
template <class T> struct _STLP_alloc_proxy {          // the end-of-storage pointer over an empty allocator
    T _M_data;
    _STLP_alloc_proxy(const SAllocator&, T p) : _M_data(p) {}
};
struct CCharStringVector {                             // vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    _STLP_alloc_proxy<CCharString*> _M_end_of_storage;
    CCharStringVector(const SAllocator& a = SAllocator()) : _M_end_of_storage(a, 0) { _M_finish = 0; _M_start = 0; }
    ~CCharStringVector();                              // 0x00414EA0
    CCharString* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CCharString& operator[](unsigned long n) { return *(begin() + n); }
    void reserve(unsigned long count);                 // 0x004BA7A0
    void push_back(const CCharString& value);          // 0x0044BFF0
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    long& operator[](unsigned long n) { return *(begin() + n); }
};
struct CPersistContext {
    bool IsSaving();                                   // 0x004AEF50
    void Transfer(const char* name, CCharStringVector* value);   // 0x0049B8D0
};
struct CDefinitionManager {
    CDefString GetDefNameFromGlobalIndex(long index);  // 0x009ACCC0
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CTCInventoryBase_Retail {
    void OnSerialise(CPersistContext* context, bool inGameSave);   // 0x005BFD5E
    CDefinitionManager* GetDefinitionManager() const;  // 0x006869D0 (CTCBase)
};
struct CTCInventory : CTCInventoryBase_Retail {
    unsigned char _pad_0x000[0x148];
    CLongVector QuickAccessItems;                      // +0x148
    void OnSerialise(CPersistContext* context, bool inGameSave);
};

void CTCInventory::OnSerialise(CPersistContext* context, bool inGameSave)
{
    CTCInventoryBase_Retail::OnSerialise(context, inGameSave);
    bool saving = context->IsSaving();
    CCharStringVector names;
    if (saving) {
        names.reserve(QuickAccessItems.size());
        for (unsigned long i = 0; i < QuickAccessItems.size(); ++i)
            if (QuickAccessItems[i] > 0)
                names.push_back(GetDefinitionManager()->GetDefNameFromGlobalIndex(QuickAccessItems[i]));
        context->Transfer("QuickAccessItemNames", &names);
    } else {
        context->Transfer("QuickAccessItemNames", &names);
        for (unsigned long i = 0; i < names.size(); ++i)
            QuickAccessItems[i] = GetDefinitionManager()->GetDefGlobalIndexFromName(names[i]);
    }
}
