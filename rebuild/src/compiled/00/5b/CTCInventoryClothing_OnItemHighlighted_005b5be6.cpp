// CTCInventoryClothing::OnItemHighlighted(long itemDefIndex) -- while the inventory is open, previews the highlighted clothing on the mannequin: a whole suit directly, otherwise the selected clothes with the highlighted piece in its suit part
// Retail 0x005B5BE6, 274 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: NInventory::CTCInventoryBase::OnItemHighlighted(long) 0x005BC4C0 (manifest "$E2"), CTCBase::GetDefinitionManager
// const 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const CThingBaseDef>*) const 0x0042AF3C,
// CDefClassBase::GetSubDef<CHeroSuitDef> 0x005935C4 and GetSubDef<CAppearanceModifierDef> 0x0057FF92,
// SetThingAsWearingClothingSuit(CThing&, long) 0x005B3D7A, SetUpSelectedClothes() 0x005B4795, map<EClothingSuitPart, long>::
// operator[] 0x005BBBF3, SetThingAsWearingSelectedClothes(CThing&) 0x005B406D, the defs' slot 1 on release.
// Shape: the def and the appearance modifier are out slots released by hand (the modifier without a null test, the def
// re-read from its slot with one), the suit sub-def a CDefPointer with an inline null-tested release; every compare
// uses the shared zero register (ebx); a non-positive index only re-applies the selected clothes.
// Layout (retail): InventoryOpen +0x58, CurrentClothingSelected +0x140, PMannequin +0x158, UseMannequin +0x15C (as the
// landed ctor 0x005B7049); CAppearanceModifierDef suit part +0x4C.
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing;
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
struct CSuitPartMap {                                // map<EClothingSuitPart, long>
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    long& operator[](const long& part);              // 0x005BBBF3
};

class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
class CTCBase { public:                              // the component base (members this row reads)
    virtual ~CTCBase();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
namespace NInventory {
class CTCInventoryBase : public CTCBase { public:
    virtual void OnItemHighlighted(long itemDefIndex);   // 0x005BC4C0 (Ego_r.pdb UAE; the empty base body, called qualified)
};
}
class CTCInventoryClothing : public NInventory::CTCInventoryBase { public:
    unsigned char _pad_0x004[0x58 - 0x4];
    bool InventoryOpen;                              // +0x58
    unsigned char _pad_0x059[0x140 - 0x59];
    CSuitPartMap CurrentClothingSelected;            // +0x140
    unsigned char _pad_0x14c[0x158 - 0x14c];
    CThing* PMannequin;                              // +0x158
    bool UseMannequin;                               // +0x15c
    virtual void OnItemHighlighted(long itemDefIndex);   // Ego_r.pdb ?OnItemHighlighted@CTCInventoryClothing@@UAEXJ@Z
private:
    void SetThingAsWearingClothingSuit(CThing& thing, long suitDefIndex);   // 0x005B3D7A (Ego_r.pdb AAE)
    void SetUpSelectedClothes();                     // 0x005B4795 (Ego_r.pdb AAE)
    void SetThingAsWearingSelectedClothes(CThing& thing);   // 0x005B406D (Ego_r.pdb AAE)
};

void CTCInventoryClothing::OnItemHighlighted(long itemDefIndex)
{
    if (!InventoryOpen)
        return;
    NInventory::CTCInventoryBase::OnItemHighlighted(itemDefIndex);
    if (!PMannequin)
        return;
    if (itemDefIndex > 0) {
        CDefPointer<const CThingBaseDef> def;
        def.Object = 0;
        GetDefinitionManager().GetDef(itemDefIndex, &def);
        {
            CDefPointer<const CHeroSuitDef> suit;
            suit.Object = 0;
            if (static_cast<CThingBaseDef*>(def.Object)->GetSubDef(&suit)) {
                if (UseMannequin)
                    SetThingAsWearingClothingSuit(*PMannequin, itemDefIndex);
            } else {
                SetUpSelectedClothes();
                CDefPointer<const CAppearanceModifierDef> modifierSlot;
                modifierSlot.Object = 0;
                static_cast<CThingBaseDef*>(def.Object)->GetSubDef(&modifierSlot);
                CAppearanceModifierDef* modifier = static_cast<CAppearanceModifierDef*>(modifierSlot.Object);
                if (modifier->SuitPart != 0)
                    CurrentClothingSelected[modifier->SuitPart] = itemDefIndex;
                if (UseMannequin)
                    SetThingAsWearingSelectedClothes(*PMannequin);
                modifier->Release();
            }
            if (suit.Object)
                suit.Object->Release();
        }
        if (def.Object)
            def.Object->Release();
    } else {
        SetUpSelectedClothes();
        if (UseMannequin)
            SetThingAsWearingSelectedClothes(*PMannequin);
    }
}