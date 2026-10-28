// CAnimFilterOf3Groups::CAnimFilterOf3Groups(const CCharString& group1, const CCharString& group2, const CCharString& group3) -- copy the three group names and cache their CRCs
// Leaf: ??0CAnimFilterOf3Groups@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073E0D0, 178 bytes (+14 int3), __fastcall `ret 0Ch`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x3), Calc @ 0x004014A0 (x3, the CRC-32 kernel; the
//   shared empty literal 0x0129AAF4 stands in for a name with no data block). Vtable 0x01267480 = CAnimFilterOf3Groups
//   (rtti-vtables.tsv: slot 0 0x0073E1D0, slot 1 Clone 0x0073E2F0, slot 2 the deleting dtor 0x0073E190).
// Caller: the landed CAnimFilterOf3Groups::Clone 0x0073E2F0 (and the creature-mode constructors).
// Label: the manifest's "CAnimFilterOf3Groups_Init" is descriptive; Ego_r.pdb names the constructor
//   `??0CAnimFilterOf3Groups@@QAE@ABVCCharString@@00@Z`. Layout = Ego_r.pdb CAnimFilterOf3Groups (28 bytes): Group1..3
//   +4/+8/+0xC, Group1CRC..Group3CRC +0x10/+0x14/+0x18.
// Shape: the vtable store, the three copy-constructed names (init list), then each CRC from the PARAMETER (`edi`, `ebx`,
//   `ebp`), never the member: `GroupNCRC = Calc(0, groupN.GetPString(), groupN.GetLength());`; the CAnimFilterBase
//   constructor is inline and its vtable store is dead.
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

class CAnimFilterOf3Groups : public CAnimFilterBase {
public:
    CCharString   Group1;                                       // +0x04
    CCharString   Group2;                                       // +0x08
    CCharString   Group3;                                       // +0x0C
    unsigned long Group1CRC;                                    // +0x10
    unsigned long Group2CRC;                                    // +0x14
    unsigned long Group3CRC;                                    // +0x18
    virtual bool operator()(const CAnimationEntry& anim) const;
    virtual CAnimFilterBase* Clone() const;
    CAnimFilterOf3Groups(const CCharString& group1, const CCharString& group2, const CCharString& group3);
};

CAnimFilterOf3Groups::CAnimFilterOf3Groups(const CCharString& group1, const CCharString& group2, const CCharString& group3)
    : Group1(group1)
    , Group2(group2)
    , Group3(group3)
{
    Group1CRC = CCRC::Calc(0, group1.GetPString(), group1.GetLength());
    Group2CRC = CCRC::Calc(0, group2.GetPString(), group2.GetLength());
    Group3CRC = CCRC::Calc(0, group3.GetPString(), group3.GetLength());
}