// CSoundEventManager::Play(long sound, long flags, const CPKVoiceParams* params) -- EH-framed: under a scoped lock over the library lock @ 0x0143BB14, the entry found out of line with (sound, flags, params, 0) (@ 0x00C12CD0) yields its +0xc word (the voice handle); -1 when nothing was found
// Leaf: ?Play@CSoundEventManager@@QAEJJJPBUCPKVoiceParams@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13140, oracle length 122 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; ...
// Naming (naming wave 21, DECISIONS_W20_RESOLVED / chunk 687): the play request CAmbientTrigger::Play 0x00C62530, CAmbientChannel::Update
//   0x00C621B0, the sound loop's Update 0x00C37840 and CAmbientEvent::Trigger 0x00C61D10 call with (sound, 0, &params) on the object at
//   CSoundSystem+0x38 -- the landed CSoundEventManager (ctor 0x00C11D00, vtable 0x012B2748; this body sits in its method range
//   0x00C10C90..0x00C13470 under the same library lock). The manifest label CTCHeroExperience::GetBlockParryReward is propagated.
//   Descriptive (libpk, no PDB); the lookup callee 0x00C12CD0 is named by its address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CPKVoiceParams;

struct CSoundEventEntry_00c13140 {
    unsigned char _pad[0xc];
    long m_handle;                         // +0xc
};

struct CSoundEventManager {
    CSoundEventEntry_00c13140* FindEntry_00c12cd0(long sound, long flags, const CPKVoiceParams* params, long mode);   // 0x00C12CD0
    long Play(long sound, long flags, const CPKVoiceParams* params);
};

long CSoundEventManager::Play(long sound, long flags, const CPKVoiceParams* params)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CSoundEventEntry_00c13140* entry = FindEntry_00c12cd0(sound, flags, params, 0);
    long handle = entry ? entry->m_handle : -1;
    return handle;
}
