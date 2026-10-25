// CODA_GetSourcePosition(CAudioObject* source) -- the libpk `__stdcall` API wrapper returning a CODA source's play position: its voice's slot-13 virtual when the source has a voice (+0x50) and any of the 0x216 playing flags is set, else the cached position at +0xf0
// Leaf: _CODA_GetSourcePosition@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35CC0, length 40 bytes. No direct callees
// (virtual slot 13, +0x34, on the object at +0x50 of the source).
// Naming (sweep1, 2026-10-25): was landed as _unattributed::GetEffectiveValue_00c35cc0(CEntry*) (manifest
//   CAIBrain::GetCurrentStateGroupDebugText, propagated); descriptive (no PDB covers the libpk / GameCODA code). Its three
//   callers are the landed CCodaPlayback rows (0x00C276DD in Update 0x00C276B0, 0x00C277CE in LogStatus 0x00C27760,
//   0x00C278D9), which declare it `unsigned long __stdcall CODA_GetSourcePosition(CODASource* source)` and pass the same
//   +0x0C source pointer they hand CODA_SetSourceVolume 0x00C35AF0 (FINDINGS chunk 435, agent435 NOTES); like that
//   sibling it is part of the `extern "C"` CODA API (the callers' /EHsc lock scopes carry no EH state for it). The
//   source is typed CAudioObject as in the sibling wrapper.
// The argument is read from [esp+4] with ecx untouched and the frame is cleaned with `ret 4`, so this is a __stdcall
// free function. The flag test is one `test dword ptr [eax+10h],216h` on a dword flag word; the voice pointer is
// re-read for the call.
struct CAudioVoice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual unsigned long GetPosition();    // slot 13 (+0x34)
};

class CAudioObject {
public:
    unsigned char _pad0[0x10];
    unsigned long m_flags;                  // +0x10
    unsigned char _pad1[0x3c];
    CAudioVoice*  m_pVoice;                 // +0x50
    unsigned char _pad2[0x9c];
    unsigned long m_cachedPosition;         // +0xf0
};

extern "C" unsigned long __stdcall CODA_GetSourcePosition(CAudioObject* source)
{
    if (source->m_pVoice != 0 && (source->m_flags & 0x216) != 0)
        return source->m_pVoice->GetPosition();
    return source->m_cachedPosition;
}
