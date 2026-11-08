// CTCRegionFollower::IsCreatureFollowingHeroAcrossRegions(const CThingAICreature& creature) const -- children follow when their creature def allows it; a wife follows only when permitted; summoned creatures and the ghostly sword always follow; otherwise ask the parent's followed component
// Leaf: ?IsCreatureFollowingHeroAcrossRegions@CTCRegionFollower@@QBE_NABVCThingAICreature@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AEEA0, 429 bytes, `ret 4`; Ego_r.pdb ?IsCreatureFollowingHeroAcrossRegions@CTCRegionFollower@@
//   QBE_NABVCThingAICreature@@@Z (the creature is a const CThingAICreature&; the old draft had a CThingCreatureBase pointer).
// Callees: CThingCreatureBase::IsChild const 0x00661F70, CDefClassBase::GetSubDef<CCreatureDef> const 0x00485815, CVectorMap<
//   ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (TCI_WIFE 215, TCI_SUMMONABLE_CREATURE 160, TCI_FOLLOWED 188),
//   CTCWife::IsMarriedToThePlayer const 0x006CEC80, CCharString ctor/dtor 0x0099EBF0/0x0099EAE0, CThing::GetDefName const
//   0x004C7CC0, CDefStringTable::GetStringAsCharArray const 0x009D49F0 (GDefStringTable 0x013CA828), _stricmp 0x00BFEBA8,
//   CTCFollowed::IsCreatureValidFollower(CThing*, bool*) 0x006D4E60.
// Fix (chunk 759): the three component lookups are the PeekTC(ETCInterfaceType, T**) const shape over the real engine/CVectorMap.h
//   (key copied into a local, explicit LowerBound / end() test, the header's same-TU noinline LowerBound); with that the eax/ecx/edx
//   rotation of the old HasTC + GetTC form (7 diff lines) is gone. GetSubDef<CCreatureDef> keeps a same-TU noinline body (the landed
//   CTCWound::AddWound 0x00757B30 stand-in shape, `__declspec(noinline)`, no pragma): an external declaration splits the def
//   release into two blocks (21 diff lines). CCharString is fable_string.h (GetAsCharArray, NullChar 0x0129AAF4).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
class CDefStringTable { public:
    const char* GetStringAsCharArray(long index) const;             // 0x009D49F0
};
extern CDefStringTable GDefStringTable;                // 0x013CA828
class CDefString { public:                              // 4-byte def-name handle (user constructor: hidden-pointer return)
    long TablePos;
    CDefString();
    bool operator==(const CCharString& text) const
    {
        return _stricmp(GDefStringTable.GetStringAsCharArray(TablePos), text.GetAsCharArray()) == 0;
    }
};
class CDefBase { public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                                      // +0x04
};
class CCreatureDef : public CDefBase { public:
    unsigned char _pad08[0xcb - 0x08];
    bool ChildThatCanRegionFollow;                      // +0xcb (PDB +0xdb; retail CDefBase-derived defs are 0x10 smaller)
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    bool IsValid() const { return Object != 0; }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CSubDefInfo { public: long DefIndex; long OriginalParentDefIndex; };
class CDefinitionManager { public:
    CDefPointer<const CCreatureDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString& name) const;   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* PDefManager;                    // +0x1c
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};
// Same-TU body of GetSubDef<CCreatureDef> 0x00485815 (template member, visible in retail's TU; out of line there): the landed
// CTCWound::AddWound 0x00757B30 stand-in shape. VC7.1 then knows the def pointer's address does not escape.
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    const CSubDefInfo* info;
    {
        CCharString name("CCreatureDef");
        info = GetPSubDefInfo(name);
    }
    if (info) {
        CDefPointer<const CCreatureDef> result = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        *out = result;
        return true;
    }
    return false;
}
class CTCBase { public: virtual ~CTCBase(); };
class CThing;
class CThingCreatureBase;
class CTCWife : public CTCBase { public:                                        // interface 0xD7
    unsigned char _pad04[0x1c - 0x04];
    bool PermittedToRegionFollow;                       // +0x1c
    bool IsMarriedToThePlayer() const;                  // 0x006CEC80
};
class CTCSummonableCreature : public CTCBase { public:                          // interface 0xA0
    unsigned char _pad04[0x10 - 0x04];
    bool HasBeenSummoned;                               // +0x10
};
class CTCFollowed : public CTCBase { public:                                    // interface 0xBC
    bool IsCreatureValidFollower(CThing* creature, bool* reason);   // 0x006D4E60
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    std::bitset<274> UsingComponents;               // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char  _pad54[0x70 - 0x54];
    CDefClassBase* PDef;                            // +0x70
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> FABLE_FORCEINLINE bool PeekTC(ETCInterfaceType id, T** ptc) const
    {
        if (HasTC(id)) {
            ETCInterfaceType type = id;
            const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            const std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            if (it == map->end() || id < it->first)
                it = map->end();
            *ptc = static_cast<T*>(it->second);
            return true;
        }
        return false;
    }
    CDefString GetDefName() const;                  // 0x004C7CC0
};
class CThingCreatureBase : public CThing {
public:
    bool IsChild() const;                           // 0x00661F70
};
class CThingAICreature : public CThingCreatureBase { };
class CTCRegionFollower { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    bool IsCreatureFollowingHeroAcrossRegions(const CThingAICreature& creature) const;
};

bool CTCRegionFollower::IsCreatureFollowingHeroAcrossRegions(const CThingAICreature& creature) const
{
    if (creature.IsChild()) {
        CDefPointer<const CCreatureDef> def;
        creature.PDef->GetSubDef(&def);
        if (def.IsValid() && def->ChildThatCanRegionFollow)
            return true;
        return false;
    }
    CTCWife* wife;
    if (creature.PeekTC(TCI_WIFE, &wife)) {
        if (wife->IsMarriedToThePlayer() && !wife->PermittedToRegionFollow)
            return false;
    }
    CTCSummonableCreature* summonable;
    if (creature.PeekTC(TCI_SUMMONABLE_CREATURE, &summonable)) {
        if (summonable->HasBeenSummoned)
            return true;
    }
    if (creature.GetDefName() == "GHOSTLY_SWORD")
        return true;
    CTCFollowed* followed;
    ParentThing->PeekTC(TCI_FOLLOWED, &followed);
    return followed->IsCreatureValidFollower(const_cast<CThingAICreature*>(&creature), 0);
}