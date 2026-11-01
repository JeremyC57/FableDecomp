# family stools (hand): CTCBase::GetThingSearchTools 0x00686D80 ?GetThingSearchTools@CTCBase@@IAEAAVCThingSearchTools@@XZ and
# CTCBase::PeekThingSearchTools 0x00686D90 ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ (owner decision
# 2026-11-01: retail 0x00686D80 / 0x00686D90 are `mov ecx,[0x013B8A1C] ; jmp` onto CThingManager::GetThingSearchTools 0x0051E490 /
# PeekThingSearchTools 0x0051E480; Ego_r 0x006B2170 / 0x006B2180 and FableWin carry exactly these spellings). The accessor
# moves to a CTCBase base (empty here, or holding the first virtual slot when the row's class is polymorphic, so `this`
# stays at offset 0); the search-tools callees reached through the const reference are const (Ego_r / FableWin declare every
# CThingSearchTools search QBE). HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
PEEK = 'const CThingSearchTools& PeekThingSearchTools() const;   // 0x00686D90 ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ (Ego_r / FableWin)'
# 0x005BDC77: the free function was the non-const protected member
edit('005bdc77', 'src', 'CThingSearchTools* GetThingSearchTools();       // 0x00686D80\nstruct CTCInventoryBase_Retail {',
     'class CTCBase { protected:                      // the PDB base the accessor belongs to (empty here: offset 0)\n'
     '    CThingSearchTools& GetThingSearchTools();   // 0x00686D80 ?GetThingSearchTools@CTCBase@@IAEAAVCThingSearchTools@@XZ (Ego_r / FableWin)\n'
     '};\nstruct CTCInventoryBase_Retail : public CTCBase {')
edit('005bdc77', 'src', '    GetThingSearchTools()->GetPFirstLeafThing(', '    GetThingSearchTools().GetPFirstLeafThing(')
# 0x006C5E90 CTCHeroOpinionDeedLog (no virtual functions in the model: empty base)
A = '006c5e90'
edit(A, 'src', 'class CTCHeroOpinionDeedLog { public:\n    void*   __vftable;                               // +0x00\n    CThing* ParentThing;                             // +0x04 CTCBase\n    CThingSearchTools* GetThingSearchTools() const;  // 0x00686D90 (CTCBase)\n',
     'class CTCBase { protected:                          // the PDB base the accessor belongs to (empty here: offset 0)\n    ' + PEEK + '\n};\n'
     'class CTCHeroOpinionDeedLog : public CTCBase { public:\n    void*   __vftable;                               // +0x00\n    CThing* ParentThing;                             // +0x04 CTCBase\n')
edit(A, 'src', '    template <class T, class C, class F> void GetAllThings(const F& filter, C& found);   // 0x004ACF90',
     '    template <class T, class C, class F> void GetAllThings(const F& filter, C& found) const;   // 0x004ACF90 (const: Ego_r / FableWin)')
edit(A, 'src', 'template <class T, class C, class F> void CThingSearchTools::GetAllThings(const F& filter, C& found)\n{',
     'template <class T, class C, class F> void CThingSearchTools::GetAllThings(const F& filter, C& found) const\n{')
edit(A, 'src', '    GetThingSearchTools()->GetAllThings<CThing>(filter, creatures);', '    PeekThingSearchTools().GetAllThings<CThing>(filter, creatures);')
# 0x00799790 CTCShopKeeper (polymorphic model: the base holds its first virtual slot)
A = '00799790'
edit(A, 'src', '    void GetAllThings(const CThingFilter_HasTC& filter, vector<CThing*>& out);   // 0x00799FB0',
     '    void GetAllThings(const CThingFilter_HasTC& filter, vector<CThing*>& out) const;   // 0x00799FB0 (const: Ego_r / FableWin)')
edit(A, 'src', 'class CTCShopKeeper { public:\n    virtual void v00();\n',
     'class CTCBase { public:                             // the PDB base the accessor belongs to (its first virtual slot only: offset 0)\n'
     '    virtual void v00();\nprotected:\n    ' + PEEK + '\n};\nclass CTCShopKeeper : public CTCBase { public:\n    virtual void v00();\n')
edit(A, 'src', '    CThingSearchTools* GetThingSearchTools() const;   // 0x00686D90 (CTCBase)\n', '')
edit(A, 'src', '        GetThingSearchTools()->GetAllThings(CThingFilter_HasTC(0x53), shops);', '        PeekThingSearchTools().GetAllThings(CThingFilter_HasTC(0x53), shops);')
# 0x007C04A0 CTCShotDefinitionBase (derives from the row's empty CTCBase already)
A = '007c04a0'
edit(A, 'src', 'class CTCBase { protected:\n    CWorld& GetWorld() const;   // 0x00686A70 (Ego_r.pdb CTCBase, protected)\n',
     'class CTCBase { protected:\n    CWorld& GetWorld() const;   // 0x00686A70 (Ego_r.pdb CTCBase, protected)\n    ' + PEEK + '\n')
edit(A, 'src', '    CThingSearchTools* GetThingSearchTools() const; // 0x00686D90\n', '')
edit(A, 'src', '    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_IsNot& filter, CThingVector& out);   // 0x00725F10\n    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_All& filter, CThingVector& out);     // 0x007197C0',
     '    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_IsNot& filter, CThingVector& out) const;   // 0x00725F10 (const: Ego_r / FableWin)\n'
     '    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_All& filter, CThingVector& out) const;     // 0x007197C0 (const)')
edit(A, 'src', '        GetThingSearchTools()->GetAllThingsInRoughArea(area, filter, things);', '        PeekThingSearchTools().GetAllThingsInRoughArea(area, filter, things);')
edit(A, 'src', '        GetThingSearchTools()->GetAllThingsInRoughArea(area, CThingFilter_All(), things);', '        PeekThingSearchTools().GetAllThingsInRoughArea(area, CThingFilter_All(), things);')
# 0x007E5D20 CTCOracleMinigame (polymorphic model)
A = '007e5d20'
edit(A, 'src', '    bool GetPFirstThing(const CThingFilter_HasTrackName& filter, CThing** out);         // 0x007E6EC0\n    bool GetPFirstSpeechThing(const CThingFilter_HasTrackName& filter, CThing** out);   // 0x0056D050 (second instantiation)',
     '    bool GetPFirstThing(const CThingFilter_HasTrackName& filter, CThing** out) const;         // 0x007E6EC0 (const: Ego_r / FableWin)\n'
     '    bool GetPFirstSpeechThing(const CThingFilter_HasTrackName& filter, CThing** out) const;   // 0x0056D050 (second instantiation)')
edit(A, 'src', 'class CTCOracleMinigame { public:\n    virtual void v00();\n',
     'class CTCBase { public:                             // the PDB base the accessor belongs to (its first virtual slot only: offset 0)\n'
     '    virtual void v00();\nprotected:\n    ' + PEEK + '\n};\nclass CTCOracleMinigame : public CTCBase { public:\n    virtual void v00();\n')
edit(A, 'src', '    CThingSearchTools* GetThingSearchTools() const;     // 0x00686D90 (CTCBase)\n', '')
edit_all(A, 'src', '        GetThingSearchTools()->GetPFirst', '        PeekThingSearchTools().GetPFirst', 5)
# 0x007FBC90 CTCKraken (its CTCBase model carries the accessor)
A = '007fbc90'
edit(A, 'src', 'public:\n    CWorld*             GetWorld_00686d60();                     // 0x00686D60\n    CThingSearchTools*  GetThingSearchTools() const;    // 0x00686D90\n',
     '    ' + PEEK + '\npublic:\n    CWorld*             GetWorld_00686d60();                     // 0x00686D60\n')
edit(A, 'src', '        GetThingSearchTools()->GetPFirstThingInRoughArea(area, CThingFilter_HasDefGlobalIndex(chamberIndex), &found);',
     '        PeekThingSearchTools().GetPFirstThingInRoughArea(area, CThingFilter_HasDefGlobalIndex(chamberIndex), &found);')
edit(A, 'src', '            GetThingSearchTools()->GetPFirstThingInRoughArea(area, CThingFilter_HasTrackName(name), &found);',
     '            PeekThingSearchTools().GetPFirstThingInRoughArea(area, CThingFilter_HasTrackName(name), &found);')
