#ifndef _MSC_VER
#define __forceinline inline
#define __cdecl
#define __fastcall
#define __stdcall
#endif

// CTCInventory::RevertVideoChanges() -- with the video options screen up (state 4), put every video slider and text
// slider of every PC screen back to its stored value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00582754, 496 bytes, plain `ret`.
// Leaf: RevertVideoChanges
// Callees: CCharString(const char*, long) @ 0x0099EBF0 (each name built in its by-value argument slot), the screen's
// FindChild (virtual slot 3, +0x0C), the found slider's revert (slot 149, +0x254) and text slider's revert (slot 142, +0x238).
// Layout: retail CTCInventory PC screens vector<CComponent*> at +0x0C, PC inventory state at +0x21C (4 = video options);
// called from the landed CTCInGameMenu::ConfirmedVideoOptionsNotAccepted (0x0047DA50).
// Shape: size TU without a frame pointer (`optimize("s", on)` only): the loop index lives in the `push ecx` slot, every
// lookup re-reads the vector start and the element (ebx) and its vtable (ebp) before the name is built.

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
namespace NUISystem {
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name); // slot 3 (+0x0C)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); 
    virtual void RevertTextSlider_slot142();         // slot 142 (+0x238)
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); 
    virtual void RevertSlider_slot149();             // slot 149 (+0x254)
};
}

struct CComponentVector {                            // std::vector<NUISystem::CComponent*>
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent* operator[](unsigned long n) const { return _M_start[n]; }
};

struct CTCInventory {
    unsigned char    _pad_0x00[0x0C];
    CComponentVector PCScreens;                      // +0x0C
    unsigned char    _pad_0x18[0x21C - 0x18];
    long             PCInventoryState;               // +0x21C
    void RevertVideoChanges();
};

void CTCInventory::RevertVideoChanges()
{
    if (PCInventoryState == 4) {
        for (unsigned long i = 0; i < PCScreens.size(); ++i) {
            NUISystem::CComponent* child;
            if ((child = PCScreens[i]->FindChild(CCharString("PC_SLIDER_TEXTURE_DETAIL", -1))) != 0)
                child->RevertSlider_slot149();
            if ((child = PCScreens[i]->FindChild(CCharString("PC_SLIDER_MESH_DETAIL", -1))) != 0)
                child->RevertSlider_slot149();
            if ((child = PCScreens[i]->FindChild(CCharString("PC_SLIDER_SHADOW_DETAIL", -1))) != 0)
                child->RevertSlider_slot149();
            if ((child = PCScreens[i]->FindChild(CCharString("PC_SLIDER_BRIGHTNESS", -1))) != 0)
                child->RevertSlider_slot149();
            if ((child = PCScreens[i]->FindChild(CCharString("PC_SLIDER_EFFECTS_DETAIL", -1))) != 0)
                child->RevertSlider_slot149();
            if ((child = PCScreens[i]->FindChild(CCharString("PC_OPTIONS_RESOLUTION_TEXT_SLIDER", -1))) != 0)
                child->RevertTextSlider_slot142();
            if ((child = PCScreens[i]->FindChild(CCharString("PC_OPTIONS_REFRESH_RATE_TEXT_SLIDER", -1))) != 0)
                child->RevertTextSlider_slot142();
            if ((child = PCScreens[i]->FindChild(CCharString("PC_OPTIONS_ANTIALIASING_TEXT_SLIDER", -1))) != 0)
                child->RevertTextSlider_slot142();
            if ((child = PCScreens[i]->FindChild(CCharString("PC_OPTIONS_VERTICAL_SYNC_TEXT_SLIDER", -1))) != 0)
                child->RevertTextSlider_slot142();
            if ((child = PCScreens[i]->FindChild(CCharString("PC_OPTIONS_SCREEN_ASPECT_RATIO_TEXT_SLIDER", -1))) != 0)
                child->RevertTextSlider_slot142();
        }
    }
}

#include <stdio.h>
#include <string.h>
static int g_built = 0, g_freed = 0;
CCharString::CCharString(const char* text, long) { PStringData = (CCharStringData*)text; ++g_built; }
CCharString::CCharString(const CCharString& o) { PStringData = o.PStringData; ++g_built; }
CCharString::~CCharString() { ++g_freed; }
static char g_log[2048];
namespace NUISystem {
void CComponent::v000() { }
void CComponent::v001() { }
void CComponent::v002() { }
void CComponent::v004() { }
void CComponent::v005() { }
void CComponent::v006() { }
void CComponent::v007() { }
void CComponent::v008() { }
void CComponent::v009() { }
void CComponent::v010() { }
void CComponent::v011() { }
void CComponent::v012() { }
void CComponent::v013() { }
void CComponent::v014() { }
void CComponent::v015() { }
void CComponent::v016() { }
void CComponent::v017() { }
void CComponent::v018() { }
void CComponent::v019() { }
void CComponent::v020() { }
void CComponent::v021() { }
void CComponent::v022() { }
void CComponent::v023() { }
void CComponent::v024() { }
void CComponent::v025() { }
void CComponent::v026() { }
void CComponent::v027() { }
void CComponent::v028() { }
void CComponent::v029() { }
void CComponent::v030() { }
void CComponent::v031() { }
void CComponent::v032() { }
void CComponent::v033() { }
void CComponent::v034() { }
void CComponent::v035() { }
void CComponent::v036() { }
void CComponent::v037() { }
void CComponent::v038() { }
void CComponent::v039() { }
void CComponent::v040() { }
void CComponent::v041() { }
void CComponent::v042() { }
void CComponent::v043() { }
void CComponent::v044() { }
void CComponent::v045() { }
void CComponent::v046() { }
void CComponent::v047() { }
void CComponent::v048() { }
void CComponent::v049() { }
void CComponent::v050() { }
void CComponent::v051() { }
void CComponent::v052() { }
void CComponent::v053() { }
void CComponent::v054() { }
void CComponent::v055() { }
void CComponent::v056() { }
void CComponent::v057() { }
void CComponent::v058() { }
void CComponent::v059() { }
void CComponent::v060() { }
void CComponent::v061() { }
void CComponent::v062() { }
void CComponent::v063() { }
void CComponent::v064() { }
void CComponent::v065() { }
void CComponent::v066() { }
void CComponent::v067() { }
void CComponent::v068() { }
void CComponent::v069() { }
void CComponent::v070() { }
void CComponent::v071() { }
void CComponent::v072() { }
void CComponent::v073() { }
void CComponent::v074() { }
void CComponent::v075() { }
void CComponent::v076() { }
void CComponent::v077() { }
void CComponent::v078() { }
void CComponent::v079() { }
void CComponent::v080() { }
void CComponent::v081() { }
void CComponent::v082() { }
void CComponent::v083() { }
void CComponent::v084() { }
void CComponent::v085() { }
void CComponent::v086() { }
void CComponent::v087() { }
void CComponent::v088() { }
void CComponent::v089() { }
void CComponent::v090() { }
void CComponent::v091() { }
void CComponent::v092() { }
void CComponent::v093() { }
void CComponent::v094() { }
void CComponent::v095() { }
void CComponent::v096() { }
void CComponent::v097() { }
void CComponent::v098() { }
void CComponent::v099() { }
void CComponent::v100() { }
void CComponent::v101() { }
void CComponent::v102() { }
void CComponent::v103() { }
void CComponent::v104() { }
void CComponent::v105() { }
void CComponent::v106() { }
void CComponent::v107() { }
void CComponent::v108() { }
void CComponent::v109() { }
void CComponent::v110() { }
void CComponent::v111() { }
void CComponent::v112() { }
void CComponent::v113() { }
void CComponent::v114() { }
void CComponent::v115() { }
void CComponent::v116() { }
void CComponent::v117() { }
void CComponent::v118() { }
void CComponent::v119() { }
void CComponent::v120() { }
void CComponent::v121() { }
void CComponent::v122() { }
void CComponent::v123() { }
void CComponent::v124() { }
void CComponent::v125() { }
void CComponent::v126() { }
void CComponent::v127() { }
void CComponent::v128() { }
void CComponent::v129() { }
void CComponent::v130() { }
void CComponent::v131() { }
void CComponent::v132() { }
void CComponent::v133() { }
void CComponent::v134() { }
void CComponent::v135() { }
void CComponent::v136() { }
void CComponent::v137() { }
void CComponent::v138() { }
void CComponent::v139() { }
void CComponent::v140() { }
void CComponent::v141() { }
void CComponent::v143() { }
void CComponent::v144() { }
void CComponent::v145() { }
void CComponent::v146() { }
void CComponent::v147() { }
void CComponent::v148() { }

NUISystem::CComponent* CComponent::FindChild(CCharString) { return 0; }
void CComponent::RevertTextSlider_slot142() { }
void CComponent::RevertSlider_slot149() { }
}
struct CChild : NUISystem::CComponent {
    const char* name;
    void RevertTextSlider_slot142() { strcat(g_log, "T:"); strcat(g_log, name); strcat(g_log, ";"); }
    void RevertSlider_slot149() { strcat(g_log, "S:"); strcat(g_log, name); strcat(g_log, ";"); }
};
static CChild g_children[16];
static int g_nchildren = 0;
struct CScreen : NUISystem::CComponent {
    int id;
    NUISystem::CComponent* FindChild(CCharString name)
    {
        const char* n = (const char*)name.PStringData;
        if (id == 1 && (strcmp(n, "PC_SLIDER_MESH_DETAIL") == 0 || strcmp(n, "PC_OPTIONS_VERTICAL_SYNC_TEXT_SLIDER") == 0)) {
            g_children[g_nchildren].name = n;
            return &g_children[g_nchildren++];
        }
        if (id == 2 && strcmp(n, "PC_SLIDER_BRIGHTNESS") == 0) {
            g_children[g_nchildren].name = n;
            return &g_children[g_nchildren++];
        }
        return 0;
    }
};
int main()
{
    CScreen a, b;
    a.id = 1; b.id = 2;
    NUISystem::CComponent* screens[2] = { &a, &b };
    CTCInventory inv;
    memset(&inv, 0, sizeof(inv));
    inv.PCScreens._M_start = screens;
    inv.PCScreens._M_finish = screens + 2;
    inv.PCScreens._M_end_of_storage = screens + 2;
    inv.PCInventoryState = 3;
    inv.RevertVideoChanges();
    int ok = g_log[0] == 0 && g_built == 0;
    inv.PCInventoryState = 4;
    inv.RevertVideoChanges();
    ok = ok && strcmp(g_log, "S:PC_SLIDER_MESH_DETAIL;T:PC_OPTIONS_VERTICAL_SYNC_TEXT_SLIDER;S:PC_SLIDER_BRIGHTNESS;") == 0;
    ok = ok && g_built >= 20 && g_built == g_freed;
    printf(ok ? "00582754_TEST PASS\n" : "00582754_TEST FAIL %s %d %d\n", g_log, g_built, g_freed);
    return ok ? 0 : 1;
}
