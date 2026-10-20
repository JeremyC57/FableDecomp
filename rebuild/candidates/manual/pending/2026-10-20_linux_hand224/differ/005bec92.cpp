// NInventory::CItem::AddItem(long, long, long) -- add items of a type
// Grade: DIFFER(ours v retail) -- the GetSubDef out slot gets its own frame word (retail shares [ebp-4] with the resize zero temporary and the loop index; frame 0xc v 8), the detached slot keeps an `and [slot],0`, and the def release re-reads the slot (`cmp [ecx+4],0`) instead of `dec [ecx+4] ; jne`
#pragma optimize("y", off)
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
bool __fastcall IsAugmentation(long itemType);     // 0x005D97F0
long __fastcall GetMaxNumberOfItems(long itemType);   // 0x005D99F0 (labelled GetInventoryCategory)
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
    void resize(unsigned long count, const T& value);   // 0x004428F0
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
    T* Detach() { T* p = Object; Object = 0; return p; }
};
template <class T> struct CDefHolder {             // adopts a reference (no addref)
    T* Object;
    CDefHolder(T* p) : Object(p) { }
    __forceinline ~CDefHolder() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CAugmentationDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    long NumberOfSlots;                            // +0x28
    unsigned char _pad2c[0x40 - 0x2c];
    vector<unsigned long> SlotValues;              // +0x40
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x0042AF9E
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
    template <class T> bool GetDef(long index, CDefPointer<T>& out) const;   // 0x0042AF3C
};
struct CPerItemData {                              // 0x24 bytes
    vector<unsigned long> SlotValues;              // +0x00
    unsigned char _body[0x24 - 0x0c];
    CPerItemData();                                // 0x005C14A6
};
struct CCountedPointer_PerItemData {
    CPerItemData* m_pObject;
    void*         m_pCount;
    void Reset(CPerItemData* object);              // 0x005C1946
};
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
struct CItem {
    long                        ItemType;          // +0x00
    long                        NumberOfItems;     // +0x04
    CCountedPointer_PerItemData PPerItemData;      // +0x08
    long                        AverageBoughtPrice;   // +0x10
    void AddItem(long itemType, long number, long price);
};

void CItem::AddItem(long itemType, long number, long price)
{
    if (NumberOfItems == 0) {
        AverageBoughtPrice = 0;
        if (IsAugmentation(itemType)) {
            PPerItemData.Reset(new CPerItemData());
            CDefPointer<CDefClassBase> def;
            CGameDefinitionManager::Get().GetDef(itemType, def);
            bool found;
            CAugmentationDef* raw;
            {
                CDefPointer<CAugmentationDef> out;
                found = def->GetSubDef(out);
                raw = out.Detach();
            }
            CDefHolder<CAugmentationDef> augmentation(raw);
            if (found) {
                PPerItemData.m_pObject->SlotValues.resize(augmentation->NumberOfSlots, 0);
                for (unsigned long i = 0; i < augmentation->SlotValues.size() && i < PPerItemData.m_pObject->SlotValues.size(); ++i)
                    PPerItemData.m_pObject->SlotValues[i] = augmentation->SlotValues[i];
            }
        }
    }
    ItemType = itemType;
    long room = GetMaxNumberOfItems(itemType) - NumberOfItems;
    long added = Min(number, room);
    if (price != -1) {
        AverageBoughtPrice = AverageBoughtPrice * NumberOfItems + added * price;
        NumberOfItems += added;
        AverageBoughtPrice /= NumberOfItems;
    } else {
        NumberOfItems += added;
    }
}
