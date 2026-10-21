// test for 0055b9a0 CHoverable::OnUnhovered
#include <stdio.h>
#include <string.h>
namespace NUISystem {
class CAction;
template <class T> class CCountedPointer { T* Data; void* Info; };
}
namespace std { template <class T> class list; }
namespace NUISystem {
typedef std::list<CCountedPointer<CAction> > TActionList;
class CHoverable {
public:
    virtual ~CHoverable();                               // slot 0
    virtual void _slot001();
    virtual void _slot002();
    virtual void _slot003();
    virtual void _slot004();
    virtual void _slot005();
    virtual void _slot006();
    virtual void _slot007();
    virtual void _slot008();
    virtual void _slot009();
    virtual void _slot010();
    virtual void _slot011();
    virtual void _slot012();
    virtual void _slot013();
    virtual void _slot014();
    virtual void _slot015();
    virtual void _slot016();
    virtual void _slot017();
    virtual void _slot018();
    virtual void _slot019();
    virtual void _slot020();
    virtual void _slot021();
    virtual void _slot022();
    virtual void _slot023();
    virtual void _slot024();
    virtual void _slot025();
    virtual void _slot026();
    virtual void _slot027();
    virtual void _slot028();
    virtual void _slot029();
    virtual void _slot030();
    virtual void _slot031();
    virtual void _slot032();
    virtual void _slot033();
    virtual void _slot034();
    virtual void _slot035();
    virtual void _slot036();
    virtual void _slot037();
    virtual void _slot038();
    virtual void _slot039();
    virtual void _slot040();
    virtual void _slot041();
    virtual void _slot042();
    virtual void _slot043();
    virtual void _slot044();
    virtual void _slot045();
    virtual void _slot046();
    virtual void _slot047();
    virtual void ChangeState(unsigned long state);       // slot 48
    virtual void _slot049();
    virtual void _slot050();
    virtual void _slot051();
    virtual void _slot052();
    virtual void _slot053();
    virtual void _slot054();
    virtual void _slot055();
    virtual void _slot056();
    virtual void _slot057();
    virtual void _slot058();
    virtual void _slot059();
    virtual void _slot060();
    virtual void _slot061();
    virtual void _slot062();
    virtual void _slot063();
    virtual void _slot064();
    virtual void _slot065();
    virtual void _slot066();
    virtual void _slot067();
    virtual void _slot068();
    virtual void _slot069();
    virtual void _slot070();
    virtual void _slot071();
    virtual void _slot072();
    virtual void _slot073();
    virtual void _slot074();
    virtual void _slot075();
    virtual void _slot076();
    virtual void _slot077();
    virtual void _slot078();
    virtual void _slot079();
    virtual void _slot080();
    virtual void _slot081();
    virtual void _slot082();
    virtual void _slot083();
    virtual void _slot084();
    virtual void _slot085();
    virtual void _slot086();
    virtual void _slot087();
    virtual void _slot088();
    virtual void _slot089();
    virtual void _slot090();
    virtual void _slot091();
    virtual void _slot092();
    virtual void _slot093();
    virtual void _slot094();
    virtual void _slot095();
    virtual void _slot096();
    virtual void _slot097();
    virtual void _slot098();
    virtual void _slot099();
    virtual void _slot100();
    virtual void _slot101();
    virtual void _slot102();
    virtual void _slot103();
    virtual void _slot104();
    virtual void _slot105();
    virtual void _slot106();
    virtual void _slot107();
    virtual void _slot108();
    virtual void _slot109();
    virtual void _slot110();
    virtual void _slot111();
    virtual void _slot112();
    virtual void _slot113();
    virtual void _slot114();
    virtual void _slot115();
    virtual void _slot116();
    virtual void _slot117();
    virtual void _slot118();
    virtual void _slot119();
    virtual void _slot120();
    virtual void _slot121();
    virtual void _slot122();
    virtual void _slot123();
    virtual void _slot124();
    virtual void _slot125();
    virtual void _slot126();
    virtual void _slot127();
    virtual void _slot128();
    virtual void _slot129();
    virtual void _slot130();
    virtual void ExecuteActions(TActionList* actions);   // slot 131
    virtual void _slot132();
    virtual void _slot133();
    virtual void _slot134();
    virtual void _slot135();
    virtual void _slot136();
    virtual void _slot137();
    virtual void _slot138();
    virtual void _slot139();
    virtual void _slot140();
    virtual void _slot141();
    virtual void _slot142();
    virtual void _slot143();
public:
    virtual void OnUnhovered();                          // slot 144
public:
    unsigned char _pad04[0x158];                         // +0x04
    long          StateBeforeHovered;                    // +0x15c
    bool          WasHovered;                            // +0x160
    TActionList*  PActionsOnHovered;                     // +0x164
    TActionList*  PActionsOnUnhovered;                   // +0x168
};
void CHoverable::OnUnhovered()
{
    ChangeState(StateBeforeHovered);
    ExecuteActions(PActionsOnUnhovered);
}
}

static unsigned long g_state = 0xffff; static void* g_actions = (void*)1; static int g_order;
NUISystem::CHoverable::~CHoverable() {}
void NUISystem::CHoverable::_slot001() {}
void NUISystem::CHoverable::_slot002() {}
void NUISystem::CHoverable::_slot003() {}
void NUISystem::CHoverable::_slot004() {}
void NUISystem::CHoverable::_slot005() {}
void NUISystem::CHoverable::_slot006() {}
void NUISystem::CHoverable::_slot007() {}
void NUISystem::CHoverable::_slot008() {}
void NUISystem::CHoverable::_slot009() {}
void NUISystem::CHoverable::_slot010() {}
void NUISystem::CHoverable::_slot011() {}
void NUISystem::CHoverable::_slot012() {}
void NUISystem::CHoverable::_slot013() {}
void NUISystem::CHoverable::_slot014() {}
void NUISystem::CHoverable::_slot015() {}
void NUISystem::CHoverable::_slot016() {}
void NUISystem::CHoverable::_slot017() {}
void NUISystem::CHoverable::_slot018() {}
void NUISystem::CHoverable::_slot019() {}
void NUISystem::CHoverable::_slot020() {}
void NUISystem::CHoverable::_slot021() {}
void NUISystem::CHoverable::_slot022() {}
void NUISystem::CHoverable::_slot023() {}
void NUISystem::CHoverable::_slot024() {}
void NUISystem::CHoverable::_slot025() {}
void NUISystem::CHoverable::_slot026() {}
void NUISystem::CHoverable::_slot027() {}
void NUISystem::CHoverable::_slot028() {}
void NUISystem::CHoverable::_slot029() {}
void NUISystem::CHoverable::_slot030() {}
void NUISystem::CHoverable::_slot031() {}
void NUISystem::CHoverable::_slot032() {}
void NUISystem::CHoverable::_slot033() {}
void NUISystem::CHoverable::_slot034() {}
void NUISystem::CHoverable::_slot035() {}
void NUISystem::CHoverable::_slot036() {}
void NUISystem::CHoverable::_slot037() {}
void NUISystem::CHoverable::_slot038() {}
void NUISystem::CHoverable::_slot039() {}
void NUISystem::CHoverable::_slot040() {}
void NUISystem::CHoverable::_slot041() {}
void NUISystem::CHoverable::_slot042() {}
void NUISystem::CHoverable::_slot043() {}
void NUISystem::CHoverable::_slot044() {}
void NUISystem::CHoverable::_slot045() {}
void NUISystem::CHoverable::_slot046() {}
void NUISystem::CHoverable::_slot047() {}
void NUISystem::CHoverable::_slot049() {}
void NUISystem::CHoverable::_slot050() {}
void NUISystem::CHoverable::_slot051() {}
void NUISystem::CHoverable::_slot052() {}
void NUISystem::CHoverable::_slot053() {}
void NUISystem::CHoverable::_slot054() {}
void NUISystem::CHoverable::_slot055() {}
void NUISystem::CHoverable::_slot056() {}
void NUISystem::CHoverable::_slot057() {}
void NUISystem::CHoverable::_slot058() {}
void NUISystem::CHoverable::_slot059() {}
void NUISystem::CHoverable::_slot060() {}
void NUISystem::CHoverable::_slot061() {}
void NUISystem::CHoverable::_slot062() {}
void NUISystem::CHoverable::_slot063() {}
void NUISystem::CHoverable::_slot064() {}
void NUISystem::CHoverable::_slot065() {}
void NUISystem::CHoverable::_slot066() {}
void NUISystem::CHoverable::_slot067() {}
void NUISystem::CHoverable::_slot068() {}
void NUISystem::CHoverable::_slot069() {}
void NUISystem::CHoverable::_slot070() {}
void NUISystem::CHoverable::_slot071() {}
void NUISystem::CHoverable::_slot072() {}
void NUISystem::CHoverable::_slot073() {}
void NUISystem::CHoverable::_slot074() {}
void NUISystem::CHoverable::_slot075() {}
void NUISystem::CHoverable::_slot076() {}
void NUISystem::CHoverable::_slot077() {}
void NUISystem::CHoverable::_slot078() {}
void NUISystem::CHoverable::_slot079() {}
void NUISystem::CHoverable::_slot080() {}
void NUISystem::CHoverable::_slot081() {}
void NUISystem::CHoverable::_slot082() {}
void NUISystem::CHoverable::_slot083() {}
void NUISystem::CHoverable::_slot084() {}
void NUISystem::CHoverable::_slot085() {}
void NUISystem::CHoverable::_slot086() {}
void NUISystem::CHoverable::_slot087() {}
void NUISystem::CHoverable::_slot088() {}
void NUISystem::CHoverable::_slot089() {}
void NUISystem::CHoverable::_slot090() {}
void NUISystem::CHoverable::_slot091() {}
void NUISystem::CHoverable::_slot092() {}
void NUISystem::CHoverable::_slot093() {}
void NUISystem::CHoverable::_slot094() {}
void NUISystem::CHoverable::_slot095() {}
void NUISystem::CHoverable::_slot096() {}
void NUISystem::CHoverable::_slot097() {}
void NUISystem::CHoverable::_slot098() {}
void NUISystem::CHoverable::_slot099() {}
void NUISystem::CHoverable::_slot100() {}
void NUISystem::CHoverable::_slot101() {}
void NUISystem::CHoverable::_slot102() {}
void NUISystem::CHoverable::_slot103() {}
void NUISystem::CHoverable::_slot104() {}
void NUISystem::CHoverable::_slot105() {}
void NUISystem::CHoverable::_slot106() {}
void NUISystem::CHoverable::_slot107() {}
void NUISystem::CHoverable::_slot108() {}
void NUISystem::CHoverable::_slot109() {}
void NUISystem::CHoverable::_slot110() {}
void NUISystem::CHoverable::_slot111() {}
void NUISystem::CHoverable::_slot112() {}
void NUISystem::CHoverable::_slot113() {}
void NUISystem::CHoverable::_slot114() {}
void NUISystem::CHoverable::_slot115() {}
void NUISystem::CHoverable::_slot116() {}
void NUISystem::CHoverable::_slot117() {}
void NUISystem::CHoverable::_slot118() {}
void NUISystem::CHoverable::_slot119() {}
void NUISystem::CHoverable::_slot120() {}
void NUISystem::CHoverable::_slot121() {}
void NUISystem::CHoverable::_slot122() {}
void NUISystem::CHoverable::_slot123() {}
void NUISystem::CHoverable::_slot124() {}
void NUISystem::CHoverable::_slot125() {}
void NUISystem::CHoverable::_slot126() {}
void NUISystem::CHoverable::_slot127() {}
void NUISystem::CHoverable::_slot128() {}
void NUISystem::CHoverable::_slot129() {}
void NUISystem::CHoverable::_slot130() {}
void NUISystem::CHoverable::_slot132() {}
void NUISystem::CHoverable::_slot133() {}
void NUISystem::CHoverable::_slot134() {}
void NUISystem::CHoverable::_slot135() {}
void NUISystem::CHoverable::_slot136() {}
void NUISystem::CHoverable::_slot137() {}
void NUISystem::CHoverable::_slot138() {}
void NUISystem::CHoverable::_slot139() {}
void NUISystem::CHoverable::_slot140() {}
void NUISystem::CHoverable::_slot141() {}
void NUISystem::CHoverable::_slot142() {}
void NUISystem::CHoverable::_slot143() {}
void NUISystem::CHoverable::ChangeState(unsigned long s) { g_state = s; g_order = g_order * 10 + 1; }
void NUISystem::CHoverable::ExecuteActions(NUISystem::TActionList* a) { g_actions = a; g_order = g_order * 10 + 2; }
int main()
{
    NUISystem::CHoverable* h = new NUISystem::CHoverable;
    h->StateBeforeHovered = 7; h->PActionsOnUnhovered = (NUISystem::TActionList*)0x5150; h->PActionsOnHovered = 0;
    h->OnUnhovered();
    int ok = g_state == 7 && g_actions == (void*)0x5150 && g_order == 12
          && (char*)&h->StateBeforeHovered - (char*)h == 0x15c && (char*)&h->PActionsOnUnhovered - (char*)h == 0x168;
    printf(ok ? "0055b9a0_TEST PASS\n" : "0055b9a0_TEST FAIL\n");
    return ok ? 0 : 1;
}