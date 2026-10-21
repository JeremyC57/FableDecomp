// CTCHeroAttachableAppearanceModifiers::OnSerialise(CPersistContext& context, bool firstPass) -- persists the worn hair modifiers by def name ("HairModifierNames"): saving (context mode 1 or 3) maps each hair modifier's def index to its name; loading re-adds every named def that resolves (index > 0) with replace and marks the modifiers loaded
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706F40, 400 bytes (`ret 8`; labelled vfunc_1). Ego_r.pdb
// `?OnSerialise@CTCHeroAttachableAppearanceModifiers@@UAEXAAVCPersistContext@@_N@Z`.
// Callees: vector<CCharString>(size_type) @ 0x0057FCDD, CTCBase::GetDefinitions @ 0x006869D0, CDefinitionManager::
// GetDefNameFromGlobalIndex(long) @ 0x009ACCC0 (CDefString by hidden pointer), CDefStringTable::GetString @ 0x009D49B0 on
// g_DefStringTable @ 0x013CA828, CCharString::operator=(const CCharString&) @ 0x0099EFB0, ~CCharString @ 0x0099EAE0,
// CPersistContext::Transfer(const char*, vector<CCharString>&) @ 0x0049B8D0 (shared COMDAT body; manifest TransferSignedChar),
// CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, AddAppearanceModifierWithReplace(long) @ 0x00706950, free @ 0x00BFEA14.
// Layout: as the landed RemoveAllAppearanceModifiers 0x00706640 (AppearanceModifiers +0x30, element 0 = the hair list; the
// AppearanceModifiersLoaded bit at +0x14 bit 2); CPersistContext mode +0x18.
// Shape: the save test is an inline IsSaving() (mode 1 or 3; the plain condition lays the load path first); each name is built
// into a named local before the element assignment (RHS first); the name vector's destructor is inline (element destructor
// loop over a by-value range, then the member re-read for free).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct CThing;
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
    CCharString& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CDefString {
    long ID;
};
struct CDefStringTable {
    CCharString GetString(CDefString id);               // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
struct CDefinitionManager {
    CDefString GetDefNameFromGlobalIndex(long index);   // 0x009ACCC0
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CPersistContext {
    unsigned char _pad00[0x18];
    long Mode;                                          // +0x18
    void Transfer(const char* name, CCharStringVector& value);   // 0x0049B8D0
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
};
struct CModifierEntry {
    long DefIndex;
    bool Rendered;
};
struct CModifierVector {
    CModifierEntry* _Myfirst;
    CModifierEntry* _Mylast;
    CModifierEntry* _Myend;
    unsigned long size() const { return _Mylast - _Myfirst; }
    CModifierEntry& operator[](unsigned long i) { return _Myfirst[i]; }
};
struct CModifierVectorVector {
    CModifierVector* _Myfirst;
    CModifierVector* _Mylast;
    CModifierVector* _Myend;
    CModifierVector& operator[](long i) { return _Myfirst[i]; }
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    CDefinitionManager* GetDefinitions();               // 0x006869D0
};
struct CTCHeroAttachableAppearanceModifiers : CTCBase {
    void* __vftable_0c;                                 // +0x0c secondary vptr
    unsigned long FrameLastChanged;                     // +0x10
    bool Changed : 1;                                   // +0x14 bit 0
    bool RecreatePrimitives : 1;                        // +0x14 bit 1
    bool AppearanceModifiersLoaded : 1;                 // +0x14 bit 2
    unsigned char PrimHandles[0xc];                     // +0x18
    unsigned char HiddenMaterialNames[0xc];             // +0x24
    CModifierVectorVector AppearanceModifiers;          // +0x30
    unsigned char Graphics[0xc];                        // +0x3c
    void AddAppearanceModifierWithReplace(long defIndex);   // 0x00706950
    void OnSerialise(CPersistContext& context, bool firstPass);
};

void CTCHeroAttachableAppearanceModifiers::OnSerialise(CPersistContext& context, bool firstPass)
{
    if (!firstPass)
        return;
    if (context.IsSaving()) {
        CModifierVector& hair = AppearanceModifiers[0];
        CCharStringVector names(hair.size());
        for (unsigned long i = 0; i < hair.size(); ++i) {
            CCharString name = g_DefStringTable.GetString(GetDefinitions()->GetDefNameFromGlobalIndex(hair[i].DefIndex));
            names[i] = name;
        }
        context.Transfer("HairModifierNames", names);
    } else {
        CCharStringVector names;
        context.Transfer("HairModifierNames", names);
        for (unsigned long i = 0; i < names.size(); ++i) {
            long defIndex = GetDefinitions()->GetDefGlobalIndexFromName(names[i]);
            if (defIndex > 0)
                AddAppearanceModifierWithReplace(defIndex);
        }
        AppearanceModifiersLoaded = true;
    }
}
