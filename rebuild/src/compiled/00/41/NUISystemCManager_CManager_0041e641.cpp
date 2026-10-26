// NUISystem::CManager::~CManager() -- destructor of the front-end UI manager: own vtable; clear a showing error message; hand the front-end manager singleton empty font and graphics banks; then the members (+0x88 down to +0x10) and the base are destroyed
// Leaf: ??1CManager@NUISystem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041E641, length 146 bytes. Size-optimised TU: `#pragma optimize("s", on)`.
// Callees: CManager::ClearErrorMessage @ 0x0041CE6A, CFrontEndManager::GetInstance @ 0x0041E5F2 (`__fastcall`, lazily creates the
//   singleton), the bank setters @ 0x0041D198 (counted pointer at +0x18) and @ 0x0042A9B7 (landed SetGraphicsBank, +0x10), the member
//   destructors @ 0x0042CE63 / 0x004291DE / 0x0042B7D7 / 0x0042AB1C / 0x0042AB73 / 0x0042B796 / 0x0042918C / 0x00419108 (their manifest
//   labels -- CActiveFile::OnReadFinished, _Dest_val<...> -- are byte-twin propagations), the base destructor @ 0x0042BEC0 (tail jump).
// Layout: retail NUISystem::CManager (vtable 0x01230134): a `__declspec(novtable)` base of 0x10 (its destructor stores no vtable; members at
//   +4/+0xc), counted banks +0x10 / +0x18, members at +0x24, +0x5c, +0x60, +0x70, +0x80, +0x88, the error-showing flag +0x9c
//   (engine/CManager.h is the debug layout, 0x10 higher for the banks -- modelled locally, as the landed 0x0042A9B7).
// Shape: the empty counted-pointer arguments are default-constructed in their argument slots (a declared copy constructor), zeroed from
//   the size-TU zero register ebx (`push ecx ; push ecx ; mov eax,esp ; mov [eax],ebx`); GetInstance is evaluated after the argument.
#pragma optimize("s", on)
class CGraphicDataBank;
class CFontBank;
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              m_pObject;
    CCPPointerInfo* m_pInfo;
    CCountedPointer() : m_pObject(0), m_pInfo(0) { }
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();
};
struct SMember10 { void* a; void* b; ~SMember10(); };            // 0x00419108
struct SMember18 { void* a; void* b; ~SMember18(); };            // 0x0042918C
struct SMember24 { unsigned char _d[0x38]; ~SMember24(); };       // 0x0042B796
struct SMember5c { void* a; ~SMember5c(); };                      // 0x0042AB73
struct SMember60 { unsigned char _d[0x10]; ~SMember60(); };       // 0x0042AB1C
struct SMember70 { unsigned char _d[0x10]; ~SMember70(); };       // 0x0042B7D7
struct SMember80 { unsigned char _d[0x8]; ~SMember80(); };        // 0x004291DE
struct SMember88 { unsigned char _d[0x14]; ~SMember88(); };       // 0x0042CE63
namespace NUISystem {
struct __declspec(novtable) CManagerBase {
    virtual ~CManagerBase();                       // 0x0042BEC0
    unsigned char _d04[0xc];
};
class CFrontEndManager { public:
    static CFrontEndManager* __fastcall GetInstance();                        // 0x0041E5F2
    void SetFontBank(CCountedPointer<CFontBank> bank);                        // 0x0041D198
    void SetGraphicsBank(CCountedPointer<CGraphicDataBank> bank);             // 0x0042A9B7
};
class CManager : public CManagerBase { public:
    SMember10     m_graphicsBank;                  // +0x10
    SMember18     m_fontBank;                      // +0x18
    unsigned char _pad20[4];
    SMember24     m_24;                            // +0x24
    SMember5c     m_5c;                            // +0x5c
    SMember60     m_60;                            // +0x60
    SMember70     m_70;                            // +0x70
    SMember80     m_80;                            // +0x80
    SMember88     m_88;                            // +0x88
    bool          m_errorMessageShowing;           // +0x9c
    void ClearErrorMessage();                      // 0x0041CE6A
    virtual ~CManager();
};
CManager::~CManager()
{
    if (m_errorMessageShowing)
        ClearErrorMessage();
    CFrontEndManager::GetInstance()->SetFontBank(CCountedPointer<CFontBank>());
    CFrontEndManager::GetInstance()->SetGraphicsBank(CCountedPointer<CGraphicDataBank>());
}
}