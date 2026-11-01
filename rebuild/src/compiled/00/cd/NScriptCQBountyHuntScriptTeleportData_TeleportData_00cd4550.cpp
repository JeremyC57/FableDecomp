// NScript::CQ_BountyHuntScript::TeleportData::~TeleportData() -- implicit destructor: destroy teleportMkr, teleportTo, scriptName, then defName
// Leaf: ??1TeleportData@CQ_BountyHuntScript@NScript@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD4550, 35 bytes, thiscall: ~CCharString(+0x14), ~CCharString(+0x10), ~CCharString(+0x0C),
//   then `jmp ~CCharString` on +0x00 (the last member destroyed is the first one, so the call becomes a tail jump).
// Identity: ten callers, all in the Q_BountyHunt quest-script code (CBH_GangDeputy2::Main 0x00D1913B / 0x00D19169 and the
//   sibling script bodies 0x00D15075, 0x00D18279, 0x00D182B2, 0x00D19F20, 0x00D1AA91, 0x00D1AAAE, 0x00D1B548, 0x00D1B565).
//   Each destroys a 24-byte stack local built by the landed TeleportData::TeleportData() 0x00D15140 and handed to the landed
//   vector<TeleportData>::push_back 0x00D18C00 (CQ_BountyHuntScript::TeleportInfo). Ego_r.pdb: TeleportData is 24 bytes,
//   CCharString defName +0, ulong appearanceSeed +4, float health +8, CCharString scriptName +0xC, teleportTo +0x10,
//   teleportMkr +0x14 (the only PDB struct with CCharStrings at exactly 0/0xC/0x10/0x14), and ??1TeleportData@CQ_BountyHuntScript
//   @NScript@@QAE@XZ sits in Ego_r between ??1CTimer and ??0RegionData (donor 0x00CB5D30) exactly as retail places 0x00CD4550
//   after CTimer 0x00CD4450/0x00CD4470. The manifest "_Dest_val<allocator<CMacro>,CMacro>" and the twin 0x004047A0
//   (~CStringParser) are other functions.
// Callee: CCharString::~CCharString 0x0099EAE0 (??1CCharString@@QAE@XZ).
// Shape: no user-declared destructor (the members' destructors make the compiler generate a non-trivial one); an emission
//   helper calls it explicitly so VC7.1 emits ??1 as its own COMDAT.
class CCharStringData;
class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;
};

namespace NScript {
class CQ_BountyHuntScript {
public:
    struct TeleportData {                               // 24 bytes (Ego_r.pdb)
        CCharString   defName;                          // +0x00
        unsigned long appearanceSeed;                   // +0x04
        float         health;                           // +0x08
        CCharString   scriptName;                       // +0x0c
        CCharString   teleportTo;                       // +0x10
        CCharString   teleportMkr;                      // +0x14
    };
};
}

// Emission helper (not a retail symbol): forces the implicit destructor out of line as its own COMDAT.
void EmitTeleportDataDestructor(NScript::CQ_BountyHuntScript::TeleportData* data)
{
    data->NScript::CQ_BountyHuntScript::TeleportData::~TeleportData();
}