// Grade: DIFFER(199v186) -- frame: retail builds the GetString temporary in the dead out-pointer word (F0) and the CDefString at F4 (8-byte frame), ours opens a third word; retail re-reads OpenerObject into eax after testing OpenersRequired, ours keeps it and tests in ecx
// CTCChest::GetNumberOfKeysToUnlock(CCharString& keyName) const -- how many opener objects the chest needs and the display name of that opener; 0 and "" when the chest def asks for none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750050, 186 bytes (`ret 4`).
// Callees: CDefClassBase::GetSubDef<CChestDef>(CDefPointer<const CChestDef>*) @ 0x0074F7D0 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", byte twin, as the landed GetOpenAnimationForCreature 0x00750240),
// CTCBase::GetDefinitionManager() @ 0x006869D0, CDefinitionManager::GetDefNameFromGlobalIndex(long) @ 0x009ACCC0 (a CDefString
// by hidden pointer), CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable @ 0x013CA828, CCharString::operator=
// (const CCharString&) @ 0x0099EFB0 and (const char*) @ 0x0099EFE0, ~CCharString @ 0x0099EAE0, the def's virtual slot 1 on the
// last release. Literal "" @ 0x0122D70E.
// Layout: CTCBase ParentThing +0x04; CThing def +0x70; CChestDef OpenerObject +0x34, OpenersRequired +0x38 (GOTCHAS chest facts).
// Shape: the GetOpenAnimationForCreature shape: the def is taken out of the out pointer into a holder (esi) released on both
// returns -- without the null test where it was already dereferenced (the count reload interleaved with the result load),
// with it on the empty path; the GetDefNameFromGlobalIndex arguments are pushed before the manager accessor runs.
struct CCharString {
    void* PStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);           // 0x0099EFE0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefString { long Index; };
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
struct CDefinitionManager {
    CDefString GetDefNameFromGlobalIndex(long index) const;   // 0x009ACCC0
};
struct CDefPointeeBase {
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
struct CChestDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    long OpenerObject;                                  // +0x34 (global def index)
    long OpenersRequired;                               // +0x38
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def taken out of the out pointer, released when the function returns
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CChestDef>* out);        // 0x0074F7D0
};
struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
struct CTCChest {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    const CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    __forceinline CChestDef* GetChestDef(bool& found) const
    {
        CDefClassBase* thingDef = ParentThing->PDef;
        CDefPointer<CChestDef> out;
        found = thingDef->GetSubDef(&out);
        return out.Object;
    }
    long GetNumberOfKeysToUnlock(CCharString& keyName) const;
};

long CTCChest::GetNumberOfKeysToUnlock(CCharString& keyName) const
{
    bool found;
    CDefHolder<CChestDef> def(GetChestDef(found));
    if (found && def->OpenerObject > 0 && def->OpenersRequired > 0) {
        {
            CCharString name = g_DefStringTable.GetString(GetDefinitionManager()->GetDefNameFromGlobalIndex(def->OpenerObject).Index);
            keyName = name;
        }
        return def->OpenersRequired;
    }
    keyName = "";
    return 0;
}
