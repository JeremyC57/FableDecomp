// CTCInventoryBase::AddCategory(CTCInventoryBase* inventory, long defIndex, CThing* owner) -- looks up the category def and, when present, appends a CCategory built from it and hands the new back() its owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C024A, length 119 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0, CDefinitionManager::GetDef<CInventoryCategoryDef>(long,
// CDefPointer*) @ 0x005C0C99 (manifest "GetDef<CSpecialAbilitiesDrunkennessDef>", byte twin), NInventory::CCategory::CCategory(
// CDefPointer<const CInventoryCategoryDef>, CTCInventoryBase*, long) @ 0x005BFCD9, vector<CCategory>::push_back @ 0x005C2D32,
// ~CCategory @ 0x005C2739 (manifest "~CCreatureAction_FireMissileWeapon", byte twin), CCategory owner setter (builds a
// CIntelligentPointer<CThing>) @ 0x005BD183 (manifest "swap<CTCBase::CRegisteredTCInfo>", byte twin).
// Shape: the GetDef out-slot starts null (`and [ebp-4],0`) and is read ONCE into a pointer local right after the call
// (esi, scheduled between `test al,al` and the branch); the by-value CDefPointer argument is built in the argument area
// by an inline add-ref constructor that tests its PARAMETER (testing the member re-reads the slot); the release at the
// end is written by hand on the same pointer local (null test, `dec [p+4] ; jne`, virtual slot 1). The temporary
// CCategory (0x2C bytes) occupies [ebp-30h].
// Layout: retail CTCInventoryBase Categories {first,last,end} at +0x20.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
struct CTCInventoryBase_Retail;
class CInventoryCategoryDef { public:
    virtual void v00();
    virtual void Destroy();                                       // slot 1 (+0x04)
    long RefCount;                                                // +0x04
};
struct CDefPointer {                                              // CDefPointer<const CInventoryCategoryDef>
    CInventoryCategoryDef* Object;
    CDefPointer() : Object(0) {}
    __forceinline CDefPointer(CInventoryCategoryDef* object) : Object(object)
    {
        if (object)
            ++object->RefCount;
    }
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Destroy();
        }
    }
};
struct CDefPointerSlot {                                          // the out-parameter view of a CDefPointer (released by hand)
    CInventoryCategoryDef* Object;
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                         // 0x0044C6B0
    bool GetDef(long defIndex, CDefPointerSlot* def);                 // 0x005C0C99
};
namespace NInventory {
class CCategory { public:
    unsigned char _body[0x2C];
    CCategory(CDefPointer def, CTCInventoryBase_Retail* inventory, long defIndex);   // 0x005BFCD9
    ~CCategory();                                                 // 0x005C2739
    void SetOwner(CThing* owner);                                 // 0x005BD183
};
}
struct CCategoryVector {
    NInventory::CCategory* _M_start;
    NInventory::CCategory* _M_finish;
    NInventory::CCategory* _M_end_of_storage;
    void push_back(const NInventory::CCategory& category);        // 0x005C2D32
    NInventory::CCategory& back() { return *(_M_finish - 1); }
};
struct CTCInventoryBase_Retail {
    void* __vftable;                                              // +0x00
    unsigned char _pad_0x04[0x1C];
    CCategoryVector Categories;                                   // +0x20
    void AddCategory(CTCInventoryBase_Retail* inventory, long defIndex, CThing* owner);
};
void CTCInventoryBase_Retail::AddCategory(CTCInventoryBase_Retail* inventory, long defIndex, CThing* owner)
{
    CDefPointerSlot def;
    def.Object = 0;
    bool found = CGameDefinitionManager::Get().GetDef(defIndex, &def);
    CInventoryCategoryDef* object = def.Object;
    if (found) {
        Categories.push_back(NInventory::CCategory(CDefPointer(object), inventory, defIndex));
        Categories.back().SetOwner(owner);
    }
    if (object) {
        if (--object->RefCount == 0)
            object->Destroy();
    }
}