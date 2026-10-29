// CTCObjectAugmentations::OnSerialise(CPersistContext& context, bool savedInGame) -- persist the augmentation defs by name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00767200, length 441 bytes (+ int3 padding), `ret 8` (vtable 0x0126A3A4 slot 1).
// Callees: CPersistContext::Transfer(const char*, bool&, const bool&) @ 0x004045C0 and (const char*, vector<CCharString>&)
// @ 0x0049B8D0, vector<CCharString>(size_type) @ 0x0057FCDD, the def's virtual slot 10 (+0x28, its CDefString name by hidden
// pointer), CDefStringTable::GetString @ 0x009D49B0 on g_DefStringTable @ 0x013CA828, CCharString::operator= @ 0x0099EFB0 /
// ~CCharString @ 0x0099EAE0, vector<CDefPointer<const CThingBaseDef> >::resize(n, const T&) @ 0x0076BF80,
// CGameDefinitionManager::Get @ 0x0044C6B0, CGameDefinitionManager::GetDef(const CCharString&, CDefPointer&) @ 0x00528760,
// free @ 0x00BFEA14.
// Strings: "SavedInGame" @ 0x0126A394, "AugmentationDefNames" @ 0x0126A37C.
// Shape: the sibling of the landed CTCHeroAttachableAppearanceModifiers::OnSerialise 0x00706F40. Saving transfers the bool
// PARAMETER (its slot holds the value; the `false` default lives in the dead context slot); loading transfers into
// LoadedFromSaveGame, clamps the name count with an inline min against the def's MaximumNumberOfSlots and resizes with a
// default CDefPointer temporary. Both vectors index through an inline begin() (`*(begin() + n)`): with raw `_M_start + n`
// VC shares one `i*4` between the two element addresses of the GetDef call.
// Layout: Ego_r.pdb CTCObjectAugmentations (PDef +0x0c, LoadedFromSaveGame +0x10, Augmentations +0x14); CObjectAugmentationsDef
// MaximumNumberOfSlots +0x2c.
// Leaf: OnSerialise
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
inline void DestroyRange(CCharString* first, CCharString* last)
{
    for (; first != last; ++first)
        first->~CCharString();
}
struct CCharStringVector {                              // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    explicit CCharStringVector(unsigned long count);    // 0x0057FCDD
    ~CCharStringVector()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    CCharString* begin() { return _M_start; }
    CCharString& operator[](unsigned long n) { return *(begin() + n); }
};
class CDefString { public:
    long ID;
    CDefString() { }
};
struct CDefStringTable {
    CCharString GetString(CDefString id);               // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long Mode;                                          // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: bool 0x004045C0
    template <class T> void Transfer(const char* name, T& value);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: CCharStringVector 0x0049B8D0
};
struct CDefBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetDefName() const;              // slot 10 (+0x28)
    mutable long m_refCount;                            // +0x04
};
class CThingBaseDef : public CDefBase { public: };
class CObjectAugmentationsDef : public CDefBase { public:
    unsigned char _pad08[0x2c - 0x08];
    long          MaximumNumberOfSlots;                 // +0x2c
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
struct CDefPointerVector {                              // STLport vector<CDefPointer<const CThingBaseDef> >
    CDefPointer<const CThingBaseDef>* _M_start;
    CDefPointer<const CThingBaseDef>* _M_finish;
    CDefPointer<const CThingBaseDef>* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CDefPointer<const CThingBaseDef>* begin() { return _M_start; }
    CDefPointer<const CThingBaseDef>& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned long n, const CDefPointer<const CThingBaseDef>& value);   // 0x0076BF80
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
    bool GetDef(const CCharString& name, CDefPointer<const CThingBaseDef>& out);   // 0x00528760
};
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
class CThing;
class CTCObjectAugmentations { public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    CDefPointer<const CObjectAugmentationsDef> PDef;    // +0x0c
    bool          LoadedFromSaveGame;                   // +0x10
    CDefPointerVector Augmentations;                    // +0x14
    void OnSerialise(CPersistContext& context, bool savedInGame);
};

void CTCObjectAugmentations::OnSerialise(CPersistContext& context, bool savedInGame)
{
    if (context.IsSaving()) {
        context.Transfer("SavedInGame", savedInGame, false);
        CCharStringVector names(Augmentations.size());
        if (savedInGame) {
            for (unsigned long i = 0; i < Augmentations.size(); ++i) {
                if (Augmentations[i].Object) {
                    CCharString name = g_DefStringTable.GetString(Augmentations[i]->GetDefName());
                    names[i] = name;
                }
            }
        }
        context.Transfer("AugmentationDefNames", names);
    } else {
        context.Transfer("SavedInGame", LoadedFromSaveGame, false);
        CCharStringVector names;
        context.Transfer("AugmentationDefNames", names);
        if (LoadedFromSaveGame) {
            long count = names.size();
            long n = Min(count, PDef->MaximumNumberOfSlots);
            Augmentations.resize(n, CDefPointer<const CThingBaseDef>());
            for (long i = 0; i < n; ++i)
                CGameDefinitionManager::Get().GetDef(names[i], Augmentations[i]);
        }
    }
}