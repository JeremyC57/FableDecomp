// CVtable_012fb358::CVtable_012fb358(const CVtable_012fb358& other) -- copy constructor of a script-module def: CDefClassBase copy, own vtable, two {CCharString, word} pairs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F25DD0, length 63 bytes (`ret 4`, eax = this). Callees:
// CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C, CCharString::CCharString(const CCharString&) @ 0x0099EC30
// (twice). The manifest label (COpinionDeedReactionDef::COpinionDeedReactionDef) is propagated and wrong: the vtable
// stored is 0x012FB358, which has no RTTI complete-object locator (a no-RTTI script module, hence the CVtable_<va>
// name), and the layout (two CCharStrings at +0x28 / +0x30 with a word after each) is not COpinionDeedReactionDef's.
// Its callers are the array copy-construct helpers of the CQ_* quest-script code around 0x00F25D67 / 0x00F260E7.
// Shape: CDefClassBase is 0x28 bytes (as in the landed CDialogueLayerDef copy constructor @ 0x004663E7); a speed TU
// (ecx/edx alternate for the plain word copies), the vtable store between the first member's `lea ecx` and its call.
// Leaf: ??0CVtable_012fb358
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
};

class CDefClassBase {                              // 0x28 bytes (vptr + resource links, def manager, index, flags)
public:
    virtual void v000();
    unsigned char _body[0x24];
    CDefClassBase(const CDefClassBase& other);     // 0x0046644C
};

class CVtable_012fb358 : public CDefClassBase {    // vtable 0x012FB358 (no RTTI locator)
public:
    virtual void v000();
    CCharString   m_first;                         // +0x28
    unsigned long m_firstValue;                    // +0x2C
    CCharString   m_second;                        // +0x30
    unsigned long m_secondValue;                   // +0x34
    CVtable_012fb358(const CVtable_012fb358& other);
};

CVtable_012fb358::CVtable_012fb358(const CVtable_012fb358& other)
    : CDefClassBase(other),
      m_first(other.m_first),
      m_firstValue(other.m_firstValue),
      m_second(other.m_second),
      m_secondValue(other.m_secondValue)
{
}