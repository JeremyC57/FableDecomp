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

class CFrontendModePointer_00495fe0
{
    void* m_Data;

public:
    void Assign(const CFrontendModePointer_00495fe0*);
};

extern CFrontendModePointer_00495fe0 g_EmptyFrontendModePointer_00495fe0;

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
    CFrontendModePointer_00495fe0 m_ModePointer;
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
        m_ModePointer.Assign(&g_EmptyFrontendModePointer_00495fe0);
        m_SaveIndex = 0;
        return;
    }

    if (mode == 12)
    {
        m_ProfileList.Refresh();
        m_ProfileIndex = 0;
    }
}
