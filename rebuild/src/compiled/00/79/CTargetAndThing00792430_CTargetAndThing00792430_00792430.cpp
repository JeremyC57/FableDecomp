// CTargetAndThing_00792430::CTargetAndThing_00792430() -- a creature pointer, an untouched dword, a thing pointer, three cleared flags.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00792430, length 48 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 twice -- the object's first member at +0
// (vtable 0x0123E7D8, CIntelligentPointer<CThingCreatureBase>) and the member at +0xc (vtable 0x01238C6C,
// CIntelligentPointer<CThing>). No vtable of its own: the first store is the first member's.
// Shape: two default-constructed intelligent pointers (each: base call, then its vtable store), the dword at +8
// left alone, then the three bools at +0x14..+0x16 cleared from one `xor al,al`. The manifest label
// "CAIStateGroup_OpinionReactionRoot ctor" is propagated from a byte-identical shell (that class is a state group
// with a 0x14-byte base); the owner is a plain aggregate used by the creature-action module, named descriptively.
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // vtable 0x0123E7D8
    CIntelligentPointerCreature() : CBaseIntelligentPointer() { }
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // vtable 0x01238C6C
    CIntelligentPointerThing() : CBaseIntelligentPointer() { }
};

struct CTargetAndThing_00792430 {
    CIntelligentPointerCreature PTargetCreature;   // +0x00
    long m_value;                               // +0x08 (not initialised here)
    CIntelligentPointerThing PThing;            // +0x0c
    bool m_flag14;                              // +0x14
    bool m_flag15;                              // +0x15
    bool m_flag16;                              // +0x16
    CTargetAndThing_00792430();
};

CTargetAndThing_00792430::CTargetAndThing_00792430()
    : PTargetCreature()
    , PThing()
{
    m_flag14 = false;
    m_flag15 = false;
    m_flag16 = false;
}