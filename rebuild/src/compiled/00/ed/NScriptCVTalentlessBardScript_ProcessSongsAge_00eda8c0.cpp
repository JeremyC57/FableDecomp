// NScript::CV_TalentlessBardScript::ProcessSongsAge() -- adds the age verse to the bard's song list: OLD above BardHeroAgeHigh, MEDIUM above BardHeroAgeMedium, YOUNG otherwise
// Leaf: ?ProcessSongsAge@CV_TalentlessBardScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDA8C0, 236 bytes (padded to 240), plain `ret`.
//   PDB name ?ProcessSongsAge@CV_TalentlessBardScript@NScript@@QAEXXZ.
// Callees: CGameScriptInterface slot 171 GetHeroAge() (+0x2ac, float); CCharString(const char*, long) 0x0099EBF0,
//   copy ctor 0x0099EC30, ~CCharString 0x0099EAE0; std::vector<CCharString>::push_back 0x0044BFF0 (out of line) and
//   its overflow helper _M_insert_overflow 0x00433530.
// Data: the script def 0x0143E90C (BardHeroAgeHigh +0x7d8, BardHeroAgeMedium +0x7dc: Ego_r.pdb 0x838/0x83c - 0x60).
// Strings: "TEXT_QST_163_AGE_OLD" 0x012F3B14, "TEXT_QST_163_AGE_MEDIUM" 0x012F3AFC, "TEXT_QST_163_AGE_YOUNG" 0x012F3AE4.
// Shape: STLport push_back sits in VC7.1's inline-once cost band (the lane inlines every site), so the first call site
//   uses a __forceinline copy of the same body and the other two call the out-of-line member; the first site is
//   expanded (placement copy with VC's null test, finish re-read and advanced, overflow helper with a __false_type
//   tag in a frame byte), the other two call the out-of-line copy; the age compares are `GetHeroAge() > (long)`
//   (`fild ; fcompp ; test ah,5 ; jp`).
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
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const CCharString& x);               // 0x0044BFF0 (out of line)
    __forceinline void push_back_inline(const CCharString& x)   // the same STLport body, expanded at the first site
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CScriptDef {
    unsigned char _pad[0x7d8];
    long BardHeroAgeHigh;                               // +0x7d8
    long BardHeroAgeMedium;                             // +0x7dc
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CGameScriptInterface {
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
    virtual void v170();
    virtual float GetHeroAge();   // slot 171 (+0x2ac)
};
namespace NScript {
struct CV_TalentlessBardScript {
    unsigned char _base[0x40];
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    unsigned char _flags48[0x20];                       // +0x48 .. stBard, BardWanderTime
    CCharStringVector SongItems;                        // +0x68
    void ProcessSongsAge();
};
void CV_TalentlessBardScript::ProcessSongsAge()
{
    CScriptDef* def = g_pScriptDef;
    if (Game->GetHeroAge() > def->BardHeroAgeHigh) {
        CCharString verse("TEXT_QST_163_AGE_OLD", -1);
        SongItems.push_back_inline(verse);
    }
    else {
        CScriptDef* def = g_pScriptDef;
        if (Game->GetHeroAge() > def->BardHeroAgeMedium) {
            CCharString verse("TEXT_QST_163_AGE_MEDIUM", -1);
            SongItems.push_back(verse);
        }
        else {
            CCharString verse("TEXT_QST_163_AGE_YOUNG", -1);
            SongItems.push_back(verse);
        }
    }
}
}