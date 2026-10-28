// NUISystem::CEditBox::ProcessEvent(EEventType event) -- while the edit box is active, a character event types the manager's last key into it (AddCharacter) and a key event passes it to KeyPressed
// Leaf: ?ProcessEvent@CEditBox@NUISystem@@UAEXW4EEventType@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00540120, 92 bytes, `ret 4`; slot 1 of CEditBox's CObserver-in-CEditBox
//   vtable 0x01246B64, so `this` is the CObserver base at +4 and the primary-vtable calls go through `add esi,-4`.
// Identity (proven): rtti-vtables CEditBox 0x01246B64 slot 1; the primary vtable 0x01246B8C slots 145 / 146 are
//   AddCharacter 0x0053FE50 / KeyPressed 0x00540320 (both landed or catalogued under those names).
// Callees: CManager::GetInstance 0x0041E5F2 (__fastcall), CManager slot 13 (+0x34) 0x0042D506 (the 16-bit key at +0xC0,
//   as the landed CEditBox callers name it GetKeyPressed), CEditBox::AddCharacter (slot 145, +0x244), KeyPressed (slot 146, +0x248).
// Layout: Ego_r.pdb CEditBox::Active +0x1B8, 0x40 lower in retail = +0x178 (+0x174 from the observer).
//   Bases per RTTI: CChangingStateComponent -> CComponent -> CUIInterface @0, CObserver @4 (CActionParam @0x18 omitted).
//   Event values 0x22 / 0x21 are compared as is (their enumerator names are not recovered).
namespace NUISystem {
enum EEventType { EVENT_KEY = 0x21, EVENT_CHARACTER = 0x22 };
class CManager { public:
    virtual void _m000(); virtual void _m001(); virtual void _m002(); virtual void _m003(); virtual void _m004(); virtual void _m005();
    virtual void _m006(); virtual void _m007(); virtual void _m008(); virtual void _m009(); virtual void _m010(); virtual void _m011();
    virtual void _m012();
    virtual unsigned short GetKeyPressed();                     // slot 13 (+0x34), 0x0042D506
    static CManager* __fastcall GetInstance();                  // 0x0041E5F2
};
class CUIInterface { public:
    virtual void _c000(); virtual void _c001(); virtual void _c002(); virtual void _c003(); virtual void _c004(); virtual void _c005(); virtual void _c006(); virtual void _c007();
    virtual void _c008(); virtual void _c009(); virtual void _c010(); virtual void _c011(); virtual void _c012(); virtual void _c013(); virtual void _c014(); virtual void _c015();
    virtual void _c016(); virtual void _c017(); virtual void _c018(); virtual void _c019(); virtual void _c020(); virtual void _c021(); virtual void _c022(); virtual void _c023();
    virtual void _c024(); virtual void _c025(); virtual void _c026(); virtual void _c027(); virtual void _c028(); virtual void _c029(); virtual void _c030(); virtual void _c031();
    virtual void _c032(); virtual void _c033(); virtual void _c034(); virtual void _c035(); virtual void _c036(); virtual void _c037(); virtual void _c038(); virtual void _c039();
    virtual void _c040(); virtual void _c041(); virtual void _c042(); virtual void _c043(); virtual void _c044(); virtual void _c045(); virtual void _c046(); virtual void _c047();
    virtual void _c048(); virtual void _c049(); virtual void _c050(); virtual void _c051(); virtual void _c052(); virtual void _c053(); virtual void _c054(); virtual void _c055();
    virtual void _c056(); virtual void _c057(); virtual void _c058(); virtual void _c059(); virtual void _c060(); virtual void _c061(); virtual void _c062(); virtual void _c063();
    virtual void _c064(); virtual void _c065(); virtual void _c066(); virtual void _c067(); virtual void _c068(); virtual void _c069(); virtual void _c070(); virtual void _c071();
    virtual void _c072(); virtual void _c073(); virtual void _c074(); virtual void _c075(); virtual void _c076(); virtual void _c077(); virtual void _c078(); virtual void _c079();
    virtual void _c080(); virtual void _c081(); virtual void _c082(); virtual void _c083(); virtual void _c084(); virtual void _c085(); virtual void _c086(); virtual void _c087();
    virtual void _c088(); virtual void _c089(); virtual void _c090(); virtual void _c091(); virtual void _c092(); virtual void _c093(); virtual void _c094(); virtual void _c095();
    virtual void _c096(); virtual void _c097(); virtual void _c098(); virtual void _c099(); virtual void _c100(); virtual void _c101(); virtual void _c102(); virtual void _c103();
    virtual void _c104(); virtual void _c105(); virtual void _c106(); virtual void _c107(); virtual void _c108(); virtual void _c109(); virtual void _c110(); virtual void _c111();
    virtual void _c112(); virtual void _c113(); virtual void _c114(); virtual void _c115(); virtual void _c116(); virtual void _c117(); virtual void _c118(); virtual void _c119();
    virtual void _c120(); virtual void _c121(); virtual void _c122(); virtual void _c123(); virtual void _c124(); virtual void _c125(); virtual void _c126(); virtual void _c127();
    virtual void _c128(); virtual void _c129(); virtual void _c130(); virtual void _c131(); virtual void _c132(); virtual void _c133(); virtual void _c134(); virtual void _c135();
    virtual void _c136(); virtual void _c137(); virtual void _c138(); virtual void _c139(); virtual void _c140(); virtual void _c141(); virtual void _c142(); virtual void _c143();
    virtual void _c144();
    virtual void AddCharacter(unsigned short c);                // slot 145 (+0x244), 0x0053FE50
    virtual void KeyPressed(unsigned short c);                  // slot 146 (+0x248), 0x00540320
};
class CObserver { public:
    virtual ~CObserver();
    virtual void ProcessEvent(EEventType event) = 0;            // slot 1
};
class CComponent : public CUIInterface, public CObserver { public:
};
class CChangingStateComponent : public CComponent { public:
};
class CEditBox : public CChangingStateComponent { public:
    unsigned char _pad08[0x178 - 8];
    bool Active;                                                // +0x178
    void ProcessEvent(EEventType event);
};
}

void NUISystem::CEditBox::ProcessEvent(EEventType event)
{
    if (Active) {
        if (event == EVENT_CHARACTER)
            AddCharacter(CManager::GetInstance()->GetKeyPressed());
        else if (event == EVENT_KEY)
            KeyPressed(CManager::GetInstance()->GetKeyPressed());
    }
}