// CTCInventory::CreateMenuDescPC(const CCharString& descriptionSymbol) -- create the description component from its UI definition ("PC_DESCRIPTION"), set its text to the game text bank's entry for the symbol, and return it counted
// Grade: DIFFER(137v135) -- 137v135 the component pointer is loaded into eax then copied to esi (retail loads it into esi directly); 11 grades: GetInstance / bank / component locals, implicit vs explicit CCharString, operator* / .Data / reference forms, QFE 4035
// Leaf: ?CreateMenuDescPC@CTCInventory@@AAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00582673, 135 bytes, thiscall, `ret 8` (hidden CCountedPointer result + symbol), ebp frame
//   in a size-optimised TU (unaligned address, `leave`).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CreateMenuDescPC@CTCInventory@@AAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@ABVCCharString@@@Z
//   (private); the definition string "PC_DESCRIPTION" and the caller CTCInventory::ConstructItemCategoryListPC 0x005907AB (landed; it declares this callee public, QAE).
// Callees: NUISystem::CFrontEndManager::GetInstance 0x0041E5F2, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, NUISystem::CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D, CCountedPointer<CComponent>
//   (CComponent*) 0x00429C15 / copy 0x00428928 / ~CCountedPointer 0x004291DE, NGameText::CDataBank::GetTextBySymbol 0x009C95E0,
//   ~CWideString 0x0099B510; the component's SetText(const CWideString&, bool, bool) vtable slot 144 (+0x240); global
//   g_pMainGameComponent 0x013B86A0 (Ego_r.pdb CMainGameComponent::PTextBank +0x14).
// Shape (family L013): `#pragma optimize("y", off)` + `("s", on)`; the definition string goes through the implicit
//   const char* -> const CCharString& conversion (the temporary's address is reloaded for the push); the text bank is
//   read into a local before the call; the CWideString temporary is homed on the symbol's argument slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWideString {
public:
    void* PStringData;
    ~CWideString();
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);
    ~CCharString();
};
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;
};
}
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
    CCountedPointer(T* data);
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();
    T* operator->() const { return Data; }
};
namespace NUISystem {
class CComponent {
public:
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
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);
};
class CManager {
public:
    CComponent* CreateComponent(const CCharString& definition, bool b);
};
class CFrontEndManager : public CManager {
public:
    static CFrontEndManager* __fastcall GetInstance();
};
}
class CMainGameComponent {
public:
    unsigned char _pad[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;
};
extern CMainGameComponent* g_pMainGameComponent;
class CTCInventory {
private:
    CCountedPointer<NUISystem::CComponent> CreateMenuDescPC(const CCharString& descriptionSymbol);
};
CCountedPointer<NUISystem::CComponent> CTCInventory::CreateMenuDescPC(const CCharString& descriptionSymbol)
{
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> description(manager->CreateComponent("PC_DESCRIPTION", false));
    const NGameText::CDataBank* bank = g_pMainGameComponent->PTextBank.operator->();
    description->SetText(bank->GetTextBySymbol(descriptionSymbol), true, true);
    return description;
}
