// CGameScriptInterface::EntitySetAsLocked(const CScriptThing& thing, bool locked) const -- when the script thing is valid and its thing has a CTCActionUseScriptedHook component (interface type 0xC2), call CTCActionUseScriptedHook::SetEntranceAsLocked(locked)
// Leaf: ?EntitySetAsLocked@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089DC10, 91 bytes (+ int3 padding), thiscall, `ret 8`; CGameScriptInterface vtable 0x012615B8 slot 91.
// Identity: CGameScriptInterface::EntitySetAsLocked(const CScriptThing&, bool) const -- proven; evidence: FableWin.pdb name set holds
//   ?EntitySetAsLocked@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z (the catalog label agrees); it calls CTCActionUseScriptedHook::SetEntranceAsLocked
//   0x00729C30 (FableWin.pdb ?SetEntranceAsLocked@CTCActionUseScriptedHook@@QAEX_N@Z; landed) on the interface-type-0xC2 component. Family G045 (8 script setters).
// Callees: the script thing's virtual slot 75 (+0x12C, IsValid) and slot 11 (+0x2C, GetThing), CInterfaceMap::LowerBound
//   0x0040F020 (the thing's {type, component} map at +0x44), CTCActionUseScriptedHook::SetEntranceAsLocked(bool) 0x00729C30.
// Shape: `this` unused; the GetInterface template is `__forceinline` and the lookup key lives in the dead `thing` argument
//   slot; the component is not null-tested; the value is forwarded as a dword.
class CTCBase;
struct CInterfaceMap {                                       // a thing's sorted {type, component} vector map
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                          // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                        // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                            // slot 75 (+0x12c)
};
class CTCActionUseScriptedHook {
public:
    void SetEntranceAsLocked(bool locked);                              // 0x00729C30
};
class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface();
    virtual void EntitySetAsLocked(const CScriptThing& thing, bool locked) const;   // slot 91
};

void CGameScriptInterface::EntitySetAsLocked(const CScriptThing& thing, bool locked) const
{
    if (thing.IsValid()) {
        CThing* pThing = thing.GetThing();
        CTCActionUseScriptedHook* component;
        if (pThing->GetInterface(&component, 0xC2))
            component->SetEntranceAsLocked(locked);
    }
}