// Grade: DIFFER(929v926) -- frame 0x1c v 0x0c: retail keeps each def-pointer copy in esi (no frame word) and packs mask/key/iterator into two words; ours homes the GetDef() copies; this register reuse (thing in esi v edi)
// Tried: inline FindTrail helper with one shared iterator (977), block-scoped iterators per find (929), mask operand order reversed (929, VC reorders), sequential |= mask (959).
// CTCShotDefinitionBase::GetWeaponTrailGraphic() -- the trail graphic for the shooting weapon: the def's entry for the weapon's full augmentation mask, else the first single-augmentation entry (0x10, 8, 4, 2, 1), else the global default
// Leaf: GetWeaponTrailGraphic
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BFA90, 926 bytes, `ret`.
// Callees: the CTCBase game-data getter @ 0x006869D0 (catalog `_Cons_val`), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//   @ 0x004365B0, CTCObjectAugmentations::HasAugmentationOfType @ 0x007660C0, std::map<int, int>::find @ 0x007C2B30 (iterator by
//   hidden pointer); the def's virtual slot 1 on the last release.
// Layout: CThing interface bitset +0x20 / map +0x44; the interface-0x48 weapon component's def pointer +0x0c; the weapon def's
//   trail map +0xb0 (STLport header first; node value +0x10 / +0x14).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T>
    __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CTCObjectAugmentations {
    bool HasAugmentationOfType(long type);              // 0x007660C0
};
struct STrailNode {
    long        Color;
    STrailNode* Parent;
    STrailNode* Left;
    STrailNode* Right;
    long        Key;                                    // +0x10
    long        Graphic;                                // +0x14
};
struct CTrailMap {
    STrailNode* Header;
    unsigned long Count;
    struct iterator {
        STrailNode* Node;
        iterator() {}
        iterator(STrailNode* node) : Node(node) {}
        bool operator==(const iterator& o) const { return Node == o.Node; }
        bool operator!=(const iterator& o) const { return Node != o.Node; }
    };
    iterator find(const long& key);                     // 0x007C2B30
    iterator end() const { return iterator(Header); }
};
struct CWeaponDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    unsigned char _pad08[0xb0 - 0x08];
    CTrailMap TrailGraphics;                            // +0xb0
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CTCWeapon {
    unsigned char _pad00[0x0c];
    CDefPointer<CWeaponDef> PDef;                       // +0x0c
    CDefPointer<CWeaponDef> GetDef() const { return PDef; }
};
struct CGameData { unsigned char _pad[0x64]; long DefaultWeaponTrail; };
struct CGameDataHolder { unsigned char _pad[0xd0]; CGameData* Data; };

struct CTCShotDefinitionBase {
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    CGameDataHolder* GetGameData();                     // 0x006869D0
    long GetWeaponTrailGraphic();
};

long CTCShotDefinitionBase::GetWeaponTrailGraphic()
{
    long graphic = GetGameData()->Data->DefaultWeaponTrail;
    CThing* thing = ParentThing;
    if (thing->HasTC(0x48)) {
        CTCWeapon* weapon = thing->GetTC<CTCWeapon>(0x48);
        if (thing->HasTC(0xbf)) {
            CTCObjectAugmentations* aug = thing->GetTC<CTCObjectAugmentations>(0xbf);
            long mask = (aug->HasAugmentationOfType(1) ? 1 : 0) | (aug->HasAugmentationOfType(2) ? 2 : 0)
                        | (aug->HasAugmentationOfType(4) ? 4 : 0) | (aug->HasAugmentationOfType(8) ? 8 : 0)
                        | (aug->HasAugmentationOfType(0x10) ? 0x10 : 0);
            CTrailMap::iterator it = weapon->GetDef()->TrailGraphics.find(mask);
            if (it == weapon->GetDef()->TrailGraphics.end()) {
                if (aug->HasAugmentationOfType(0x10)) {
                    CTrailMap::iterator single = weapon->GetDef()->TrailGraphics.find(0x10);
                    if (single != weapon->GetDef()->TrailGraphics.end())
                        return graphic = single.Node->Graphic;
                }
                if (aug->HasAugmentationOfType(8)) {
                    CTrailMap::iterator single = weapon->GetDef()->TrailGraphics.find(8);
                    if (single != weapon->GetDef()->TrailGraphics.end())
                        return graphic = single.Node->Graphic;
                }
                if (aug->HasAugmentationOfType(4)) {
                    CTrailMap::iterator single = weapon->GetDef()->TrailGraphics.find(4);
                    if (single != weapon->GetDef()->TrailGraphics.end())
                        return graphic = single.Node->Graphic;
                }
                if (aug->HasAugmentationOfType(2)) {
                    CTrailMap::iterator single = weapon->GetDef()->TrailGraphics.find(2);
                    if (single != weapon->GetDef()->TrailGraphics.end())
                        return graphic = single.Node->Graphic;
                }
                if (aug->HasAugmentationOfType(1)) {
                    CTrailMap::iterator single = weapon->GetDef()->TrailGraphics.find(1);
                    if (single != weapon->GetDef()->TrailGraphics.end())
                        return graphic = single.Node->Graphic;
                }
            }
            else
                graphic = it.Node->Graphic;
        }
    }
    return graphic;
}
