// CTCBuyableHouse::GetValueOfTrophies() const -- sum the value of every live trophy mounted in the house (the trophy def's value, or 50 when the trophy has no trophy def)
// Leaf: GetValueOfTrophies
// Retail 0x006BFFD0, 336 bytes, `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (const) 0x004365B0 (interfaces 0x21 thing list, 0xDA wall
// mount), CBaseIntelligentPointer::GetPItem 0x00A01B50, the wall mount's mounted-thing accessor 0x007DE4F0,
// CDefClassBase::GetSubDef<CTrophyDef> 0x0059350C; the defs' slot 1 release.
// Layout: CTCBase ParentThing +4; the interface-0x21 component's list header +0x0C (landed CountMoneyBags 0x006C04D0
// model); CThing bitset +0x20 (0xDA = word 6 bit 26), map +0x44, def +0x70, dead flag +0x91; CTrophyDef value +0x28.
// Shape: the list walk of CountMoneyBags; the trophy's def is an inline-copied CDefPointer (add-ref after the out pointer
// is zeroed, register-held, released right after GetSubDef); the out CDefPointer is re-read from the frame for the value
// and its release.
// Shape (stand-ins): CDefClassBase::GetSubDef<CTrophyDef> (a template instantiation, out of line in retail) has a same-TU
//   stand-in body that writes the out slot only, and the CVectorMap LowerBound (const) instantiation has its same-TU
//   binary-search body; with both visible VC7.1 keeps the running value in ebp and releases the out def with one load.
struct CTCBase;
struct CBaseObject;
struct CDefPointeeBase {
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CTrophyDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long          Value;                                // +0x28
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CTrophyDef>* out);       // 0x0059350C
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CTrophyDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
struct CThing;
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* Get() const { return (T*)GetPItem(); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;         // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
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

struct SThingNode {
    SThingNode*                 next;
    SThingNode*                 prev;
    CIntelligentPointer<CThing> thing;                  // +0x08
};
struct CTCThingList_0x21 {
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                               // +0x0c
};
struct CThing {
    unsigned char              _pad0[0x20];
    unsigned long              m_presentBits[9];        // +0x20
    CInterfaceMap              m_interfaces;            // +0x44
    unsigned char              _pad_0x4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;                    // +0x70
    unsigned char              _pad_0x74[0x91 - 0x74];
    unsigned char              m_stateFlags;            // +0x91 (bit 0 = dead)
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CTCWallMount {
    CThing* GetMountedThing() const;                    // 0x007DE4F0
};
inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !(p.Get()->m_stateFlags & 1);
}
class CTCBuyableHouse {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    long GetValueOfTrophies() const;
};

long CTCBuyableHouse::GetValueOfTrophies() const
{
    long value = 0;
    CTCThingList_0x21* list;
    if (ParentThing->GetInterface(&list, 0x21)) {
        for (SThingNode* node = list->m_head->next; node != list->m_head; node = node->next) {
            CTCWallMount* mount;
            if (IsThingAlive(node->thing) && node->thing.Get()->GetInterface(&mount, 0xda)) {
                CThing* trophy = mount->GetMountedThing();
                if (trophy && !(trophy->m_stateFlags & 1)) {
                    CDefPointer<CTrophyDef> def;
                    if (mount->GetMountedThing()->GetDef()->GetSubDef(&def))
                        value += def->Value;
                    else
                        value += 50;
                }
            }
        }
    }
    return value;
}