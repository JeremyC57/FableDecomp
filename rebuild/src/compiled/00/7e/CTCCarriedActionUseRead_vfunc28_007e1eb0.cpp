// CTCCarriedActionUseRead::vfunc_28(CThing* user) -- the first read of a book applies its bonus-item def to the reader's hero stats, then the book's interface-0x1a component is notified
// Leaf: vfunc_28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1EB0, 187 bytes (`ret 4`). OnActionUse(CThing&) slot 28.
// Callees: CDefClassBase::GetSubDef<CBonusItemDef> @ 0x007E1D30, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020
// (two inlined GetInterface lookups), CTCHeroStats::ApplyBonusItemEffects(CBonusItemDef*) @ 0x0057C4F1, the interface-0x1a
// component's OnShowEnded @ 0x005D8E50 (descriptive, as CEventShownItem 0x008116D0), the def's virtual slot 1 on the last release.
// Layout: CTCCarriedActionUseRead (Ego_r.pdb 0x1c) AlreadyRead +0x18; CThing bitset +0x20 (hero stats = type 4, `test byte
// [+20h],10h`; type 0x1a = bit 26), component map +0x44, def +0x70.
// Shape: the def object is taken out of the out pointer right after GetSubDef into a holder (ebx) whose destructor releases it
// with `dec [ebx+4] ; jne` (GOTCHAS 24f/retry 8: a holder with a direct `--count`); the AlreadyRead store sits inside the block.
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
struct CBonusItemDef : CDefPointeeBase { };
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def taken out of the out pointer, released at the end of the block
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CBonusItemDef>* out);    // 0x007E1D30
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CBonusItemDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                // +0x70
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};
struct CTCHeroStats {
    void ApplyBonusItemEffects(CBonusItemDef* def);     // 0x0057C4F1
};
struct CTCShowableItem {
    void OnShowEnded();                                 // 0x005D8E50
};
struct CTCCarriedActionUseRead {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x18 - 0x08];
    bool AlreadyRead;                                   // +0x18
    void vfunc_28(CThing* user);
};

void CTCCarriedActionUseRead::vfunc_28(CThing* user)
{
    if (!AlreadyRead) {
        CTCHeroStats* stats;
        CDefClassBase* thingDef = ParentThing->PDef;
        CDefPointer<CBonusItemDef> out;
        bool found = thingDef->GetSubDef(&out);
        CDefHolder<CBonusItemDef> def(out.Object);
        if (found && user->GetInterface(&stats, 4))
            stats->ApplyBonusItemEffects(def.Object);
        AlreadyRead = true;
    }
    if (ParentThing->HasTC(0x1a))
        ((CTCShowableItem*)ParentThing->GetTC(0x1a))->OnShowEnded();
}