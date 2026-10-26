// NCreatureMode::CModeDragonFireBeam::CModeDragonFireBeam(CThing*) -- CModeDragonHover(thing), Timer 0, empty beam-effect pointer, then the hover filter re-assigned to "FIRE_BEAM"
// Leaf: ??0CModeDragonFireBeam
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8620, 191 bytes (`ret 4`). Vtable 0x01285900 (NCreatureMode::CModeDragonFireBeam).
// Callees: NCreatureMode::CModeDragonHover::CModeDragonHover(CThing*) 0x00741B60 (landed);
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10; CCharString(const char*, long) 0x0099EBF0,
// CCharString() 0x0099E4B0, operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0.
// Layout (PDB CModeDragonFireBeam 0x94): CModeDragonHover 0x80 (Filter +0x68: CAnimFilterOfGroup), Timer +0x80,
// PBeamWeaponEffect +0x84 (CIntelligentPointer<CThing>, vtable 0x01238C6C), LastExplodeFrame +0x8c, BeamActive +0x90.
// Shape: Timer is an init-list member (stored before the pointer's base ctor); the body assigns a whole
// CAnimFilterOfGroup temporary to the inherited Filter: the temporary is built in the frame (derived vptr
// 0x01267568 only -- the CAnimFilter base store is dead), copied by the implicit memberwise operator=
// (CCharString operator= + the CRC word, no vptr copy), then destroyed: the implicit CAnimFilterOfGroup
// dtor runs ~Group and the inline user ~CAnimFilter() stores the base vtable 0x0122FFCC; the literal's
// implicit CCharString conversion temporary dies last.
class CThing;
class CBaseObject;

class CCharStringData { public:
    const char*   m_pData;                 // +0
    unsigned long m_length;                // +4
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

struct CAnimFilter {                       // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilter() { }
};

class CAnimFilterOfGroup : public CAnimFilter { public:  // vtable 0x01267568
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    virtual void v00();
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.GetPString(), group.GetLength());
    }
};

struct CBaseIntelligentPointer {           // vptr + item
    virtual void v00();
    CBaseObject* PItem;                    // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(void* item);             // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
};

struct CModeStandBase {                    // retail sizeof 0x68
    virtual void v00();
    unsigned char _body[0x64];
};

namespace NCreatureMode {

class CModeDragonHover : public CModeStandBase { public: // vtable 0x0126845C, 0x80 bytes
    CAnimFilterOfGroup       Filter;       // +0x68
    CIntelligentPointerThing PTarget;      // +0x74
    bool                     UseHeadTurning;   // +0x7c
    CModeDragonHover(CThing* thing);       // 0x00741B60
};

struct CModeDragonFireBeam : CModeDragonHover {   // vtable 0x01285900
    long                     Timer;              // +0x80
    CIntelligentPointerThing PBeamWeaponEffect;  // +0x84
    long                     LastExplodeFrame;   // +0x8c
    bool                     BeamActive;         // +0x90
    virtual void v00();
    CModeDragonFireBeam(CThing* thing);
};

CModeDragonFireBeam::CModeDragonFireBeam(CThing* thing)
    : CModeDragonHover(thing)
    , Timer(0)
{
    Filter = CAnimFilterOfGroup("FIRE_BEAM");   // literal @ 0x012858F0 (masked)
}

}