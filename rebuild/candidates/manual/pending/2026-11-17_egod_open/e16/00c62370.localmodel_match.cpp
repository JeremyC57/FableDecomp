// GetRandomPointInSphere(float radius) -- a random point inside a sphere of the given radius: azimuth phi = 2*pi*u, polar angle theta = acos(2u-1), distance r = u*radius (u = three uniform randoms), returned as (sin(theta)cos(phi)r, cos(theta)sin(phi)r, cos(theta)r)
// Leaf: ?GetRandomPointInSphere@@YG?AVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62370, 118 bytes, `ret 4` (same-TU static: retail's private convention, the hidden
//   result pointer in esi, returned in eax, the radius on the stack).
// Identity: libpk ambient-sound code (no PDB, no RTTI, nothing in Ego_d / Ego_r / FableWin): descriptive name, the one the landed
//   caller CAmbientTrigger::Play 0x00C62530 already binds (rebuild/oracles/reloc_bindings.tsv). The catalog's
//   `Quaternion_FromAxisAngle` is an autoname (no quaternion: three floats from two angles and a length); the egod_tu.txt object
//   (combat_sequence_isvalid_predicates.obj) came from the propagated label NBalverineCombat::SetLungedThisFrame on 0x00C62350.
// Same TU: this file is the TU of the landed row 0x00C62530 (its only caller; a static helper needs that caller to keep retail's
//   private convention). Callee 0x00C62350 (rand() * 1/32767, spelled as the landed row spells it) and MSVCR71 _CIacos via /Oi.
#include <math.h>
#pragma intrinsic(sin, cos, acos)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetMagnitude() const;                         // 0x00C25970
};
C3DVector __fastcall operator+(const C3DVector& a, const C3DVector& b);   // 0x00C257E0
extern "C" __declspec(dllimport) int __cdecl rand();   // IAT __imp__rand 0x01440124
extern float g_scale_0129c81c;
__declspec(noinline) float SetLungedThisFrame()         // 0x00C62350 (same TU)
{
    return (float)rand() * g_scale_0129c81c;
}

class CUnprovenPackedParameterRange {
public:
    unsigned char UnprovenPrefix[16];
    unsigned short MinimumWord, MaximumWord;
    float GetMinimum() const;                           // 0x00C1BA60
    float GetMaximum() const;                           // 0x00C1BA70
};
struct CAmbientSoundDef_00c62530 {
    unsigned char _pad[8];
    CUnprovenPackedParameterRange Range;                // +0x08
};

struct CMessageEventList { void* head; };
struct CMessageListOwner_00c203a0 {
    long  m_state;
    short m_index;
    short m_count;
    CMessageEventList m_messages;
    CMessageListOwner_00c203a0();                       // 0x00C203A0
};
struct SEventRecord {
    C3DVector     m_position;
    unsigned char m_flag;
    SEventRecord();                                     // 0x00C20390
};
struct CPKVoiceParams {
    CMessageListOwner_00c203a0 m_filter;
    unsigned char _unset00c[0x10];
    long  m_loopCount;
    long  m_delay;
    float m_volume;
    float m_pitch;
    long  m_priority;
    SEventRecord m_links;
    long  m_flags;
    short m_group;
    long  m_owner;
    long  m_user;
    CPKVoiceParams()
        : m_loopCount(0), m_delay(0), m_volume(1.0f), m_pitch(1.0f), m_priority(0),
          m_flags(0), m_group(0), m_owner(0), m_user(0) {}
    void SetMinDistance(float value);                    // 0x00C20480
    void SetMaxDistance(float value);                    // 0x00C204A0
    void SetScale(float value);                    // 0x00C20420
};
struct CSoundEventManager {
    long Play(long soundID, long flags, const CPKVoiceParams* params);   // 0x00C13140
};
struct CRegisteredItem;
struct CRegistryLayer_00c383d0 {
    CRegisteredItem* Add(CRegisteredItem* item, bool flagA, bool flagB);   // 0x00C383D0
};
struct CAmbientOwner_00c62530 {
    unsigned char _pad[0xc];
    long      m_id;                                     // +0x0c
    unsigned char _pad10[4];
    C3DVector m_position;                               // +0x14
};
class CAmbientSource {
public:
    CAmbientOwner_00c62530*  m_pOwner;                  // +0x00
    CRegistryLayer_00c383d0* m_pLayer;                  // +0x04
    float GetVolume();                                  // 0x00C46F40
};
class CHandleTable;
class CAmbientTrigger {
public:
    CAmbientSoundDef_00c62530* m_pSound;                // +0x00
    unsigned long m_time;                               // +0x04
    long          m_handle;                             // +0x08
    C3DVector     m_offset;                             // +0x0c
    CHandleTable* GetHandleTable(CAmbientSource* source);   // 0x00C62440
    void RemoveHandle(CAmbientSource* source);          // 0x00C62500
    void Play(CAmbientSource* source);
};

static __declspec(noinline) C3DVector __stdcall GetRandomPointInSphere(float radius)
{
    float phi = SetLungedThisFrame() * 6.28318548f;
    float theta = (float)acos(SetLungedThisFrame() * 2.0f - 1.0f);
    float r = SetLungedThisFrame() * radius;
    C3DVector point((float)sin(theta) * (float)cos(phi) * r, (float)cos(theta) * (float)sin(phi) * r, (float)cos(theta) * r);
    return point;
}

void CAmbientTrigger::Play(CAmbientSource* source)
{
    RemoveHandle(source);
    if (source->GetVolume() <= 0.0f)
        return;
    CPKVoiceParams params;
    m_offset = GetRandomPointInSphere(m_pSound->Range.GetMinimum());
    float distance = m_offset.GetMagnitude();
    params.m_links.m_position = m_offset + source->m_pOwner->m_position;
    params.m_links.m_flag = 0;
    params.m_volume = source->GetVolume();
    params.m_loopCount = source->m_pOwner->m_id;
    params.m_priority = 1;
    params.SetMinDistance(m_pSound->Range.GetMinimum() - distance);
    params.SetMaxDistance(m_pSound->Range.GetMaximum() - distance);
    params.SetScale(1.0f);
    source->m_pLayer->Add((CRegisteredItem*)&params, true, true);
    m_handle = ((CSoundEventManager*)GetHandleTable(source))->Play((long)m_pSound, 0, &params);
}