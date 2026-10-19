// CGameScriptInterface::SetThingAsUsable(const CScriptThing& thing, bool usable) const -- set the usable flag on the
// Grade: DIFFER(ours v retail) -- 203v205: hook arms keep separate tails (ours mov eax,[eax+4] + store in each arm; retail one hook local in esi, AddTC result mov esi,eax before the string dtor)
// thing's usable component (TC 0x0D), else on its scripted-use hook (TC 0xC2), adding the hook when it has neither
// Leaf: ?SetThingAsUsable@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008967F0, length 208 bytes, `ret 8`.
// Callees: the script thing's virtual slot 75 IsValid (+0x12C) and slot 11 GetThing (+0x2C), the TC map lower bound
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CCharString::CCharString(const char*, long)
// @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60, ~CCharString @ 0x0099EAE0.
// String 0x01239FE4 = "CTCActionUseScriptedHook".
// Layout: CThing interface bitset +0x20 (TC 0x0D = +0x20 bit 13, TC 0xC2 = +0x38 bit 2), TC map +0x44 {first,last};
// TC 0x0D's usable byte +0x10, CTCActionUseScriptedHook::Usable +0x0C.
// Shape: the out-parameter GetInterface template (landed 0x00896000 model) homes both lookup keys in the dead `thing`
// argument slot; the hook name is a named CCharString in its own block (built in the same dead slot); the hook
// pointer is one local assigned on both hook paths, so the final store is tail-duplicated per return.

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CTCBase;
struct CTCParamsBase;

struct CInterfaceMap_Retail {                    // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct CTCUsable_0D {                     // the TC 0x0D component
    unsigned char _pad[0x10];
    bool Usable;                          // +0x10
};

struct CTCActionUseScriptedHook {
    unsigned char _pad[0xc];
    bool Usable;                          // +0x0C
};

struct CThing {
    virtual void v000();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];       // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail m_interfaces;           // +0x44
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&interfaceType);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;              // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};

struct CGameScriptInterface {
    virtual void SetThingAsUsable(const CScriptThing& thing, bool usable) const;
};

void CGameScriptInterface::SetThingAsUsable(const CScriptThing& thing, bool usable) const
{
    if (thing.IsValid()) {
        CThing* t = thing.GetThing();
        CTCUsable_0D* usableTC;
        CTCActionUseScriptedHook* hook;
        if (t->GetInterface(&usableTC, 0x0d)) {
            usableTC->Usable = usable;
        } else if (t->GetInterface(&hook, 0xc2)) {
            hook->Usable = usable;
        } else {
            CCharString name("CTCActionUseScriptedHook", -1);
            hook = (CTCActionUseScriptedHook*)t->AddTC(name, false, 0);
            hook->Usable = usable;
        }
    }
}
