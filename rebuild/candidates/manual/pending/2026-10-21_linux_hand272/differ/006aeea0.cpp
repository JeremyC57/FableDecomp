// CTCRegionFollower::IsCreatureFollowingHeroAcrossRegions(const CThingCreatureBase* creature) const -- children follow when their creature def allows it; a wife follows only when permitted; summoned creatures and the ghostly sword always follow; otherwise ask the parent's followed component
// Grade: DIFFER(429v429) -- scratch-register rotation only: retail picks eax/ecx/edx for the GetDefName hidden pointer, the def-name word and the 0xBC lookup key; ours picks ecx/edx/eax. Tried: free/member/by-value operator==, CDefString copy ctor/dtor, named CDefString, IsDefNamed helper, named parent, explicit CCharString temp, non-const creature (16 probes, all 429v429 same rotation).
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
extern const char g_EmptyCharString[];             // 0x0129AAF4
struct CCharStringData {
    const char*   m_pChars;
    unsigned long m_length;
};
struct CCharString {
    CCharStringData* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const char* GetChars() const { return m_pData == 0 ? g_EmptyCharString : m_pData->m_pChars; }
};
struct CDefStringTable {
    char* GetStringAsCharArray(long index);             // 0x009D49F0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
struct CDefString {                                     // 4-byte def-name handle (user constructor: hidden-pointer return)
    long m_tablePos;
    CDefString();
    bool operator==(const CCharString& text) const
    {
        return _stricmp(g_DefStringTable.GetStringAsCharArray(m_tablePos), text.GetChars()) == 0;
    }
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
struct CCreatureDef : CDefBase {
    unsigned char _pad08[0xcb - 0x08];
    bool ChildThatCanRegionFollow;                      // +0xcb (PDB +0xdb; retail CDefBase-derived defs are 0x10 smaller)
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    bool IsValid() const { return Object != 0; }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CCreatureDef>* out);     // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_00485815_0();
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_00485815_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThingCreatureBase;
struct CTCWife {                                        // interface 0xD7
    unsigned char _pad00[0x1c];
    bool PermittedToRegionFollow;                       // +0x1c
    bool IsMarriedToThePlayer() const;                  // 0x006CEC80
};
struct CTCSummonableCreature {                          // interface 0xA0
    unsigned char _pad00[0x10];
    bool HasBeenSummoned;                               // +0x10
};
struct CTCFollowed {                                    // interface 0xBC
    bool IsCreatureValidFollower(const CThingCreatureBase* creature, bool* reason) const;   // 0x006D4E60
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                // +0x20
    CInterfaceMap  m_interfaces;                    // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                            // +0x70
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    CDefString GetDefName() const;                  // 0x004C7CC0
};
class CThingCreatureBase : public CThing {
public:
    bool IsChild() const;                           // 0x00661F70
};
struct CTCRegionFollower {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    bool IsCreatureFollowingHeroAcrossRegions(const CThingCreatureBase* creature) const;
};

bool CTCRegionFollower::IsCreatureFollowingHeroAcrossRegions(const CThingCreatureBase* creature) const
{
    if (creature->IsChild()) {
        CDefPointer<CCreatureDef> def;
        creature->PDef->GetSubDef(&def);
        if (def.IsValid() && def->ChildThatCanRegionFollow)
            return true;
        return false;
    }
    if (creature->HasTC(0xd7)) {
        const CTCWife* wife = creature->GetTC<CTCWife>(0xd7);
        if (wife->IsMarriedToThePlayer() && !wife->PermittedToRegionFollow)
            return false;
    }
    if (creature->HasTC(0xa0)) {
        if (creature->GetTC<CTCSummonableCreature>(0xa0)->HasBeenSummoned)
            return true;
    }
    if (creature->GetDefName() == "GHOSTLY_SWORD")
        return true;
    const CTCFollowed* followed;
    if (ParentThing->HasTC(0xbc))
        followed = ParentThing->GetTC<CTCFollowed>(0xbc);
    return followed->IsCreatureValidFollower(creature, 0);
}
