// CTCInventoryBase::AddItemToInventory(long, long, bool, bool, long, bool) -- adds items of a type to their category; augmentations go to the owner's augmentation component instead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF32D, length 153 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: IsAugmentation(long) @ 0x005D98F0 (__fastcall), GetInventoryCategory(long) @ 0x005D97B0 (__fastcall),
// std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020, component 0x29 method @ 0x006CA400
// (manifest "CTCHero::SetTitle", propagated), find(first, last, ident) over the categories @ 0x005C08D3 (LTCG
// __fastcall: ecx = first, edx = last, ident pushed; manifest "_Umove<EWeaponClass*>" is a byte twin),
// NInventory::CCategory::AddItem(long, bool, long, long) @ 0x005BF060, virtual slot 84 (+0x150, item-added notification).
// Shape: the inline GetInterface keeps the key in the dead slot of `itemType` (held in ebx); the not-found and
// augmentation paths share the `xor al,al` exit.
// Layout: retail CTCInventoryBase ParentThing +0x04, Categories {first,last,end} +0x20 (0x2C-byte CCategory).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCAugmentations {                            // interface type 0x29
    void AddAugmentation(long itemType);             // 0x006CA400
};
namespace NInventory {
struct CCategory {
    unsigned char _body[0x2C];
    bool AddItem(long itemType, bool silent, long price, long count);   // 0x005BF060
};
}
bool __fastcall IsAugmentation_005d98f0(long itemType);                  // 0x005D98F0
long __fastcall GetInventoryCategory(long itemType);            // 0x005D97B0
NInventory::CCategory* __fastcall FindCategory(NInventory::CCategory* first, NInventory::CCategory* last, long ident);   // 0x005C08D3

struct CTCInventoryBase_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void OnItemAdded(long itemType, bool b, bool c);   // slot 84 (+0x150)
    CThing* ParentThing;                                       // +0x04
    unsigned char _pad_0x08[0x18];
    NInventory::CCategory* CategoriesFirst;                    // +0x20
    NInventory::CCategory* CategoriesLast;                     // +0x24
    NInventory::CCategory* CategoriesEnd;                      // +0x28
    bool AddItemToInventory(long itemType, long count, bool silent, bool notify, long price, bool flag);
};

bool CTCInventoryBase_Retail::AddItemToInventory(long itemType, long count, bool silent, bool notify, long price, bool flag)
{
    if (IsAugmentation_005d98f0(itemType)) {
        CTCAugmentations* augmentations;
        if (ParentThing->GetInterface(&augmentations, 0x29))
            augmentations->AddAugmentation(itemType);
    } else {
        NInventory::CCategory* category = FindCategory(CategoriesFirst, CategoriesLast, GetInventoryCategory(itemType));
        if (category->AddItem(itemType, silent, price, count)) {
            OnItemAdded(itemType, notify, flag);
            return true;
        }
    }
    return false;
}