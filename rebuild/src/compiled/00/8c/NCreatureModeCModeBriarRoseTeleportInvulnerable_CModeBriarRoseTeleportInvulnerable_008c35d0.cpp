// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// NCreatureMode::CModeBriarRoseTeleportInvulnerable::CModeBriarRoseTeleportInvulnerable(CThing* thing, CThingCreatureBase* target)
//   -- CModeStandBase(thing), own vtable, the "INVULNERABLE" animation filter, PTarget = target, PEffect = null
// Leaf: ??0CModeBriarRoseTeleportInvulnerable
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C35D0 (`ret 8`); called from 0x008C1F3E. Vtable 0x01280228 =
//   NCreatureMode::CModeBriarRoseTeleportInvulnerable (rtti-vtables.tsv).
// Callees: CModeStandBase::CModeStandBase(CThing*) @ 0x00861E00 (manifest label "CTCCreatureNavigation" is propagated),
//   CCharString @ 0x0099EBF0 / 0x0099E4B0 / operator= 0x0099EFB0 / ~ 0x0099EAE0, CRC Calc @ 0x004014A0,
//   CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 / SetPItem @ 0x00A01B90.
// Shape: the filter (vtable 0x01267568 = CAnimFilterOfGroup) is built from an implicit CCharString conversion of the
//   literal (temporary destroyed after the filter); its CRC is ONE `p ? .. : ..` pair of guards that VC folds into a
//   single null test; then PTarget (vtable 0x0123E7D8, CIntelligentPointer<CThingCreatureBase>) and PEffect (vtable
//   0x01238C6C, CIntelligentPointer<CThing>). Timer is left uninitialised.
// Layout: PDB (Ego_r) CModeBriarRoseTeleportInvulnerable; retail offsets equal the PDB: CModeStandBase 0x68,
//   Filter +0x68 (0xc), Timer +0x74, PTarget +0x78, PEffect +0x80. Local model: no engine/ header for these classes.
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
};

struct CBaseIntelligentPointer {           // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};
class CThingCreatureBase;

struct CModeStandBase {                    // retail sizeof 0x68 (CModeBase 0x1c + the stand-mode members)
    virtual void v00();
    unsigned char _body[0x64];
    CModeStandBase(CThing* thing);         // 0x00861E00
};

namespace NCreatureMode {

struct CModeBriarRoseTeleportInvulnerable : CModeStandBase {
    CAnimFilterOfGroup Filter;                          // +0x68
    long Timer;                                         // +0x74
    CIntelligentPointer<CThingCreatureBase> PTarget;    // +0x78
    CIntelligentPointer<CThing> PEffect;                // +0x80
    virtual void v00();
    CModeBriarRoseTeleportInvulnerable(CThing* thing, CThingCreatureBase* target);
};

CModeBriarRoseTeleportInvulnerable::CModeBriarRoseTeleportInvulnerable(CThing* thing, CThingCreatureBase* target)
    : CModeStandBase(thing)
    , Filter("INVULNERABLE")
    , PTarget(target)
    , PEffect()
{
}

}