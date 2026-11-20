// CThingFilter_HasScriptData::operator()(CThing* thing) -- true when the thing's script-data string (+0xE8) equals the filter's Data (CCharString equality inlined)
// Leaf: ??RCThingFilter_HasScriptData@@QBE_NPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7F30, length 60 bytes (+ int3 padding).
// Callees: Compare @ 0x00411570 (`__fastcall(a, b)`, 0 when equal).
// Layout: PDB CThingFilter_HasScriptData { CCharString Data; } (+0). The thing-side string sits at +0xE8 of the argument
// (a CThing-derived class in retail; local model). CCharString is one pointer to {chars +0, length +4} (landed model
// of 0x00A14EE0).
// Shape: the landed inline `operator==` (same block -> true; a null block -> false; lengths differ -> false; else
// `Compare(a, b) == 0` as `neg ; sbb al,al ; inc al`).
#include "fable_compiler.h"
// Name fix (NF199, 2026-11-20, q4451): the local CCharString / CCharStringData model, its Compare_00411570 stand-in for 0x00411570 and
//   the free operator== are replaced by the shared fable_string.h (FABLE_RETAIL_CCHARSTRING; the plain inline
//   operators match, _FORCEINLINE_EQ is not needed): CCharString::operator== -> CBasicString<char>::operator== (length, then Compare) binds the
//   protected static ?Compare@?$CBasicString@D@@KIJPBD0@Z (Ego_r.pdb), the self row 0x00411570's Leaf.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
class CThing { public:
    unsigned char _pad00[0xe8];
    CCharString   m_scriptData;                     // +0xe8
};

class CThingFilter_HasScriptData { public:
    CCharString Data;                               // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_HasScriptData::operator()(CThing* thing) const
{
    return thing->m_scriptData == Data;
}
