// CV_ChapelOfEvilScript::WatchForExpressions() -- every script frame, while the hero is inside the chapel, poll for a performed expression (the retail branch body only re-checks thread termination)
// Inline CCharString ==(""): the null-data arm is `if (strcmp(text, "") == 0) return true; return false;`, so VC7.1 branches straight off the repe cmpsb flags (`return strcmp(text, "") == 0;` materialises a sete al).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6A000, length 168 bytes (176 with padding).
// Callees: CCharString::CCharString() @ 0x0099E4B0, ~CCharString @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
// @ 0x00CB7940 (landed), Compare(const char*, const char*) @ 0x00411570 (landed, `__fastcall`, the inlined
// CBasicString<char>::operator==), and through the game script interface (+0x40): NewScriptFrame (slot 7, +0x1c),
// MsgOnExpressionPerformed(CCharString&) (slot 43, +0xac). Literal "" @ 0x0122D70E.
// Identity: the catalog label stands; InsideChapel is +0x59 (Ego_r.pdb CV_ChapelOfEvilScript +0x6d less 0x14).
// The real method name is from the manifest; what the original did with the expression is gone from the retail body:
// only `expression == ""` survives (null block: the strcmp intrinsic over the pooled empty literal; else the block's
// inlined operator== calling Compare and `? false : true`, whose result byte is homed in the frame).
// Shape: one CCharString local for the whole loop; `while (!terminating) { if (InsideChapel && Msg(expr)) {...}
// NewScriptFrame(); }`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CGameScriptInterface { public:                       // CGameScriptInterface, retail vtable @ 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) CGameScriptInterface::NewScriptFrame @ 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual bool MsgOnExpressionPerformed(CCharString& expression) const;   // slot 43 (+0xac) CGameScriptInterface::MsgOnExpressionPerformed @ 0x00893B00
};

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

namespace NScript {
class CV_ChapelOfEvilScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x11];                             // +0x48 (donation members)
    bool InsideChapel;                                      // +0x59
    void WatchForExpressions();
};
} // namespace NScript
using NScript::CV_ChapelOfEvilScript;

void NScript::CV_ChapelOfEvilScript::WatchForExpressions()
{
    CCharString expression;
    while (!IsActiveThreadTerminating()) {
        if (InsideChapel && GSI->MsgOnExpressionPerformed(expression)) {
            if (IsActiveThreadTerminating())
                return;
            if (expression == "") {
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        GSI->NewScriptFrame();
    }
}