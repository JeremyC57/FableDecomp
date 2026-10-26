// CUserProfileManager::SetAllTutorialsAsGiven() -- add every tutorial (1..94) with a text to the hero's log book under its _TITLE key (a "_PC" text key loses the suffix first) and mark each category as given
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00408A70, length 538 bytes, plain `ret`.
// Callees: PeekGuiDef 0x00438570 (__fastcall, CPlayerGuiDef*), GetMainPlayer 0x00449970 (on g_pMainGameComponent 0x013B86A0
//   +0x1c), CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0
//   / operator!=(const char*) 0x0099E960 / GetSubString 0x0099EC70 / operator= 0x0099EFB0 / operator+=(const char*)
//   0x0099F100 / ToWideString 0x0099E480, operator+(const CCharString&, const char*) 0x0099F600 (__fastcall, hidden
//   result in ecx), ~CWideString 0x0099B510, CHeroLogBook::AddLogEntry 0x0047A9F0, vector<ETutorialCategory>::
//   _M_insert_overflow 0x00411A10. Strings "", "_PC", "_TITLE"; CCharString empty text 0x0129AAF4.
// Layout: CPlayer (Ego_r.pdb) PHeroLogBook +0xf8 (CCountedPointer data); CUserProfileManager GivenTutorialCategories retail
//   +0x8c (PDB +0x98, 12-byte retail vectors); the def's tutorial text-key vector at retail +0x91c (PDB TutorialText +0x980).
// Shape: CCharString::EndsWith is an inline member (length difference, then the strcmp intrinsic on the tail).
#include <string.h>
#pragma intrinsic(strcmp)
class CCharStringData { public: const char* PData; long Length; };
class CWideString { public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
class CCharString { public:
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
class CPlayerGuiDef { public:
    unsigned char       _pad000[0x91c];
    vector<CCharString> TutorialText;                       // +0x91c
};
CPlayerGuiDef* __fastcall PeekGuiDef();                     // 0x00438570
class CHeroLogBook { public:
    void AddLogEntry(const CWideString& title, const CWideString& heading, const CWideString& text, long category, bool given);   // 0x0047A9F0
};
class CPlayer { public:
    unsigned char _pad000[0xf8];
    CHeroLogBook* PHeroLogBook;                             // +0xf8 (CCountedPointer data)
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                               // 0x00449970
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                          // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE };
struct __true_type { };
struct CTutorialCategoryVector {                            // STLport vector<ETutorialCategory>
    ETutorialCategory* _M_start;
    ETutorialCategory* _M_finish;
    ETutorialCategory* _M_end_of_storage;
    void _M_insert_overflow(ETutorialCategory* pos, const ETutorialCategory& x, const __true_type&, unsigned int count, bool atend);   // 0x00411A10
    void push_back(const ETutorialCategory& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else {
            __true_type trivial;
            _M_insert_overflow(_M_finish, x, trivial, 1, true);
        }
    }
};
class CUserProfileManager { public:
    unsigned char           _pad00[0x8c];
    CTutorialCategoryVector GivenTutorialCategories;        // +0x8c
    void SetAllTutorialsAsGiven();
};

void CUserProfileManager::SetAllTutorialsAsGiven()
{
    CPlayerGuiDef* def = PeekGuiDef();
    CHeroLogBook* logBook = g_pMainGameComponent->PlayerManager->GetMainPlayer()->PHeroLogBook;
    for (long i = 0; i < 95; ++i) {
        if (i > 0) {
            CCharString text = def->TutorialText[i];
            if (text != "") {
                CCharString title(text);
                if (title.EndsWith("_PC"))
                    title = title.GetSubString(0, title.GetLength() - 3) + "_TITLE";
                else
                    title += "_TITLE";
                logBook->AddLogEntry(title.ToWideString(), title.ToWideString(), text.ToWideString(), 2, true);
            }
            GivenTutorialCategories.push_back((ETutorialCategory)i);
        }
    }
}