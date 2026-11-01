// CAmbientEvent::Trigger(CAmbientEmitter* emitter) -- fire one ambient event on its emitter: skip when the emitter has no sounds, when the random slot test misses (random events) or the emitter is silent; otherwise build the voice parameters (volume scaled by the ambient cube for non-random events, the zone's position when the bank is positional, a 1.0 scale), register them with the emitter's owner, pick a random sound, jitter its position by the scene spread, add the height-field sample plus a random height in the zone's range, and start it through the emitter's voice table
// Grade: DIFFER(579v581) -- the Play call's argument pushes are interleaved with the height x87 chain (retail finishes the chain, loads the float bits and m_pSound, then pushes); everything before the last rand() call matches
// Leaf: ?Trigger@CAmbientEvent@@QAEXPAVCAmbientEmitter@@@Z
extern "C" __declspec(dllimport) int __cdecl rand(void);

struct CPKArray {                                   // libpk array (landed CPKArray::GetRandom 0x00C61C00)
    unsigned long  m_capacity;                      // +0x00
    unsigned long* m_pData;                         // +0x04
    unsigned long  m_count;                         // +0x08
    unsigned long GetCapacity() const;              // 0x00C61BC0
    unsigned long GetCount() const;                 // 0x00C61C60
    unsigned long GetRandom();                      // 0x00C61C00
};

struct CFloatPair {                                 // landed CFloatPair::GetX/GetY 0x00C1BD40/0x00C1BD50
    float x;
    float y;
    float GetX() const;                             // 0x00C1BD40
    float GetY() const;                             // 0x00C1BD50
};

struct CIndexedValue {                              // landed 0x00C25790 result type
    long  Index;
    float Value;
};

struct CPairOwner_Retail {                          // landed 0x00C25790
    long  Index;
    float Value;
    CIndexedValue GetPair();                        // 0x00C25790
};

struct SPosition2D {                                // landed CHeightSource::Evaluate 0x00C2A390
    float x;
    float y;
    void Add(const CIndexedValue& offset);          // 0x00C46DA0 (landed as CQuadric::OperatorPlusEq)
};

struct SPosition3D {
    float x;
    float y;
    float z;
};

class CAmbientCube {
public:
    float GetLevel() const;                         // 0x00C45940
};

struct CHeightSource {                              // the bank (landed CHeightSource::Evaluate)
    unsigned char _pad00[0xce];
    bool          m_bPositional;                    // +0xce
    float Evaluate(const SPosition2D& position);    // 0x00C2A390
};

struct CAmbientScene {
    CHeightSource* m_pBank;                         // +0x00
    float          m_spread;                        // +0x04
    unsigned char  _pad08[0xc - 8];
    struct CSoundTable* m_pTable;                   // +0x0c
    unsigned char  _pad10[0x40 - 0x10];
    CPairOwner_Retail m_origin;                     // +0x40
    SPosition2D* Lookup(unsigned long id);          // 0x00C36C70
};

struct CAmbientZone {
    CAmbientScene*   m_pScene;                      // +0x00
    unsigned char    _pad04[0xc - 4];
    CFloatPair       m_heightRange;                 // +0x0c
    unsigned char    _pad14[0x44 - 0x14];
    CFloatPair       m_position;                    // +0x44
    CAmbientCube     m_cube;                        // +0x4c
};

struct CPKVoiceParams;

struct CRegistryLayer_00c383d0 {
    struct CRegisteredItem* Add(struct CRegisteredItem* item, bool flagA, bool flagB);   // 0x00C383D0
};

class CAmbientEmitter {
public:
    CAmbientZone*            m_pZone;               // +0x00
    CRegistryLayer_00c383d0* m_pOwner;              // +0x04
    unsigned char            _pad08[0x80 - 8];
    CPKArray                 m_sounds;              // +0x80
    float GetVolume() const;                        // 0x00C44A90
};

struct CMessageListOwner {
    unsigned char _body[0x1c];
    CMessageListOwner();                            // 0x00C203A0
};
struct CNavigationLayer {
    void* m_pFirst; void* m_pSecond; void* m_pThird; unsigned char m_flag;
    CNavigationLayer();                             // 0x00C20390
};

struct CPKVoiceParams {                             // 0x50 bytes (landed as SNavigationRequest, ctor 0x00C10A30)
    CMessageListOwner m_messages;                   // +0x00
    long              m_1c;                         // +0x1c
    long              m_20;                         // +0x20
    float             m_volume;                     // +0x24
    float             m_28;                         // +0x28
    long              m_2c;                         // +0x2c
    SPosition2D       m_position;                   // +0x30
    unsigned char     _pad38[0x48 - 0x38];
    long              m_owner;                        // +0x48
    unsigned char     m_4c;                         // +0x4c
    CPKVoiceParams();                               // 0x00C10A30
    void SetX(float x);                             // 0x00C20480
    void SetY(float y);                             // 0x00C204A0
    void SetScale(float s);                         // 0x00C20420
    void SetPosition(const float* xyz) { m_position.x = xyz[0]; m_position.y = xyz[1]; m_owner = *(const long*)&xyz[2]; }
};

class CPKVoiceTable {
public:
    long Play(struct CPKSoundKey* key, void* b, CPKVoiceParams* params);   // 0x00C13140
};

class CAmbientEvent {
public:
    struct CPKSoundKey* m_pSound;                   // +0x00
    unsigned long       m_time;                     // +0x04
    bool                m_bRandom;                  // +0x08
    CPKVoiceTable* GetTable(CAmbientEmitter* emitter);   // 0x00C61CF0
    void Trigger(CAmbientEmitter* emitter);
};

void CAmbientEvent::Trigger(CAmbientEmitter* emitter)
{
    if (emitter->m_sounds.GetCount() == 0)
        return;
    if (m_bRandom) {
        unsigned long index = (unsigned long)((float)(emitter->m_sounds.GetCapacity() * rand()) * (1.0f / 32767.0f));
        if (index >= emitter->m_sounds.GetCount())
            return;
    }
    if (emitter->GetVolume() <= 0.0f)
        return;
    {
        CPKVoiceParams params;
        params.m_volume = emitter->GetVolume();
        if (!m_bRandom)
            params.m_volume *= emitter->m_pZone->m_cube.GetLevel();
        params.m_1c = 0x45;
        params.m_2c = 1;
        CAmbientZone* zone = emitter->m_pZone;
        if (zone->m_pScene->m_pBank->m_bPositional) {
            params.SetX(zone->m_position.GetX());
            params.SetY(emitter->m_pZone->m_position.GetY());
        }
        params.SetScale(1.0f);
        emitter->m_pOwner->Add((CRegisteredItem*)&params, true, true);
        SPosition2D pos = *emitter->m_pZone->m_pScene->Lookup(emitter->m_sounds.GetRandom());
        pos.Add(emitter->m_pZone->m_pScene->m_origin.GetPair());
        float xyz[3];
        xyz[0] = emitter->m_pZone->m_pScene->m_spread * ((float)rand() * (1.0f / 32767.0f)) + pos.x;
        CAmbientScene* scene = emitter->m_pZone->m_pScene;
        xyz[1] = scene->m_spread * ((float)rand() * (1.0f / 32767.0f)) + pos.y;
        xyz[2] = scene->m_pBank->Evaluate(pos);
        CFloatPair range = emitter->m_pZone->m_heightRange;
        xyz[2] += (range.y - range.x) * ((float)rand() * (1.0f / 32767.0f)) + range.x;
        params.SetPosition(xyz);
        params.m_4c = 0;
        GetTable(emitter)->Play(m_pSound, 0, &params);
    }
}
