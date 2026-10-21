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
#pragma optimize("s", on)

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
