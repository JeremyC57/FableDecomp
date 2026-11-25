// CTCBuyableHouse::GetValueOfTrophies() const -- sum the value of every live trophy mounted in the house (the trophy def's value, or 50 when the trophy has no trophy def)
// Leaf: GetValueOfTrophies
// Retail 0x006BFFD0, 336 bytes, `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (const) 0x004365B0 (interfaces 0x21 thing list, 0xDA wall
// mount), CBaseIntelligentPointer::GetPItem 0x00A01B50, the wall mount's mounted-thing accessor 0x007DE4F0,
// CDefClassBase::GetSubDef<CStockItemDef> 0x0059350C; the defs' slot 1 release.
// Layout: CTCBase ParentThing +4; the interface-0x21 component's list header +0x0C (landed CountMoneyBags 0x006C04D0
// model); CThing bitset +0x20 (0xDA = word 6 bit 26), map +0x44, def +0x70, dead flag +0x91; CStockItemDef value +0x28.
// Shape: the list walk of CountMoneyBags; the trophy's def is an inline-copied CDefPointer (add-ref after the out pointer
// is zeroed, register-held, released right after GetSubDef); the out CDefPointer is re-read from the frame for the value
// and its release.
// Shape (stand-ins): CDefClassBase::GetSubDef<CStockItemDef> (a template instantiation, out of line in retail) has a same-TU
//   stand-in body that writes the out slot only, and the CVectorMap LowerBound (const) instantiation has its same-TU
//   binary-search body; with both visible VC7.1 keeps the running value in ebp and releases the out def with one load.
#include "fable_compiler.h"
class CTCBase;
class CBaseObject;
class CDefPointeeBase { public:
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    typename CDefNonConst<T>::Type* operator->() const { return Object; }
};
class CStockItemDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long          Value;                                // +0x28
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0059350C GetSubDef<CStockItemDef> (Ego_r.pdb member template)
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CStockItemDef>(CDefPointer<const CStockItemDef>* out) const
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
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
};
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in CInterfaceMap (?LowerBound@CInterfaceMap@@QBEPAUSEntry@1@PBJ@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): CThing::Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QBE spelling at 0x004365B0.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.

struct SThingNode {
    SThingNode*                 next;
    SThingNode*                 prev;
    CIntelligentPointer<CThing> thing;                  // +0x08
};
struct CTCThingList_0x21 {
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                               // +0x0c
};
class CThing { public:
    unsigned char              _pad0[0x20];
    unsigned long              m_presentBits[9];        // +0x20
    CInterfaceMap              m_interfaces;            // +0x44
    unsigned char              _pad_0x54[0x70 - 0x54];  // CVectorMap is 0x10
    CDefPointer<CDefClassBase> PDef;                    // +0x70
    unsigned char              _pad_0x74[0x91 - 0x74];
    unsigned char              m_stateFlags;            // +0x91 (bit 0 = dead)
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template <class T> FABLE_FORCEINLINE T* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
};
class CTCWallMount { public:
    const CThing* PeekObjectAttached() const;           // 0x007DE4F0 ?PeekObjectAttached@CTCWallMount@@QBEPBVCThing@@XZ (Ego_r.pdb; NF168 respelled in place)
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
                const CThing* trophy = mount->PeekObjectAttached();
                if (trophy && !(trophy->m_stateFlags & 1)) {
                    CDefPointer<const CStockItemDef> def;
                    if (mount->PeekObjectAttached()->GetDef()->GetSubDef(&def))
                        value += def->Value;
                    else
                        value += 50;
                }
            }
        }
    }
    return value;
}