// CAmbientSource::CreateChannels() -- create one channel per channel sound id of the source's definition (definition +8 -> data +0x08 pointer array) and add each to the source's channel list (+0x38)
// Leaf: ?CreateChannels@CAmbientSource@@QAEXXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C470E0, 133 bytes (+ int3 padding), __fastcall `ret`, EH-framed (handler
//   0x01215630: state 0 = the `new` block while its constructor runs).
// Library: libpk ambient sound (no PDB/RTTI): descriptive names. The source is built by 0x00C47340 (owner set +0,
//   definition +4, trigger list +0x08, channel list +0x38; it adds itself to the set's +0x28 list, then runs
//   CreateTriggers 0x00C47050 and CreateChannels 0x00C470E0) and updated by Update 0x00C47300.
//   The manifest's "CGame_CreateSolidToThingFilters" is propagated.
// Callees: operator new 0x00BFEA1A (8 bytes), CAmbientChannel::CAmbientChannel 0x00C62120 ({id, -1}), the list's
//   virtual Add (slot 9).
// Shape: as CreateTriggers 0x00C47050: the by-value GetAt inline gives retail's ecx reuse; the list address is
//   hoisted into ebx.
void* __cdecl operator new(unsigned int size);   // 0x00BFEA1A
void __cdecl operator delete(void* p) throw();  // 0x00BFE9BC (the unwind funclet)

class CAmbientSource;

class CPKList {                             // libpk list base
public:
    virtual ~CPKList();                     // slot 0
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void Remove(void* item);        // slot 8
    virtual void Add(void* item);           // slot 9
    virtual void v10();
    virtual void* GetHead();                // slot 11
    void* m_pImpl;                          // +0x04
};

template <class T>
class PKCTRList : public CPKList {          // PKCTRList.h (vtables 0x012B7F88 triggers, 0x012B7FD0 channels)
public:
    T* GetFirst() { return (T*)GetHead(); }
    void RemoveItem(T* item) { Remove(item); }
    void AddItem(T* item) { Add(item); }
    unsigned char m_impl[0x28];             // +0x08
};

class CAmbientTrigger {                     // 0x18 bytes
public:
    unsigned long m_soundId;                // +0x00
    unsigned long m_time;                   // +0x04 due time
    long          m_handle;                 // +0x08 (-1)
    unsigned long m_words[3];               // +0x0c
    CAmbientTrigger(unsigned long soundId); // 0x00C62320
    void Fire(CAmbientSource* source);      // 0x00C62530
    void Schedule(CAmbientSource* source);  // 0x00C62400: pick the next due time, add to the source's +0x08 list
};

class CAmbientChannel {                     // 8 bytes
public:
    unsigned long m_soundId;                // +0x00
    long          m_handle;                 // +0x04 (-1)
    CAmbientChannel(unsigned long soundId); // 0x00C62120
};

class CClock {
public:
    unsigned long GetTime() const;          // 0x00C2D0F0
};

struct CSoundSystem {
    unsigned char m_pad[0x70];
    CClock        m_clock;                  // +0x70
};

struct CSoundBank {
    CSoundSystem* m_pSystem;                // +0x00
};

struct CAmbientSet {
    unsigned long m_unknown0;               // +0x00
    CSoundBank*   m_pBank;                  // +0x04
    unsigned char m_pad08[0x24 - 8];
    bool          m_bActive;                // +0x24
};

template <class T>
struct CPtrArray {
    T*            m_data;                   // +0
    unsigned long m_capacity;               // +4
    unsigned long m_count;                  // +8
    T GetAt(unsigned long index) const { return m_data[index]; }
};

struct CAmbientSourceData {
    unsigned long m_header[2];
    CPtrArray<unsigned long> m_channelIds;  // +0x08
    CPtrArray<unsigned long> m_triggerIds;  // +0x14
};

struct CAmbientSourceDef {
    unsigned long       m_words[2];
    CAmbientSourceData* m_pData;            // +0x08
    bool IsFinished() const;                // 0x00C384E0 (compares the floats at +0x20 / +0x24)
};

class CAmbientSource {
public:
    CAmbientSet*                m_pSet;     // +0x00
    CAmbientSourceDef*          m_pDef;     // +0x04
    PKCTRList<CAmbientTrigger>  m_triggers; // +0x08 time-ordered pending triggers
    PKCTRList<CAmbientChannel>  m_channels; // +0x38
    void UpdateChannels();                  // 0x00C47210
    void UpdateTriggers();                  // 0x00C47260
    void Finish();                          // 0x00C472B0
    void CreateChannels();
};

void CAmbientSource::CreateChannels()
{
    CPtrArray<unsigned long>& ids = m_pDef->m_pData->m_channelIds;
    for (unsigned long i = 0; i < ids.m_count; ++i)
        m_channels.AddItem(new CAmbientChannel(ids.GetAt(i)));
}