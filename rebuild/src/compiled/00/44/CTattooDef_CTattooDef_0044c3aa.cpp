// CTattooDef::CTattooDef() -- the CSubDefClassBase base, this def's vtable, then its three class-type members' constructors
// Leaf: ??0CTattooDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044C3AA, 42 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x012323B4 (RTTI .?AVCTattooDef@@ : CSubDefClassBase : CDefClassBase : ...); FableWin.pdb / Ego_r.pdb
//   ??0CTattooDef@@QAE@XZ; the constructed members are the Ego_r.pdb CTattooDef members at their retail offsets (retail = Ego_r.pdb).
// Callees: CSubDefClassBase::CSubDefClassBase() 0x0044C0C0, CCharString::CCharString() 0x0099E4B0 (TattooName +0x28, CustomFileName +0x4C, CustomFileNameBase +0x50).
// Shape (family K090, representative 0x0044C3AA): the implicit-style default constructor: base call, vtable store, the
//   members' out-of-line default constructors in declaration order; the POD members stay uninitialised.
class CCharString {
public:
    CCharString();                                       // 0x0099E4B0
    void* PStringData;
};
enum ECompositeBlendType { };                           // enumerators not needed here
class CSubDefClassBase {                                            // retail 0x28 bytes (members not touched here)
public:
    CSubDefClassBase();                                             // 0x0044C0C0
    virtual ~CSubDefClassBase();
    unsigned char _body[0x28 - 4];
};
class CTattooDef : public CSubDefClassBase {
public:
    CTattooDef();
    CCharString                               TattooName;                          // +0x28
    long                                      TextureLayer;                        // +0x2C
    long                                      ReplacingTextureIndex;               // +0x30
    long                                      BankIndex;                           // +0x34
    ECompositeBlendType                       Blend;                               // +0x38
    bool                                      RemovesAllTattoos;                   // +0x3C
    bool                                      Permanent;                           // +0x3D
    long                                      CoversBodyAreaFlags;                 // +0x40
    float                                     VisibilityMultiplier;                // +0x44
    long                                      SpecificCoversBodyAreaFlags;         // +0x48
    CCharString                               CustomFileName;                      // +0x4C
    CCharString                               CustomFileNameBase;                  // +0x50
    float                                     Attractiveness;                      // +0x54
    float                                     Scariness;                           // +0x58
    float                                     Goodstrength;                        // +0x5C
};
CTattooDef::CTattooDef()
{
}