// CHeroSoulsCreatureDef::CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef& other) -- copy constructor: CDefClassBase copy, own vtable, then CreatureType, Location and NumCreatures
// Leaf: ??0CHeroSoulsCreatureDef@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F27060, 57 bytes, __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<CCharString,CSymbolMap::CSymbolInfo>" is propagated. The body stores
//   vtable 0x012FB500 (no RTTI locator) whose slot 20 (0x00F270A0) returns sizeof 0x34, slot 18 (0x00F27100) persists
//   "CreatureType" (+0x28), "Location" (+0x2C) and "NumCreatures" (+0x30), and slot 19 (0x00F27180) jumps to the twin
//   operator= 0x00F27190. Ego_r.pdb CHeroSoulsCreatureDef (52 bytes): CDefClassBase, CreatureType +40, Location +44
//   (CCharString), NumCreatures +48 (long). Its default constructor is the landed 0x00F27650 (catalog "CFireheartMinigameDef").
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C, CCharString::CCharString(const CCharString&)
//   @ 0x0099EC30 (twice).
// Model: the shared FABLE_RETAIL_DEF_CLASS_BASE CDefClassBase (0x28 bytes, vtable 0x01230BCC); no CHeroSoulsCreatureDef header.
// Shape: the memberwise copy constructor; VC7.1 forms the first member's argument and `this` before the vtable store.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                      // copy ctor 0x0046644C, operator= 0x00431F10

class CHeroSoulsCreatureDef : public CDefClassBase {    // vtable 0x012FB500 (no RTTI); slot 0 0x00F270B0
public:
    CCharString CreatureType;                           // +0x28
    CCharString Location;                               // +0x2C
    long        NumCreatures;                           // +0x30
    CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef& other);
    virtual void Transfer(CPersistContext&);            // slot 18 0x00F27100
    virtual void Copy(const CDefClassBase*);            // slot 19 0x00F27180
    virtual unsigned long GetSizeofClass();             // slot 20 0x00F270A0
};

CHeroSoulsCreatureDef::CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef& other)
    : CDefClassBase(other), CreatureType(other.CreatureType), Location(other.Location), NumCreatures(other.NumCreatures)
{
}