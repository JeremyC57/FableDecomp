// CAmbientEmitter::AcquireSounds() -- fill the emitter's sound list (+0x50) from its definition: for every sound id of the definition's sound set, acquire the sound from the bank and add it to the list
// Leaf: ?AcquireSounds@CAmbientEmitter@@QAEXXZ
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44CE0, 71 bytes (+ int3 padding), __fastcall `ret`.
// Library: libpk ambient sound (no PDB/RTTI): descriptive names. Only caller the emitter's constructor 0x00C44EB0 (at
//   0x00C44F59). The manifest's "World_RegisterResourceStrings" is propagated.
// Layout: definition +4 -> sound set +8 -> a pointer array {data, capacity, count} at its +8 (agent 434's CPtrArray).
// Callees: the bank's Acquire 0x00C2A3D0, the list's virtual Add (slot 9).
// Shape: needs /Oa and the id read through the array's by-value GetAt inline (or into a named local): then the id
//   load and its push come before the zone -> scene -> bank loads (`mov edx,[ebp] ; push eax ; mov eax,[edx] ; mov
//   ecx,[eax]`); a direct `ids.m_data[i]` argument schedules the chain first. Count and data are re-read every pass.
struct CSoundResource;
class CAmbientEmitter;

class CPKList {                             // libpk list base (slots 5.. 0x00C2AD20..: forward to the implementation at +4)
public:
    virtual ~CPKList();                     // slot 0
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void Remove(void* item);        // slot 8 (0x00C2AD60)
    virtual void Add(void* item);           // slot 9 (0x00C2AD80)
    virtual void v10();
    virtual void* GetHead();                // slot 11 (0x00C2ADA0)
    void* m_pImpl;                          // +0x04
};

template <class T>
class PKCTRList : public CPKList {          // PKCTRList.h (vtable 0x012B7CE8 for the sound list)
public:
    T* GetFirst() { return (T*)GetHead(); }
    void RemoveItem(T* item) { Remove(item); }
    void AddItem(T* item) { Add(item); }
    unsigned char m_impl[0x28];             // +0x08
};

class CClock {
public:
    unsigned long GetTime() const;          // 0x00C2D0F0
};

struct CSoundSystem {
    unsigned char m_pad[0x70];
    CClock        m_clock;                  // +0x70
};

class CSoundBank {
public:
    CSoundSystem* m_pSystem;                // +0x00
    unsigned char m_pad04[0xcd - 4];
    bool          m_bMuted;                 // +0xcd
    void Release(CSoundResource* sound);    // 0x00C2A600: drop a reference, delete at zero
    CSoundResource* Acquire(unsigned long id);  // 0x00C2A3D0
};

struct CAmbientScene {
    CSoundBank* m_pBank;                    // +0x00
};

struct CAmbientZone {
    CAmbientScene* m_pScene;                // +0x00
};

class CAmbientEvent {
public:
    void*         m_vptr;
    unsigned long m_time;                   // +0x04 due time
    void Trigger(CAmbientEmitter* emitter);     // 0x00C61D10
    void Reschedule(CAmbientEmitter* emitter);  // 0x00C61CA0: pick the next time, re-add to the emitter's +0x20 list
};

template <class T>
struct CPtrArray {
    T*            m_data;                   // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    T GetAt(unsigned long index) const { return m_data[index]; }
};

struct CSoundSet {
    unsigned long m_header[2];
    CPtrArray<unsigned long> m_soundIds;    // +0x08
};

struct CAmbientDef {
    unsigned long m_words[2];
    CSoundSet*    m_pSounds;                // +0x08
};

class CAmbientEmitter {
public:
    CAmbientZone* m_pZone;                  // +0x00
    CAmbientDef*  m_pDef;                   // +0x04
    unsigned char m_pad08[0x20 - 8];
    PKCTRList<CAmbientEvent>  m_events;     // +0x20 time-ordered pending events
    PKCTRList<CSoundResource> m_sounds;     // +0x50 sounds held by the emitter
    unsigned char m_pad80[0x8c - 0x80];
    CSoundBank* GetBank() const { return m_pZone->m_pScene->m_pBank; }
    void AcquireSounds();
};

void CAmbientEmitter::AcquireSounds()
{
    CPtrArray<unsigned long>& ids = m_pDef->m_pSounds->m_soundIds;
    for (unsigned long i = 0; i < ids.m_count; ++i)
        m_sounds.AddItem(GetBank()->Acquire(ids.GetAt(i)));
}