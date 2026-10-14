// CAnimFilterOfGroup::CAnimFilterOfGroup(const CCharString&) -- own vtable, Group default-built then assigned, GroupCRC = CRC of the group name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073E8F0, length 80 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0, CCharString::operator=(const CCharString&)
// @ 0x0099EFB0, Calc @ 0x004014A0 (`__fastcall(seed, data, length)`, the engine's CRC-32 kernel; run
// over the shared empty literal 0x0129AAF4 when the name has no data block). Vtable 0x01267568 =
// CAnimFilterOfGroup (rtti-vtables.tsv); the manifest's "CConsole::SendMessageToUser" label is
// propagated. Layout = Ego_r.pdb CAnimFilterOfGroup (12 bytes): Group +4, GroupCRC +8.
// Unlike the copy inlined into the CMode* constructors (landed 0x007409C0, one shared null test),
// this body tests the data block TWICE: the CRC call takes `GetLength()` and `GetPString()` as two
// separate inline accessors, evaluated right to left (length into ecx first, then the data pointer),
// each with its own `p ? ... : ...` guard.
struct CCharStringData {
    const char*   m_pData;                                      // +0  (CBasicString<char>::PData)
    unsigned long m_length;                                     // +4  (StringLength)
};

struct CCharString {
    CCharStringData* m_pData;
    CCharString();                                              // 0x0099E4B0
    CCharString& operator=(const CCharString& other);           // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }   // null case written first: the load is the taken branch
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // "" = the shared empty literal 0x0129AAF4
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC-32 kernel

struct CAnimFilterOfGroup {
    virtual void v00();                                         // vtable 0x01267568
    CCharString   Group;                                        // +4
    unsigned long GroupCRC;                                     // +8
    CAnimFilterOfGroup(const CCharString& name);
};

CAnimFilterOfGroup::CAnimFilterOfGroup(const CCharString& name)
    : Group()
{
    Group = name;
    GroupCRC = Calc_004014a0(0, name.GetPString(), name.GetLength());
}