// CMovieSubtitles::Play(float time, bool display) -- per-frame movie subtitle update: lay the subtitle window out in the bottom of the render target (10% side margins, 300..30 pixels up) in virtual pixels; when the front subtitle's start time is reached format its text into the window and play its "SND_" sample, when its end time is reached drop it; draw the current text centred when asked; count the frame
// Leaf: ?Play@CMovieSubtitles@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BD910, 496 bytes, thiscall `ret 8`. PDB ?Play@CMovieSubtitles@@QAEXM_N@Z;
//   single caller 0x00628C56 (the movie player, subtitles at +0x30). Catalog label
//   "CEngineInternalPrimitiveBase_UpdateSubtitleDisplay" is descriptive.
// Callees: GFGetSystemManager 0x009A4EC0, CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (hidden C2DExtentsI),
//   GFABoxToVBox 0x009E2020, NGameText::CTextEntry::GetText 0x009C7A40, FormatTextForVWindow 0x009BC9D0, ~CWideString
//   0x0099B510, the sound-bank lookup 0x009917E0 on PSoundManager (ICF-shared with CSymbolMap::FindFirstSymbolWithVal),
//   CCharString copy 0x0099EC30 / operator+ 0x0099F690 ("SND_" 0x01239ED4) / dtor 0x0099EAE0,
//   CASoundBank::GetSoundIndexFromSymbol 0x00A01920, PSoundManager virtual +0x24 then that object's virtual +0x0C
//   (bank, sample, 1, init, 0), list<CSubTitle>::pop_front 0x009BE100, DisplayCenteredText 0x009BCA20.
// Globals: the subtitle font bank pointer 0x013CA7F8; 0.1f 0x0122EE58.
// Layout: CMovieSubtitles / CSubTitle as the landed constructor 0x009BDBA0 (retail STLport list = the sentinel pointer);
//   NGameText::CTextEntry BankName +0x1c / SoundName +0x24 (landed 0x0066BA80); CSoundInit = Ego_r.pdb.
// Shape: GFGetSystemManager() is a named local (retail calls it before pushing the extents' hidden pointer); the screen box
//   is a named C2DBoxF built from two C2DVector temporaries (top-left, bottom-right) -- the 4-float constructor keeps the
//   width on the x87 stack where retail spills it and stores the bottom edge through a float temporary; the font reference
//   is a named local read before GetText(); CSoundInit's inline default constructor assigns in the 0x0041C930 order
//   (PlaybackType last) and GainFaderBits = 8 overrides its zero; size() is the STLport counting walk.
typedef unsigned short wchar_t;

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};

struct C2DExtentsI {
    long Width, Height;
    C2DExtentsI() { }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    C2DBoxF(const C2DVector& tl, const C2DVector& br) : TLX(tl.X), TLY(tl.Y), BRX(br.X), BRY(br.Y) { }
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

class CDisplayManager {
public:
    C2DExtentsI GetRenderTargetDimensions() const;          // 0x009BEDC0
};
class CSystemManager {
public:
    unsigned char _pad00[0x60];
    CDisplayManager* PDisplayManager;                       // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0
void __fastcall GFABoxToVBox(const C2DBoxF& abox, C2DBoxF& vbox);   // 0x009E2020

class CFontBank;
extern CFontBank* g_pSubtitleFont;                          // 0x013CA7F8
bool __fastcall FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text,
                                     CWideString& outText, C2DBoxF& outBox);   // 0x009BC9D0
void __fastcall DisplayCenteredText(const CFontBank& font, const CWideString& text, const C2DBoxF& box);   // 0x009BCA20

namespace NGameText {
class CTextEntry {
public:
    unsigned char _pad00[0x1c];
    CCharString BankName;                                   // +0x1c
    unsigned char _pad20[0x24 - 0x20];
    CCharString SoundName;                                  // +0x24
    CWideString GetText() const;                            // 0x009C7A40
};
}
template <class T> class CIVCountedPointer {
public:
    T* Data;
    T* operator->() const { return Data; }
};

struct CSubTitle {                                          // 0x1c bytes
    CIVCountedPointer<const NGameText::CTextEntry> TextEntry;   // +0x00
    CWideString FormattedText;                              // +0x04
    CCharString Sound;                                      // +0x08
    long        FrameStart;                                 // +0x0c
    long        FrameEnd;                                   // +0x10
    float       StartTime;                                  // +0x14
    float       EndTime;                                    // +0x18
};

namespace std {
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> class list {
public:
    _List_node<T>* _M_node;
    unsigned int size() const
    {
        unsigned int n = 0;
        for (_List_node<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    T& front() { return _M_node->_M_next->_M_data; }
    void pop_front();                                       // 0x009BE100
};
}

class CASoundBank {
public:
    long GetSoundIndexFromSymbol(const CCharString& symbol) const;   // 0x00A01920
};
class CSoundInit {
public:
    float Volume, Pitch, Pan;
    long NoPlays, PlaybackType;
    unsigned long GainFaderBits, PitchFaderBits;
    long OverrideID;
    CSoundInit()
    {
        Volume = 1.0f;
        Pitch = 1.0f;
        Pan = 0.0f;
        NoPlays = 1;
        GainFaderBits = 0;
        PitchFaderBits = 0;
        OverrideID = -1;
        PlaybackType = 1;
    }
};
class CASoundPlayer {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void PlaySample(CASoundBank* bank, long sample, long count, const CSoundInit& init, long flags);   // slot 3
};
class CASoundManager {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual CASoundPlayer* GetPlayer();                     // slot 9 (+0x24)
    CASoundBank* FindSoundBank(const CCharString* bankName) const;   // 0x009917E0
};
class CDataBank;

class CMovieSubtitles {
public:
    std::list<CSubTitle> SubTitles;                         // +0x00
    const CDataBank*     pTextBank;                         // +0x04
    long                 Fps;                               // +0x08
    CASoundManager*      PSoundManager;                     // +0x0c
    bool                 IsSubtitleTime;                    // +0x10
    CWideString          FormattedText;                     // +0x14
    C2DBoxF              VSubtitleWindow;                   // +0x18
    long                 FrameCounter;                      // +0x28
    void Play(float time, bool display);
};

extern const char g_szSoundPrefix_01239ed4[];               // 0x01239ED4 "SND_"

void CMovieSubtitles::Play(float time, bool display)
{
    CSystemManager* systemManager = GFGetSystemManager();
    C2DExtentsI dims = systemManager->PDisplayManager->GetRenderTargetDimensions();
    float width = (float)dims.Width;
    float margin = width * 0.1f;
    C2DBoxF screenBox(C2DVector(margin, (float)(dims.Height - 300)), C2DVector(width - margin, (float)(dims.Height - 30)));
    GFABoxToVBox(screenBox, VSubtitleWindow);
    if (SubTitles.size() != 0) {
        CSubTitle& subtitle = SubTitles.front();
        if (!IsSubtitleTime) {
            if (time >= subtitle.StartTime) {
                IsSubtitleTime = true;
                const CFontBank& font = *g_pSubtitleFont;
                C2DBoxF box;
                FormatTextForVWindow(VSubtitleWindow, font, subtitle.TextEntry->GetText(), FormattedText, box);
                CASoundBank* bank = PSoundManager->FindSoundBank(&subtitle.TextEntry->BankName);
                long sample = 0;
                if (bank) {
                    CCharString soundName = subtitle.TextEntry->SoundName;
                    sample = bank->GetSoundIndexFromSymbol(soundName);
                    if (sample == 0)
                        sample = bank->GetSoundIndexFromSymbol(g_szSoundPrefix_01239ed4 + soundName);
                }
                CSoundInit init;
                init.GainFaderBits = 8;
                PSoundManager->GetPlayer()->PlaySample(bank, sample, 1, init, 0);
            }
        }
        else if (time >= subtitle.EndTime) {
            IsSubtitleTime = false;
            SubTitles.pop_front();
        }
    }
    if (IsSubtitleTime && display)
        DisplayCenteredText(*g_pSubtitleFont, FormattedText, VSubtitleWindow);
    ++FrameCounter;
}