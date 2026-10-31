// CSoundEvent::UpdateTrackedParameters(CSoundEventManager* manager) -- for each parameter the event tracks (+0xAE bits: 1 user gain, 2 the +0x70 value, 4 the 3D position when the mode is non-zero or the 2D pan when it is zero) hand the current value to the manager's +0x7CC source for update, then store back every value the source changed through the per-value setters
// Grade: DIFFER(413v415) -- only the first record read: retail copies the getter's 16-byte result for the position from its frame slot ([esp+2Ch]/[esp+30h]/[esp+34h], z loaded first, the spilled pan pointer reloaded between the stores); ours copies through the returned pointer in eax (`mov ecx,ebx` then dword moves). Register allocation, the and/neg/sbb pointer selection, both GetMode calls, the x87 compares, the second record copy and every call match. A named record local (block or inline accessor) reads the frame but turns ebp into a zero register and spills the position pointer instead of the pan pointer (399v415).
// Leaf: ?UpdateTrackedParameters@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C213C0, 415 bytes, thiscall `ret 4`. Descriptive names (libpk has no PDB or
//   RTTI): the spelling is the one the landed CSoundEvent::Update 0x00C21A50 declares and binds for its call
//   (`UpdateTrackedParameters(manager); // 0x00C213C0`); the manifest's "Thiscall_CreatureOpinionUpdate" is propagated.
//   Only caller 0x00C21A50.
// Callees (all landed CSoundEvent members, thiscall): GetMode 0x00C20EE0 (twice), SetUserGain 0x00C20C80, SetParam70
//   0x00C20C50, Set3DPosition 0x00C20E50, Set2DPosition 0x00C20EB0; the parameter block's record getter 0x00C34600
//   (hidden result, twice); the source's slot 0 (virtual).
// Shape: the track word stays in edx across both GetMode calls because GetMode and its callee CheckState are compiled
//   above this body in the same TU (VC7.1 callee register-usage knowledge; same-TU stand-ins below, only this leaf is
//   the row). The three optional float pointers are `flags & bit ? &local : 0` (and/neg/sbb/and); the pan pointer is
//   spilled. The record copied for Set3DPosition keeps the getter's flag word and takes the source's position.
//   The vector is zero-initialised.
struct CSoundEventManager;

struct CVector3 {                           // the 12-byte position the source fills
    float x, y, z;
};

class CUnprovenVector_00c345b0 {            // the parameter block's 16-byte 3D record (position + flag word)
public:
    CVector3      m_position;               // +0x00
    unsigned char m_flag;                   // +0x0C
    unsigned char _pad0d[3];
};
struct SEventRecord : CUnprovenVector_00c345b0 {   // same record under the getter's landed spelling
    SEventRecord() { }
};

struct CSoundParams {                       // +0x38 the event's parameter block
    SEventRecord  m_record;                 // +0x00 (event +0x38)
    long          m_mode;                   // +0x10 (event +0x48)
    unsigned char _pad14[0x28 - 0x14];
    float         m_pan;                    // +0x28 (event +0x60)
    unsigned char _pad2c[4];
    float         m_userGain;               // +0x30 (event +0x68)
    unsigned char _pad34[4];
    float         m_param70;                // +0x38 (event +0x70)
    unsigned char _pad3c[4];
    SEventRecord GetRecord() const;         // 0x00C34600 (hidden result)
};

struct CSoundEventSource {                  // the manager's +0x7CC source of tracked values
    virtual void UpdateValues(long handle, float* gain, float* param70, CVector3* position, float* pan);   // slot 0
};

struct CSoundEventManager {
    unsigned char      _pad000[0x7cc];
    CSoundEventSource* m_pSource;           // +0x7CC
};

struct CSoundEvent {                        // 0xB4 bytes, vtable 0x012B3674 (no RTTI)
    void*          __vftable;               // +0x00
    unsigned char  _pad04[0x24 - 0x04];
    long           m_handle;                // +0x24
    unsigned char  _pad28[0x38 - 0x28];
    CSoundParams   m_params;                // +0x38
    unsigned char  _pad78[0xa8 - 0x78];
    unsigned long  m_dirty;                 // +0xA8
    unsigned short m_state;                 // +0xAC
    unsigned short m_trackFlags;            // +0xAE
    unsigned char  _padb0[4];
    void CheckState(long expected);         // 0x00C20720
    long GetMode();                         // 0x00C20EE0
    void SetParam70(float value);           // 0x00C20C50
    void SetUserGain(float gain);           // 0x00C20C80
    void Set3DPosition(const CUnprovenVector_00c345b0& position);   // 0x00C20E50
    void Set2DPosition(float pan);          // 0x00C20EB0
    void UpdateTrackedParameters(CSoundEventManager* manager);
};

// Same-TU stand-ins of the retail helpers compiled above this body (retail: CheckState is a byte-indexed jump table
// over the state word whose cases are all empty; GetMode is the landed body).
#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
__declspec(noinline) long CSoundEvent::GetMode()
{
    CheckState(-1);
    return m_params.m_mode;
}
#pragma auto_inline(on)

void CSoundEvent::UpdateTrackedParameters(CSoundEventManager* manager)
{
    unsigned short flags = m_trackFlags;
    if (!flags || !manager->m_pSource)
        return;
    float gain;
    float param70;
    CVector3 position = { 0, 0, 0 };
    float pan;
    float* pGain = (flags & 1) ? &gain : 0;
    float* pParam70 = (flags & 2) ? &param70 : 0;
    CVector3* pPosition = (GetMode() && (flags & 4)) ? &position : 0;
    float* pPan = (!GetMode() && (flags & 4)) ? &pan : 0;
    if (pGain)
        *pGain = m_params.m_userGain;
    if (pParam70)
        *pParam70 = m_params.m_param70;
    if (pPosition)
        *pPosition = m_params.GetRecord().m_position;
    else if (pPan)
        *pPan = m_params.m_pan;
    manager->m_pSource->UpdateValues(m_handle, pGain, pParam70, pPosition, pPan);
    if (pGain && *pGain != m_params.m_userGain)
        SetUserGain(*pGain);
    if (pParam70 && *pParam70 != m_params.m_param70)
        SetParam70(*pParam70);
    if (pPosition) {
        SEventRecord record = m_params.GetRecord();
        record.m_position = *pPosition;
        Set3DPosition(record);
    } else if (pPan && *pPan != m_params.m_pan)
        Set2DPosition(*pPan);
}
