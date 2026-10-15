// NUISystem::CRedefinerList::CRedefinerList(CComponent*) -- the control-redefinition list: CScrollableList(parent), not active, no fade pending, no undefined controls, no warning / accept components
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00556350, length 77 bytes.
// Callees: NUISystem::CScrollableList::CScrollableList(CComponent*) @ 0x00555180 (out of line; it stores the
// CScrollableList vtables 0x0124A72C / 0x0124A708 / 0x0124A700; the manifest's "CTCHeroPodium::CTCHeroPodium" is
// propagated). Called by 0x0041D3B2.
// Label correction: the manifest calls this "CTCWeapon::CTCWeapon"; the vtables it stores at +0, +4 and +0x18,
// 0x0124A994 / 0x0124A970 / 0x0124A968, are NUISystem::CRedefinerList's (rtti-vtables.tsv).
// Layout: Ego_r.pdb CRedefinerList (448 bytes, members from +0x1ac) sits 0x30 above retail; the retail members follow
// the CScrollableList base at +0x17c in the PDB order: IsActivated +0x17c, RefreshAfterFade +0x17d, FadeOutTime +0x180,
// HaveUndefinedControls +0x184, PUndefinedWarning +0x188, PAcceptButton +0x18c. The base has three vptrs (+0, +4,
// +0x18): a primary polymorphic base plus two secondary ones (wave 24o rule).
// Shape: one zero register feeds every initialiser in declaration order, the 0.0f FadeOutTime included (VC7.1 shares
// the integer zero register for a float zero here); the three derived vtable stores are scheduled after the members.
namespace NUISystem {

struct CComponent;

struct CUIPrimaryBase {                                     // vptr +0x00
    virtual void p00();
};

struct CUISecondaryBaseA {                                  // vptr +0x04, 0x14 bytes
    virtual void a00();
    unsigned char _body[0x10];
};

struct CUISecondaryBaseB {                                  // vptr +0x18
    virtual void b00();
};

struct CScrollableList : CUIPrimaryBase, CUISecondaryBaseA, CUISecondaryBaseB {   // 0x17c bytes in retail
    unsigned char _body[0x17c - 0x1c];
    CScrollableList(CComponent* parent);                    // 0x00555180
};

struct CRedefinerList : CScrollableList {
    bool          IsActivated;                              // +0x17c
    bool          RefreshAfterFade;                         // +0x17d
    float         FadeOutTime;                              // +0x180
    bool          HaveUndefinedControls;                    // +0x184
    CComponent*   PUndefinedWarning;                        // +0x188
    CComponent*   PAcceptButton;                            // +0x18c
    virtual void p00();                                     // vtable 0x0124A994
    virtual void a00();                                     // vtable 0x0124A970
    virtual void b00();                                     // vtable 0x0124A968
    CRedefinerList(CComponent* parent);
};

CRedefinerList::CRedefinerList(CComponent* parent)
    : CScrollableList(parent)
    , IsActivated(false)
    , RefreshAfterFade(false)
    , FadeOutTime(0.0f)
    , HaveUndefinedControls(false)
    , PUndefinedWarning(0)
    , PAcceptButton(0)
{
}

}