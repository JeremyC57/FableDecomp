// CGameScriptInterface::EntityResetAsPureAINarrator(const CScriptThing& thing) const -- for a valid creature thing, set the +0x34 flag of its interface-0xf component
// Leaf: ?EntityResetAsPureAINarrator@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Retail: Fable.exe @ 0x0089B0B0, 87 bytes, `ret 4`; virtual (vtable reference 0x012614B8); Ego_r.pdb
// ?EntityResetAsPureAINarrator@CGameScriptInterface@@UBEXABVCScriptThing@@@Z.
// Body: if thing.IsValid() (slot 75, +0x12c): t = thing.GetThing() (slot 11, +0x2c); if t is a creature (type byte +0x10
// == 1) and holds interface 0xf (bit 15 of UsingComponents +0x20), the component from the Components map (LowerBound
// 0x0040F020, key in the dead `thing` slot) gets its +0x34 byte = true.
// Layout: CThing as the matched 0x0089D8B0 / 0x0089D7C0 rows plus the type byte +0x10 (the landed 0x0089D830 model:
// 1 = a creature). The component class and the +0x34 member name are not confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
struct CTCBase;
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
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
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
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
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
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
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
class CTCInterface0F {                                  // interface 0xf (class not confirmed)
public:
    enum { INTERFACE_TYPE = 0xf };
    unsigned char _pad00[0x34];
    bool          PureAINarrator;                        // +0x34 (name not confirmed)
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
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
    virtual bool IsValid() const;                        // slot 75 (+0x12c)
};
class CGameScriptInterface {
public:
    virtual void EntityResetAsPureAINarrator(const CScriptThing& thing) const;
    void* World;                                         // +0x04
};
void CGameScriptInterface::EntityResetAsPureAINarrator(const CScriptThing& thing) const
{
    if (thing.IsValid())
    {
        CThing* t = thing.GetThing();
        CTCInterface0F* tc;
        if (t->Type == 1 && t->GetTC(&tc))
            tc->PureAINarrator = true;
    }
}