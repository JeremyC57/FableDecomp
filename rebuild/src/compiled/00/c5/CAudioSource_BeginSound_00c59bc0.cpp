// CAudioSource::BeginSound(CAudioSound* sound) -- prime the source from the sound's current queue slot: take the slot's due time as the start time, hand the slot's buffer (or null for an empty queue) with the sound's loop bit to the voice, then the sound's format record
// Leaf: ?BeginSound@CAudioSource@@QAEXPAVCAudioObject@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C59BC0, 89 bytes, thiscall, `ret 4`.
// Identity: bound by the landed callers CAudioSource::Update 0x00C72FE0 and CSoftwareVoice::StartSound 0x00C5B250 (by a
//   cast on the voice); it sits in the GameCODA voice TU next to CCodaVoice::Play 0x00C59720 (the same SetBuffer /
//   SetFormat pair, there from the sound's single buffer +0x78). Descriptive (no PDB or RTTI covers GameCODA).
//   Label correction: the manifest row "CManager::GetCurrentScreen" is propagated.
// Callees (virtual): slot 18 (+0x48) SetBuffer 0x00C59880 (base), slot 14 (+0x38) SetFormat.
// Layout: CAudioSound's CircularQueue at +0x54 (items +0x54, times +0x58, capacity +0x5C; the landed CircularQueue
//   model), the current slot +0x64 (landed CAudioSource::Update), flag word +0x10 (bit 8: loop), format +0xF0;
//   the source's 64-bit start time +0x10.
// Shape: the inline capacity-checked item read first (eax), then the 8-byte time copy, then the byte-narrowed loop bit
//   (as CCodaVoice::Play).
struct CItem;
class CircularQueue {                                   // landed 0x00C442C0 model
public:
    CItem**       m_items;                              // +0x00
    __int64*      m_times;                              // +0x04 per-slot times
    unsigned long m_capacity;                           // +0x08
};
class CAudioObject { public:
    unsigned char  _pad00[0x10];
    unsigned long  m_flags;                             // +0x10 (bit 8: loop)
    unsigned char  _pad14[0x54 - 0x14];
    CircularQueue  m_queue;                             // +0x54
    unsigned char  _pad60[4];
    unsigned long  m_current;                           // +0x64 index of the current slot
    unsigned char  _pad68[0xf0 - 0x68];
    void*          m_pFormat;                           // +0xF0
};
#pragma pack(push, 4)
class CAudioSource {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void SetFormat(void* format);               // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    virtual void SetBuffer(void* buffer, unsigned char loop);   // slot 18 (+0x48)
    unsigned char _pad04[8];
    void*         m_pBuffer;                            // +0x0C
    __int64       m_startTime;                          // +0x10
    void BeginSound(CAudioObject* sound);
};
#pragma pack(pop)

void CAudioSource::BeginSound(CAudioObject* sound)
{
    CItem* item = sound->m_queue.m_capacity ? sound->m_queue.m_items[sound->m_current] : 0;
    m_startTime = sound->m_queue.m_times[sound->m_current];
    SetBuffer(item, (sound->m_flags >> 8) & 1);
    SetFormat(sound->m_pFormat);
}