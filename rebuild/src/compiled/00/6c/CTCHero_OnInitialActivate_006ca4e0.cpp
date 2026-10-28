// CTCHero::OnInitialActivate() -- relink the last equipped weapon from its saved UID, then restore the hero title: the saved title object def, else the hero def's DefaultTitle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA4E0, length 185 bytes (+ int3 padding). __fastcall, `ret`.
//         Ego_r.pdb: `?OnInitialActivate@CTCHero@@UAEXXZ`.
// Callees: CTCBase::GetThingManager @ 0x00686AE0, CThingManager::GetThingWithUID(__int64) @ 0x0051ED80;
//          CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C); CTCHero::SetTitle(long) @ 0x006CA400;
//          CDefClassBase::GetSubDef<CHeroDef>(CDefPointer<const T>*) const @ 0x00580034; the def's virtual slot 1.
// Shape: the weapon is assigned through a converting temporary (`SetPItem(GetPItem())`); a saved title returns at once;
//        the hero def's out slot lives in its own block (so it shares the temporary's frame word) and is declared after
//        the base-def read (its zero store follows that load), then read once into a local released by hand.
// Layout: Ego_r.pdb CTCHero matches retail: HeroTitleObjectDef +0x14, LastWeaponEquipped +0x1D (odd offset: the pointer
//         classes are pack(1)), LastWeaponEquippedID +0x28; retail CThing PBaseDef.Object +0x70; CHeroDef::DefaultTitle +0x28.
class CThing;
class CBaseObject;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem((CBaseObject*)other.GetPItem());
        return *this;
    }
};
#pragma pack(pop)

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00580034 for CHeroDef
};
class CHeroDef : public CDefClassBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    long DefaultTitle;                              // +0x28
};

class CThingManager {
public:
    CThing* GetThingWithUID(__int64 uid);           // 0x0051ED80
};

class CThing {
public:
    unsigned char _pad00[0x70];
    CDefClassBase* PBaseDef;                        // +0x70 CDefPointer<const CThingBaseDef>::Object
};

class CTCBase
{
protected:
    CThingManager& GetThingManager();   // 0x00686AE0
};

class CTCHero : public CTCBase {                                     // the vptr as a plain word (an __int64 member pads a real one to 8)
public:
    void* __vfptr;                                  // +0x00
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    bool WeaponsUsable;                             // +0x0C
    bool WillUsable;                                // +0x0D
    bool WeaponUseOverriddenByScripts;              // +0x0E
    bool WillUseOverriddenByScripts;                // +0x0F
    long Title;                                     // +0x10 EHeroTitle
    long HeroTitleObjectDef;                        // +0x14
    void* PHeroTitleSubDef;                         // +0x18 CDefPointer<const CHeroTitleDef>
    bool OnBoastingPodium;                          // +0x1C
    CIntelligentPointer<CThing> LastWeaponEquipped; // +0x1D
    unsigned char _pad25[3];
    __int64 LastWeaponEquippedID;                   // +0x28
    void SetTitle(long titleObjectDef);             // 0x006CA400
    void OnInitialActivate();
};

void CTCHero::OnInitialActivate()
{
    if (LastWeaponEquippedID != 0)
        LastWeaponEquipped = CIntelligentPointer<CThing>(GetThingManager().GetThingWithUID(LastWeaponEquippedID));
    if (HeroTitleObjectDef) {
        SetTitle(HeroTitleObjectDef);
        return;
    }
    {
        CDefClassBase* baseDef = ParentThing->PBaseDef;
        CDefPointer<const CHeroDef> out;
        if (baseDef) {
            bool found = baseDef->GetSubDef(&out);
            const CHeroDef* heroDef = out.Object;
            if (found && heroDef->DefaultTitle)
                SetTitle(heroDef->DefaultTitle);
            if (heroDef && --heroDef->RefCount == 0)
                heroDef->Destroy();
        }
    }
}