// NUISystem::CSpellContainer::AcceptAssignments() -- forget the pending spell and make the current children the new base spells
// Leaf: AcceptAssignments
// Retail: Fable.exe (TLC, VC7.1) @ 0x00545670, 163 bytes (plain `ret`).
// Callees: each released count block's `__fastcall` destroy thunk (+0x04, on the item at +0x08), operator delete @ 0x00BFE9BC.
// Label: CSpellContainer vtable 0x0124786C slot 146 (rtti-vtables.tsv); Ego_r.pdb names
// ?AcceptAssignments@CSpellContainer@NUISystem@@UAEXXZ.
// Layout: CComponent::Children (vector<CCountedPointer<CComponent>>, 12 bytes) at retail +0xb0; Ego_r.pdb CSpellContainer
// BaseSpells +0x18c (the same vector type), NewSpell (pair<CComponent*, unsigned long>) +0x19c = retail +0x15c / +0x168.
// CCountedPointer is {Data, Info}, Info = {Count, destroy thunk, item} (the landed AddActionOnLeftClicked 0x00992470 model).
// Shape: an index loop re-evaluating Children.size() (`sar 3`, unsigned) each pass with the index spilled to the `push ecx`
// word across the release; BaseSpells[i] = Children[i] is the inline counted-pointer assignment with the source Info read
// BEFORE its Data, the old count decremented and re-read through the member (`dec [eax] ; mov eax,[esi+4] ; cmp [eax],0`).
void operator delete(void* block);               // 0x00BFE9BC

struct CCPPointerInfo {
    long  Count;                                   // +0x00
    void (__fastcall* Destroy)(void* item);        // +0x04
    void* Item;                                    // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        CCPPointerInfo* info = o.Info;
        T* data = o.Data;
        if (Info != info) {
            if (Info) {
                --Info->Count;
                if (Info->Count == 0) {
                    Info->Destroy(Info->Item);
                    ::operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->Count;
        }
        return *this;
    }
};
template <class T> class CCountedVector {         // STLport vector<CCountedPointer<T>> (12 bytes)
public:
    CCountedPointer<T>* _M_start;
    CCountedPointer<T>* _M_finish;
    CCountedPointer<T>* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CCountedPointer<T>& operator[](unsigned long n) { return _M_start[n]; }
};

namespace NUISystem {
class CComponent {
public:
    virtual void v000();
    unsigned char _pad04[0xb0 - 4];
    CCountedVector<CComponent> Children;           // +0xb0
    unsigned char _padbc[0x15c - 0xbc];
};
class CSpellContainer : public CComponent {
public:
    CCountedVector<CComponent> BaseSpells;         // +0x15c
    CComponent*   NewSpellComponent;               // +0x168 NewSpell.first
    unsigned long NewSpellIndex;                   // +0x16c NewSpell.second
    void AcceptAssignments();
};

void CSpellContainer::AcceptAssignments()
{
    NewSpellComponent = 0;
    NewSpellIndex = 0;
    for (unsigned long i = 0; i < Children.size(); ++i)
        BaseSpells[i] = Children[i];
}
}