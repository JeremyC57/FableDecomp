// CTCInventory::DeactivateAllEditBoxes(bool apply) -- deactivates every scoreboard edit box; with apply, first stores each box's text into the user profile (login, password, proxy ip, proxy and server ports) and saves it
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <string.h>

class CWideString;
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;          // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                                   // 0x0099EAE0
    CWideString ToWideString() const;                 // 0x0099E480
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CWideString {
public:
    void* m_pStringData;
    CWideString();                                    // 0x0099AED0
    CWideString(const CWideString& other);            // 0x0099B720
    ~CWideString();                                   // 0x0099B510
    CWideString& operator=(const CWideString& other); // 0x0099B7D0
    CCharString ConvertToCharString() const;          // 0x0099B2C0
};
CCharString __fastcall GFIntToCharString(long value); // 0x0099F830
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0

class CUserProfileManager { public:                          // retail offsets (Ego_r.pdb +4 above these)
    unsigned char _pad00[0x110];
    CWideString   ScoreBoardUserName;                 // +0x110 (PDB +0x114)
    CWideString   ScoreBoardPassword;                 // +0x114
    CWideString   ScoreBoardProxyIp;                  // +0x118
    long          ScoreBoardProxyPort;                // +0x11C
    long          ScoreBoardServerPort;               // +0x120
    static CUserProfileManager* __fastcall Get();     // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    void Save();                                      // 0x0040A740
    void Apply();                                     // 0x0040BC80
    CWideString GetScoreBoardUserName() const { return ScoreBoardUserName; }
    CWideString GetScoreBoardPassword() const { return ScoreBoardPassword; }
    CWideString GetScoreBoardProxyIp() const { return ScoreBoardProxyIp; }
    void SetScoreBoardUserName(const CWideString& s) { ScoreBoardUserName = s; }
    void SetScoreBoardPassword(const CWideString& s) { ScoreBoardPassword = s; }
    void SetScoreBoardProxyIp(const CWideString& s) { ScoreBoardProxyIp = s; }
    long GetScoreBoardProxyPort() const { return ScoreBoardProxyPort; }
    long GetScoreBoardServerPort() const { return ScoreBoardServerPort; }
};

#define V10(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); virtual void n##4(); \
    virtual void n##5(); virtual void n##6(); virtual void n##7(); virtual void n##8(); virtual void n##9();
namespace NUISystem {
class CEditBox { public:
    V10(v0) V10(v1) V10(v2) V10(v3) V10(v4) V10(v5) V10(v6) V10(v7)
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual CCharString GetName() const;              // slot 84 (+0x150)
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    V10(v9) V10(w0) V10(w1) V10(w2) V10(w3)
    virtual void w40(); virtual void w41();
    virtual CWideString GetText() const;              // slot 142 (+0x238)
    virtual void SetText(const CWideString& text);    // slot 143 (+0x23C)
    virtual void w44(); virtual void w45(); virtual void w46();
    virtual void RefreshText();                       // slot 147 (+0x24C)
    virtual void w48(); virtual void w49();
    virtual void SetActive(bool active);              // slot 150 (+0x258)
};
}
struct CEditBoxNode {
    CEditBoxNode*        _M_next;
    CEditBoxNode*        _M_prev;
    NUISystem::CEditBox* _M_data;
};
struct CEditBoxIterator {
    CEditBoxNode* _M_node;
    CEditBoxIterator(CEditBoxNode* n) : _M_node(n) { }
    CEditBoxIterator(const CEditBoxIterator& o) : _M_node(o._M_node) { }
    NUISystem::CEditBox* operator*() const { return _M_node->_M_data; }
    CEditBoxIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const CEditBoxIterator& o) const { return _M_node != o._M_node; }
};
struct CEditBoxList {                                 // list<NUISystem::CEditBox*> (one word)
    CEditBoxNode* _M_node;
    CEditBoxIterator begin() const { return CEditBoxIterator(_M_node->_M_next); }
    CEditBoxIterator end() const { return CEditBoxIterator(_M_node); }
};
class CTCInventory { public:
    void*         __vftable;                          // +0x00
    unsigned char _pad_0x04[0x140];
    CEditBoxList  EditBoxes;                          // +0x144
    void SetEditBoxValues();
    void DeactivateAllEditBoxes(bool apply);
};

void CTCInventory::DeactivateAllEditBoxes(bool apply)
{
    for (CEditBoxIterator it = EditBoxes.begin(); it != EditBoxes.end(); ++it) {
        if (apply) {
            CWideString text = (*it)->GetText();
            if ((*it)->GetName() == "UI_SCOREBOARD_LOGIN_EDITBOX")
                CUserProfileManager::Get()->SetScoreBoardUserName(text);
            if ((*it)->GetName() == "UI_SCOREBOARD_PASSWORD_EDITBOX")
                CUserProfileManager::Get()->SetScoreBoardPassword(text);
            if ((*it)->GetName() == "UI_SCOREBOARD_PROXY_IP_EDITBOX")
                CUserProfileManager::Get()->SetScoreBoardProxyIp(text);
            if ((*it)->GetName() == "UI_SCOREBOARD_PROXY_PORT_EDITBOX") {
                long port = GFCharStringToInt(text.ConvertToCharString());
                CUserProfileManager::Get()->ScoreBoardProxyPort = port;
            }
            if ((*it)->GetName() == "UI_SCOREBOARD_SERVER_PORT_EDITBOX") {
                long port = GFCharStringToInt(text.ConvertToCharString());
                CUserProfileManager::Get()->ScoreBoardServerPort = port;
            }
            CUserProfileManager::Get()->Save();
        }
        (*it)->SetActive(false);
    }
}