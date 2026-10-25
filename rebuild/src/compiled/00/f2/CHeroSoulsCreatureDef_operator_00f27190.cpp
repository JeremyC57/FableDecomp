// CHeroSoulsCreatureDef::operator=(const CHeroSoulsCreatureDef& other) -- copy assignment: CDefClassBase assignment, then CreatureType, Location and NumCreatures
// Leaf: ??4CHeroSoulsCreatureDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F27190, 51 bytes, __fastcall `ret 4`, returns `*this`.
// Identity (label correction): the catalog's "pair<CCharString,CSymbolMap::CSymbolInfo>" is propagated. The body stores
//   vtable 0x012FB500 (no RTTI locator) whose slot 20 (0x00F270A0) returns sizeof 0x34, slot 18 (0x00F27100) persists
//   "CreatureType" (+0x28), "Location" (+0x2C) and "NumCreatures" (+0x30), and slot 19 (0x00F27180) is a bare `jmp` to this
//   operator=. Twin of the copy constructor 0x00F27060. Ego_r.pdb CHeroSoulsCreatureDef (52 bytes): CDefClassBase, CreatureType +40, Location +44
//   (CCharString), NumCreatures +48 (long). Its default constructor is the landed 0x00F27650 (catalog "CFireheartMinigameDef").
// Callees: CDefClassBase::operator=(const CDefClassBase&) @ 0x00431F10, CCharString::operator=(const CCharString&)
//   @ 0x0099EFB0 (twice).
// Model: engine/CDefClassBase.h flattens CResource and the flag bits and has no member functions, so the base is modelled
//   locally (vptr + 0x24 bytes, with its out-of-line copy constructor and assignment); no CHeroSoulsCreatureDef header.
// Shape: the memberwise assignment (base first, members in declaration order, `return *this`); no vtable store.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
};

CHeroSoulsCreatureDef& CHeroSoulsCreatureDef::operator=(const CHeroSoulsCreatureDef& other)
{
    CDefClassBase::operator=(other);
    CreatureType = other.CreatureType;
    Location = other.Location;
    NumCreatures = other.NumCreatures;
    return *this;
}