// IntToWideString(long value) -- format `value` with L"%d" into a CWideString temporary and return a copy of it (__fastcall: the hidden result in ecx, the value in edx)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00405F00, length 64 bytes.
// Callees: CWideString::CWideString() @ 0x0099AED0 (the folded default constructor shared with CCharString), the wide formatter @ 0x0099BA70 (`__cdecl` CWideString::Sprintf(const wchar_t*, ...), `this` pushed first,
// `add esp,0Ch`; manifest "CCharString::SetFromFormatV"), CWideString::CWideString(const CWideString&) @ 0x0099B720 into the hidden result, ~CWideString @ 0x0099B510.
// The literal @ 0x0122DE44 is L"%d". Shape: a one-word frame for the temporary, the value kept in edi, the hidden pointer in esi and returned. The chunk label
// "CGuiControlNumber::SetSLongValue" is propagated; the body is a free `__fastcall` formatter (0x0040xxxx module built with /Gr).
// Leaf: ?GFIntToWideString@@YI?AVCWideString@@J@Z
// NF143 (2026-11-20, one definition): Ego_r.pdb / FableWin.pdb ?GFIntToWideString@@YI?AVCWideString@@J@Z is an inline function
//   (engine/GFIntToWideString.h, whose body was copied from this row); retail's out-of-line copy here is a COMDAT. The landed includers of
//   that header (0x00435530, 0x0055E6B0, 0x0055F3B0, 0x00646762, 0x0074BC20, 0x007DA640; now 0x00406610) emit it as a pick-any COMDAT,
//   which clashed (LNK2005) with this row's strong definition. The row now takes the body from the shared header and emits it as the
//   pick-any COMDAT (the nf86 / nf92 scaffolding below), in place of its local CWideString model.
#include "fable_string.h"
#include "engine/GFIntToWideString.h"

namespace {
// Fidelity: scaffolding, not retail: a call of the header's inline GFIntToWideString with inline expansion switched off for this one
// function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x00405F00). Internal to this TU; never called.
#pragma inline_depth(0)
CWideString ReferenceGFIntToWideString(long value)
{
    return GFIntToWideString(value);
}
#pragma inline_depth()
}
