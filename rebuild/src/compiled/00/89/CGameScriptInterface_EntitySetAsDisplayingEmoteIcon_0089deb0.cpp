// CGameScriptInterface::EntitySetAsDisplayingEmoteIcon(const CScriptThing& thing, bool display) const -- enable or disable the emote icon of a valid thing with a CTCEmoteIcon (interface 0x45)
// Leaf: ?EntitySetAsDisplayingEmoteIcon@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089DEB0, thiscall, virtual.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 530; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb
//   vtable order) names it ?EntitySetAsDisplayingEmoteIcon@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z.
// Guard: the script thing is valid (slot 75); the component is CTCEmoteIcon (interface 0x45: GetInterfaceType of its RTTI vtable, or its Ego_r.pdb method).
// Callees: CScriptThing slots 75/11, CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&) 0x0040F020 (the inlined CThing::GetInterface), none (an inline store).
// Shape: the landed L083/L084 GetInterface model (2026-10-27_linux_hand955); the lookup key is homed in the dead thing-argument slot.
// Layout: local models -- engine/CGameScriptInterface.h carries its vptr as a data member and the CThing/CScriptThing/TC headers are flat
//   Ego_r layouts (Dinkumware offsets); component offsets are retail (Ego_r minus 4 per STLport container before the member).
enum ETCInterfaceType { ETCInterfaceType_FIRST };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                     // +0x00 (CArray: STLport vector)
    value_type* Last;                                      // +0x04
    value_type* EndOfStorage;                              // +0x08
    TCompare    Compare;                                   // +0x0C
    bool        Dirty;                                     // +0x0D
    value_type* end() { return Last; }
    value_type* LowerBound(const K& key);                  // 0x0040F020 for the interface map
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0xC];
    unsigned char ThingType;                               // +0x10 (1 = creature; Ego_r +0x10 is unnamed)
    unsigned char _pad11[0xF];
    unsigned long UsingComponents[9];                      // +0x20 (bitset<274> of present interface types)
    CInterfaceMap Components;                              // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                                // +0x91 bit 0 (Ego_r +0xA1)
    template <class T> __forceinline bool GetInterface(T** out, ETCInterfaceType type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                      // slot 11 (+0x2C)
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
    virtual bool IsValid() const;                          // slot 75 (+0x12C)
};
class CTCEmoteIcon {
public:
    unsigned char _pad00[0x26];
    bool          Enabled;                                // +0x26 (Ego_r)
};
class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface();
    virtual void EntitySetAsDisplayingEmoteIcon(const CScriptThing& thing, bool display) const;
};
void CGameScriptInterface::EntitySetAsDisplayingEmoteIcon(const CScriptThing& thing, bool display) const
{
    if (thing.IsValid()) {
        CTCEmoteIcon* icon;
        if (thing.GetThing()->GetInterface(&icon, (ETCInterfaceType)0x45))
            icon->Enabled = display;
    }
}