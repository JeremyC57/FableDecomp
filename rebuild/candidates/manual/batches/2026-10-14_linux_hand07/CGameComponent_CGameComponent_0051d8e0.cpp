// CGameComponent::CGameComponent(CGame* pGame) -- one-argument constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051D8E0, length 49 bytes.
// Manifest label "CAIStateGroup_GazeAtHome" is propagated; the vtables stored are 0x0122ECB0 (+0) and
// 0x0122ECA4 (+4), both CGameComponent (rtti-vtables.tsv): a class with two polymorphic bases.
// Callee: CBase::CBase @ 0x0099A2F0 (first base, out of line); the second base's constructor is inline
// and only stores its vtable 0x0122DE50 at +4 (the same listener base as CMeshBankManager @ 0x00A0A110).
//
// Shape: base #1 ctor, base #2 vtable, then the initialiser list: two false flags at +8/+9 and the owner
// pointer argument at +0xc, then both derived vtables (sunk below the member stores by the scheduler).
// `ret 4`: one stack argument. Member names are descriptive.
struct CGame;

struct CBase {
    virtual void v00();
    CBase();                                    // 0x0099A2F0
};

struct CGameListener {                          // second base, vtable 0x0122DE50
    virtual void v00();
    CGameListener() { }
};

struct CGameComponent : CBase, CGameListener {
    bool m_initialised;                         // +0x08
    bool m_active;                              // +0x09
    CGame* m_pGame;                             // +0x0c
    virtual void v00();
    CGameComponent(CGame* pGame);
};

CGameComponent::CGameComponent(CGame* pGame)
    : CBase()
    , CGameListener()
    , m_initialised(false)
    , m_active(false)
    , m_pGame(pGame)
{
}
