// NUISystem::CText::SetValue(const CWideString& text, bool lookUpTag, bool keepSize) -- optionally resolve the text's game-text tag (from the CRC of its narrow copy, through the game's or the front end's text bank), then set the text
// Leaf: ?SetValue@CText@NUISystem@@UAEXABVCWideString@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054F410, 160 bytes, virtual, `ret 0Ch`.
// Vtable 0x01249CCC (NUISystem::CText) slot 144; Ego_r.pdb `?SetValue@CText@NUISystem@@UAEXABVCWideString@@_N1@Z`.
// Callees: CWideString::ConvertToCharString 0x0099B2C0, the CRC Calc 0x004014A0, CCharString::GetCRC's out-of-line
//   copy 0x00404310, NGameText::NGameText::CDataBank::LookupValue 0x009CC410, ~CCharString 0x0099EAE0, and the
//   (K, const CWideString&, bool) overload, slot 143 (+0x23c).
// Globals: g_pMainGameComponent 0x013B86A0 (game-text bank +0x14), g_pNewFrontendGameComponent 0x013B871C (+0x60).
// Shape: as the CCharString overload 0x0054F370, with the narrow copy as a temporary in the dead `lookUpTag` slot:
//   GetCRC is inlined at the first site and called out of line at the second, the two arms' identical
//   `GetTagFromCRC ; ~CCharString` tails are cross-jumped into one. The bank is read through an inline const getter
//   (name descriptive), which places its load after the conversion call and before Calc, as retail.
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0

class CCharStringData { public:
    char* PData;                                    // +0x00
    unsigned long StringLength;                     // +0x04
};
class CWideString;
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                                 // 0x0099EAE0
    CWideString ToWideString() const;               // 0x0099E480
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const { return CCRC::Calc(0, GetString(), GetLength()); }   // out-of-line copy 0x00404310
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                 // 0x0099B510
    CCharString ConvertToCharString() const;        // 0x0099B2C0
};

namespace NGameText {
class CDataBank {
public:
public:
    unsigned long LookupValue(unsigned long) const;   // 0x009CC410
};
}
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;        // +0x14
    NGameText::CDataBank* GetGameTextDataBank() const { return PGameTextDataBank; }
};
class CNewFrontendGameComponent { public:
    unsigned char _pad00[0x60];
    NGameText::CDataBank* PGameTextDataBank;        // +0x60
    NGameText::CDataBank* GetGameTextDataBank() const { return PGameTextDataBank; }
};
extern CMainGameComponent* g_pMainGameComponent;                 // 0x013B86A0
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C

namespace NUISystem {
class CText {
public:
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
    virtual void v142();
    // Overloaded virtuals are laid out in reverse declaration order: (K, ...) lands in slot 143, the wide-string
    // form in 144, the char-string form in 145, as in vtable 0x01249CCC.
    virtual void SetValue(const CCharString& text, bool lookUpTag, bool keepSize);
    virtual void SetValue(const CWideString& text, bool lookUpTag, bool keepSize);
    virtual void SetValue(unsigned long tag, const CWideString& text, bool keepSize);
};

void CText::SetValue(const CWideString& text, bool lookUpTag, bool keepSize)
{
    unsigned long tag = 0;
    if (lookUpTag) {
        if (g_pMainGameComponent) {
            tag = g_pMainGameComponent->GetGameTextDataBank()->LookupValue(text.ConvertToCharString().GetCRC());
        }
        else if (g_pNewFrontendGameComponent) {
            tag = g_pNewFrontendGameComponent->GetGameTextDataBank()->LookupValue(text.ConvertToCharString().GetCRC());
        }
    }
    SetValue(tag, text, keepSize);
}
}