#ifndef FABLE_ENGINE_CSYSTEMREGISTRY_H
#define FABLE_ENGINE_CSYSTEMREGISTRY_H
// New header (B24C4, row 0x00402510 GFMain): CSystemRegistry, Ego_r.pdb layout (hRoot +0, hKey +4, 8 bytes) and publics
// ??0CSystemRegistry@@QAE@ABVCWideString@@J@Z 0x0099CF10, ??1CSystemRegistry@@QAE@XZ 0x0099CF70,
// ?WriteBOOL@CSystemRegistry@@QBEXABVCWideString@@H@Z 0x0099D100 and the two public static constant key names (0x013BCA28 /
// 0x013BCA2C, the landed spellings). Out of line in retail. Declarations only.
#include "rebuild_abi.h"
#include "fable_string.h"
struct HKEY__; typedef HKEY__* HKEY;
class CSystemRegistry {
public:
    static const CWideString GAME_REG_KEY;                  // 0x013BCA28
    static const CWideString GFX_RESET_KEY_NAME;            // 0x013BCA2C
    CSystemRegistry(const CWideString& key, long root);     // 0x0099CF10
    ~CSystemRegistry();                                     // 0x0099CF70
    void WriteBOOL(const CWideString& value_name, int value) const;   // 0x0099D100
protected:
    HKEY hRoot;                                             // +0x00
    HKEY hKey;                                              // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CSystemRegistry) == 8);
#endif
