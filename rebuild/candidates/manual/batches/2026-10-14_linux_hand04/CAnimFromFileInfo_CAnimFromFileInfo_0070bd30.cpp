// CAnimFromFileInfo::CAnimFromFileInfo() -- inactive, empty path name (L""), zero segment times
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070BD30, oracle length 44 bytes.
// Callees: CWideString::CWideString(const wchar_t*) @ 0x0099B6B0 (landed) with the pooled L"" literal at 0x0122D70C.
//
// Retail bytes:  push esi ; mov esi,ecx ; push offset L"" ; lea ecx,[esi+4] ; mov byte ptr [esi],0 ; call CWideString::CWideString ;
//                mov dword ptr [esi+8],0 ; mov dword ptr [esi+0Ch],0 ; mov dword ptr [esi+10h],0 ; mov eax,esi ; pop esi ; ret
// Identity: the manifest label "C3DSkeletalMorphTarget::C3DSkeletalMorphTarget" is propagated and rejected
// (that class starts with a CCharString). PDB layout (struct_layouts_egor.tsv, CAnimFromFileInfo = 24 bytes):
// +0 Active (bool), +4 PathName (CWideString), +8 SegmentStartTime, +0xC SegmentEndTime, +0x10 Duration
// (floats), +0x14 KeyFrameCount (long, left uninitialised). The neighbours at 0x0070Bxxx are CAnimateThing
// code, which owns this helper. The float zeroes are stored as immediate dwords (constant 0.0f).
typedef unsigned short wchar_t;          // VC7.1 without /Zc:wchar_t: wchar_t is the CRT typedef

struct CWideString {
    wchar_t* m_pString;
    CWideString(const wchar_t* text);      // 0x0099B6B0
};

struct CAnimFromFileInfo {
    bool        Active;                    // +0x00
    CWideString PathName;                  // +0x04
    float       SegmentStartTime;          // +0x08
    float       SegmentEndTime;            // +0x0C
    float       Duration;                  // +0x10
    long        KeyFrameCount;             // +0x14 (not initialised)
    CAnimFromFileInfo();
};

CAnimFromFileInfo::CAnimFromFileInfo()
    : Active(false), PathName(L""), SegmentStartTime(0.0f), SegmentEndTime(0.0f), Duration(0.0f)
{
}
