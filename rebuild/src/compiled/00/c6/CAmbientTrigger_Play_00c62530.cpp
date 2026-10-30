// CAmbientTrigger::Play(CAmbientSource* source) -- (re)start the trigger's ambient sound: drop any playing voice, then if the source is audible pick a random offset inside the sound's minimum-distance sphere, place the voice at the source owner's position plus that offset with the source's volume, set the distance attenuation range (minimum / maximum minus the offset length), register the parameter block on the source's layer and play it, keeping the voice handle
// Leaf: ?Play@CAmbientTrigger@@QAEXPAVCAmbientSource@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62530, 354 bytes, thiscall `ret 4`, frameless (0x5c-byte frame).
// Identity: libpk ambient sound (no PDB / RTTI: descriptive names). `this` is the 0x18-byte CAmbientTrigger of the landed
//   CAmbientTrigger::UpdateVolume 0x00C624C0 (sound +0, handle +8, offset vector +0xC) and the argument its CAmbientSource
//   (GetVolume 0x00C46F40 is called on it, owner +0, registry layer +4); its siblings RemoveHandle 0x00C62500 and
//   GetHandleTable 0x00C62440 are the ones UpdateVolume / PlaceAt use. Method name descriptive. The catalog label
//   CItem_UpdateWeaponGeometry is propagated and wrong.
// Callees: CAmbientTrigger::RemoveHandle 0x00C62500 / GetHandleTable 0x00C62440; CAmbientSource::GetVolume 0x00C46F40;
//   CUnprovenPackedParameterRange::GetMinimum 0x00C1BA60 / GetMaximum 0x00C1BA70 (the sound's distance range at +8);
//   C3DVector::GetMagnitude 0x00C25970, operator+ 0x00C257E0 (__fastcall, hidden result in ecx); the 0x50-byte play
//   parameter block of the landed CSoundLoop_00c37840::Update 0x00C37840 (member ctors CMessageListOwner_00c203a0
//   0x00C203A0 and SEventRecord 0x00C20390, inline scalar initialisers) and its setters 0x00C20480 / 0x00C204A0 /
//   0x00C20420 (float arguments: `fstp [esp]`); CRegistryLayer_00c383d0::Add 0x00C383D0 (item, true, true);
//   CSoundSystem_00c37840::Play_00c13140 0x00C13140 (sound, 0, &params) on the table GetHandleTable returns.
// Same-TU helpers (stand-ins, not graded here): the random point in a sphere 0x00C62370 (static, retail's private
//   convention: the hidden result pointer in esi, the radius on the stack, `ret 4` -- VC7.1 gives a same-TU
//   `static __declspec(noinline) __stdcall` exactly this, and its body here byte-matches retail) and the random
//   number SetLungedThisFrame 0x00C62350 (catalog name; defined earlier in the TU so the helper keeps theta on the x87
//   stack across the call, as retail does).
// Shape: the volume test is `if (GetVolume() <= 0.0f) return;` (`test ah,41h ; jnp`); the offset length is a float homed
//   in the dead argument slot; the table returned by GetHandleTable is the sound system (CHandleTable in UpdateVolume's
//   spelling, CSoundSystem_00c37840 in Play_00c13140's), hence the cast.
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
struct CSoundPlayParams_00c37840 {
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
    CSoundPlayParams_00c37840()
        : m_loopCount(0), m_delay(0), m_volume(1.0f), m_pitch(1.0f), m_priority(0),
          m_flags(0), m_group(0), m_owner(0), m_user(0) {}
    void Set2_00c20480(float value);                    // 0x00C20480
    void Set3_00c204a0(float value);                    // 0x00C204A0
    void Set9_00c20420(float value);                    // 0x00C20420
};
struct CSoundSystem_00c37840 {
    long Play_00c13140(long soundID, long flags, const CSoundPlayParams_00c37840* params);   // 0x00C13140
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
    CSoundPlayParams_00c37840 params;
    m_offset = GetRandomPointInSphere(m_pSound->Range.GetMinimum());
    float distance = m_offset.GetMagnitude();
    params.m_links.m_position = m_offset + source->m_pOwner->m_position;
    params.m_links.m_flag = 0;
    params.m_volume = source->GetVolume();
    params.m_loopCount = source->m_pOwner->m_id;
    params.m_priority = 1;
    params.Set2_00c20480(m_pSound->Range.GetMinimum() - distance);
    params.Set3_00c204a0(m_pSound->Range.GetMaximum() - distance);
    params.Set9_00c20420(1.0f);
    source->m_pLayer->Add((CRegisteredItem*)&params, true, true);
    m_handle = ((CSoundSystem_00c37840*)GetHandleTable(source))->Play_00c13140((long)m_pSound, 0, &params);
}