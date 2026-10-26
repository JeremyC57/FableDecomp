// CQ_NewOakValeIntroScript::AddBadDeed(long deed) -- the hero did a bad deed in the Oakvale intro: count it, take away the per-deed morality, and the first time (with no good deeds yet) show the first-bad-deed message and log the map tutorial, otherwise show the bad-deed message once per kind of deed
// Leaf: ?AddBadDeed@CQ_NewOakValeIntroScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DAEA70, length 383 bytes (+ int3 padding), `ret 4`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940 (landed), CCharString::CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0, the file-static __fastcall AddLogbookTutorialEntry(const CCharString&)
// 0x00CBE9EE (landed); through the game script interface (+0x40, vtable 0x01260F0C): NewScriptFrame (slot 7),
// MsgIsGameInfoClickedPast (40), DisplayGameInfo(const CCharString&) (115), GiveHeroMorality(float) (156).
// Data: pGlobalScriptDef 0x0143E90C OVI_MoralityChangePerDeed at retail +0xD64 (PDB +0xE14, the retail def sits 0xB0 lower
// here, as the landed OVBR rows show); strings 0x012D8774 "TEXT_QST_048_SCRMSG_DID_FIRST_BAD_DEED", 0x012D875C
// "TEXT_QST_LOG_BASICS_MAP", 0x012D8738 "TEXT_QST_048_SCRMSG_DID_BAD_DEED".
// Layout: retail = Ego_r.pdb - 0x14 (as the landed ctor 0x00DAAC00): GoodDeedsPerformed +0x54, BadDeedsPerformed
// +0x58, WhichBadDeedsPerformed +0xFC (bool per deed).
// Shape: the script wait idiom (`while (!msg) { frame; if (terminating) return; } if (terminating) return;`); the two
// arms are an if/else with ONE `WhichBadDeedsPerformed[deed] = true` after them (VC7.1 duplicates the store into each
// arm and keeps `deed` in edi from the top; an early `return` in the first arm leaves `deed` in memory and adds a
// frame); each string is a named CCharString local in its own block (built in the dead `deed` argument slot).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CGameScriptInterface { public:                          // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual bool MsgIsGameInfoClickedPast() const;   // slot 40 (+0xa0)
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114();
    virtual void DisplayGameInfo(const CCharString& textTag) const;   // slot 115 (+0x1cc)
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void GiveHeroMorality(float amount) const;   // slot 156 (+0x270)
};

void __fastcall AddLogbookTutorialEntry(const CCharString& tag);   // 0x00CBE9EE

class CScriptDef { public:
    unsigned char _pad000[0xd64];
    float         OVI_MoralityChangePerDeed;            // +0xd64 (PDB +0xe14)
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

class CScriptBase { public:                                    // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

namespace NScript {
class CQ_NewOakValeIntroScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x54 - 0x48];
    long                  GoodDeedsPerformed;           // +0x54 (PDB +0x68)
    long                  BadDeedsPerformed;            // +0x58 (PDB +0x6c)
    unsigned char         _pad5c[0xfc - 0x5c];
    bool                  WhichBadDeedsPerformed[8];    // +0xfc
    void AddBadDeed(long deed);
};
} // namespace NScript
using NScript::CQ_NewOakValeIntroScript;

void NScript::CQ_NewOakValeIntroScript::AddBadDeed(long deed)
{
    ++BadDeedsPerformed;
    GSI->GiveHeroMorality(-pGlobalScriptDef->OVI_MoralityChangePerDeed);
    if (BadDeedsPerformed == 1 && GoodDeedsPerformed == 0) {
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString text("TEXT_QST_048_SCRMSG_DID_FIRST_BAD_DEED");
            GSI->DisplayGameInfo(text);
        }
        while (!GSI->MsgIsGameInfoClickedPast()) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            CCharString tag("TEXT_QST_LOG_BASICS_MAP");
            AddLogbookTutorialEntry(tag);
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        if (!WhichBadDeedsPerformed[deed]) {
            if (IsActiveThreadTerminating())
                return;
            {
                CCharString text("TEXT_QST_048_SCRMSG_DID_BAD_DEED");
                GSI->DisplayGameInfo(text);
            }
            while (!GSI->MsgIsGameInfoClickedPast()) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    WhichBadDeedsPerformed[deed] = true;
}