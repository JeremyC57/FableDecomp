// CGameScriptInterface::EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing& entity, bool able) const -- script call: sets a live wife's permitted-to-region-follow flag (CTCWife, interface 0xd7)
// Leaf: ?EntitySetAsAbleToRegionFollowWhenMarried@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Retail: Fable.exe @ 0x008978E0, 86 bytes, thiscall `ret 8`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 207 holds this body; slot 207 is
//   ?EntitySetAsAbleToRegionFollowWhenMarried@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z. Interface 0xd7 = CTCWife
//   (vtable 0x0125ED5C slot 21 = landed 0x006D4540, returns 0xd7). Setter: FableWin.pdb
//   ?SetAsPermittedToRegionFollow@CTCWife@@QAEX_N@Z, inline (byte +0x1c; member name descriptive).
// Body: t = entity.GetThing() (slot 11); live t holding interface 0xd7 (UsingComponents word 6 bit 23): GetTC (non-const
//   LowerBound 0x0040F020) and store the flag.
class CTCBase;
class CInterfaceMap {                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const long* type);          // 0x0040F020
    const SEntry* LowerBound(const long* type) const;    // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (T*)it->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (const T*)it->component;
        return true;
    }
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
};
class CTCWife {                                         // interface 0xd7
public:
    enum { INTERFACE_TYPE = 0xd7 };
    unsigned char _pad00[0x1c];
    bool PermittedToRegionFollow;                        // +0x1c
    void SetAsPermittedToRegionFollow(bool permitted) { PermittedToRegionFollow = permitted; }
};
class CGameScriptInterface {
public:
    virtual void EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing& entity, bool able) const;
};
void CGameScriptInterface::EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing& entity, bool able) const
{
    CThing* t = entity.GetThing();
    CTCWife* wife;
    if (t && !t->IsDead() && t->GetTC(&wife))
        wife->SetAsPermittedToRegionFollow(able);
}