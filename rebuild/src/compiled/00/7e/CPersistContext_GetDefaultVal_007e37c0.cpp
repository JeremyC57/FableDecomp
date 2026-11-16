// CPersistContext::GetDefaultVal<CDefString>() -- the default value a named Transfer<CDefString> passes when the setting is absent: a
// def string with no string-table entry (TablePos = -1), returned through the hidden result pointer (ecx under __fastcall)
// Leaf: ??$GetDefaultVal@VCDefString@@@CPersistContext@@KI?AVCDefString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E37C0, 9 bytes -- a protected static __fastcall member template (no arguments).
// NF39 (2026-11-16): renamed from CDefString::CDefString() onto FableWin.pdb ??$GetDefaultVal@VCDefString@@@CPersistContext@@KI?AVCDefString@@XZ.
//   Its only caller, CScriptDef::Transfer 0x00F2D6E0 (35 sites 0x00F2EA95..0x00F2F55B, `lea ecx,[esp+14h] ; call 0x007E37C0 ; push eax`),
//   is the expanded `Transfer(name, member)` = `Transfer(name, member, GetDefaultVal<CDefString>())` (engine/CPersistContext.h), and the
//   landed 0x00F2D6E0 already binds this symbol here; the long / float / CCharString expansions of the same Transfer call their own
//   GetDefaultVal instances (0x0040E160 / 0x0040E250 / 0x0040E1E0). ??0CDefString@@QAE@XZ (both PDBs) compiles to the same 9 bytes; no landed row binds it here (probe: 0x00F2D6E0 is the only caller; no DIR32 reference).
// Body: the same source shape as the landed siblings GetDefaultVal<unsigned long> 0x0040E260 / GetDefaultVal<bool> 0x0040E240.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CDefString.h"
#include "engine/CPersistContext.h"

template <class T> T FABLE_FASTCALL CPersistContext::GetDefaultVal()
{
    T value = T();
    return value;
}

template CDefString FABLE_FASTCALL CPersistContext::GetDefaultVal<CDefString>();
