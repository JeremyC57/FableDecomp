// NInventory::CItem::AddItem(long, long, long) -- add items of a type
// Leaf: ?AddItem@CItem@NInventory@@QAEXJJJ@Z
// NF93 (q11232): the local CPerItemData / CCountedPointer_PerItemData / CItem stand-ins replaced by engine/NInventory_CTCInventoryBase.h
//   (CItem, CPerItemData with its inline ctor, CCountedPointer<CPerItemData>): the ctor call 0x005C14A6 now binds
//   ??0CPerItemData@NInventory@@QAE@XZ (FableWin.pdb) and Reset 0x005C1946 the CCountedPointer<NInventory::CPerItemData> member.
// Callees (NF14): CDefinitionManager::GetDef<CThingBaseDef> const 0x0042AF3C, CDefClassBase::GetSubDef<CObjectAugmentationsDef> const
//   0x0042AF9E (its StatGetTypeName literal 0x0122FF9C is "CObjectAugmentationsDef"; was spelt GetSubDef<CAugmentationDef>).
#include "fable_compiler.h"
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
bool __fastcall IsAugmentation(long itemType);     // 0x005D97F0
// NF64 (round 7, q10648): the free stand-in `GetMaxNumberOfItems` is the static __fastcall CTCInventoryItem::GetMaxNumberOfItems (Ego_r.pdb / FableWin.pdb; the
//   NF59 self row defines it), declared by engine/CTCInventoryItem.h.
// NF64 (round 7, q10639): the local vector<T> stand-in replaced by stlport/fable_containers.h; both members are vector<long> in Ego_r.pdb
//   (CObjectAugmentationsDef::InitialAugmentations, CPerItemData::Augmentations). resize kept out of line (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE),
//   binding ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXIABJ@Z at 0x004428F0.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_GETMAXNUMBEROFITEMS
#include "engine/CTCInventoryItem.h"
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR
#define FABLE_RETAIL_NINVENTORY_CITEM_ADDITEM
#include "engine/NInventory_CTCInventoryBase.h"
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
};
template <class T> class CDefPointer { public:
    CDefBase* Object;                              // the pointee as its non-const base (engine/CDefPointer.h)
    CDefPointer() : Object(0) { }
    FABLE_FORCEINLINE ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return static_cast<T*>(Object); }
    CDefBase* Detach() { CDefBase* p = Object; Object = 0; return p; }
};
template <class T> struct CDefHolder {             // adopts a reference (no addref)
    T* Object;
    CDefHolder(T* p) : Object(p) { }
    FABLE_FORCEINLINE ~CDefHolder() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0042AF9E GetSubDef<CObjectAugmentationsDef> (Ego_r / FableWin template)
};
class CObjectAugmentationsDef : public CDefClassBase { public:   // Ego_r.pdb layout, retail STLport 12-byte map
    unsigned char _pad08[0x28 - 0x08];
    long NumberOfSlots;                            // +0x28
    unsigned char _pad2c[0x40 - 0x2c];
    std::vector<long> InitialAugmentations;        // +0x40 (PDB +0x44, Ego_r.pdb vector<long>)
};
class CThingBaseDef : public CDefClassBase { public: };
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef> (FableWin.pdb template)
};
class CGameDefinitionManager : public CDefinitionManager { public:   // RTTI CGameDefinitionManager : CDefinitionManager
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
void* FindDef_s7r_1();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long p0, CDefPointer<const T>* out) const
{
    void* def = FindDef_s7r_1();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
void NInventory::CItem::AddItem(long itemType, long number, long price)
{
    if (NumberOfItems == 0) {
        AverageBoughtPrice = 0;
        if (IsAugmentation(itemType)) {
            PPerItemData.Reset(new CPerItemData());
            CDefPointer<const CThingBaseDef> def;
            CGameDefinitionManager::Get().GetDef(itemType, &def);
            bool found;
            CObjectAugmentationsDef* raw;
            {
                CDefPointer<const CObjectAugmentationsDef> out;
                found = def->GetSubDef(&out);
                raw = static_cast<CObjectAugmentationsDef*>(out.Detach());
            }
            CDefHolder<CObjectAugmentationsDef> augmentation(raw);
            if (found) {
                PPerItemData->Augmentations.resize(augmentation->NumberOfSlots, 0);
                for (unsigned long i = 0; i < augmentation->InitialAugmentations.size() && i < PPerItemData->Augmentations.size(); ++i)
                    PPerItemData->Augmentations[i] = augmentation->InitialAugmentations[i];
            }
        }
    }
    ItemType = itemType;
    long room = CTCInventoryItem::GetMaxNumberOfItems(itemType) - NumberOfItems;
    long added = Min(number, room);
    if (price != -1) {
        AverageBoughtPrice = AverageBoughtPrice * NumberOfItems + added * price;
        NumberOfItems += added;
        AverageBoughtPrice /= NumberOfItems;
    } else {
        NumberOfItems += added;
    }
}