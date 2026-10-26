// NUISystem::CFrontEndScreen::~CFrontEndScreen() -- release the left-click action, then the CChangingStateComponent base
// Leaf: ??1CFrontEndScreen@NUISystem@@UAE@XZ
// Retail: Fable.exe @ 0x0054E450, 95 bytes, tail `jmp` to ~CChangingStateComponent 0x0052CCF0. Stores the three
// CFrontEndScreen vptrs (0x012497E4 at +0, 0x012497BC at +4, 0x012497B4 at +0x18; COL .?AVCFrontEndScreen@NUISystem@@ :
// CChangingStateComponent : CComponent : CUIInterface, CObserver at +4, CActionParam at +0x18, per slots.py), then the
// PLeftClickAction counted-pointer destructor (+0x160: --count on the info block, re-read; at zero its destroy thunk on
// the item and operator delete 0x00BFE9BC on the block; Data = Info = 0).
// Layout: retail CFrontEndScreen PLeftClickAction +0x160 (Ego_r.pdb +0x190; engine/CFrontEndScreen.h carries the PDB
// offsets, retail's 12-byte STLport containers put it 0x30 lower). CCountedPointer model as the ??_G 0x0041BDF0 row.
// Shape: /O2 (16-aligned entry); empty destructor body.
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
namespace NUISystem {
class CAction;
class CUIInterface {
public:
    virtual ~CUIInterface();
};
class CObserver {
public:
    virtual ~CObserver();
    unsigned char _pad04[0x10];                          // observer body (+0x08..+0x17 of the screen)
};
class CActionParam {
public:
    virtual ~CActionParam();
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    virtual ~CComponent();
};
class CChangingStateComponent : public CComponent {
public:
    virtual ~CChangingStateComponent();                  // 0x0052CCF0
    unsigned char _pad1c[0x160 - 0x1c];                  // component and changing-state members
};
class CFrontEndScreen : public CChangingStateComponent {
public:
    CCountedPointer<CAction> PLeftClickAction;           // +0x160
    virtual ~CFrontEndScreen();
};
CFrontEndScreen::~CFrontEndScreen()
{
}
}