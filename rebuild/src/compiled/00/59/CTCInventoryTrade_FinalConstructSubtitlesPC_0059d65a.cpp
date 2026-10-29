// CTCInventoryTrade::FinalConstructSubtitlesPC(NUISystem::CComponent* component, CSubtitleMap& subtitles, NUISystem::CComponent* list) -- PC layout: build a PC_SUBTITLE text component for `component` (its own type-6 child text when it has no entry in `subtitles`, otherwise the entry's text plus " x N" for a positive count), add it under `component` and register it with `list`
// Leaf: FinalConstructSubtitlesPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059D65A, length 263 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) +
// ("s",on). FableWin.pdb ?FinalConstructSubtitlesPC@CTCInventoryTrade@@AAEXPAVCComponent@NUISystem@@AAV?$map@...@std@@0@Z.
// Callees: as the console twin FinalConstructSubtitles 0x0059D0B8 ("PC_SUBTITLE" @ 0x01230428 instead of
// "UI_ONE_OFF_SUBTITLE"), with component virtual slot 60 (+0xF0) for the add and `list` virtual slot 124 (+0x1F0, the raw
// component pointer pushed from the counted pointer's object word) after it; both slot names are descriptive.
// Shape: the console twin's (see 0x0059D0B8): manager first, the stand-in `find` (same-TU noinline body as in the landed
// FinalRefreshSubtitles 0x0059D761), the name conversion homed in the dead `component` slot, the counted-pointer local.
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
template<class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                         // 0x00429C15
    ~CCountedPointer();                                 // 0x004291DE
    T* operator->() const { return m_pObject; }
};
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
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual CTextSource* GetChildOfType(long type);     // slot 9 (+0x24)
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57();
    virtual void v58();
    virtual void AddChild(const CCountedPointer<CComponent>& child);   // slot 59 (+0xEC)
    virtual void AddChildAfterFocus(const CCountedPointer<CComponent>& child);   // slot 60 (+0xF0) (name descriptive)
    virtual void v61(); virtual void v62(); virtual void v63();
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
    virtual void RegisterSubtitle(CComponent* subtitle);   // slot 124 (+0x1F0) (name descriptive)
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
};
class CManager { public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
};
}
struct SSubtitleNode {                                  // STLport _Rb_tree_node<pair<CComponent* const, pair<CWideString,long>>>
    unsigned char          _base[0x10];
    NUISystem::CComponent* Component;                   // +0x10
    CWideString            Text;                        // +0x14
    long                   Count;                       // +0x18
};
struct CSubtitleMap {                                   // std::map<NUISystem::CComponent*, ...> (12 bytes)
    SSubtitleNode* _M_header;
    unsigned long  _M_node_count;
    long           _M_key_compare;
    SSubtitleNode* find(NUISystem::CComponent* const& key);   // 0x005B1A8D
    SSubtitleNode* end() const { return _M_header; }
};
void* FindRef_CSubtitleMap_find_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) SSubtitleNode* CSubtitleMap::find(NUISystem::CComponent* const& key)
{
    void* p = FindRef_CSubtitleMap_find_0();
    if (!p)
        return 0;
    *(void**)&key = p;
    return 0;
}
#pragma auto_inline(on)

namespace NInventory {
struct CTCInventoryTrade {
    void FinalConstructSubtitlesPC(NUISystem::CComponent* component, CSubtitleMap& subtitles, NUISystem::CComponent* list);
};
void CTCInventoryTrade::FinalConstructSubtitlesPC(NUISystem::CComponent* component, CSubtitleMap& subtitles, NUISystem::CComponent* list)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    SSubtitleNode* found = subtitles.find(component);
    NUISystem::CComponent* owner = component;
    CWideString text;
    if (found == subtitles.end()) {
        text = owner->GetChildOfType(6)->GetText();
    } else {
        text = found->Text;
        if (found->Count > 0) {
            text += GetXString();
            text += IntToWideString(found->Count);
        }
    }
    CCountedPointer<NUISystem::CComponent> subtitle(manager->CreateComponent("PC_SUBTITLE", false));
    subtitle->SetText(text, true, true);
    owner->AddChildAfterFocus(subtitle);
    list->RegisterSubtitle(subtitle.m_pObject);
}
}