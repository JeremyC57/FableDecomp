// NUISystem::CComponentContainer::AddTextLine(const CWideString& text, CRGBColour colour, CCharString fontName, long flags) -- resolve the named font through the front end's font bank (none for an empty name) and add the text line through the virtual AddText
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546140, 133 bytes (144 with padding). Vtable 0x01247AF4 slot 142.
// Callees: NUISystem::CFrontEndManager::GetInstance 0x0041E5F2, the font bank's GetFont(const CCharString&)
//          0x009CCDC0 (a map lookup returning the stored pointer or null; manifest label
//          CQuestManager::OnQuestStartScreenDisplayed is propagated), operator delete 0x00BFE9BC, this
//          component's slot 143 (+0x23c, AddText), CCharString::~CCharString 0x0099EAE0 (the by-value
//          parameter, destroyed by the callee).
//
// Shape: the manager is fetched first; a non-empty name (data block non-null with a non-zero length)
// copies the manager's counted font-bank pointer (+0x10 object, +0x14 count block) into a local that
// lives in registers (add-ref `inc [esi]`, release `dec [esi] ; jne`), passes the name by reference
// (its own argument slot) and keeps the result in edi. The colour is passed by value: CRGBColour's
// copy constructor goes through its inline four-channel Set (D3DCOLOR byte order, as the landed
// CScriptScreenText copy 0x0043F490), so it is built in the reserved argument slot.
namespace NUISystem {

struct SStringData {
    long m_refCount;                        // +0
    long m_length;                          // +4
};

class CCharString {
public:
    SStringData* m_pData;
    ~CCharString();                         // 0x0099EAE0
};

struct CWideString;
struct CFont;

struct CRGBColour {                         // D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};

struct CFontBank {
    CFont* GetFont(const CCharString& name);   // 0x009CCDC0
};

struct CCountedFontBank {
    CFontBank* m_pObject;                   // +0
    SCountBlock* m_pCount;                  // +4
    CCountedFontBank(const CCountedFontBank& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedFontBank()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            operator delete(m_pCount);
        }
    }
    CFontBank* operator->() const { return m_pObject; }
};

class CManager { public:
    unsigned char _pad[0x10];
    CCountedFontBank m_pFontBank;           // +0x10
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
};

class CComponentContainer { public:
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
    virtual void AddTextLine(const CWideString& text, CRGBColour colour, CCharString fontName, long flags);   // slot 142
    virtual void AddText(const CWideString& text, CRGBColour colour, CFont* font, long flags);   // slot 143 (+0x23c)
};

void CComponentContainer::AddTextLine(const CWideString& text, CRGBColour colour, CCharString fontName, long flags)
{
    CManager* frontEnd = CManager::GetInstance();
    CFont* font = 0;
    if (fontName.m_pData != 0 && fontName.m_pData->m_length != 0) {
        CCountedFontBank bank = frontEnd->m_pFontBank;
        font = bank->GetFont(fontName);
    }
    AddText(text, colour, font, flags);
}

}