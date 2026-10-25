// CBankProcessor::AddDriver(const SDriverRecord* record) -- look the driver record's sample up in the sample map (logging "Could not find resource number <n>" when it is missing); when the record carries play parameters (flag 4), fill a CSoundPlayParameters block from it, gated by the processor's options, create the driver through the host (CScriptHost) and add it to the driver map under the record's index
// Flags: /O2 /Oy
// Leaf: ?AddDriver@CBankProcessor@@QAEXPBUSDriverRecord@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E2B0, 455 bytes, __thiscall, `ret 4`.
// Naming (sweep1, 2026-10-25): was landed as `ResourceProcessor_00c2e2b0::Process` (owner unknown at the time; the
//   manifest's "C3DAnimationSequence_ProcessResourceChunk" is propagated). Descriptive (no PDB covers libpk). Chunk 436:
//   its only caller is the landed CBankProcessor::AddDrivers 0x00C2E480 (one call per record of the bank data's driver
//   list); `this` is the bank processor built by 0x00C2E0C0 (manager +4, load log +8, driver map +0x0c, sample map
//   +0x3c, options +0x74..+0x78).
// Callees: the sample map's Get(index) @ 0x00C3B160, the load log's AddMessage @ 0x00C307E0 (returns the new message's
//   CString), CString::operator<<(const char*) @ 0x00C0DEC0 / operator<<(unsigned long) @ 0x00C0DF10, the parameter
//   block's constructor @ 0x00C1BD10 (Init) and setters 0x00C1BB20..0x00C1BC70 (named as the landed
//   CSoundTemplateRecord::FillPlayParameters 0x00C35770 declares them: the same 1/127 volume constant 0x0129F780 and
//   0.01 pitch/pan constant 0x01230A5C feed them there and here), CScriptHost::CreateDriver(sample, params)
//   @ 0x00C0E870 and the driver map's Add(log, driver, index) @ 0x00C3AB50.
// Parameter types are the landed ones (they decide `xor ; mov cx` versus `movzx` for the word arguments).
struct CSoundPlayParameters {                   // the packed 0x14-byte play-parameter block
    unsigned data[5];
    CSoundPlayParameters();                     // 0x00C1BD10
    void SetLooped(bool);                       // 0x00C1BC10 (bit 0 of +2)
    void SetCategory(unsigned);                 // 0x00C1BC00 (+0x0c)
    void SetDelay(unsigned);                    // 0x00C1BBD0 (+0x04)
    void SetLoopEnd(unsigned);                  // 0x00C1BBA0 (+0x12)
    void SetLoopStart(unsigned);                // 0x00C1BB80 (+0x10)
    void SetPitch(float);                       // 0x00C1BB40 (+0x08)
    void SetPan(float);                         // 0x00C1BB60 (+0x01)
    void SetPriority(unsigned);                 // 0x00C1BBC0 (+0x0a)
    void SetFrequencyScale(float);              // 0x00C1BC70 (+0x0e)
    void SetVolume(float);                      // 0x00C1BB20 (+0x00)
    void SetFadeIn(unsigned short);             // 0x00C1BBE0 (+0x06)
    void SetGroup(unsigned short);              // 0x00C1BBF0 (+0x03)
    void SetStreamed(bool);                     // 0x00C1BC30 (bit 1 of +2)
    void SetPositional(bool);                   // 0x00C1BC50 (bit 2 of +2)
};

class CString {
public:
    char* m_pchData;
    CString& __fastcall operator<<(const char* text);       // 0x00C0DEC0
    CString& __fastcall operator<<(unsigned long value);    // 0x00C0DF10
};

struct CLoadLog { CString& AddMessage(); };                  // 0x00C307E0
struct CScriptHost { void* CreateDriver(void* sample, const CSoundPlayParameters& params); };     // 0x00C0E870
struct CSampleMap { void* Get(unsigned index); };           // 0x00C3B160
struct CDriverMap { void Add(CLoadLog* log, void* driver, unsigned index); };   // 0x00C3AB50

struct SDriverRecord {                          // one driver record of the bank data
    unsigned index, sample, unused;             // +0x00 / +0x04 / +0x08
    unsigned short driverGroup, category;       // +0x0c (read by CBankProcessor::ProcessAllDrivers) / +0x0e
    unsigned priority, delay;                   // +0x10 / +0x14
    unsigned short group, fadeIn, volume, pitch;   // +0x18 / +0x1a / +0x1c / +0x1e
    unsigned char flags, pad21;                 // +0x20: 1 streamed, 2 positional, 4 has parameters, 8 looped, 0x10 loop start, 0x20 loop end
    unsigned short pan;                         // +0x22
    unsigned loopStart, loopEnd;                // +0x24 / +0x28
    int frequencyDivisor;                       // +0x2c (-1: none)
};

extern const float Scale_01230a5c, AngleScale_0129f780;   // 0.01f, 1/127

struct CBankProcessor {
    unsigned pad0;                              // +0x00 the bank data
    CScriptHost* manager;                       // +0x04 (the sound bank host, agent431's CScriptHost)
    CLoadLog* log;                              // +0x08
    char drivers[48];                           // +0x0c the driver map
    char samples[56];                           // +0x3c the sample map
    bool applyFadeIn, applyGroup, pad76, allowStreamed, allowPositional;   // +0x74..+0x78
    void AddDriver(const SDriverRecord* record);
};

void CBankProcessor::AddDriver(const SDriverRecord* record)
{
    void* sample = ((CSampleMap*)samples)->Get(record->sample);
    if (!sample) { log->AddMessage() << "Could not find resource number " << record->sample; return; }
    if (record->flags & 4) {
        CSoundPlayParameters p;
        p.SetLooped((record->flags >> 3) & 1);
        p.SetCategory(record->category);
        p.SetDelay(record->delay);
        if (record->flags & 32) p.SetLoopEnd(record->loopEnd);
        if (record->flags & 16) p.SetLoopStart(record->loopStart);
        p.SetPitch(record->pitch * Scale_01230a5c);
        p.SetPan(record->pan * Scale_01230a5c);
        if (record->priority) p.SetPriority(record->priority);
        if (record->frequencyDivisor != -1) p.SetFrequencyScale(1.0f / record->frequencyDivisor);
        p.SetVolume(record->volume * AngleScale_0129f780);
        if (applyFadeIn) p.SetFadeIn(record->fadeIn);
        if (applyGroup) p.SetGroup(record->group);
        if (allowStreamed && (record->flags & 1)) p.SetStreamed(true);
        if (allowPositional && (record->flags & 2)) p.SetPositional(true);
        void* driver = manager->CreateDriver(sample, p);
        ((CDriverMap*)drivers)->Add(log, driver, record->index);
    }
}
