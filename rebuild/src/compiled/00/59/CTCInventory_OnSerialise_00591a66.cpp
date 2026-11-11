// CTCInventory::OnSerialise(CPersistContext& context, bool inGameSave) -- serialises the base inventory, then the quick-access slots as def NAMES: saving converts each set slot's def index to its name, loading maps the names back to indices
// Leaf: ?OnSerialise@CTCInventory@
// Retail 0x00591A66, 281 bytes, `ret 8`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: NInventory::CTCInventoryBase::OnSerialise(CPersistContext&, bool) 0x005BFD5E, CPersistContext::IsSaving const 0x004AEF50 (manifest
// "IsBinary"), vector<CCharString>::reserve(unsigned long) 0x004BA7A0, CTCBase::GetDefinitionManager const 0x006869D0,
// CDefinitionManager::GetDefNameFromGlobalIndex(long) const 0x009ACCC0 (CDefString by hidden pointer), CDefString::GetString()
// const 0x00415DD0 (a byte twin of the landed operator CCharString() 0x00415D70; FableWin.pdb order), vector<CCharString>::
// push_back 0x0044BFF0, ~CCharString 0x0099EAE0, CPersistContext::TransferVector<CCharString>(const char*, vector<CCharString>&) 0x0049B8D0,
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) 0x009AD410, ~vector<CCharString> 0x00414EA0.
// Shape: `if (context->IsSaving()) { vector names; ... } else { vector names; ... }` -- ONE vector local per arm, with the
// STLport ctor zeroing start, finish, end_of_storage in init-list order. VC7.1 hoists the two identical ctor store runs
// above the `je` (in reverse order: end, finish, start) and tests IsSaving with `test al,al` because the compare is lowered
// before the hoisted zero register exists; a single vector declared before the test gives `cmp al,bl`. Saving reserves the slot count, pushes the name of every positive slot (the
// named CCharString (GetString's result) homed in the dead `inGameSave` slot, the CDefString result in a frame word) and
// transfers; loading transfers first and writes each looked-up index into the slot (the slot address is formed before
// the lookup call).
// Layout (retail): QuickAccessItems vector<long> +0x148.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString { public:
    void* m_pStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                    // 0x0099EAE0
};
class CDefString { public:
    void* m_pData;
    CDefString(const CDefString& other);
    CCharString GetString() const;                     // 0x00415DD0 ?GetString@CDefString@@QBE?AVCCharString@@XZ (FableWin.pdb; donor order operator CCharString / GetString = retail 0x00415D70 / 0x00415DD0)
};
struct SAllocator { SAllocator() {} };
template <class T> struct _STLP_alloc_proxy {          // the end-of-storage pointer over an empty allocator
    T _M_data;
    _STLP_alloc_proxy(const SAllocator&, T p) : _M_data(p) {}
};
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc> class vector;
template <> class vector<CCharString, allocator<CCharString> > { public:   // STLport vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    _STLP_alloc_proxy<CCharString*> _M_end_of_storage;
    vector(const SAllocator& a = SAllocator()) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
    ~vector();                              // 0x00414EA0
    CCharString* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CCharString& operator[](unsigned long n) { return *(begin() + n); }
    void reserve(unsigned int count);                  // 0x004BA7A0
    void push_back(const CCharString& value);          // 0x0044BFF0
};
}
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    long& operator[](unsigned long n) { return *(begin() + n); }
};
class CPersistContext { public:
    bool IsSaving() const;                             // 0x004AEF50 (Ego_r.pdb QBE)
    template <class T> void TransferVector(const char* name, std::vector<T, std::allocator<T> >& value);   // 0x0049B8D0 <CCharString>
};
class CDefinitionManager { public:
    CDefString GetDefNameFromGlobalIndex(long index) const;   // 0x009ACCC0 (Ego_r.pdb QBE)
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
class CTCBase { public:                                // the component base (members this row reads)
    virtual ~CTCBase();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
namespace NInventory {
class CTCInventoryBase : public CTCBase {
protected:
    virtual void OnSerialise(CPersistContext& context, bool inGameSave);   // 0x005BFD5E (Ego_r.pdb MAE; called qualified)
};
}
class CTCInventory : public NInventory::CTCInventoryBase { public:
    unsigned char _pad_0x004[0x148 - 0x4];
    CLongVector QuickAccessItems;                      // +0x148
    virtual void OnSerialise(CPersistContext& context, bool inGameSave);   // Ego_r.pdb ?OnSerialise@CTCInventory@@UAEXAAVCPersistContext@@_N@Z
};

void CTCInventory::OnSerialise(CPersistContext& context, bool inGameSave)
{
    NInventory::CTCInventoryBase::OnSerialise(context, inGameSave);
    if (context.IsSaving()) {
        std::vector<CCharString, std::allocator<CCharString> > names;
        names.reserve(QuickAccessItems.size());
        for (unsigned long i = 0; i < QuickAccessItems.size(); ++i)
            if (QuickAccessItems[i] > 0) {
                CCharString name = GetDefinitionManager().GetDefNameFromGlobalIndex(QuickAccessItems[i]).GetString();
                names.push_back(name);
            }
        context.TransferVector("QuickAccessItemNames", names);
    } else {
        std::vector<CCharString, std::allocator<CCharString> > names;
        context.TransferVector("QuickAccessItemNames", names);
        for (unsigned long i = 0; i < names.size(); ++i)
            QuickAccessItems[i] = GetDefinitionManager().GetDefGlobalIndexFromName(names[i]);
    }
}