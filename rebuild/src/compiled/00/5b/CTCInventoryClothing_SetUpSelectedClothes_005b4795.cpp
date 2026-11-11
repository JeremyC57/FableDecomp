// CTCInventoryClothing::SetUpSelectedClothes() -- rebuilds the worn-clothing map: every selected item that is not a whole suit and whose appearance modifier names a suit part is recorded as that part's item
// Retail 0x005B4795, 269 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: map<EClothingSuitPart, long>::clear 0x004DB795 (manifest "CTCCarryable::OnKill" is propagated),
// NInventory::CTCInventoryBase::GetAllSelectedItems(vector<long>&) const 0x005BEAF5, CTCBase::GetDefinitionManager const 0x006869D0,
// CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const CThingBaseDef>*) const 0x0042AF3C, CDefClassBase::GetSubDef<CHeroSuitDef> 0x005935C4 and
// GetSubDef<CAppearanceModifierDef> 0x0057FF92, map<EClothingSuitPart, long>::operator[](const key&) 0x005BBBF3, free
// 0x00BFEA14, the defs' slot 1 on release.
// Shape: the selected item vector is a zeroed local freed inline at the end; each positive item fetches its def, then
// (unless it is a suit) its appearance modifier into the def's now-dead out slot; a non-zero suit part maps to the item
// (the element address is formed before the map call); the modifier is released without a null test, the suit and the
// def with one (the shared zero register `esi`).
// Layout (retail): CurrentClothingSelected map +0x140 (as the landed ctor 0x005B7049); CAppearanceModifierDef suit part
// +0x4C.
#include "fable_compiler.h"
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);           // 0x00BFEA14

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:     // the out slot of the def lookups (released by hand at its scope end)
    CDefPointeeBase* Object;                         // the pointee as its non-const base (engine/CDefPointer.h)
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // GetSubDef<CHeroSuitDef> 0x005935C4 / <CAppearanceModifierDef> 0x0057FF92 (Ego_r / FableWin templates)
};
class CHeroSuitDef : public CDefClassBase { public:};
class CAppearanceModifierDef : public CDefClassBase { public:
    unsigned char _pad_0x08[0x4c - 0x08];
    long SuitPart;                                   // +0x4c EClothingSuitPart
};
class CThingBaseDef : public CDefClassBase { public: };
void* FindDef_s7r_0();
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work FindDef_s7r_0 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_0 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_0()
{
    long r = TmplSink_s7_0(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CHeroSuitDef>(CDefPointer<const CHeroSuitDef>* out) const
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
void* FindDef_s7r_1();
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindDef_s7r_1 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_1 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_1()
{
    long r = TmplSink_s7_1(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const CAppearanceModifierDef>* out) const
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
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef> (FableWin.pdb template)
};
void* FindDef_s7r_2();
long TmplSink_s7_2(const void* self, long seen);   // stand-in: the work FindDef_s7r_2 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_2 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_2()
{
    long r = TmplSink_s7_2(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CThingBaseDef>(long p0, CDefPointer<const CThingBaseDef>* out) const
{
    void* def = FindDef_s7r_2();
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
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc> class vector;
template <> class vector<long, allocator<long> > { public:   // STLport vector<long> (12 bytes; the members this row uses)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    FABLE_FORCEINLINE ~vector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    long& operator[](unsigned long n) { return *(_M_start + n); }
};
}
struct CSuitPartMap {                                // map<EClothingSuitPart, long>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    void clear();                                    // 0x004DB795
    long& operator[](const long& part);              // 0x005BBBF3
};

class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
class CTCBase { public:                              // the component base (members this row reads)
    virtual ~CTCBase();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
namespace NInventory {
class CTCInventoryBase : public CTCBase {
protected:
    void GetAllSelectedItems(std::vector<long, std::allocator<long> >& items) const;   // 0x005BEAF5 (Ego_r.pdb IBE)
};
}
class CTCInventoryClothing : public NInventory::CTCInventoryBase { public:
    unsigned char _pad_0x004[0x140 - 0x4];
    CSuitPartMap CurrentClothingSelected;            // +0x140
private:
    void SetUpSelectedClothes();                     // Ego_r.pdb ?SetUpSelectedClothes@CTCInventoryClothing@@AAEXXZ
};

void CTCInventoryClothing::SetUpSelectedClothes()
{
    CurrentClothingSelected.clear();
    std::vector<long, std::allocator<long> > selected;
    GetAllSelectedItems(selected);
    for (unsigned long i = 0; i < selected.size(); ++i) {
        if (selected[i] > 0) {
            CDefPointer<const CThingBaseDef> def;
            def.Object = 0;
            GetDefinitionManager().GetDef(selected[i], &def);
            CThingBaseDef* thingDef = static_cast<CThingBaseDef*>(def.Object);
            {
                CDefPointer<const CHeroSuitDef> suit;
                suit.Object = 0;
                if (!thingDef->GetSubDef(&suit)) {
                    CDefPointer<const CAppearanceModifierDef> modifierSlot;
                    modifierSlot.Object = 0;
                    thingDef->GetSubDef(&modifierSlot);
                    CAppearanceModifierDef* modifier = static_cast<CAppearanceModifierDef*>(modifierSlot.Object);
                    if (modifier->SuitPart != 0)
                        CurrentClothingSelected[modifier->SuitPart] = selected[i];
                    modifier->Release();
                }
                if (suit.Object)
                    suit.Object->Release();
            }
            if (thingDef)
                thingDef->Release();
        }
    }
}