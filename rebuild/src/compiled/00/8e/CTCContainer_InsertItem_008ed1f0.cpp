// CTCContainer::InsertItem(long defIndex, const CCharString& name) -- append (def index, name) to the container's item list; a container whose thing does not persist its game state yet starts doing so when the item's def asks for it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ED1F0, 184 bytes (`ret 8`).
// Callees: CCharString(const CCharString&) @ 0x0099EC30 (the pair, then the placement copy), the item vector's
// _M_insert_overflow @ 0x0074FCE0 (manifest "std::vector_InsertRangeWithCopy"), ~CCharString @ 0x0099EAE0,
// CThing::IsGameStatePersisted() const @ 0x004C8280, CTCBase::GetDefinitionManager() @ 0x006869D0, CDefinitionManager::GetDef<T>
// (long, CDefPointer*) @ 0x0042AF3C (manifest "GetDef<CSpecialAbilitiesDrunkennessDef>", byte twin), CThing::
// SetAsGameStatePersisted(bool) @ 0x004C7880, the def's virtual slot 1 on the last release.
// Layout: CTCContainer (Ego_r.pdb) ContainerItems vector<pair<long, CCharString>> +0x0c (8-byte elements); the item def's flag
// byte +0x42 bit 1 (named descriptively).
// Shape: a pair<long, CCharString> local (index stored next to the string copy) handed to the inline STLport push_back
// (placement memberwise copy / _M_insert_overflow with the __false_type tag in the dead index slot); the def out pointer also
// reuses the dead index slot, the def taken out of it into a holder whose release loses its null test after the flag read.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct SContainerItem {                                 // std::pair<long, CCharString>
    long        first;
    CCharString second;
    SContainerItem(const long& a, const CCharString& b) : first(a), second(b) { }
    SContainerItem(const SContainerItem& other) : first(other.first), second(other.second) { }
};
struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
struct CContainerItemVector {                           // STLport vector<pair<long, CCharString>>, 12 bytes
    SContainerItem* _M_start;
    SContainerItem* _M_finish;
    SContainerItem* _M_end_of_storage;
    void _M_insert_overflow(SContainerItem* position, const SContainerItem& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x0074FCE0
    void push_back(const SContainerItem& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SContainerItem(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CThingDef {
    virtual void v00();
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
    unsigned char _pad08[0x42 - 0x08];
    unsigned char Flags42;                              // +0x42 (bit 1: persist the container's game state)
    bool PersistsGameState() const { return (Flags42 & 2) != 0; }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def taken out of the out pointer, released at the end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object && --Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingDef>* out) const;   // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CThingDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    bool IsGameStatePersisted() const;                  // 0x004C8280
    void SetAsGameStatePersisted(bool persisted);       // 0x004C7880
};
class CTCContainer { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CContainerItemVector ContainerItems;                // +0x0c
    const CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    __forceinline CThingDef* GetItemDef(long defIndex) const
    {
        CDefPointer<CThingDef> out;
        GetDefinitionManager()->GetDef(defIndex, &out);
        return out.Object;
    }
    void InsertItem(long defIndex, const CCharString& name);
};

void CTCContainer::InsertItem(long defIndex, const CCharString& name)
{
    ContainerItems.push_back(SContainerItem(defIndex, name));
    if (!ParentThing->IsGameStatePersisted()) {
        CDefHolder<CThingDef> def(GetItemDef(defIndex));
        if (def->PersistsGameState())
            ParentThing->SetAsGameStatePersisted(true);
    }
}