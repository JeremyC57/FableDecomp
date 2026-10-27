// CSoundManagerLH::GetNextEmitterID_00a3e420() -- returns the next emitter serial number and advances the counter
// Retail: Fable.exe 0x00A3E420, 10 bytes: `mov eax,[ecx+50h] ; lea edx,[eax+1] ; mov [ecx+50h],edx ; ret` (post-increment).
// Identity: the catalog label CGameScriptThing::DecrementScriptCounter is wrong (this increments +0x50 and returns the
// old value). Its two callers are CSoundEmitterLH constructors (0x00AAC530 region and 0x00AAC580, vtable 0x0129E6EC =
// .?AVCSoundEmitterLH@@): `ID = SoundManager->X()` stored at emitter +0x1C, with ecx = the manager pointer at emitter +4.
// It sits among the CSoundManagerLH methods (0x00A3E330..0x00A3E4B0). CSoundManagerLH (RTTI, base CASoundManager) is a
// PC-only class absent from both PDBs, so the name is descriptive.
// Leaf: ?GetNextEmitterID_00a3e420@CSoundManagerLH@@QAEJXZ
class CSoundManagerLH {
public:
    long GetNextEmitterID_00a3e420();
    unsigned char _pad_0x00[0x50];
    long m_nextEmitterID;            // +0x50 (descriptive)
};

long CSoundManagerLH::GetNextEmitterID_00a3e420()
{
    return m_nextEmitterID++;
}