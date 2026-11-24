// global::operator+(const CWideString& left,wchar_t right) -- Copy the left string, append the right value and return the result.
// Leaf: ??H@YI?AVCWideString@@ABV0@G@Z
// Flags: /Gr
// Identity: Ego_r.pdb 0x00B03400 / FableWin.pdb 0x02F67910 ??H@YI?AVCWideString@@ABV0@_W@Z (G here: wchar_t is unsigned short under the
//   retail flags), a free fastcall overload. Callees: operator+=(wchar_t) 0x0099B9A0, UnassignString 0x0099B4D0 (out of line inside the
//   expanded copy constructor), free 0x00BFEA14 / operator delete 0x00BFE9BC (the expanded destructor); NoStrings 0x013BCA20.
// Shape: `CWideString result(left); result += right; return result;` -- no NRVO, so the return copies through the expanded copy
//   constructor and the local runs the expanded destructor (the CWideString TU defines both inline: FABLE_RETAIL_CWIDESTRING_COPYCTOR_INLINE /
//   _DTOR_INLINE, bodies of the owners 0x0099B720 / 0x0099B510).
// Name fix (NF220, 2026-11-21, q12401): ported off the local CWideString / CWideStringData / STLport models onto fable_string.h
//   (FABLE_RETAIL_CWIDESTRING + _SPRINTF_ORIGINAL, the landed 0x0099BA70 / 0x0099B4D0 setup), so this TU no longer defines
//   UnassignString strongly (59 B v retail 62): its copy is now the header's pick-any 62-byte body (0x0099B4D0's).
// Fidelity: retail's CWideString TU keeps the local's data pointer in esi across operator+= and UnassignString: c2 needs the
//   address-taking callees' bodies in the TU. Measured (NF220): header only = DIFFER 184 v 177 (the local reloaded after each call);
//   with visible noinline bodies of operator+=(wchar_t), MakeOwnCopyOfData and AllocStringData = RELOCATION_MATCH. The three
//   definitions below are the HEAD row's same-TU strong copies kept (HEAD debt: their owners are 0x0099B9A0 / 0x0099B560 /
//   0x0099B3C0), now on the header class through declaration-only opt-ins (CWideStringData ctor / Append(wchar_t), MakeOwnCopyOfData).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_SPRINTF_ORIGINAL
#define FABLE_RETAIL_CWIDESTRING_COPYCTOR_INLINE
#define FABLE_RETAIL_CWIDESTRING_DTOR_INLINE
#define FABLE_RETAIL_CWIDESTRING_ADDCHAR
#define FABLE_RETAIL_CWIDESTRING_MAKEOWNCOPYOFDATA
#define FABLE_RETAIL_CWIDESTRING_TOCHARSTRING_BODY
#define FABLE_RETAIL_CWIDESTRINGDATA_CTOR_TEXT
#define FABLE_RETAIL_CWIDESTRINGDATA_APPEND_CHAR
#include <wchar.h>
#include "fable_string.h"

// Fidelity: same-TU strong definition of 0x0099B3C0 (retail TU-mate; HEAD debt)
__declspec(noinline) CWideStringData* CWideString::AllocStringData(const wchar_t* text, long length)
{
    if (length < 0) {
        length = 0;
        while (text[length])
            ++length;
    }
    return new CWideStringData(text, length);
}
// Fidelity: same-TU strong definition of 0x0099B560 (retail TU-mate; HEAD debt)
__declspec(noinline) void CWideString::MakeOwnCopyOfData()
{
    CWideStringData* data = PStringData;
    if (data && data->NoRefs > 1) {
        CWideStringData* copy = AllocStringData(data->PeekStringData(), -1);
        UnassignString();
        PStringData = copy;
    }
}
// Fidelity: same-TU strong definition of 0x0099B9A0 (retail TU-mate; HEAD debt)
__declspec(noinline) const CWideString& CWideString::operator+=(wchar_t c)
{
    if (PStringData) {
        MakeOwnCopyOfData();
        PStringData->Append(c);
    } else {
        wchar_t text[2];
        text[0] = c;
        text[1] = 0;
        if (c)
            PStringData = AllocStringData(text, -1);
    }
    return *this;
}

CWideString __fastcall operator+(const CWideString& left, wchar_t right)
{
    CWideString result(left);
    result += right;
    return result;
}
