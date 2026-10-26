// CThingBuildingDef::CThingBuildingDef() -- CThingBaseDef base, this class's vtable, then the embedded record at +0x78 cleared by its inline constructor (a long, two floats, two flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00454B5F, length 41 bytes.
// Leaf: ??0CThingBuildingDef
// Callees: CThingBaseDef::CThingBaseDef() @ 0x006772D0 (initialises +0x04..+0x6c).
// The manifest label "CSimVoice::CSimVoice" is propagated: vtable 0x01233144 is CThingBuildingDef (rtti-vtables.tsv;
// CSimVoice's constructor is 0x00455E39 with vtable 0x012337AC). The PDB layout (Ego_r) does not match the retail
// offsets here, so the record's members are descriptive.
// Shape: size-optimised def module (`#pragma optimize("s", on)`): the floats are zeroed through the x87 (`fldz ;
// fstp`), the record is addressed once (`lea eax,[esi+78h]`) because its constructor is an inline member function
// of the record, the long and both flag bytes share the `xor ecx,ecx` zero.
#pragma optimize("s", on)

class CThingBaseDef { public:                          // vtable 0x0125B664
    virtual void v00();
    unsigned char _body[0x6c];                  // +0x04..+0x6f
    CThingBaseDef();                            // 0x006772D0
};

struct SBuildingRecord {                        // +0x78 in CThingBuildingDef
    long  m_value;                              // +0x00
    float m_levelA;                             // +0x04
    float m_levelB;                             // +0x08
    bool  m_flagA;                              // +0x0c
    bool  m_flagB;                              // +0x0d
    SBuildingRecord() : m_value(0), m_levelA(0.0f), m_levelB(0.0f), m_flagA(false), m_flagB(false) { }
};

class CThingBuildingDef : public CThingBaseDef { public:      // vtable 0x01233144
    unsigned char   _pad70[0x8];                // +0x70..+0x77 (not initialised here)
    SBuildingRecord m_record;                   // +0x78
    virtual void v00();
    CThingBuildingDef();
};

CThingBuildingDef::CThingBuildingDef()
{
}