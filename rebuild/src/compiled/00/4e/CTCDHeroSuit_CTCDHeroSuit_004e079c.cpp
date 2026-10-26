// CTCDHeroSuit::CTCDHeroSuit(CThing* parent) -- CTCDriverBase(parent), own vtable, null def pointer and empty SuitParts vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E079C, length 38 bytes.
// Callees: CTCDriverBase::CTCDriverBase(CThing*) @ 0x00771200 (vtable 0x0126A79C = CTCDriverBase, rtti-vtables.tsv).
// Vtable 0x01242004 = CTCDHeroSuit (rtti-vtables.tsv). PDB CTCDHeroSuit: PDef
// (CDefPointer<const CHeroSuitDef>, one pointer) +0x0c, SuitParts (std::vector<pair<EClothingSuitPart,bool>>)
// +0x10; the retail vector is 12 bytes (first/last/end at +0x10/+0x14/+0x18), not the PDB's 16.
// Shape: the base argument is passed through (`push dword ptr [esp+8]`, the /Os form, as the landed
// CTCDecapitation ctor 0x004DB207 in the same module), then `xor eax,eax`, the vtable store FIRST and
// the four zero stores after it: both members are member objects with inline constructors (an init-list
// scalar would sink the vtable store below the zero stores).
#pragma optimize("s", on)
class CThing;
class CHeroSuitDef;

struct CTCDriverBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCDriverBase_Calls(CThing* parent);                    // 0x00771200
};

template<class T> class CDefPointer { public:
    T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

template<class T> struct CRetailVector {                    // retail Dinkumware vector: 3 pointers
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct SSuitPart {                                          // std::pair<EClothingSuitPart, bool>
    long Part;
    bool Worn;
};

class CTCDHeroSuit : public CTCDriverBase_Calls { public:
    CDefPointer<const CHeroSuitDef> PDef;                   // +0x0c
    CRetailVector<SSuitPart>        SuitParts;              // +0x10
    virtual void v00();                                     // own vtable 0x01242004
    CTCDHeroSuit(CThing* parent);
};

CTCDHeroSuit::CTCDHeroSuit(CThing* parent)
    : CTCDriverBase_Calls(parent)
    , PDef()
    , SuitParts()
{
}