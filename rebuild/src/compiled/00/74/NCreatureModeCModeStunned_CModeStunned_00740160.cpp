// NCreatureMode::CModeStunned::CModeStunned(CThing&) -- out-of-action mode ctor: CModeOutOfActionBase (counter 0, flag set, empty attacker pointer) then the STUNNED CAnimFilterOfGroup at +0x78
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00740160, 160 bytes (padding included).
// Leaf: ??0CModeStunned@NCreatureMode@@QAE@AAVCThing@@@Z
// Naming (NF463, queue nf455 identity line 0x00740160): the self takes the PDB parameter form, Ego_r.pdb + FableWin.pdb
//   ??0CModeStunned@NCreatureMode@@QAE@AAVCThing@@@Z (was ...QAE@PAVCThing@@@Z; a reference and a pointer pass the same bytes), with its
//   two retail callers in the same unit: the factory 0x007427F0 (now ?Alloc@CModeStunned@NCreatureMode@@...) and
//   CCombatAction_FallStunned::Initialise 0x008ED950 (was the wrong-class token CModeDazed(CThing*)). The inline
//   CModeOutOfActionBase ctor keeps its HEAD (CThing*) form: VC7.1 also emits it as a pick-any COMDAT (same as the landed CModeDazed row
//   0x00740250's copy), and respelling it here would add a new copy under the PDB name ??0CModeOutOfActionBase@NCreatureMode@@QAE@AAVCThing@@@Z
//   that no table line binds. Members carry their Ego_r.pdb names (RecoveryCount +0x68, Recoverable +0x6c, PEmitter +0x6d, Filter +0x78).
//   Left as is (open): the base call to 0x00861E00 keeps the stand-in CModeBase_00861e00(CThing*) and the one-slot local vftables,
//   because deriving from engine/CModeStandBase.h would grow ??_7CModeStunned / ??_7CModeOutOfActionBase (the r459a short-vftable
//   question; NF463 RULINGS r463b).
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Label correction: the catalog row says CModeDazed, but the vtable store 0x01267C74 is
// NCreatureMode::CModeStunned (rtti-vtables.tsv); 0x00740250 is the real CModeDazed ctor.
// Shape: base mode ctor @ 0x00861E00, then the inline CModeOutOfActionBase ctor (vtable 0x0126743C):
// +0x68 = 0, +0x6c = true and the CIntelligentPointer<CThing> at the unaligned +0x6d (out-of-line
// CBaseIntelligentPointer ctor 0x00A01B10, then its vtable 0x01238C6C), then this class's vtable
// 0x01267C74 and the CAnimFilterOfGroup at +0x78 from the literal @ 0x01267CB4 (default string
// 0x0099E4B0, assign 0x0099EFB0, CRC 0x004014A0 via the GetPString/GetLength accessors, temporary
// freed by 0x0099EAE0). The base is byte-packed (#pragma pack(1)): that is the real layout, the
// pointer sits at +0x6d in retail.
class CThing;
class CThingCreatureBase;

class CCharStringData { public:
    const char* m_pData;                   // +0
    unsigned long m_length;                // +4
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // "" = the shared empty literal 0x0129AAF4
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

class CAnimFilterOfGroup { public:
    virtual void v00();
    CCharString m_string;                  // +4
    unsigned long m_hash;                  // +8
    CAnimFilterOfGroup(const CCharString& name)
        : m_string()
    {
        m_string = name;
        m_hash = CCRC::Calc(0, name.GetPString(), name.GetLength());
    }
};

struct CModeBase_00861e00 {
    virtual void v00();
    unsigned char _body[0x64];
    CModeBase_00861e00(CThing* thing);     // 0x00861E00
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


namespace NCreatureMode {

// The out-of-action base packs its pointer member right after the flag byte (lea edi,[esi+6Dh]).
#pragma pack(push, 1)
struct CModeOutOfActionBase : CModeBase_00861e00 {     // vtable 0x0126743C
    long RecoveryCount;                        // +0x68
    bool Recoverable;                          // +0x6c
    CIntelligentPointer<CThing> PEmitter;         // +0x6d
    CModeOutOfActionBase(CThing* thing)
        : CModeBase_00861e00(thing)
        , RecoveryCount(0)
        , Recoverable(true)
    {
    }
};
#pragma pack(pop)

struct CModeStunned : CModeOutOfActionBase {
    CAnimFilterOfGroup Filter;                 // +0x78
    virtual void v00();
    CModeStunned(CThing& thing);
};

CModeStunned::CModeStunned(CThing& thing)
    : CModeOutOfActionBase(&thing)
    , Filter("STUNNED")              // literal @ 0x01267CB4 (masked)
{
}

}