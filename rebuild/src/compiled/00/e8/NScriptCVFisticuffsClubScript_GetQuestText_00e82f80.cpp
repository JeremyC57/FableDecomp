// CV_FisticuffsClubScript::GetQuestText(CCharString suffix) -- build the TEXT_QST_<quest>_<suffix> text key of the punch club the hero is in
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E82F80, 0x295 bytes, `ret 8` (hidden result + the by-value suffix, destroyed by the callee).
// Signature: FableWin.pdb ?GetQuestText@CV_FisticuffsClubScript@NScript@@QAE?AVCCharString@@V3@@Z.
// Callees: CCharString(const char*, long) 0x0099EBF0, CCharString(const CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0,
//   operator+(const CCharString&, const char*) 0x0099F600, operator+(const CCharString&, const CCharString&) 0x0099F570
//   (both __fastcall, result in ecx, left in edx).
// Strings: "Should not happen" 0x012EB098, "TEXT_QST_" 0x012EB088, "_" 0x01244DB4, quest numbers "008" / "079" / "009" / "040".
// Shape: a switch on RelevantRegion (1..4, jump table; default shares case 4's "040" but keeps its own copy). Each case
//   returns `"TEXT_QST_" + number + "_" + suffix`; the "_" is a CCharString temporary (built first, right-to-left), so
//   the second join is the CCharString+CCharString overload. The unused "Should not happen" local is copy-initialised
//   from a reference-bound temporary (`(const CCharString&)"..."`, which blocks VC7.1 copy elision) --
//   through a temporary (ctor, copy ctor, dtor) and destroyed in the shared tail with the parameter.
// Table: 0x00E83218 = { 0x00E82FC3 "008" 0x012EB094, 0x00E830A7 "079" 0x012EB080, 0x00E83119 "009" 0x012EB07C,
//   0x00E83035 "040" 0x012EB084 } for regions 1..4; the default arm 0x00E83188 also uses "040". The arms sit in the bytes
//   as regions 1, 4, 2, 3, default (the GetCutsceneName 0x00E8A1D0 order), so the cases are written in that order; an
//   earlier version listed them 1..4: same behaviour, but table slots 2-4 pointed at arms with other strings than retail's.
// Layout: Ego_r.pdb CV_FisticuffsClubScript - 0x14 (retail CScriptBase is 0x40): RelevantRegion +0x48.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

class CGameScriptInterface;

class CScriptBase { public:                                     // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* Game;                          // +0x40
    void* Owner;                                         // +0x44
};

namespace NScript {
class CV_FisticuffsClubScript : public CQuestScriptBase {
public:
    long RelevantRegion;                                 // +0x48
    CCharString GetQuestText(CCharString suffix);
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;

CCharString NScript::CV_FisticuffsClubScript::GetQuestText(CCharString suffix)
{
    CCharString text = (const CCharString&)"Should not happen";
    switch (RelevantRegion) {
    case 1:
        return CCharString("TEXT_QST_") + "008" + CCharString("_") + suffix;
    case 4:
        return CCharString("TEXT_QST_") + "040" + CCharString("_") + suffix;
    case 2:
        return CCharString("TEXT_QST_") + "079" + CCharString("_") + suffix;
    case 3:
        return CCharString("TEXT_QST_") + "009" + CCharString("_") + suffix;
    default:
        return CCharString("TEXT_QST_") + "040" + CCharString("_") + suffix;
    }
}