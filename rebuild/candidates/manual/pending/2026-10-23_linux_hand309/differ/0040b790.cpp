// CUserProfileManager::GiveTutorialForCategory(ETutorialCategory category, bool force) -- show a tutorial category once: queue it while a cut-scene or the GUI blocks it, otherwise show its guildmaster message or subtitle, add its log-book entry and record it as given (then save the profile)
// Grade: DIFFER(938v918) -- retail caches the category in ebx (tag temporary in the dead parameter slot, a 0x14 frame); ours keeps it in memory (0x18 frame, tag in a local byte), so every later block shifts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040B790, length 918 bytes, `ret 8`.
// Callees: std::__find 0x0040E170 (__fastcall), PeekGuiDef 0x00438570, GetMainPlayer 0x00449970, CPlayerGui::
//   ShowGuildMasterMessage 0x00437800, CSubtitleRenderer::IsAllowedToSetText 0x00490927 / SetText 0x00493049, CCharString
//   0x0099EBF0 / 0x0099EC30 / 0x0099EAE0 / operator!= 0x0099E960 / GetSubString 0x0099EC70 / operator= 0x0099EFB0 /
//   operator+= 0x0099F100 / ToWideString 0x0099E480, operator+(const CCharString&, const char*) 0x0099F600, ~CWideString
//   0x0099B510, CHeroLogBook::AddLogEntry 0x0047A9F0, vector<ETutorialCategory>::push_back 0x004121D0, deque::
//   _M_push_back_aux_v 0x004127E0, CUserProfileManager::Save 0x0040A740; a slot-395 (+0x62c) query on the object at
//   g_pMainGameComponent +0x24 -> +0x38 (descriptive: IsCutsceneRunning). Globals: tutorials-active bool 0x01375740,
//   g_pMainGameComponent 0x013B86A0, g_pPlayerGui 0x013B8790.
// Layout: CUserProfileManager (Ego_r.pdb, retail 12-byte vectors and 40-byte STLport deque): UseNewProfileValues +0x0c,
//   TutorialSystemEnabled +0x17, GivenTutorialCategories +0x8c, PendingTutorialCategories +0x98 (deque), DisplayTutorials-
//   DuringCutscenes +0xcc, TutorialSystemEnabledNewProfile +0xd7. CPlayerGuiDef (retail): the guildmaster message vector
//   +0x910, TutorialText +0x91c, the message id vector +0x928. CPlayerGui: SubtitleRenderer +0x18, a blocking bool +0xf6.
//   The main component's font bank at +0x1614C (as the landed CGameScriptInterface::DisplayGameInfo 0x008929D0).
// Shape: CCharString::EndsWith is an inline member; the four empty substitutions are named locals.
#include <string.h>
#pragma intrinsic(strcmp)
struct CCharStringData { const char* PData; long Length; };
struct CWideString {
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    bool operator!=(const char* text) const;                // 0x0099E960
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    CCharString& operator+=(const char* text);              // 0x0099F100
    CCharString GetSubString(long start, long length) const;   // 0x0099EC70
    CWideString ToWideString() const;                       // 0x0099E480
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }   // "" = 0x0129AAF4
    __forceinline bool EndsWith(const CCharString& suffix) const
    {
        long start = GetLength() - suffix.GetLength();
        if (start >= 0)
            return strcmp(GetString() + start, suffix.GetString()) == 0;
        return false;
    }
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
template <class T> struct vector {                          // STLport vector: {_M_start, _M_finish, _M_end_of_storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int i) { return _M_start[i]; }
};
struct CPlayerGuiDef {
    unsigned char       _pad000[0x910];
    vector<CCharString> GuildMasterMessages;                // +0x910
    vector<CCharString> TutorialText;                       // +0x91c
    vector<long>        GuildMasterMessageIDs;              // +0x928
};
CPlayerGuiDef* __fastcall PeekGuiDef();                     // 0x00438570
struct CHeroLogBook {
    void AddLogEntry(const CWideString& title, const CWideString& heading, const CWideString& text, long category, bool given);   // 0x0047A9F0
};
struct CPlayer {
    unsigned char _pad000[0xf8];
    CHeroLogBook* PHeroLogBook;                             // +0xf8 (CCountedPointer data)
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                               // 0x00449970
};
struct CCutsceneQuery {
    virtual void q000(); virtual void q001(); virtual void q002(); virtual void q003(); virtual void q004(); virtual void q005(); virtual void q006(); virtual void q007();
    virtual void q008(); virtual void q009(); virtual void q010(); virtual void q011(); virtual void q012(); virtual void q013(); virtual void q014(); virtual void q015();
    virtual void q016(); virtual void q017(); virtual void q018(); virtual void q019(); virtual void q020(); virtual void q021(); virtual void q022(); virtual void q023();
    virtual void q024(); virtual void q025(); virtual void q026(); virtual void q027(); virtual void q028(); virtual void q029(); virtual void q030(); virtual void q031();
    virtual void q032(); virtual void q033(); virtual void q034(); virtual void q035(); virtual void q036(); virtual void q037(); virtual void q038(); virtual void q039();
    virtual void q040(); virtual void q041(); virtual void q042(); virtual void q043(); virtual void q044(); virtual void q045(); virtual void q046(); virtual void q047();
    virtual void q048(); virtual void q049(); virtual void q050(); virtual void q051(); virtual void q052(); virtual void q053(); virtual void q054(); virtual void q055();
    virtual void q056(); virtual void q057(); virtual void q058(); virtual void q059(); virtual void q060(); virtual void q061(); virtual void q062(); virtual void q063();
    virtual void q064(); virtual void q065(); virtual void q066(); virtual void q067(); virtual void q068(); virtual void q069(); virtual void q070(); virtual void q071();
    virtual void q072(); virtual void q073(); virtual void q074(); virtual void q075(); virtual void q076(); virtual void q077(); virtual void q078(); virtual void q079();
    virtual void q080(); virtual void q081(); virtual void q082(); virtual void q083(); virtual void q084(); virtual void q085(); virtual void q086(); virtual void q087();
    virtual void q088(); virtual void q089(); virtual void q090(); virtual void q091(); virtual void q092(); virtual void q093(); virtual void q094(); virtual void q095();
    virtual void q096(); virtual void q097(); virtual void q098(); virtual void q099(); virtual void q100(); virtual void q101(); virtual void q102(); virtual void q103();
    virtual void q104(); virtual void q105(); virtual void q106(); virtual void q107(); virtual void q108(); virtual void q109(); virtual void q110(); virtual void q111();
    virtual void q112(); virtual void q113(); virtual void q114(); virtual void q115(); virtual void q116(); virtual void q117(); virtual void q118(); virtual void q119();
    virtual void q120(); virtual void q121(); virtual void q122(); virtual void q123(); virtual void q124(); virtual void q125(); virtual void q126(); virtual void q127();
    virtual void q128(); virtual void q129(); virtual void q130(); virtual void q131(); virtual void q132(); virtual void q133(); virtual void q134(); virtual void q135();
    virtual void q136(); virtual void q137(); virtual void q138(); virtual void q139(); virtual void q140(); virtual void q141(); virtual void q142(); virtual void q143();
    virtual void q144(); virtual void q145(); virtual void q146(); virtual void q147(); virtual void q148(); virtual void q149(); virtual void q150(); virtual void q151();
    virtual void q152(); virtual void q153(); virtual void q154(); virtual void q155(); virtual void q156(); virtual void q157(); virtual void q158(); virtual void q159();
    virtual void q160(); virtual void q161(); virtual void q162(); virtual void q163(); virtual void q164(); virtual void q165(); virtual void q166(); virtual void q167();
    virtual void q168(); virtual void q169(); virtual void q170(); virtual void q171(); virtual void q172(); virtual void q173(); virtual void q174(); virtual void q175();
    virtual void q176(); virtual void q177(); virtual void q178(); virtual void q179(); virtual void q180(); virtual void q181(); virtual void q182(); virtual void q183();
    virtual void q184(); virtual void q185(); virtual void q186(); virtual void q187(); virtual void q188(); virtual void q189(); virtual void q190(); virtual void q191();
    virtual void q192(); virtual void q193(); virtual void q194(); virtual void q195(); virtual void q196(); virtual void q197(); virtual void q198(); virtual void q199();
    virtual void q200(); virtual void q201(); virtual void q202(); virtual void q203(); virtual void q204(); virtual void q205(); virtual void q206(); virtual void q207();
    virtual void q208(); virtual void q209(); virtual void q210(); virtual void q211(); virtual void q212(); virtual void q213(); virtual void q214(); virtual void q215();
    virtual void q216(); virtual void q217(); virtual void q218(); virtual void q219(); virtual void q220(); virtual void q221(); virtual void q222(); virtual void q223();
    virtual void q224(); virtual void q225(); virtual void q226(); virtual void q227(); virtual void q228(); virtual void q229(); virtual void q230(); virtual void q231();
    virtual void q232(); virtual void q233(); virtual void q234(); virtual void q235(); virtual void q236(); virtual void q237(); virtual void q238(); virtual void q239();
    virtual void q240(); virtual void q241(); virtual void q242(); virtual void q243(); virtual void q244(); virtual void q245(); virtual void q246(); virtual void q247();
    virtual void q248(); virtual void q249(); virtual void q250(); virtual void q251(); virtual void q252(); virtual void q253(); virtual void q254(); virtual void q255();
    virtual void q256(); virtual void q257(); virtual void q258(); virtual void q259(); virtual void q260(); virtual void q261(); virtual void q262(); virtual void q263();
    virtual void q264(); virtual void q265(); virtual void q266(); virtual void q267(); virtual void q268(); virtual void q269(); virtual void q270(); virtual void q271();
    virtual void q272(); virtual void q273(); virtual void q274(); virtual void q275(); virtual void q276(); virtual void q277(); virtual void q278(); virtual void q279();
    virtual void q280(); virtual void q281(); virtual void q282(); virtual void q283(); virtual void q284(); virtual void q285(); virtual void q286(); virtual void q287();
    virtual void q288(); virtual void q289(); virtual void q290(); virtual void q291(); virtual void q292(); virtual void q293(); virtual void q294(); virtual void q295();
    virtual void q296(); virtual void q297(); virtual void q298(); virtual void q299(); virtual void q300(); virtual void q301(); virtual void q302(); virtual void q303();
    virtual void q304(); virtual void q305(); virtual void q306(); virtual void q307(); virtual void q308(); virtual void q309(); virtual void q310(); virtual void q311();
    virtual void q312(); virtual void q313(); virtual void q314(); virtual void q315(); virtual void q316(); virtual void q317(); virtual void q318(); virtual void q319();
    virtual void q320(); virtual void q321(); virtual void q322(); virtual void q323(); virtual void q324(); virtual void q325(); virtual void q326(); virtual void q327();
    virtual void q328(); virtual void q329(); virtual void q330(); virtual void q331(); virtual void q332(); virtual void q333(); virtual void q334(); virtual void q335();
    virtual void q336(); virtual void q337(); virtual void q338(); virtual void q339(); virtual void q340(); virtual void q341(); virtual void q342(); virtual void q343();
    virtual void q344(); virtual void q345(); virtual void q346(); virtual void q347(); virtual void q348(); virtual void q349(); virtual void q350(); virtual void q351();
    virtual void q352(); virtual void q353(); virtual void q354(); virtual void q355(); virtual void q356(); virtual void q357(); virtual void q358(); virtual void q359();
    virtual void q360(); virtual void q361(); virtual void q362(); virtual void q363(); virtual void q364(); virtual void q365(); virtual void q366(); virtual void q367();
    virtual void q368(); virtual void q369(); virtual void q370(); virtual void q371(); virtual void q372(); virtual void q373(); virtual void q374(); virtual void q375();
    virtual void q376(); virtual void q377(); virtual void q378(); virtual void q379(); virtual void q380(); virtual void q381(); virtual void q382(); virtual void q383();
    virtual void q384(); virtual void q385(); virtual void q386(); virtual void q387(); virtual void q388(); virtual void q389(); virtual void q390(); virtual void q391();
    virtual void q392(); virtual void q393(); virtual void q394();
    virtual bool IsCutsceneRunning();                       // slot 395 (+0x62c)
};
struct CCutsceneOwner {
    unsigned char   _pad00[0x38];
    CCutsceneQuery* Query;                                  // +0x38
};
struct CFontBank;
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                          // +0x1c
    unsigned char   _pad20[0x24 - 0x20];
    CCutsceneOwner* CutsceneOwner;                          // +0x24
    unsigned char   _pad28[0x1614c - 0x28];
    CFontBank*      Font;                                   // +0x1614C
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
struct CSubtitleRenderer {
    bool IsAllowedToSetText(bool force);                    // 0x00490927
    void SetText(CFontBank* font, const CCharString& text, const CCharString& a, const CCharString& b, const CCharString& c, const CCharString& d,
                 unsigned long colour, float time, bool b1, bool b2);   // 0x00493049
};
struct CPlayerGui {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                     // +0x18
    unsigned char     _pad19[0xf6 - 0x19];
    bool              Blocked;                              // +0xf6
    void ShowGuildMasterMessage(const CCharString& text, long id, bool flag);   // 0x00437800
};
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790
extern bool g_bTutorialsActive;                             // 0x01375740
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE };
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
long* __fastcall __find(long* first, long* last, const long& val, const random_access_iterator_tag&);   // 0x0040E170
}
struct CTutorialCategoryVector {                            // STLport vector<ETutorialCategory>
    ETutorialCategory* _M_start;
    ETutorialCategory* _M_finish;
    ETutorialCategory* _M_end_of_storage;
    void push_back(const ETutorialCategory& x);             // 0x004121D0
};
struct CTutorialCategoryDeque {                             // STLport deque<ETutorialCategory>, 40 bytes
    struct iterator { ETutorialCategory* _M_cur; ETutorialCategory* _M_first; ETutorialCategory* _M_last; ETutorialCategory** _M_node; };
    iterator _M_start;
    iterator _M_finish;
    ETutorialCategory** _M_map;
    unsigned int _M_map_size;
    void _M_push_back_aux_v(const ETutorialCategory& t);    // 0x004127E0
    void push_back(const ETutorialCategory& t)
    {
        if (_M_finish._M_cur != _M_finish._M_last - 1) {
            if (_M_finish._M_cur)
                *_M_finish._M_cur = t;
            ++_M_finish._M_cur;
        } else
            _M_push_back_aux_v(t);
    }
};
struct CUserProfileManager {
    unsigned char           _pad00[0x0c];
    bool                    UseNewProfileValues;            // +0x0c
    unsigned char           _pad0d[0x17 - 0x0d];
    bool                    TutorialSystemEnabled;          // +0x17
    unsigned char           _pad18[0x8c - 0x18];
    CTutorialCategoryVector GivenTutorialCategories;        // +0x8c
    CTutorialCategoryDeque  PendingTutorialCategories;      // +0x98
    unsigned char           _padc0[0xcc - 0xc0];
    bool                    DisplayTutorialsDuringCutscenes;   // +0xcc
    unsigned char           _padcd[0xd7 - 0xcd];
    bool                    TutorialSystemEnabledNewProfile;   // +0xd7
    bool IsTutorialSystemEnabled() const { return UseNewProfileValues ? TutorialSystemEnabledNewProfile : TutorialSystemEnabled; }
    void Save();                                            // 0x0040A740
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);
};

bool CUserProfileManager::GiveTutorialForCategory(ETutorialCategory category, bool force)
{
    long* end = (long*)GivenTutorialCategories._M_finish;
    if (g_bTutorialsActive && IsTutorialSystemEnabled() &&
        std::__find((long*)GivenTutorialCategories._M_start, end, (long)category, std::random_access_iterator_tag()) == end) {
        CMainGameComponent* game = g_pMainGameComponent;
        if ((game->CutsceneOwner->Query->IsCutsceneRunning() && !DisplayTutorialsDuringCutscenes) || g_pPlayerGui->Blocked ||
            !g_pPlayerGui->SubtitleRenderer.IsAllowedToSetText(force)) {
            PendingTutorialCategories.push_back(category);
            return false;
        }
        CPlayerGuiDef* def = PeekGuiDef();
        const CCharString& message = def->GuildMasterMessages[category];
        if (message != "") {
            long id = def->GuildMasterMessageIDs[category];
            if (id > 0)
                g_pPlayerGui->ShowGuildMasterMessage(message, id, true);
            else {
                CCharString d("");
                CCharString c("");
                CCharString b("");
                CCharString a("");
                g_pPlayerGui->SubtitleRenderer.SetText(game->Font, message, a, b, c, d, 10, 0.0f, true, true);
            }
        }
        CCharString text = def->TutorialText[category];
        if (text != "") {
            CCharString title(text);
            if (title.EndsWith("_PC"))
                title = title.GetSubString(0, title.GetLength() - 3) + "_TITLE";
            else
                title += "_TITLE";
            CHeroLogBook* logBook = game->PlayerManager->GetMainPlayer()->PHeroLogBook;
            logBook->AddLogEntry(title.ToWideString(), title.ToWideString(), text.ToWideString(), 2, true);
        }
        GivenTutorialCategories.push_back(category);
        Save();
        return true;
    }
    return false;
}
