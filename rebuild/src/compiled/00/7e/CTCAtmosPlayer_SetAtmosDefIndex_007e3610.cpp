// CTCAtmosPlayer::SetAtmosDefIndex(long index) -- store the atmos def index; for a non-zero index create the sound emitter at the parent thing's position (when the sound system exists) and start it playing the def's name
// Leaf: ?SetAtmosDefIndex@CTCAtmosPlayer@@QAEXJ@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3610, 122 bytes (`ret 4` + int3 padding). FableWin.pdb
//   `?SetAtmosDefIndex@CTCAtmosPlayer@@QAEXJ@Z` (public thiscall); caller CTCAtmosPlayer vfunc_4 @ 0x007E3690
//   (`SetAtmosDefIndex(AtmosDefIndex)` when non-zero) and 0x006B23AD. Catalog label `Def_GetDetailsForDisplay` was
//   an autoname guess.
// Sound manager (agentSND2, 2026-11-10): PSoundManager is the shared engine/CASoundManager.h one; the local CASoundSystem model is gone and its slots carry the header names (CreateEmitter -> Create3DEmitter slot 4; the emitter's slot 5 Play -> CASoundEmitter.h PlayAtmos).
// Layout: CTCAtmosPlayer (Ego_r.pdb, 0x14 bytes): CTCBase {vptr, ParentThing +4, flags +8}, PSoundEmitter +0x0c
//   (CASoundEmitter*), AtmosDefIndex +0x10.
// Callees: the sound system singleton 0x013B8394 slot 4 (+0x10, creates an emitter at a position), CThing::GetPos
//   @ 0x004C73D0, CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall), CDefinitionManager::GetDefNameFromGlobalIndex
//   @ 0x009ACCC0 (CDefString by hidden pointer), CDefStringTable::GetString @ 0x009D49B0 on GDefStringTable 0x013CA828,
//   the emitter's slot 5 (+0x14, play by name), ~CCharString @ 0x0099EAE0. Module built without /EHsc (no frame).
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"

#include "engine/C3DVector.h"

class CDefString { public:
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
};

class CDefinitionManager {
public:
public:
    CDefString GetDefNameFromGlobalIndex(long) const;   // 0x009ACCC0
};

class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();           // 0x0044C6B0
};

class CDefStringTable {
public:
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)                        // 0x013CA828


#include "engine/CASoundManager.h"

class CTCAtmosPlayer {
public:
    virtual void v00();
    CThing*         ParentThing;                                // +0x04
    unsigned char   _pad08[4];                                  // CTCBase flags
    CASoundEmitter* PSoundEmitter;                              // +0x0c
    long            AtmosDefIndex;                              // +0x10
    void SetAtmosDefIndex(long index);
};

void CTCAtmosPlayer::SetAtmosDefIndex(long index)
{
    AtmosDefIndex = index;
    if (index) {
        if (PSoundManager)
            PSoundEmitter = PSoundManager->Create3DEmitter(ParentThing->GetPos());
        if (PSoundEmitter) {
            CCharString name = GDefStringTable.GetString(CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(AtmosDefIndex).m_index);
            PSoundEmitter->PlayAtmos(name);
        }
    }
}