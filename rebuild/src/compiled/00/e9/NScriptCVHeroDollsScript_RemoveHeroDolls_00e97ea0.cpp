// CV_HeroDollsScript::RemoveHeroDolls() -- take each of the seven hero dolls (Briar Rose, Maze, Scarlet Robe, Thunder, Whisper, Twinblade, Scythe) out of the hero's inventory
// Leaf: ?RemoveHeroDolls@CV_HeroDollsScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E97EA0, 294 bytes (304 with padding), plain `ret`.
// Callees: CGameScriptInterface slot 125 TakeObjectFromHero(const CCharString&) const (+0x1f4); CCharString(const char*, long)
//   0x0099EBF0, ~CCharString 0x0099EAE0.
// Strings: "OBJECT_DOLL_BRIAR_ROSE_01" 0x012ED08C, "OBJECT_DOLL_MAZE_01" 0x012ED078, "OBJECT_DOLL_SCARLET_ROBE_01" 0x012ED05C,
//   "OBJECT_DOLL_THUNDER_01" 0x012ED044, "OBJECT_DOLL_WHISPER_01" 0x012ED02C, "OBJECT_DOLL_TWINBLADE_01" 0x012ED010,
//   "OBJECT_DOLL_SCYTHE_01" 0x012ECFF8.
// Shape: seven statements, each literal an IMPLICIT conversion temporary (built before the interface is read, then
//   re-addressed with `lea`), all sharing the one `push ecx` frame word.
// Layout: this+0x40 is the quest script's game interface (CQuestScriptBase, landed script rows).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CGameScriptInterface { public:                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
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
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void TakeObjectFromHero(const CCharString& objectDefName) const;   // slot 125 (+0x1f4) CGameScriptInterface::TakeObjectFromHero @ 0x00898DB0
};
class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
};
namespace NScript {
class CV_HeroDollsScript : public CQuestScriptBase {
public:
    void RemoveHeroDolls();
};
} // namespace NScript
using NScript::CV_HeroDollsScript;
void NScript::CV_HeroDollsScript::RemoveHeroDolls()
{
    Game->TakeObjectFromHero("OBJECT_DOLL_BRIAR_ROSE_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_MAZE_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_SCARLET_ROBE_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_THUNDER_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_WHISPER_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_TWINBLADE_01");
    Game->TakeObjectFromHero("OBJECT_DOLL_SCYTHE_01");
}