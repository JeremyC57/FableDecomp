// Behaviour test for CAmbientTrigger::Play 0x00C62530 (body copied from src/00c62530.cpp; the libpk callees are mocked).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __declspec(x)
#endif
#include <math.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetMagnitude() const;                         // 0x00C25970
};
C3DVector __fastcall operator+(const C3DVector& a, const C3DVector& b);   // 0x00C257E0
float g_scale_0129c81c = 1.0f / 32768.0f;
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

// ---- mocks ----
float C3DVector::GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
C3DVector __fastcall operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
float CUnprovenPackedParameterRange::GetMinimum() const { return MinimumWord; }
float CUnprovenPackedParameterRange::GetMaximum() const { return MaximumWord; }
CMessageListOwner_00c203a0::CMessageListOwner_00c203a0() : m_state(0), m_index(0), m_count(0) { m_messages.head = 0; }
SEventRecord::SEventRecord() : m_position(-1, -1, -1), m_flag(7) { }
static float g_set2 = -999, g_set3 = -999, g_set9 = -999;
void CSoundPlayParams_00c37840::Set2_00c20480(float v) { g_set2 = v; m_filter.m_state |= 4; }
void CSoundPlayParams_00c37840::Set3_00c204a0(float v) { g_set3 = v; m_filter.m_state |= 8; }
void CSoundPlayParams_00c37840::Set9_00c20420(float v) { g_set9 = v; }
static CSoundPlayParams_00c37840 g_played;
static long g_playSound = 0, g_playFlags = -1;
long CSoundSystem_00c37840::Play_00c13140(long soundID, long flags, const CSoundPlayParams_00c37840* params)
{ g_playSound = soundID; g_playFlags = flags; std::memcpy(&g_played, params, sizeof(g_played)); return 42; }
static int g_adds = 0;
CRegisteredItem* CRegistryLayer_00c383d0::Add(CRegisteredItem* item, bool a, bool b) { if (a && b) ++g_adds; return item; }
static float g_volume = 0.5f;
float CAmbientSource::GetVolume() { return g_volume; }
static CSoundSystem_00c37840 g_system;
CHandleTable* CAmbientTrigger::GetHandleTable(CAmbientSource*) { return (CHandleTable*)&g_system; }
static int g_removes = 0;
void CAmbientTrigger::RemoveHandle(CAmbientSource*) { ++g_removes; m_handle = -1; }

int main()
{
    CAmbientSoundDef_00c62530 def; std::memset(&def, 0, sizeof def);
    def.Range.MinimumWord = 10; def.Range.MaximumWord = 50;
    CAmbientOwner_00c62530 owner; std::memset(&owner, 0, sizeof owner);
    owner.m_id = 77; owner.m_position = C3DVector(100, 200, 300);
    CRegistryLayer_00c383d0 layer;
    CAmbientSource source; source.m_pOwner = &owner; source.m_pLayer = &layer;
    CAmbientTrigger trigger; std::memset(&trigger, 0, sizeof trigger);
    trigger.m_pSound = &def; trigger.m_handle = 5;

    std::srand(1234);
    for (int k = 0; k < 50; ++k) {
        g_volume = 0.25f + k * 0.01f;
        trigger.Play(&source);
        float d = trigger.m_offset.GetMagnitude();
        if (d > 10.001f) { std::printf("00c62530_TEST FAIL radius %f\n", d); return 1; }
        if (trigger.m_handle != 42 || g_playSound != (long)&def || g_playFlags != 0) { std::printf("00c62530_TEST FAIL play\n"); return 1; }
        const C3DVector& p = g_played.m_links.m_position;
        if (fabs(p.X - (100 + trigger.m_offset.X)) > 1e-3f || fabs(p.Y - (200 + trigger.m_offset.Y)) > 1e-3f ||
            fabs(p.Z - (300 + trigger.m_offset.Z)) > 1e-3f || g_played.m_links.m_flag != 0) { std::printf("00c62530_TEST FAIL position\n"); return 1; }
        if (g_played.m_volume != g_volume || g_played.m_loopCount != 77 || g_played.m_priority != 1 || g_played.m_pitch != 1.0f ||
            g_played.m_delay != 0 || g_played.m_flags != 0 || g_played.m_group != 0 || g_played.m_owner != 0 || g_played.m_user != 0) {
            std::printf("00c62530_TEST FAIL params\n"); return 1;
        }
        if (fabs(g_set2 - (10 - d)) > 1e-3f || fabs(g_set3 - (50 - d)) > 1e-3f || g_set9 != 1.0f || (g_played.m_filter.m_state & 12) != 12) {
            std::printf("00c62530_TEST FAIL range\n"); return 1;
        }
    }
    if (g_removes != 50 || g_adds != 50) { std::printf("00c62530_TEST FAIL counts\n"); return 1; }
    // silent source: the voice is dropped and nothing is played
    g_volume = 0.0f; g_playSound = 0;
    trigger.Play(&source);
    if (g_removes != 51 || g_adds != 50 || g_playSound != 0 || trigger.m_handle != -1) { std::printf("00c62530_TEST FAIL silent\n"); return 1; }
    std::printf("00c62530_TEST PASS\n");
    return 0;
}