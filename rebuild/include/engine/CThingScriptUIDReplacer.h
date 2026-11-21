#ifndef FABLE_ENGINE_CTHINGSCRIPTUIDREPLACER_H
#define FABLE_ENGINE_CTHINGSCRIPTUIDREPLACER_H
// CThingScriptUIDReplacer (p25Q, new header; row 0x00521CF0 CThingManager::LoadMainPlayerCreatureState): Ego_d.pdb LF_CLASS 0x51B6E
// (field list 0x51B6D, sizeof 4 in Ego_r.pdb struct_layouts_egor.tsv): ReplacedUIDScript (CCharString) +0x00, private; the compiler-
// generated destructor (retail expands it to ~CCharString 0x0099EAE0). Methods, Ego_r.pdb / FableWin.pdb spellings:
// ??0CThingScriptUIDReplacer@@QAE@ABVCCharString@@@Z (retail 0x0083E1F0), ?GetReplacedUIDScript@CThingScriptUIDReplacer@@QBEABVCCharString@@XZ
// (out of line at 0x0083E1B0, `mov eax,ecx ; ret`, a body folded with other this-returning COMDATs), and the private helpers
// GetPlayerManager / GetWorldMap / GetThingManager / GetSwapUniqueIdentifier(long) const. Declarations only (Ego_d parameter name).
#include "fable_string.h"
class CPlayerManager;
class CWorldMap;
class CThingManager;
class CThingScriptUIDReplacer {
public:
    CThingScriptUIDReplacer(const CCharString& previous_script);         // 0x0083E1F0
    const CCharString& GetReplacedUIDScript() const;                     // 0x0083E1B0
private:
    CPlayerManager& GetPlayerManager();
    CWorldMap& GetWorldMap();
    CThingManager& GetThingManager();
    CCharString GetSwapUniqueIdentifier(long unique_identifier) const;
    CCharString ReplacedUIDScript;                                       // +0x00
};
FABLE_STATIC_ASSERT(sizeof(CThingScriptUIDReplacer) == 4);
#endif
