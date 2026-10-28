// CAnimFilterOfGroup::SetCurrentGroupName(const CCharString& name) -- take the new group name and recompute its CRC
// Leaf: ?SetCurrentGroupName@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073DFF0, 77 bytes (+3 int3), __fastcall `ret 4`.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, Calc @ 0x004014A0 (`__fastcall(seed, data, length)`, the
//   engine's CRC-32 kernel; run over the shared empty literal 0x0129AAF4 when the name has no data block).
// Label: the manifest's "CRCHashedString_SetAndHash" is descriptive; Ego_r.pdb names
//   `?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z`. Layout = Ego_r.pdb CAnimFilterOfGroup (12 bytes):
//   Group +4, GroupCRC +8 (as in the landed constructor 0x0073E8F0).
// Shape: the landed constructor's body as a member: `Group = name; GroupCRC = Calc(0, name.GetPString(),
//   name.GetLength());` -- the two inline accessors each test the data block (length into ecx first, right to left), and
//   VC7.1 duplicates the call + store + return tail into both arms of the GetPString test.
class CCharStringData {
public:
    const char*   PData;                                        // +0  (CBasicString<char>::PData)
    unsigned long StringLength;                                 // +4
};

class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                              // 0x0099E4B0
    CCharString(const CCharString& other);                      // 0x0099EC30
    const CCharString& operator=(const CCharString& other);           // 0x0099EFB0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->PData; }   // "" = the shared empty literal 0x0129AAF4
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC-32 kernel

class CAnimationEntry;

class CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& anim) const;
    virtual CAnimFilterBase* Clone() const;
    virtual ~CAnimFilterBase();
    CAnimFilterBase() {}
};

class CAnimFilterOfGroup : public CAnimFilterBase {
public:
    CCharString   Group;                                        // +0x04
    unsigned long GroupCRC;                                     // +0x08
    void SetCurrentGroupName(const CCharString& name);
};

void CAnimFilterOfGroup::SetCurrentGroupName(const CCharString& name)
{
    Group = name;
    GroupCRC = CCRC::Calc(0, name.GetPString(), name.GetLength());
}