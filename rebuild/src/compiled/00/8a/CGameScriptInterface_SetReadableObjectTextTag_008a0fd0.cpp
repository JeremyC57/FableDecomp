// CGameScriptInterface::SetReadableObjectTextTag(const CScriptThing& thing, const CCharString& tag) const -- give a live readable object (interface 0xd) a new text tag
// Leaf: ?SetReadableObjectTextTag@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x008A0FD0, 107 bytes, `ret 8`; Ego_r.pdb same symbol.
// Body: t = thing.GetThing() (slot 11); if t, not dead (bit 0 of +0x91) and holding interface 0xd (bit 13 of
// UsingComponents word 0, +0x20): r = GetTC (non-const LowerBound 0x0040F020, key in the dead `thing` slot; no null
// test on r); r->SetTextTag(tag.ToWideString()) -- ToWideString 0x0099E480 (labelled GetDefaultVal<CAnimationSet>, a
// folded body), CWideString::operator= 0x0099B7D0 on TextTag +0x18, temp dtor 0x0099B510 (labelled ~CCharString, folded).
// Layout: CThing as the matched 0x0089A330 / 0x0089B0B0 rows. CTCActionUseReadable: Ego_r.pdb TextTag (CWideString) +0x18,
// inline ?SetTextTag@CTCActionUseReadable@@QAEXABVCWideString@@@Z. Interface 0xd = CTCActionUseReadable is
// inferred from that +0x18 store, not confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
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
class CWideString {
public:
    void* m_pData;
    CWideString& operator=(const CWideString& other);    // 0x0099B7D0 (returns const& in the PDB)
    ~CWideString();                                      // 0x0099B510
};
class CCharString {
public:
    void* m_pData;
    CWideString ToWideString() const;                    // 0x0099E480
};
class CTCActionUseReadable {                             // interface 0xd
public:
    enum { INTERFACE_TYPE = 0xd };
    unsigned char _pad00[0x18];
    CWideString TextTag;                                 // +0x18
    void SetTextTag(const CWideString& tag) { TextTag = tag; }
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
    virtual void SetReadableObjectTextTag(const CScriptThing& thing, const CCharString& tag) const;
};
void CGameScriptInterface::SetReadableObjectTextTag(const CScriptThing& thing, const CCharString& tag) const
{
    CThing* t = thing.GetThing();
    if (t && !t->IsDead())
    {
        CTCActionUseReadable* readable;
        if (t->GetTC(&readable))
            readable->SetTextTag(tag.ToWideString());
    }
}