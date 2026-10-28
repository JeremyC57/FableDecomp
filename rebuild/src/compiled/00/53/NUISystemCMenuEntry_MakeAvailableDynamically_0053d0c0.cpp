// NUISystem::CMenuEntry::MakeAvailableDynamically() -- make the entry available dynamically (base CComponent step), then pass it on to the entry's text child; set IsAvailable
// Leaf: ?MakeAvailableDynamically@CMenuEntry@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053D0C0, 94 bytes, virtual thiscall, plain `ret`.
// Identity (proven): slot 95 (+0x17C) of CMenuEntry's vtable 0x0124662C, overriding CComponent's slot 95
//   (0x00530DA0, CComponent::MakeAvailableDynamically, called here qualified); slots 93/94/95 are 0x0053D000 / 0x0053D060 / 0x0053D0C0 in order.
// Callees: CComponent::MakeAvailableDynamically 0x00530DA0; the CComponent virtuals FindFirstChildOfType (slot 9), FindChildForward (slot 4,
//   CCharString by value, built in its argument slot by CCharString(const char*, long) 0x0099EBF0) and
//   FindFirstChildOfTypeNonRecursive (slot 97); the found child's MakeAvailableDynamically (slot 95).
// Layout: Ego_r.pdb CMenuEntry::IsAvailable +0x1C8, minus 0x30 for the retail STLport containers = +0x198.
// Shape: the text child is looked up three ways in turn, each hit forwarding the call; VC7.1 tail-merges the three
//   calls into one site (a single `if (text) text->X()` after the lookups reloads the vptr into edx, not eax).
//   The EType value 6 is the text type (its enumerator name is not recovered).
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);            // 0x0099EBF0
    CCharString(const CCharString& other);                      // 0x0099EC30
    ~CCharString();
};
namespace NUISystem {
enum EType { ETYPE_6 = 6 };
class CComponent;
class CUIInterface { public:
};
class CComponent : public CUIInterface { public:
    virtual void _v000(); virtual void _v001(); virtual void _v002(); virtual void _v003();
    virtual CComponent* FindChildForward(CCharString name);                  // slot 4 (+0x10), 0x00530830
    virtual void _v005(); virtual void _v006(); virtual void _v007(); virtual void _v008();
    virtual CComponent* FindFirstChildOfType(EType type);                    // slot 9 (+0x24), 0x00530460
    virtual void _v010(); virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014(); virtual void _v015();
    virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019(); virtual void _v020(); virtual void _v021();
    virtual void _v022(); virtual void _v023(); virtual void _v024(); virtual void _v025(); virtual void _v026(); virtual void _v027();
    virtual void _v028(); virtual void _v029(); virtual void _v030(); virtual void _v031(); virtual void _v032(); virtual void _v033();
    virtual void _v034(); virtual void _v035(); virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039();
    virtual void _v040(); virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044(); virtual void _v045();
    virtual void _v046(); virtual void _v047(); virtual void _v048(); virtual void _v049(); virtual void _v050(); virtual void _v051();
    virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055(); virtual void _v056(); virtual void _v057();
    virtual void _v058(); virtual void _v059(); virtual void _v060(); virtual void _v061(); virtual void _v062(); virtual void _v063();
    virtual void _v064(); virtual void _v065(); virtual void _v066(); virtual void _v067(); virtual void _v068(); virtual void _v069();
    virtual void _v070(); virtual void _v071(); virtual void _v072(); virtual void _v073(); virtual void _v074(); virtual void _v075();
    virtual void _v076(); virtual void _v077(); virtual void _v078(); virtual void _v079(); virtual void _v080(); virtual void _v081();
    virtual void _v082(); virtual void _v083(); virtual void _v084(); virtual void _v085(); virtual void _v086(); virtual void _v087();
    virtual void _v088(); virtual void _v089(); virtual void _v090(); virtual void _v091(); virtual void _v092();
    virtual void MakeUnavailableDynamically();                               // slot 93 (+0x174), 0x00530B00
    virtual void MakeAvailable();                                            // slot 94 (+0x178), 0x00530CE0
    virtual void MakeAvailableDynamically();                                 // slot 95 (+0x17C), 0x00530DA0
    virtual void _v096();
    virtual CComponent* FindFirstChildOfTypeNonRecursive(EType type);        // slot 97 (+0x184), 0x00530610
};
class CChangingStateComponent : public CComponent { public:
};
class CHoverable : public CChangingStateComponent { public:
};
class CClickable : public CHoverable { public:
};
class CMenuEntry : public CClickable { public:
    unsigned char _pad04[0x194];
    bool IsAvailable;                                           // +0x198
    virtual void MakeAvailableDynamically();
};
}

void NUISystem::CMenuEntry::MakeAvailableDynamically()
{
    if (!IsAvailable) {
        CComponent::MakeAvailableDynamically();
        CComponent* text = FindFirstChildOfType(ETYPE_6);
        if (text)
            text->MakeAvailableDynamically();
        else if ((text = FindChildForward(CCharString("MENU_ENTRY_TEXT"))) != 0)
            text->MakeAvailableDynamically();
        else if ((text = FindFirstChildOfTypeNonRecursive(ETYPE_6)) != 0)
            text->MakeAvailableDynamically();
        IsAvailable = true;
    }
}