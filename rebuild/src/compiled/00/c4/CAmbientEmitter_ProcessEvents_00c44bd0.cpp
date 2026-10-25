// CAmbientEmitter::ProcessEvents() -- fire the emitter's due events: while the first pending event (+0x20 list) is due by the sound system clock, remove it, trigger it unless the bank is muted (+0xCD), and let it reschedule itself
// Leaf: ?ProcessEvents@CAmbientEmitter@@QAEXXZ
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C44BD0, 100 bytes (+ int3 padding), __fastcall `ret`.
// Library: libpk ambient sound (no PDB/RTTI): descriptive names. Only caller the tail `jmp` of the emitter's update
//   0x00C44E80. The manifest's "CThingPlayerCreature_ProcessMovementBands" is propagated.
// Callees: the list's GetHead (slot 11) / Remove (slot 8), the clock's GetTime 0x00C2D0F0 (system +0x70; system =
//   bank +0), the event's Trigger 0x00C61D10 and Reschedule 0x00C61CA0.
// Shape: needs /Oa (assume no aliasing): without it VC7.1 rotates the registers from the Remove call on (vtable in
//   eax, the muted chain ecx/edx/eax -> cl); with /Oa the vtable goes to edx and the muted byte to al as in retail.
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
    void ProcessEvents();
};

void CAmbientEmitter::ProcessEvents()
{
    CAmbientEvent* event;
    while ((event = m_events.GetFirst()) != 0 && event->m_time <= GetBank()->m_pSystem->m_clock.GetTime()) {
        m_events.RemoveItem(event);
        if (!GetBank()->m_bMuted)
            event->Trigger(this);
        event->Reschedule(this);
    }
}