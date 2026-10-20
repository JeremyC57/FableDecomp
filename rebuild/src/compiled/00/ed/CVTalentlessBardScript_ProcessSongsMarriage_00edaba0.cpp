// CV_TalentlessBardScript::ProcessSongsMarriage() -- adds the marriage verses: BACHELOR if the hero never married, else MARRIED / DIVORCE / MURDER for each marriage fact that holds
// Leaf: ?ProcessSongsMarriage@CV_TalentlessBardScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDABA0, 287 bytes, plain `ret`.
// Callees: CGameScriptInterface slots 211 GetHeroHasMarried (+0x34c), 212 GetHeroHasCurrentMarriage (+0x350),
//   213 GetHeroHasDivorcedMarriage (+0x354), 215 GetHeroHasMurderedWife (+0x35c); CCharString(const char*, long)
//   0x0099EBF0, CCharString(const CCharString&) 0x0099EC30 (the placement copy), ~CCharString 0x0099EAE0;
//   vector<CCharString>::_M_insert_overflow 0x00433530, vector<CCharString>::push_back 0x0044BFF0.
// Strings: "TEXT_QST_163_WED_BACHELOR" 0x012F3C4C, "..._MARRIED" 0x012F3C30, "..._DIVORCE" 0x012F3C14, "..._MURDER" 0x012F3BFC.
// Shape: retail expands STLport push_back only at the first (bachelor) site -- placement copy under VC's null test,
//   `++_M_finish` re-read, `_M_insert_overflow(pos, x, __false_type(), 1, true)` with the tag in the frame's spare
//   byte -- and calls the out-of-line push_back at the other three; the one inline site is a `__forceinline` copy of
//   the STLport body. Every literal is an implicit conversion temporary sharing one frame word.
// Layout (retail): Game +0x40, SongItems +0x68 (landed ProcessSongsQuestComplete 0x00ED9B40).
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
struct CCharStringVector {                              // STLport vector<CCharString> (12 bytes in retail)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x);              // 0x0044BFF0 (the out-of-line copy)
    __forceinline void push_back_inlined(const CCharString& x)   // the same STLport body, expanded at one site
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CGameScriptInterface {                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169();
    virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189();
    virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209();
    virtual void v210();
    virtual bool GetHeroHasMarried() const;   // slot 211 (+0x34c) CGameScriptInterface::GetHeroHasMarried @ 0x00897AA0
    virtual bool GetHeroHasCurrentMarriage() const;   // slot 212 (+0x350) CGameScriptInterface::GetHeroHasCurrentMarriage @ 0x00897B10
    virtual bool GetHeroHasDivorcedMarriage() const;   // slot 213 (+0x354) CGameScriptInterface::GetHeroHasDivorcedMarriage @ 0x00897B70
    virtual void v214();
    virtual bool GetHeroHasMurderedWife() const;   // slot 215 (+0x35c) CGameScriptInterface::GetHeroHasMurderedWife @ 0x00897C30
};
struct CScriptBase {
    virtual void v00();
    unsigned char _body[0x3c];
};
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
};
struct CV_TalentlessBardScript : CQuestScriptObject {
    unsigned char _flags48[0x20];                       // +0x48 .. stBard, BardWanderTime
    CCharStringVector SongItems;                        // +0x68
    void ProcessSongsMarriage();
};
void CV_TalentlessBardScript::ProcessSongsMarriage()
{
    if (!Game->GetHeroHasMarried()) {
        SongItems.push_back_inlined("TEXT_QST_163_WED_BACHELOR");
        return;
    }
    if (Game->GetHeroHasCurrentMarriage())
        SongItems.push_back("TEXT_QST_163_WED_MARRIED");
    if (Game->GetHeroHasDivorcedMarriage())
        SongItems.push_back("TEXT_QST_163_WED_DIVORCE");
    if (Game->GetHeroHasMurderedWife())
        SongItems.push_back("TEXT_QST_163_WED_MURDER");
}