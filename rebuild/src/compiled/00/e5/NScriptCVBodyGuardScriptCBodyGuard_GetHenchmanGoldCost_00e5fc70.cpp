// NScript::CV_BodyGuardScript::CBodyGuard::GetHenchmanGoldCost() -- the henchman's wage by HenchManType: types 0 and 2 cost the script def's HenchmanCheapGoldCost, types 1 and 3 HenchmanExpensiveGoldCost, anything else 0
// Leaf: ?GetHenchmanGoldCost@CBodyGuard@CV_BodyGuardScript@NScript@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5FC70, 44 bytes of code + a 4-entry jump table (0x00E5FC9C:
//   case 0 -> 0x00E5FC7F cheap, 1 -> 0x00E5FC8B expensive, 2 -> cheap, 3 -> expensive; verified by hand), __fastcall.
// Identity: called only by CBodyGuard methods (0x00E5FBE8, deducting the wage from the parent's gold at ParentClass+0x48,
//   and SetHenchmanAsHired 0x00E621F0 at 0x00E622AE, refunding it); `this` is CV_BodyGuardScript::CBodyGuard (Ego_r.pdb,
//   60 bytes; HenchManType +0x1c). The PDB names no such member (inlined in the debug build): descriptive name.
// Data: g_pScriptDef @ 0x0143E90C; retail CScriptDef HenchmanPayTimeDelayInSeconds +0x4a4 (landed, PDB +0x4d8), so
//   HenchmanCheapGoldCost +0x4a8 / HenchmanExpensiveGoldCost +0x4ac (PDB +0x4dc/+0x4e0, the retail def is 0x34 lower here);
//   local model (no engine header for CScriptDef, and its PDB offsets do not hold in retail).
// Shape: a dense `switch` with two shared arms and a default zero -> a bounds check, a 4-entry jump table, two return
//   blocks and the `xor eax,eax` default.
struct CScriptDef {
    unsigned char _pad000[0x4a4];
    long          HenchmanPayTimeDelayInSeconds;   // +0x4a4
    long          HenchmanCheapGoldCost;           // +0x4a8
    long          HenchmanExpensiveGoldCost;       // +0x4ac
};
extern CScriptDef* g_pScriptDef;                   // 0x0143E90C

namespace NScript {
struct CQ_SunnyvaleMasterData;
class CV_BodyGuardScript {
public:
    class CBodyGuard;
};
class CV_BodyGuardScript::CBodyGuard {             // entity script, Ego_r.pdb layout (60 bytes)
public:
    unsigned char           _base[0x14];           // CEntityScriptBase {vptr, Game, Me}
    CV_BodyGuardScript*     ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    long                    HenchManType;          // +0x1c
    long GetHenchmanGoldCost() const;
};
long CV_BodyGuardScript::CBodyGuard::GetHenchmanGoldCost() const
{
    switch (HenchManType)
    {
    case 0:
    case 2:
        return g_pScriptDef->HenchmanCheapGoldCost;
    case 1:
    case 3:
        return g_pScriptDef->HenchmanExpensiveGoldCost;
    }
    return 0;
}
}