// CSoftwareVoice::StartSound(CAudioSound* sound, __int64 startTime) -- start a sound on the software voice: when no sound is active (+0xB8) clear the buffer (+0x0C), begin the sound and reset through slot 17; schedule it at startTime; if the voice clock (+0x10) is still short of startTime plus one 128-frame mix block (performance-counter ticks at 44.1 kHz) return 2 (pending), else schedule once more when a sound is active and return 1
// Leaf: ?StartSound@CSoftwareVoice@@UAEJPAVCAudioObject@@_J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5B250, 162 bytes, thiscall `ret 0xC`.
// Identity (descriptive method name; GameCODA has no PDB or RTTI): slot 8 of the CSoftwareVoice vtable 0x012B5C60
//   (entry 0x012B5C80). BeginSound 0x00C59BC0 is the landed `CAudioSource::BeginSound` spelling, reached by a cast on
//   the same `this` (one name per address). 0x00C5ACB0 is the voice's own scheduler; its catalog label
//   "C3DMeshFileXBoundingBoxChunk::LoadData" is a wrong autoname (label correction) -- descriptive ScheduleSound here.
// Callees: CAudioSource::BeginSound 0x00C59BC0, virtual slot 17 (+0x44), CSoftwareVoice::ScheduleSound 0x00C5ACB0,
//   __allmul 0x00BFEB50 / __alldiv 0x00F3FC50 on g_performanceFrequency 0x0139CA20 (spelling of landed 0x00C59B00).
class CAudioObject;
class CAudioSource {
public:
    void BeginSound(CAudioObject* sound);                // 0x00C59BC0
};
extern __int64 g_performanceFrequency;                  // 0x0139CA20

#pragma pack(push, 4)                                   // the 64-bit clock sits at +0x10 (4-byte packing)
class CSoftwareVoice {                                  // vtable 0x012B5C60
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual long StartSound(CAudioObject* sound, __int64 startTime);     // slot 8
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16();
    virtual void Reset();                               // slot 17 (+0x44)
    void ScheduleSound(CAudioObject* sound, __int64 startTime);          // 0x00C5ACB0
    unsigned char _pad04[8];
    void*         m_pBuffer;                            // +0x0c
    __int64       m_clock;                              // +0x10 voice clock (performance-counter ticks)
    unsigned char _pad18[0xa0];
    void*         m_pActiveSound;                       // +0xb8
};
#pragma pack(pop)

long CSoftwareVoice::StartSound(CAudioObject* sound, __int64 startTime)
{
    if (!m_pActiveSound) {
        m_pBuffer = 0;
        ((CAudioSource*)this)->BeginSound(sound);
        Reset();
    }
    ScheduleSound(sound, startTime);
    if (m_clock < g_performanceFrequency * 128 / 44100 + startTime)
        return 2;
    if (m_pActiveSound)
        ScheduleSound(sound, startTime);
    return 1;
}