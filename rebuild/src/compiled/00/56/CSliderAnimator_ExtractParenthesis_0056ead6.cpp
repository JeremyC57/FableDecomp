// CSliderAnimator::ExtractParenthesis(NUISystem::CText* text) -- the text's string from its first '(' to the end, as a wide string (the empty wide string when there is no '(')
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056EAD6, length 114 bytes (`ret 8`, frame pointer + `leave`).
// Callees: CText virtual slot 142 (+0x238, the displayed wide string), CWideString::ConvertToCharString @ 0x0099B2C0 (result into the
// dead `text` argument slot), CCharString::FindCharPos @ 0x0099E5A0, GetSubString @ 0x0099EC70, ToWideString @ 0x0099E480,
// ~CCharString @ 0x0099EAE0 (catalog label a propagated _Cons_val), CWideString copy ctor @ 0x0099B720 from the static 0x013BCA24.
// TU: `#pragma optimize("s", on)` + `("y", off)` (ebp frame, `leave`, one shared string-dtor exit).
// Shape: the inline GetLength is `p == 0 ? 0 : p->m_length` (the tested pointer itself is the zero result).
// Leaf: ExtractParenthesis
#pragma optimize("s", on)
#pragma optimize("y", off)
struct CCharStringData { const char* m_pData; unsigned long m_length; };
struct CWideString;
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                            // 0x0099EAE0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    long FindCharPos(char c) const;                            // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long length) const;   // 0x0099EC70
    CWideString ToWideString() const;                          // 0x0099E480
};
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);                     // 0x0099B720
    CCharString ConvertToCharString() const;                   // 0x0099B2C0
};
extern const CWideString g_EmptyWideString;                    // 0x013BCA24

namespace NUISystem {
struct CText {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual const CWideString& GetText() const;                // slot 142 (+0x238)
};
}

struct CSliderAnimator {
    CWideString ExtractParenthesis(NUISystem::CText* text);
};

CWideString CSliderAnimator::ExtractParenthesis(NUISystem::CText* text)
{
    CCharString s = text->GetText().ConvertToCharString();
    long pos = s.FindCharPos('(');
    if (pos != -1) {
        CCharString sub = s.GetSubString(pos, s.GetLength());
        return sub.ToWideString();
    }
    return g_EmptyWideString;
}