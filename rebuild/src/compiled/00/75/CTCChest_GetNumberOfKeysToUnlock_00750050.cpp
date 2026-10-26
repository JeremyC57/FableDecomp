// CTCChest::GetNumberOfKeysToUnlock(CCharString& keyName) const -- how many opener objects the chest needs and the display name of that opener; 0 and "" when the chest def asks for none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750050, 186 bytes (`ret 4`).
// Callees: CDefClassBase::GetSubDef<CChestDef>(CDefPointer<const CChestDef>*) @ 0x0074F7D0 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", byte twin, as the landed GetOpenAnimationForCreature 0x00750240),
// CTCBase::GetDefinitionManager() @ 0x006869D0, CDefinitionManager::GetDefNameFromGlobalIndex(long) @ 0x009ACCC0 (a CDefString
// by hidden pointer), CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable @ 0x013CA828, CCharString::operator=
// (const CCharString&) @ 0x0099EFB0 and (const char*) @ 0x0099EFE0, ~CCharString @ 0x0099EAE0, the def's virtual slot 1 on the
// last release. Literal "" @ 0x0122D70E.
// Layout: CTCBase ParentThing +0x04; CThing def +0x70; CChestDef OpenerObject +0x34, OpenersRequired +0x38 (GOTCHAS chest facts).
// Shape: the landed GetOpenAnimationForCreature 0x00750240 model verbatim (thing def local, out CDefPointer, `found`, holder
// released on both returns) with the same-TU GetSubDef stand-in (the out word no longer escapes: 8-byte frame, the GetString
// temporary and the CDefString share it with the out word). The condition reads OpenerObject/OpenersRequired through
// `out.Object` and the body through the holder `def`: two base variables, so VC7.1 re-reads [esi+34h] for the push (retail)
// instead of CSE-ing the test's load. The key name is a block-scoped named local (RVO into the frame slot, passed to
// operator= by its address) -- an unnamed temporary passes GetString's returned eax.
class CCharString { public:
    void* PStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);           // 0x0099EFE0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefString { public: long Index; };
struct CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
class CDefinitionManager { public:
    CDefString GetDefNameFromGlobalIndex(long index) const;   // 0x009ACCC0
};
class CGameDefinitionManager : public CDefinitionManager
{
};
class CDefPointeeBase { public:
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
class CChestDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34 - 0x08];
    long OpenerObject;                                  // +0x34 (global def index)
    long OpenersRequired;                               // +0x38
};
template <class T> class CDefPointer { public:
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
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CChestDef>* out);        // 0x0074F7D0
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CChestDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CChestDef*)def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
class CTCBase
{
public:
    virtual void v00();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
class CTCContainer : public CTCBase
{
public:
};
class CTCContainerRewardHero : public CTCContainer
{
public:
};

class CTCChest : public CTCContainerRewardHero { public:
    CThing* ParentThing;                                // +0x04 (CTCBase)
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
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefPointer<CChestDef> out;
    bool found = thingDef->GetSubDef(&out);
    CDefHolder<CChestDef> def(out.Object);
    if (found && out.Object->OpenerObject > 0 && out.Object->OpenersRequired > 0) {
        {
            CCharString name = g_DefStringTable.GetString(GetDefinitionManager().GetDefNameFromGlobalIndex(def->OpenerObject).Index);
            keyName = name;
        }
        return def->OpenersRequired;
    }
    keyName = "";
    return 0;
}