// CV_TalentlessBardScript::ProcessSongsMorality() -- adds the morality verse to the bard's song list by the hero's morality category: EVIL 0, BAD 1..2, NEUT 3, GOOD 4..5, PERFECT 6, nothing otherwise
// Leaf: ?ProcessSongsMorality@CV_TalentlessBardScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDA9B0, 248 bytes + a seven-entry jump table (row 288 with padding), plain `ret`.
// Callees: CGameScriptInterface slot 158 GetHeroMoralityCategory() (+0x278); CCharString(const char*, long) 0x0099EBF0,
//   ~CCharString 0x0099EAE0; std::vector<CCharString>::push_back(const CCharString&) 0x0044BFF0 (out of line).
// Strings: "TEXT_QST_163_MORAL_EVIL" 0x012F3B90, "..._BAD" 0x012F3B78, "..._NEUT" 0x012F3B60, "..._GOOD" 0x012F3B48,
//   "..._PERFECT" 0x012F3B2C.
// Shape: the sibling ProcessSongsRenown (landed 0x00EDAAD0): a dense switch whose case groups each pass the literal as
//   an IMPLICIT conversion temporary (one frame word per case, five words) and return through their own epilogue; the
//   last case falls into the shared exit used by the out-of-range default.
// Layout (retail): Game +0x40, SongItems +0x68 (landed ProcessSongsQuestComplete 0x00ED9B40).
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CCharStringVector {                              // STLport vector<CCharString> (12 bytes in retail)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void push_back(const CCharString& x);               // 0x0044BFF0
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
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157();
    virtual long GetHeroMoralityCategory() const;   // slot 158 (+0x278) CGameScriptInterface::GetHeroMoralityCategory @ 0x0089A000
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
    void ProcessSongsMorality();
};
void CV_TalentlessBardScript::ProcessSongsMorality()
{
    switch (Game->GetHeroMoralityCategory())
    {
    case 0: SongItems.push_back("TEXT_QST_163_MORAL_EVIL");
        return;
    case 1:
    case 2: SongItems.push_back("TEXT_QST_163_MORAL_BAD");
        return;
    case 3: SongItems.push_back("TEXT_QST_163_MORAL_NEUT");
        return;
    case 4:
    case 5: SongItems.push_back("TEXT_QST_163_MORAL_GOOD");
        return;
    case 6: SongItems.push_back("TEXT_QST_163_MORAL_PERFECT");
        return;
    }
}
