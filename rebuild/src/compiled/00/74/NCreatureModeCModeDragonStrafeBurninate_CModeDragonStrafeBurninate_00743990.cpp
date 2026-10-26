// NCreatureMode::CModeDragonStrafeBurninate::CModeDragonStrafeBurninate(CThing*) -- CModeDragonFlying(thing), own vtable, Timer = 0, a null beam-effect pointer, then the inherited filter re-pointed at "STRAFE_BURNINATE"
// Leaf: ??0CModeDragonStrafeBurninate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743990 (`ret 4`). Vtable 0x01268718 = NCreatureMode::CModeDragonStrafeBurninate.
// Callees: CModeDragonFlying::CModeDragonFlying(CThing*) @ 0x007437C0, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CCharString(const char*, long) @ 0x0099EBF0, operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0,
// CRC @ 0x004014A0.
// Layout (retail): CModeDragonFlying 0x94 (Filter +0x38: Group +0x3c, GroupCRC +0x40), Timer +0x94,
// PBeamWeaponEffect +0x98 (CIntelligentPointer<CThing>, vtable 0x01238C6C).
// Shape: the init list (Timer, the pointer) runs before the body; the body hands an implicit CCharString conversion
// of the literal to the filter's inline SetGroup (assign + CRC, no filter vtable store).
class CThing;
class CBaseObject;

class CCharStringData { public:
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0 (default argument: the literal converts implicitly)
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

class CAnimFilterOfGroup { public:                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.m_pData == 0 ? "" : group.m_pData->m_pData, group.m_pData == 0 ? 0 : group.m_pData->m_length);   // one shared null test (CSE of the two guards)
    }
    void SetGroup(const CCharString& group)
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.m_pData == 0 ? "" : group.m_pData->m_pData, group.m_pData == 0 ? 0 : group.m_pData->m_length);
    }
};

struct CBaseIntelligentPointer {           // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem((CBaseObject*)item); }
};


namespace NCreatureMode {

struct CModeDragonFlying {                 // vtable 0x0126868C, retail sizeof 0x94
    virtual void v00();
    unsigned char      _base[0x34];        // CModeDragonFlyingBase
    CAnimFilterOfGroup Filter;             // +0x38
    unsigned char      _rest[0x50];        // Destination .. UsePreviousPos
    CModeDragonFlying(CThing* thing);      // 0x007437C0
};

struct CModeDragonStrafeBurninate : CModeDragonFlying {   // vtable 0x01268718
    long                Timer;             // +0x94
    CIntelligentPointer PBeamWeaponEffect; // +0x98
    virtual void v00();
    CModeDragonStrafeBurninate(CThing* thing);
};

CModeDragonStrafeBurninate::CModeDragonStrafeBurninate(CThing* thing)
    : CModeDragonFlying(thing)
    , Timer(0)
{
    Filter.SetGroup("STRAFE_BURNINATE");
}

}