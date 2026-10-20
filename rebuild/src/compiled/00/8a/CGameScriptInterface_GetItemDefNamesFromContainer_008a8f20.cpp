// CGameScriptInterface::GetItemDefNamesFromContainer(const CScriptThing& container, std::vector<CCharString>& names) const -- clear the list, then (when the container is alive and has a container component) append the def name of every item it holds
// Leaf: ?GetItemDefNamesFromContainer@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A8F20, length 245 bytes (+ int3 padding), `ret 8`.
// Signature from Ego_r.pdb: ?GetItemDefNamesFromContainer@CGameScriptInterface@@UBEXABVCScriptThing@@AAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z.
// Callees: vector<CCharString>::erase(first, last) @ 0x0043336A (the inline clear()), CScriptThing slots 75 IsAlive and 11
//   GetThing, the const TC map lower bound @ 0x004365B0, CDefinitionManager::GetDefNameFromGlobalIndex @ 0x009ACCC0 (CDefString
//   by value), CDefStringTable::GetString @ 0x009D49B0 on the global table @ 0x013CA828, CCharString copy ctor @ 0x0099EC30 /
//   ~CCharString @ 0x0099EAE0, vector<CCharString>::_M_insert_overflow @ 0x00433530.
// Layout: CGameScriptInterface DefinitionManager +0x10; retail CThing TC 0x3C = +0x24 bit 28, map +0x44; the container
//   component's item vector (8-byte records, def index first) +0xc.
// Shape: the landed MsgOnExpressionPerformed 0x00893B00 def-name lookup (the CDefString temporary and the name share one
//   frame word); push_back inlined (null-tested placement copy, finish += 4, else the overflow with the __false_type tag in
//   the dead slot); the item count re-derived every pass.
inline void* operator new(unsigned int, void* where) { return where; }
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct __false_type { __false_type() {} };
struct CCharStringVector {                              // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString* erase(CCharString* first, CCharString* last);   // 0x0043336A
    void clear() { erase(_M_start, _M_finish); }
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
void* FindRef_CCharStringVector_erase_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) CCharString* CCharStringVector::erase(CCharString* first, CCharString* last)
{
    void* p = FindRef_CCharStringVector_erase_1();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)last = p;
    return 0;
}
#pragma auto_inline(on)
void* FindRef_CCharStringVector__M_insert_overflow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CCharStringVector::_M_insert_overflow(CCharString* position, const CCharString& x, const __false_type& p2, unsigned long fill_len, bool atend)
{
    void* p = FindRef_CCharStringVector__M_insert_overflow_0();
    if (!p)
        return;
    *(void**)position = p;
    *(void**)&x = p;
}
#pragma auto_inline(on)
struct CDefString {
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
};
struct CDefinitionManager_Retail {
    CDefString GetDefNameFromGlobalIndex(long index);   // 0x009ACCC0
};
struct CDefStringTable_Retail {
    CCharString GetString(long index);                  // 0x009D49B0
};
extern CDefStringTable_Retail g_defStringTable;         // 0x013CA828
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CContainerItem { long DefIndex; long Data; };     // 8 bytes
struct CTCContainer {
    unsigned char _pad00[0xc];
    CContainerItem* ItemsStart;                         // +0x0c
    CContainerItem* ItemsFinish;                        // +0x10
    unsigned long ItemCount() const { return ItemsFinish - ItemsStart; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template <class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
    void* PImp[2];
};
struct CGameScriptInterface {
    virtual void GetItemDefNamesFromContainer(const CScriptThing& container, CCharStringVector& names) const;
    void*                      World;                   // +0x04
    void*                      Component;               // +0x08
    void*                      DisplayEngine;           // +0x0c
    CDefinitionManager_Retail* DefinitionManager;       // +0x10
};

void CGameScriptInterface::GetItemDefNamesFromContainer(const CScriptThing& container, CCharStringVector& names) const
{
    names.clear();
    if (!container.IsAlive())
        return;
    CThing* thing = container.GetThing();
    if (!thing->HasTC(0x3c))
        return;
    const CTCContainer* items = thing->GetTC<CTCContainer>(0x3c);
    for (unsigned long i = 0; i < items->ItemCount(); ++i) {
        CCharString name = g_defStringTable.GetString(DefinitionManager->GetDefNameFromGlobalIndex(items->ItemsStart[i].DefIndex).m_index);
        names.push_back(name);
    }
}