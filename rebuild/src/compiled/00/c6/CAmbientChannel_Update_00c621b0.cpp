// CAmbientChannel::Update(CAmbientSource* source) -- keep one ambient channel's looping voice in step with its source: when the source is silent release the voice; when no voice plays start one (the 0x50-byte play-parameter block with the source's volume and owner id, priority 1, registered on the source's layer, placed at the owner's position) and keep its handle; otherwise move the playing voice to the owner's position and set its volume
// Leaf: ?Update@CAmbientChannel@@QAEXPAVCAmbientSource@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C621B0, 363 bytes, thiscall `ret 4`, frameless (0x5c-byte frame).
// Identity (descriptive; libpk ambient sound, no PDB / RTTI): `this` is the 8-byte CAmbientChannel of the landed ctor
//   0x00C62120 ({sound id +0, handle +4 = -1}); the argument is the CAmbientSource (GetVolume 0x00C46F40, owner +0 with
//   id +0xC and position +0x14, registry layer +4). The only caller is the source's per-channel functor 0x00C46F60
//   (`channel->Update(m_source)`). The sibling of CAmbientTrigger::Play 0x00C62530 (same parameter block and callees).
//   The catalog label CItem_ProcessPendingEffects is propagated and wrong.
// Callees (landed bindings, reloc_bindings.tsv): CAmbientSource::GetVolume 0x00C46F40; 0x00C62180
//   `CHandleOwner_00c62180::ReleaseHandle(SContext_00c62500*)` (releases the handle at +4: this channel's stop; caller
//   0x00C471C0 binds it); 0x00C62150 CAmbientChannel::GetHandleTable(CAmbientSource*) (the source's sound system: retail
//   passes this in ecx, so it is a thiscall member that ignores this, as CAmbientTrigger::GetHandleTable 0x00C62440 --
//   the landed `__stdcall GetHandleTable_00c62150(long)` / CTools::GetWorld spellings are wrong, NAME_FIXES.tsv); member
//   ctors 0x00C203A0 / 0x00C20390 and setters Set9_00c20420 (float) / SetFilter_00c20440 (long) of the landed
//   CSoundPlayParams_00c37840 model; CRegistryLayer_00c383d0::Add 0x00C383D0 (item, false, true);
//   CSoundSystem_00c37840::Play_00c13140 0x00C13140; the sound system's virtual slots 27 (+0x6C, position) and 25
//   (+0x64, volume).
// Shape: the volume is fetched once (homed in the dead argument slot); `if (volume <= 0.0f)` gives `test ah,41h ; jp`
//   over the release; the moved-voice position is a zeroed C3DVector local filled member by member from the owner
//   (owner pointer in eax, `[eax+14h]`..; a whole-struct assignment gives `add ecx,14h` + block copy, as the play arm's
//   params.m_links.m_position copy does). Grades: 3 (2nd: GetHandleTable as a thiscall member; 3rd: memberwise copy).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
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
    void Set9_00c20420(float value);                    // 0x00C20420
    void SetFilter_00c20440(long value);                // 0x00C20440
};
struct CSoundSystem_00c37840 {
    long Play_00c13140(long soundID, long flags, const CSoundPlayParams_00c37840* params);   // 0x00C13140
};
class CHandleTable { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021();
    virtual void Release(long handle);                  // slot 22 (+0x58)
    virtual void v023(); virtual void v024();
    virtual void SetVolume(long handle, float volume);  // slot 25 (+0x64)
    virtual void v026();
    virtual void SetPosition(long handle, const C3DVector* position);   // slot 27 (+0x6C)
};
struct SContext_00c62500;
struct CHandleOwner_00c62180 {
    unsigned char _pad[0x4];
    long          m_handle;                             // +0x4
    void ReleaseHandle(SContext_00c62500* context);     // 0x00C62180
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
class CAmbientChannel {
public:
    unsigned long m_soundId;                            // +0x00
    long          m_handle;                             // +0x04 (-1: not playing)
    CHandleTable* GetHandleTable(CAmbientSource* source);   // 0x00C62150
    void Update(CAmbientSource* source);
};

void CAmbientChannel::Update(CAmbientSource* source)
{
    float volume = source->GetVolume();
    if (volume <= 0.0f) {
        ((CHandleOwner_00c62180*)this)->ReleaseHandle((SContext_00c62500*)source);
        return;
    }
    long handle = m_handle;
    if (handle < 0) {
        CSoundPlayParams_00c37840 params;
        params.m_volume = volume;
        params.m_loopCount = source->m_pOwner->m_id;
        params.m_priority = 1;
        params.Set9_00c20420(1.0f);
        params.SetFilter_00c20440(-1);
        source->m_pLayer->Add((CRegisteredItem*)&params, false, true);
        params.m_links.m_position = source->m_pOwner->m_position;
        params.m_links.m_flag = 0;
        m_handle = ((CSoundSystem_00c37840*)GetHandleTable(source))->Play_00c13140(m_soundId, 0, &params);
    } else {
        C3DVector position(0.0f, 0.0f, 0.0f);
        position.X = source->m_pOwner->m_position.X;
        position.Y = source->m_pOwner->m_position.Y;
        position.Z = source->m_pOwner->m_position.Z;
        GetHandleTable(source)->SetPosition(handle, &position);
        GetHandleTable(source)->SetVolume(m_handle, volume);
    }
}