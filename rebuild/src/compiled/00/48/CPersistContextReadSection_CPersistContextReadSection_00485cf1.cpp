// CPersistContextReadSection::~CPersistContextReadSection() -- destroy a persist read section: its text, its string parser, then its memory input stream
// Leaf: ??1CPersistContextReadSection@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00485CF1, length 28 bytes, `jmp` tail. Callers: 0x0047F8BB / 0x0047FDF6
// (CFrontEndManager), 0x004A2CCA, 0x00597558 / 0x00597A93 -- each on a stack local first built by the landed constructor
// 0x00485CD2 (`lea ecx,[ebp-0xb0] ; call 0x485CD2` ... `call 0x485CF1`), which this destructor directly follows.
// The manifest label (_Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo>>>) is propagated and wrong.
// Layout: the landed ctor's model of Ego_r.pdb CPersistContextReadSection -- Context (CPersistContext, +0x00, no
// destructor code), InputStream (CMemoryDataInputStream, +0x2c), Parser (CStringParser, +0x48, retail 0x28 bytes),
// Text (CCharString, +0x70 retail).
// Callees: CCharString::~CCharString @ 0x0099EAE0, CStringParser::~CStringParser @ 0x004047A0 (manifest "_Dest_val<CMacro>",
// propagated: +0x14 member, two CCharStrings, CBaseClass root), CMemoryDataInputStream::~CMemoryDataInputStream @ 0x00993B10
// (folded with ~CDataStream: vtable 0x0129A69C, tail to the root).
// Shape: members in reverse order; the input stream is the tail jmp.
// sweep3: CStringParser's destructor is virtual (both PDBs: ??1CStringParser@@UAE@XZ; it derives from CBaseClass), so
// the member destructor call binds the UAE spelling that the landed 0x004047A0 defines; the direct call is unchanged.
// NF181 (2026-11-20, q4251): ported onto the shared engine/CPersistContextReadSection.h (CMemoryDataInputStream.h /
//   CDataInputStream.h / CDataStream.h / CStringParser.h): the input stream member's destructor is the implicit derived one,
//   so the tail call binds ??1CDataStream@@UAE@XZ (Ego_r.pdb 0x00AFBBB0 + FableWin.pdb 0x03080440; the body at 0x00993B10
//   stores the CDataStream vtable 0x0129A69C) instead of the ??1CMemoryDataInputStream@@UAE@XZ stand-in.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_DTOR_OUTOFLINE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXTREADSECTION
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistContextReadSection.h"

CPersistContextReadSection::~CPersistContextReadSection()
{
}
