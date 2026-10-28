// CAnimFilterOf2Groups::SetCurrentGroupNames(const CCharString& group1, const CCharString& group2) -- take both group names and recompute their CRCs
// Leaf: ?SetCurrentGroupNames@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073E040, 129 bytes (+15 int3), __fastcall `ret 8`.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0 (x2), Calc @ 0x004014A0 (x2, the CRC-32 kernel; the
//   shared empty literal 0x0129AAF4 stands in for a name with no data block).
// Caller: the landed CAnimFilterOf2Groups::CAnimFilterOf2Groups 0x0073E9D0 (it calls this address after building both
//   names).
// Label: the manifest's "CAnimFilterOf2Groups_Blend" is wrong about the action; Ego_r.pdb names
//   `?SetCurrentGroupNames@CAnimFilterOf2Groups@@QAEXABVCCharString@@0@Z`. Layout = Ego_r.pdb CAnimFilterOf2Groups (20
//   bytes): Group1 +4, Group2 +8, Group1CRC +0xC, Group2CRC +0x10.
// Shape: `Group1 = group1; Group1CRC = Calc(0, group1.GetPString(), group1.GetLength()); Group2 = group2; Group2CRC =
//   ...;` -- the first CRC's arms join before the call; the last call's tail is duplicated into both arms.
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

class CAnimFilterOf2Groups : public CAnimFilterBase {
public:
    CCharString   Group1;                                       // +0x04
    CCharString   Group2;                                       // +0x08
    unsigned long Group1CRC;                                    // +0x0C
    unsigned long Group2CRC;                                    // +0x10
    void SetCurrentGroupNames(const CCharString& group1, const CCharString& group2);
};

void CAnimFilterOf2Groups::SetCurrentGroupNames(const CCharString& group1, const CCharString& group2)
{
    Group1 = group1;
    Group1CRC = CCRC::Calc(0, group1.GetPString(), group1.GetLength());
    Group2 = group2;
    Group2CRC = CCRC::Calc(0, group2.GetPString(), group2.GetLength());
}