// CAmbientEmitter::ReleaseSounds() -- empty the emitter's sound list (+0x50): take the first sound, remove it from the list and hand it back to the sound bank (one reference dropped, deleted at zero), until the list is empty
// Leaf: ?ReleaseSounds@CAmbientEmitter@@QAEXXZ
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44B80, 59 bytes (+ int3 padding), __fastcall `ret`.
// Library: libpk ambient sound (no PDB/RTTI): descriptive names (the TU prints "Ambient cube:-" / "   Apex ("). Only
//   caller the emitter's destructor 0x00C44F80 (deleted by the API at 0x00C362AF; built by `new` 0x8C bytes at
//   0x00C361BE, ctor 0x00C44EB0). The manifest's "CWorldPartition_RenderComponents" is propagated.
// Callees: the list's virtual GetHead (slot 11) / Remove (slot 8) -- VC7.1 dispatches through the member's vtable --
//   and the bank's Release 0x00C2A600 (bank = zone +0 -> scene +0 -> bank +0).
// Shape: matches with or without /Oa; /Oa is the TU's flag (its sibling 0x00C44BD0 needs it).
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
    void ReleaseSounds();
};

void CAmbientEmitter::ReleaseSounds()
{
    CSoundResource* sound;
    while ((sound = m_sounds.GetFirst()) != 0) {
        m_sounds.RemoveItem(sound);
        GetBank()->Release(sound);
    }
}