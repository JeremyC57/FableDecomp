// CTCInventoryTrade::RefreshNameAndTitle(NUISystem::CComponent* entry, long defIndex, long count) -- write the item's name into the entry's MENU_ENTRY_TEXT and UI_TEXT_TRADE_NAME_TEMPLATE_NEW children, followed by " x N" (localised multiplier and count) for stacks of more than one non-special item
// Leaf: RefreshNameAndTitle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059CFBD, length 251 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) +
// ("s",on). FableWin.pdb ?RefreshNameAndTitle@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@JJ@Z: called from BuyItem
// (0x005AF64E) and the console sell path (0x005AECC0); the PC twin RefreshNameAndTitlePC is 0x0059CA95, the same body with
// "PC_SUBTITLE_INDEPENDANT".
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("MENU_ENTRY_TEXT" @ 0x01230B80, "UI_TEXT_TRADE_NAME_TEMPLATE_NEW" @
// 0x012302B8), NUISystem::CComponent virtual slots 4 (+0x10) and 8 (+0x20) (child lookups by name, CCharString by value) and
// 144 (+0x240, SetText(const CWideString&, bool, bool)), CWideString() @ 0x0099AED0 / operator= @ 0x0099B7D0 / operator+=
// @ 0x0099B8D0 / ~ @ 0x0099B510, GetDescription(long) @ 0x005D95C0 (`__fastcall`, CWideString by hidden pointer; manifest
// name), CTCInventoryTrade::IsSpecialCase(long) @ 0x0059C983, IntToWideString(long) @ 0x00405F00, GetXString() @ 0x005CC772,
// operator+(const CWideString&, const CWideString&) @ 0x0099BE70 (`__fastcall`, hidden result).
// Shape: the landed FinalRefreshSubtitles (0x0059D761) model: the lookup names are implicit conversions built in place
// (`push ecx ; mov ecx,esp`, `this` parked in [ebp-8]); the name local lives in the dead `entry` slot and the count arm's
// temporaries in the dead `defIndex` / `count` slots and the parked-this word; ebx = 1 serves `count > 1` and both bools.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall GetXString();                    // 0x005CC772
CWideString __fastcall IntToWideString(long value);     // 0x00405F00
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall GetDescription(long defIndex);   // 0x005D95C0
namespace NUISystem {
struct CTextSource {                                    // the typed child: only its text slot is used here
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95();
    virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual const CWideString& GetText();               // slot 142 (+0x238)
};
class CComponent { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);    // slot 3 (+0x0c)
    virtual CComponent* FindChildRecursive(CCharString name);   // slot 4 (+0x10) (name descriptive)
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual CComponent* FindChildOfAnyDepth(CCharString name);  // slot 8 (+0x20) (name descriptive)
    virtual CTextSource* GetChildOfType(long type);     // slot 9 (+0x24)
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81();
    virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87();
    virtual void v88(); virtual void v89(); virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93();
    virtual void v94(); virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
};
}

class CTCInventoryTrade { public:
    bool IsSpecialCase(long defIndex);                  // 0x0059C983
    void RefreshNameAndTitle(NUISystem::CComponent* entry, long defIndex, long count);
};

void CTCInventoryTrade::RefreshNameAndTitle(NUISystem::CComponent* entry, long defIndex, long count)
{
    NUISystem::CComponent* text = entry->FindChildRecursive("MENU_ENTRY_TEXT");
    NUISystem::CComponent* subtitle = entry->FindChildOfAnyDepth("UI_TEXT_TRADE_NAME_TEMPLATE_NEW");
    CWideString name;
    if (text || subtitle) {
        name = GetDescription(defIndex);
        if (count > 1 && !IsSpecialCase(defIndex))
            name += GetXString() + IntToWideString(count);
        if (text)
            text->SetText(name, true, true);
        if (subtitle)
            subtitle->SetText(name, true, true);
    }
}