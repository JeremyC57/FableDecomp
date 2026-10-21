// CTCInventoryMap::SetPanMode(bool panMode) -- with the map list and the zooming map present: store the pan mode and relabel the list's unselect text (region-list scroll text in pan mode); leaving pan mode also re-centres the map on the selected region's name graphic (centre minus offset times zoom, 0.4 s)
// Leaf: SetPanMode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FB8F0, 429 bytes (+int3), `ret 4`, esp frame (speed TU).
//   Ego_r.pdb: ?SetPanMode@CTCInventoryMap@@QAEX_N@Z.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, the list's FindChild(CCharString) (slot 3),
//   its child's SetText(const CWideString&, bool, bool) (slot 144, +0x240), CDataBank::GetTextBySymbol @ 0x009C95E0 (engine
//   0x013B86A0 +0x14), ~CWideString @ 0x0099B510, the list's selected value (slot 142, +0x238), map<long, CRegion>::find
//   @ 0x00615870 (iterator by hidden pointer), the zooming component's slots 12 (+0x30) / 146 (+0x248) / 31 (+0x7C)
//   (C2DVector getters by hidden pointer, MoveTo(const C2DVector&, float, bool); slot names descriptive),
//   CRegion::GetNameGraphicOffset @ 0x006BBF40. Strings: UI_UNSELECT_TEXT, TEXT_GUI_MENU_MAP_SCROLL,
//   TEXT_GUI_MENU_REGION_LIST_SCROLL. Float 0.4f.
// Shape: VC hoists the common by-value "UI_UNSELECT_TEXT" argument setup above the branch; the text bank is read through an
//   inline accessor (loaded before the argument pushes); the selected value is passed straight to find (homed in the dead
//   `panMode` slot); find gets a same-TU noinline stand-in so the iterator's node stays in ebx; the target is
//   `centre - offset * scale` through inline C2DVector operators (the scaled pair stays on the FPU stack).
// Layout: Ego_r.pdb CTCInventoryMap pmap_real +0x1B8 / MapList +0x26C / MapRegions +0x270 / PanMode +0x2A4 sit 0x30 / 0x58 /
//   0x58 / 0x5C lower in retail (12-byte STLport containers): +0x188, +0x214, +0x218, +0x248.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);       // 0x009C95E0
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank* GameTextBank;                            // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
inline CDataBank* GetGameTextBank() { return g_pMainGameComponent->GameTextBank; }
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
};
struct CRegion {
    C2DVector GetNameGraphicOffset() const;             // 0x006BBF40
};
struct SRegionNode {                                    // STLport _Rb_tree_node<pair<const long, CRegion> >
    long _M_color; SRegionNode* _M_parent; SRegionNode* _M_left; SRegionNode* _M_right;
    long first;                                         // +0x10
    CRegion second;                                     // +0x14
};
struct CRegionIterator {
    SRegionNode* _M_node;
    CRegionIterator() {}
    CRegionIterator(SRegionNode* node) : _M_node(node) {}
    bool operator!=(const CRegionIterator& other) const { return _M_node != other._M_node; }
    SRegionNode* operator->() const { return _M_node; }
};
struct CRegionMap {                                     // STLport map<long, CRegion> (header node pointer first)
    SRegionNode* _M_header;
    unsigned char _pad04[8];
    CRegionIterator end() { return CRegionIterator(_M_header); }
    CRegionIterator find(const long& key);              // 0x00615870
};
SRegionNode* FindRegionNode(long key);
#pragma auto_inline(off)
__declspec(noinline) CRegionIterator CRegionMap::find(const long& key)
{
    return CRegionIterator(FindRegionNode(key));
}
#pragma auto_inline(on)
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);    // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11();
    virtual C2DVector GetZoom();                        // slot 12 (+0x30) (name descriptive)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void MoveTo(const C2DVector& position, float time, bool immediate);   // slot 31 (+0x7C) (name descriptive)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85();
    virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89(); virtual void v90(); virtual void v91();
    virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95(); virtual void v96(); virtual void v97();
    virtual void v98(); virtual void v99(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121();
    virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141();
    virtual long GetSelectedValue();                    // slot 142 (+0x238) (name descriptive)
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
    virtual void v145();
    virtual C2DVector GetCentre();                      // slot 146 (+0x248) (name descriptive)
};
}
struct CTCInventoryMap {
    unsigned char _pad00[0x188];
    NUISystem::CComponent* pmap_real;                   // +0x188 (CZoomingComponent*)
    unsigned char _pad18c[0x214 - 0x18c];
    NUISystem::CComponent* MapList;                     // +0x214 (CList*)
    CRegionMap     MapRegions;                          // +0x218
    unsigned char _pad224[0x248 - 0x224];
    bool           PanMode;                             // +0x248
    void SetPanMode(bool panMode);
};

void CTCInventoryMap::SetPanMode(bool panMode)
{
    if (MapList && pmap_real) {
        PanMode = panMode;
        if (!panMode) {
            NUISystem::CComponent* text = MapList->FindChild("UI_UNSELECT_TEXT");
            text->SetText(GetGameTextBank()->GetTextBySymbol("TEXT_GUI_MENU_MAP_SCROLL"), true, true);
            CRegionIterator it = MapRegions.find(MapList->GetSelectedValue());
            if (it != MapRegions.end()) {
                C2DVector zoom = pmap_real->GetZoom();
                C2DVector centre = pmap_real->GetCentre();
                float scale = zoom.X;
                pmap_real->MoveTo(centre - it->second.GetNameGraphicOffset() * scale, 0.4f, false);
            }
        } else {
            NUISystem::CComponent* text = MapList->FindChild("UI_UNSELECT_TEXT");
            text->SetText(GetGameTextBank()->GetTextBySymbol("TEXT_GUI_MENU_REGION_LIST_SCROLL"), true, true);
        }
    }
}
