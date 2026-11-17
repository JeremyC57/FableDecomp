// NUISystem::CFrontEndManager::GotoProfileMenu() -- refresh the profile list and switch from the profile screen to the profile menu (or the no-profile screen), else go to the menu as a new screen
// Leaf: ?GotoProfileMenu@CFrontEndManager@NUISystem@@IAEXXZ
// Name (NF78, round 7, q10954): Ego_r.pdb 0x006041FF / FableWin.pdb 0x0201D482 ?GotoProfileMenu@CFrontEndManager@NUISystem@@IAEXXZ (protected);
//   was modelled public (QAE). Access only.
// Retail: Fable.exe @ 0x00597B20, 210 bytes, plain `ret`.
// Body: profiles = RefreshAvailableProfiles() (0x00596CCE, Ego_r.pdb IAEJXZ); if m_stackScreens.size() > 1 (0x0059A8F3)
// and top() (0x0059B039) is m_mapScreens[11] (map::operator[] 0x0059B5D7, evaluated first): leaving = top();
// leaving->OnScreenEvent(6) (slot 48); CManager::GetInstance()->RemoveObserver(leaving) (slot 5, CObserver base +4);
// pop() (0x0059AEE5); if/else each assigning PScreenToActivate = m_mapScreens[7] / m_mapScreens[12] (retail hoists the
// shared lea/push and branches only the key store); PScreenToDeactivate = leaving;
// push(PScreenToActivate) (0x0059B61C). Otherwise GotoNextScreen(m_mapScreens[7], false) (0x00596763).
// Layout: as the matched 0x0059686D (m_stackScreens +0x20, m_mapScreens +0x54, PScreenToDeactivate +0x98,
// PScreenToActivate +0x9c).
// Shape: a ternary key (m_mapScreens[profiles ? 7 : 12]) gives neg/sbb; a key variable + if puts the key in the wrong
// stack slot ([ebp-4] instead of reusing [ebp-8]); only the duplicated-lookup if/else matches.
// Opt: size TU (unaligned entry) with a frame pointer.
// NF86 (round 7, q11112 / q11113): the local std::deque / stack (and map) models are replaced by stlport/fable_deque.h (NF79 / NF83 opt-ins
//   STACK_TOP_POP + STACK_SIZE_PUSH + DEQUE_PUSH_BACK_OUTOFLINE, as NF83's 0x0059672A, and the NF86 overlay opt-in FABLE_RETAIL_STLP_STACK_EMPTY) and stlport/fable_map.h (map<FRONTEND_SCREEN, CComponent*>::operator[] 0x0059B5D7 out of line, unchanged):
//   size() now binds the STLport _M_subtract 0x0059A8F3 (was ??G?$_Deque_iterator@..), and back 0x0059B039 / pop_back 0x0059AEE5 /
//   push_back 0x0059B61C bind the deque<NUISystem::CComponent*, allocator<..> > spellings their rows define (were the allocator-less forms).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_STLP_DEQUE_PUSH_BACK_OUTOFLINE
#define FABLE_RETAIL_STLP_STACK_TOP_POP
#define FABLE_RETAIL_STLP_STACK_SIZE_PUSH
#define FABLE_RETAIL_STLP_STACK_EMPTY
#include "stlport/fable_deque.h"
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
enum FRONTEND_SCREEN { FRONTEND_SCREEN_PROFILE_MENU = 7, FRONTEND_SCREEN_11 = 11, FRONTEND_SCREEN_NO_PROFILES = 12 };
namespace NUISystem {
class CObserver {
public:
    virtual void _o00();
};
class CUIInterface {
public:
    virtual void _u00();
};
class CComponent : public CUIInterface, public CObserver {
public:
    virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005(); virtual void _v006();
    virtual void _v007(); virtual void _v008(); virtual void _v009(); virtual void _v010(); virtual void _v011(); virtual void _v012();
    virtual void _v013(); virtual void _v014(); virtual void _v015(); virtual void _v016(); virtual void _v017(); virtual void _v018();
    virtual void _v019(); virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023(); virtual void _v024();
    virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030();
    virtual void _v031(); virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035(); virtual void _v036();
    virtual void _v037(); virtual void _v038(); virtual void _v039(); virtual void _v040(); virtual void _v041(); virtual void _v042();
    virtual void _v043(); virtual void _v044(); virtual void _v045(); virtual void _v046(); virtual void _v047();
    virtual void OnScreenEvent(long event);              // slot 48 (+0xc0, name not recovered; CComponent's own is empty)
};
class CManager {
public:
    virtual void _m00(); virtual void _m01(); virtual void _m02(); virtual void _m03(); virtual void _m04();
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2
};
class CFrontEndManager {
public:
    unsigned char                          _pad00[0x20];
    std::stack<CComponent*>                m_stackScreens;   // +0x20
    unsigned char                          _pad48[0x54 - 0x48];
    std::map<FRONTEND_SCREEN, CComponent*> m_mapScreens;     // +0x54
    unsigned char                          _pad60[0x98 - 0x60];
    CComponent*                            PScreenToDeactivate;   // +0x98
    CComponent*                            PScreenToActivate;     // +0x9c
    void GotoNextScreen(CComponent* screen, bool flag);  // 0x00596763
protected:
    void GotoProfileMenu();                              // NF78 (q10954): protected in both PDBs
    // NF75 (round 7, land1117fu q10937): Ego_r.pdb 0x00603398 / FableWin.pdb 0x02018987 ?RefreshAvailableProfiles@CFrontEndManager@NUISystem@@IAEJXZ
    //   (protected; engine/CFrontEndManager.h REFRESHAVAILABLEPROFILES), the spelling the self row 0x00596CCE now defines; was public (QAE).
    long RefreshAvailableProfiles();                     // 0x00596CCE
public:
};
void CFrontEndManager::GotoProfileMenu()
{
    long profiles = RefreshAvailableProfiles();
    if (m_stackScreens.size() > 1 && m_stackScreens.top() == m_mapScreens[FRONTEND_SCREEN_11])
    {
        CComponent* leaving = m_stackScreens.top();
        leaving->OnScreenEvent(6);
        CManager::GetInstance()->RemoveObserver(leaving);
        m_stackScreens.pop();
        if (profiles)
            PScreenToActivate = m_mapScreens[FRONTEND_SCREEN_PROFILE_MENU];
        else
            PScreenToActivate = m_mapScreens[FRONTEND_SCREEN_NO_PROFILES];
        PScreenToDeactivate = leaving;
        m_stackScreens.push(PScreenToActivate);
    }
    else
        GotoNextScreen(m_mapScreens[FRONTEND_SCREEN_PROFILE_MENU], false);
}
}