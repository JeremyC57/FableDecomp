// 00856780 IsValidStrikeEventName
// Flags: /O2 /Oy /W3 
// Leaf: ?IsValidStrikeEventName@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path, as the landed 0x00870C90.)
// Name fix (NF56, round 7, queue q10446): Ego_r.pdb 0x00836640 + FableWin.pdb 0x02822FC0 ?IsValidStrikeEventName@?A0xad05d24c@@YI_NABVCCharString@@AA_N111@Z:
//   the function now sits in the anonymous namespace (was the stand-in namespace ComboStrikes); body unchanged.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the eight string handles are the global const CCharStrings of the combat-action TU
//   (Ego_r.pdb + FableWin.pdb `dynamic initializer for '...'`, initializers 0x0121E7A0..0x0121E8A0 landed by agentFAM09):
//   0x013BAC64 STRIKE_EVENT_WEAPON_START_IDENTIFIER (was ComboStrikes::PrimaryPrefix), 0x013BAC68 STRIKE_EVENT_WEAPON_END_IDENTIFIER
//   (SecondaryPrefix), 0x013BAC6C STRIKE_EVENT_START_IDENTIFIER (PrimaryAlternative), 0x013BAC70 STRIKE_EVENT_END_IDENTIFIER
//   (SecondaryAlternative), 0x013BAC78 STRIKE_EVENT_BEGIN_LEGACY (BothFlags), 0x013BAC7C STRIKE_EVENT_END_LEGACY (SecondFlag),
//   0x013BAC80 SECONDARY_STRIKE_EVENT_BEGIN_LEGACY (FirstFlag), 0x013BAC84 SECONDARY_STRIKE_EVENT_END_LEGACY (NeitherFlag). The row's
//   string handle model moved out of the stand-in namespace so the objects bind ?NAME@@3VCCharString@@B. Still open: the function is
//   the anonymous-namespace ?IsValidStrikeEventName@?A0x...@@YI_NABVCCharString@@AA_N111@Z (Ego_r.pdb), graded here inside the stand-in
//   namespace ComboStrikes, and the Compare / EmptyString stand-ins (0x00411570 / 0x0129AAF4) keep their row-local spellings.
// NF52 (round 7): CCharString is fable_string.h's (FABLE_RETAIL_CCHARSTRING): StartsWith is its inline strncmp over GetAsCharArray
// (FABLE_RETAIL_CCHARSTRING_STARTSWITH; a null string reads CCharString::NullChar 0x0129AAF4) and the legacy-name tests are its inline
// operator==(const CCharString&) (data pointers, null tests, StringLength, !Compare), binding the protected static
// ?Compare@?$CBasicString@D@@KIJPBD0@Z (0x00411570). Were a local CCharString model over the stand-in ComboStrikes::CBasicString,
// the made-up ComboStrikes::EmptyString and the Equal helper. The function itself stays in the stand-in namespace ComboStrikes (Ego_r:
// anonymous namespace, see the note above). Retail expands all four legacy tests with the name's data pointer kept in esi:
// FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ (the plain inline operator is called out of line after the first site) and the visible
// Compare body (FABLE_RETAIL_CBASICSTRING_COMPARE_BODY: VC7.1 knows the call writes no memory).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#include "fable_compiler.h"
#include <string.h>
#include "fable_string.h"
extern const CCharString STRIKE_EVENT_WEAPON_START_IDENTIFIER,STRIKE_EVENT_WEAPON_END_IDENTIFIER,STRIKE_EVENT_START_IDENTIFIER,STRIKE_EVENT_END_IDENTIFIER;   // 0x013BAC64 / 68 / 6C / 70
extern const CCharString STRIKE_EVENT_BEGIN_LEGACY,STRIKE_EVENT_END_LEGACY,SECONDARY_STRIKE_EVENT_BEGIN_LEGACY,SECONDARY_STRIKE_EVENT_END_LEGACY;   // 0x013BAC78 / 7C / 80 / 84
namespace {
bool __fastcall IsValidStrikeEventName(const CCharString&name,bool&first,bool&prefix,bool&named,bool&second){
 prefix=false;bool primary=name.StartsWith(STRIKE_EVENT_WEAPON_START_IDENTIFIER);bool secondary=name.StartsWith(STRIKE_EVENT_WEAPON_END_IDENTIFIER);
 if(!primary&&!secondary){primary=name.StartsWith(STRIKE_EVENT_START_IDENTIFIER);secondary=name.StartsWith(STRIKE_EVENT_END_IDENTIFIER);}else prefix=true;
 if(primary||secondary){named=false;first=primary;return true;}
 named=true;
 if(name==STRIKE_EVENT_BEGIN_LEGACY){first=true;second=true;return true;}
 if(name==SECONDARY_STRIKE_EVENT_BEGIN_LEGACY){first=true;second=false;return true;}
 if(name==STRIKE_EVENT_END_LEGACY){first=false;second=true;return true;}
 if(name==SECONDARY_STRIKE_EVENT_END_LEGACY){first=false;second=false;return true;}
 return false;
}
}
