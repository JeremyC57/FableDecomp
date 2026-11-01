// CFrontendGameComponent::SetMode @ 0x00495FE0.

#include <cstring>

// The two lists are different types with different refresh bodies: the save list's Refresh is 0x00495830, the
// profile list's (a disk enumeration with wide-string path pieces "*", ":", "\\") is 0x004959E0.
class CFrontendSaveList_00495fe0
{
public:
    void Refresh();                 // 0x00495830
};
class CFrontendProfileList_00495fe0
{
public:
    void Refresh();                 // 0x004959E0
};

// Naming (naming wave 24): the CWideString at +0x1E40 is Ego_r's WorldToLoad (CFrontendGameComponent PDB +0x1E4C less the three 4-byte
// vector shrinks; the landed GetWorldNames 0x00495890 models it so), cleared
// to the empty wide literal 0x0122D70C through CWideString::operator=(const wchar_t*) 0x0099B800 (??4CWideString@@QAEABV0@PBG@Z); it
// was a CFrontendModePointer_00495fe0 stand-in with an Assign member and a g_EmptyFrontendModePointer_00495fe0 object.
class CWideString
{
    void* PStringData;

public:
    const CWideString& operator=(const unsigned short* text);   // 0x0099B800
};

class CFrontendGameComponent
{
    unsigned char m_Pad0000[0x1c];
    long m_Mode;
    unsigned char m_Pad0020[0x148];
    long m_ResetValues[32];
    long m_Cursor;
    unsigned char m_Pad01ec[0x1c20];
    CFrontendProfileList_00495fe0 m_ProfileList;
    unsigned char m_Pad1e0d[0x0b];
    long m_ProfileIndex;
    CFrontendSaveList_00495fe0 m_SaveList;
    unsigned char m_Pad1e1d[0x0b];
    long m_SaveIndex;
    unsigned char m_Pad1e2c[0x14];
    CWideString WorldToLoad;
    long m_ModePointerEnabled;


public:
    void SetMode(long);
private:
    void GetWorldNames();   // 0x00495890
};

void CFrontendGameComponent::SetMode(long mode)
{
    m_Mode = mode;
    m_ModePointerEnabled = 1;
    std::memset(m_ResetValues, 0, sizeof(m_ResetValues));
    m_Cursor = 0;

    if (m_Mode == 10)
    {
        GetWorldNames();
        return;
    }

    if (mode == 11)
    {
        m_SaveList.Refresh();
        WorldToLoad = L"";
        m_SaveIndex = 0;
        return;
    }

    if (mode == 12)
    {
        m_ProfileList.Refresh();
        m_ProfileIndex = 0;
    }
}
