// CAudioSource::Update(CAudioSound* sound, __int64 now) -- per-tick update of an OpenAL streaming source: on the first tick restart the voice and take the sound's first queued buffer time; re-apply the sound's properties; until the start time comes return 1 (waiting), then force the next due buffer into the source: 0 when one was queued (or an error), 2 when nothing was ready
// Leaf: ?Update@CAudioSource@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C72FE0, 108 bytes, thiscall, ret 0Ch (the 64-bit time by value).
// Callees: the source's virtual slot 17 (+0x44), 0x00C59BC0 (thiscall, the sound: copies the queued item's time
//   (sound+0x58)[sound+0x64] to +0x10/+0x14), 0x00C72E00 (thiscall (sound, now): the shared property update that calls
//   the landed CAudioSource::InitializeProperties 0x00C72AF0 on sound / sound+0x14), CAudioSource::QueueNextBuffer
//   0x00C72C40 (landed; the sound's CircularQueue at +0x54, force = true).
// Identity: slot 8 of the three OpenAL source vtables 0x012B8CC8 / 0x012B8D48 / 0x012BEE40 (xrefs 0x012B8CE8,
//   0x012B8D68, 0x012BEE60); slot 9 0x00C73050 runs the same 0x00C72E00 update. libpk/OpenAL layer: no PDB/RTTI, so the
//   class keeps the landed CAudioSource name (QueueNextBuffer's caller 0x00C73036 is this row) and the method and callee
//   names are descriptive.
// Shape: \`r ? 0 : 2\` on QueueNextBuffer's result is neg / sbb / and -2 / add 2; `m_startTime > now` compares the high
//   words signed and the low words unsigned.
struct CItem;

class CircularQueue {                      // landed 0x00C44430 model
public:
    CItem**           m_items;             // +0x00
    __int64*          m_times;             // +0x04
    unsigned long     m_capacity;          // +0x08
};

class CAudioObject { public:                       // the sound a source plays (flag word +0x10, property block +0x14)
    unsigned char  _pad00[0x54];
    CircularQueue  m_queue;                // +0x54 decoded buffers and their due times
    unsigned char  _pad60[4];
    unsigned long  m_current;              // +0x64 index of the current item
};

struct CAudioSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual long Update(CAudioObject* sound, __int64 now);                // slot 8 (+0x20)
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16();
    virtual void Restart();                                              // slot 17 (+0x44)
    virtual void v18();
    virtual long PrepareBuffer(CItem* item, unsigned long* bufferId);    // slot 19 (+0x4C)
    unsigned char  _pad08[4];             // +0x08 (VC7.1 pads the vfptr to 8: the class holds an __int64)
    long           m_started;              // +0x0C
    __int64        m_startTime;            // +0x10
    void BeginSound(CAudioObject* sound);                                 // 0x00C59BC0
    void ApplySound(CAudioObject* sound, __int64 now);                    // 0x00C72E00
    long QueueNextBuffer(CircularQueue* queue, __int64 now, bool force); // 0x00C72C40
};

long CAudioSource::Update(CAudioObject* sound, __int64 now)
{
    if (!m_started) {
        Restart();
        BeginSound(sound);
    }
    ApplySound(sound, now);
    if (m_startTime > now)
        return 1;
    return QueueNextBuffer(&sound->m_queue, now, true) ? 0 : 2;
}