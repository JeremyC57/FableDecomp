// CHeroSoulsCreatureDef::CHeroSoulsCreatureDef() -- default constructor: CDefClassBase, own vtable, then the CreatureType and Location strings (NumCreatures left to the defaults pass)
// Leaf: ??0CHeroSoulsCreatureDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F27650, 48 bytes, thiscall, returns `this`: CDefClassBase::CDefClassBase() 0x0042D9E0,
//   vtable 0x012FB500, CCharString() 0x0099E4B0 on +0x28 and +0x2C (the first member address formed before the vtable store).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 456 "landed 0xF27650 CFireheartMinigameDef = CHeroSoulsCreatureDef default ctor,
//   and in it 0x42D9E0 is the CDefClassBase ctor (not CPSCBase)"; hand456 README): vtable 0x012FB500 is the one the landed
//   CHeroSoulsCreatureDef copy constructor 0x00F27060 stores (slot 20 returns sizeof 0x34, slot 18 persists CreatureType +0x28,
//   Location +0x2C, NumCreatures +0x30), and that row names this one as its default constructor; Ego_r.pdb
//   CHeroSoulsCreatureDef (52 bytes) = CDefClassBase, CreatureType +40, Location +44, NumCreatures +48; the base ctor
//   0x0042D9E0 is landed as CDefClassBase::CDefClassBase. Callers 0x00F275D0 / 0x00F27B6B.
// Model: the 0x00F27060 row's local CDefClassBase (engine/CDefClassBase.h has no member functions) and CCharString.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

class CDefClassBase {
public:
    virtual ~CDefClassBase();
    unsigned char _body[0x24];                          // +0x04 CResource body, PDefManager, GlobalIndex, flag bits
    CDefClassBase();                                    // 0x0042D9E0
    CDefClassBase(const CDefClassBase& other);          // 0x0046644C
};

class CHeroSoulsCreatureDef : public CDefClassBase {    // vtable 0x012FB500
public:
    CCharString CreatureType;                           // +0x28
    CCharString Location;                               // +0x2C
    long        NumCreatures;                           // +0x30
    CHeroSoulsCreatureDef();
};

CHeroSoulsCreatureDef::CHeroSoulsCreatureDef()
{
}
