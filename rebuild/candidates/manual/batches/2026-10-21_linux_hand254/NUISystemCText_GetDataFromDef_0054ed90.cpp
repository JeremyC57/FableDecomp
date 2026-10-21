// NUISystem::CText::GetDataFromDef() -- read the text component's UI def: the font bank named by the def's font string, the text window, the scale-text flag, the def's text value (with its line-break flag), and the alignment bit
// Leaf: GetDataFromDef@CText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054ED90, length 365 bytes (plain `ret`, no frame pointer; Ego_r.pdb
//   `?GetDataFromDef@CText@NUISystem@@QAEXXZ`). Called by the CText constructor 0x0054F5C0 (landed).
// Callees: CComponent::LoadAndReturnDef() const (vtable slot 108, +0x1B0; CDefPointer<const CUIDef> by hidden pointer, as
//   the landed CTextContainer::GetDataFromDef 0x00546CD0); CDefStringTable::GetString(long) 0x009D49B0 on
//   g_DefStringTable 0x013CA828 (as landed); GFGetSystemManager 0x009A4EC0 (__fastcall; its +0x84 font bank, as the landed
//   InitialiseGraphics 0x0042DDB3); the bank's GetFont(const CCharString&) 0x009E2C80 (landed label; returns the counted
//   pointer by hidden pointer); the count block's destroy function (+4, object in ecx) and operator delete 0x00BFE9BC on
//   the last release; ~CCharString 0x0099EAE0; CText::SetValue(const CWideString&, bool, bool) vtable slot 144 (+0x240);
//   the def's virtual slot 1 (destroy) on the last release.
// Layout: retail CText (the landed constructor: Ego_r.pdb minus 0x30): PFont +0x160 (Data) / +0x164 (Info), Window
//   +0x178, ScaleText +0x188; CComponent FlagsSetTwo +0x12E (bits 3/4/5 = left/centre/right alignment, as the landed
//   vfunc_170..172 0x0053CE32..50). Retail CUIDef (PDB minus 4 for the def base, minus 4 per STLport vector/map before
//   the field): Font (CDefString) +0x50, TextValue +0x54, TextWindow +0xA4, TextLineBreak +0xBD, ScaleText +0xBE,
//   Alignement +0x1FC.
// Shape: the font name comes through an inline CDefString accessor returning GetString's result by value (a direct
//   GetString temporary pushes its address before the system-manager call; a named local loses the temporary's
//   clearing stores); the bank through an inline CSystemManager getter (eax, then `mov ecx, eax`); the font is assigned
//   with CCountedPointer's inline operator= (the source words read into ebx/edi first, count-block compare, release
//   re-reading the member, add-ref) and the temporary's release clears both words; Window is a struct copy; the
//   alignment switch ORs one bit per case into the flag byte.
namespace NUISystem {
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public: void* PStringData; };
struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
struct CDefString {
    long Index;
    CCharString GetString() const { return g_DefStringTable.GetString(Index); }
};

struct CCPPointerInfo {                                 // CCountedPointer count block
    long References;                                    // +0
    void (__fastcall* Destroy)(void* object);           // +4
    void* Object;                                       // +8
};
template <class T> struct CCountedPointer {
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    void Release()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->References;
        }
        return *this;
    }
};
struct CFontBank;
struct CFontBankSource {                                // the system manager's font bank (+0x84)
    CCountedPointer<CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
struct CSystemManager {
    unsigned char    _pad00[0x84];
    CFontBankSource* FontBank;                          // +0x84
    CFontBankSource* GetFontBank() const { return FontBank; }
};
CSystemManager* __fastcall GFGetSystemManager();        // 0x009A4EC0

struct CRefCounted {                                    // second base of CDefBase: the count at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
enum ETextAlignement { TEXT_ALIGN_LEFT = 0, TEXT_ALIGN_CENTRE = 1, TEXT_ALIGN_RIGHT = 2 };
struct CUIDef : CDefVirtual, CRefCounted {
    unsigned char   _pad08[0x50 - 0x08];
    CDefString      Font;                               // +0x50
    CWideString     TextValue;                          // +0x54
    unsigned char   _pad58[0xa4 - 0x58];
    C2DBoxF         TextWindow;                         // +0xa4
    unsigned char   _padb4[0xbd - 0xb4];
    bool            TextLineBreak;                      // +0xbd
    bool            ScaleText;                          // +0xbe
    unsigned char   _padbf[0x1fc - 0xbf];
    ETextAlignement Alignement;                         // +0x1fc
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer_CUIDef {                             // CDefPointer<const NUISystem::CUIDef>
    const CUIDef* Data;
    ~CDefPointer_CUIDef() { if (Data) const_cast<CUIDef*>(Data)->ReleaseTemporary(); }
    const CUIDef* operator->() const { return Data; }
};

struct CText {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer_CUIDef LoadAndReturnDef() const;    // slot 108 (+0x1B0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void SetValue(const CWideString& value, bool b1, bool b2);   // slot 144 (+0x240)
    unsigned char              _pad004[0x12e - 0x04];
    unsigned char              FlagsSetTwo;             // +0x12e (CComponent)
    unsigned char              _pad12f[0x160 - 0x12f];
    CCountedPointer<CFontBank> PFont;                   // +0x160
    unsigned char              _pad168[0x178 - 0x168];
    C2DBoxF                    Window;                  // +0x178
    bool                       ScaleText;               // +0x188
    void GetDataFromDef();
};

void CText::GetDataFromDef()
{
    CDefPointer_CUIDef def = LoadAndReturnDef();
    PFont = GFGetSystemManager()->GetFontBank()->GetFont(def->Font.GetString());
    Window = def->TextWindow;
    ScaleText = def->ScaleText;
    SetValue(def->TextValue, true, def->TextLineBreak);
    switch (def->Alignement) {
    case TEXT_ALIGN_LEFT:
        FlagsSetTwo |= 0x08;
        break;
    case TEXT_ALIGN_CENTRE:
        FlagsSetTwo |= 0x10;
        break;
    case TEXT_ALIGN_RIGHT:
        FlagsSetTwo |= 0x20;
        break;
    }
}
}
