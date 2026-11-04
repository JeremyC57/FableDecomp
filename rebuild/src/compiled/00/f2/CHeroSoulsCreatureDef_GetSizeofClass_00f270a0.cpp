// CHeroSoulsCreatureDef::GetSizeofClass() -- slot 20: sizeof(CHeroSoulsCreatureDef) = 0x34
// Leaf: ?GetSizeofClass@CHeroSoulsCreatureDef@@UAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F270A0, 6 bytes: b834000000c3.
// Identity (naming wave 35, family def): Anniversary wave 2026-11-04 (high): vtable 0x012FB500 slot 20; Anniversary RTTI CHeroSoulsCreatureDef (string-literal match, validated 257/258); PDB name set has the GetSizeofClass member of CHeroSoulsCreatureDef; slot layout from named TLC siblings; function referenced by this vtable only; body=mov eax,imm;ret.
//   ?GetSizeofClass@CHeroSoulsCreatureDef@@UAEKXZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB also: ??1CHeroSoulsCreatureDef@@UAE@XZ.
// Was: Constant-return member: returns 0x34. __fastcall this=ecx. (catalog stand-in _gapscan).
// Class: the class model of the landed operator= row 0x00F27190 (rebuild/src/compiled/00/f2/CHeroSoulsCreatureDef_operator_00f27190.cpp); the PDB member declaration added; only this member is graded.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CDefClassBase {
public:
    virtual ~CDefClassBase();
    unsigned char _body[0x24];                          // +0x04 CResource body, PDefManager, GlobalIndex, flag bits
    CDefClassBase();
    CDefClassBase(const CDefClassBase& other);          // 0x0046644C
    CDefClassBase& operator=(const CDefClassBase& other); // 0x00431F10
};

class CHeroSoulsCreatureDef : public CDefClassBase {    // vtable 0x012FB500
public:
    CCharString CreatureType;                           // +0x28
    CCharString Location;                               // +0x2C
    long        NumCreatures;                           // +0x30
    CHeroSoulsCreatureDef& operator=(const CHeroSoulsCreatureDef& other);
public:
    virtual unsigned long GetSizeofClass();   // PDB ?GetSizeofClass@CHeroSoulsCreatureDef@@UAEKXZ
};

CHeroSoulsCreatureDef& CHeroSoulsCreatureDef::operator=(const CHeroSoulsCreatureDef& other)
{
    CDefClassBase::operator=(other);
    CreatureType = other.CreatureType;
    Location = other.Location;
    NumCreatures = other.NumCreatures;
    return *this;
}
unsigned long CHeroSoulsCreatureDef::GetSizeofClass()
{
    return sizeof(CHeroSoulsCreatureDef);
}
