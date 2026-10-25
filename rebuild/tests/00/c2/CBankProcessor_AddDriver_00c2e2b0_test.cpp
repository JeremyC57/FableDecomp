#include <assert.h>
#include <stdio.h>
#include <string.h>
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

const float Scale_01230a5c=0.00999999977648258209228515625f,AngleScale_0129f780=0.0078740157186985015869140625f;
static float pitchSeen,panSeen,freqSeen,volumeSeen;static unsigned mask;static int creates,adds,errors;static bool found=true;static CString logText;
CSoundPlayParameters::CSoundPlayParameters(){memset(data,0,sizeof(data));}
void CSoundPlayParameters::SetLooped(bool v){mask|=1U<<0;data[0]=(unsigned)v;}
void CSoundPlayParameters::SetCategory(unsigned v){mask|=1U<<1;data[0]=v;}
void CSoundPlayParameters::SetDelay(unsigned v){mask|=1U<<2;data[0]=v;}
void CSoundPlayParameters::SetLoopEnd(unsigned v){mask|=1U<<3;data[0]=v;}
void CSoundPlayParameters::SetLoopStart(unsigned v){mask|=1U<<4;data[0]=v;}
void CSoundPlayParameters::SetPitch(float v){pitchSeen=v;mask|=1U<<5;}
void CSoundPlayParameters::SetPan(float v){panSeen=v;mask|=1U<<6;}
void CSoundPlayParameters::SetPriority(unsigned v){mask|=1U<<7;data[0]=v;}
void CSoundPlayParameters::SetFrequencyScale(float v){freqSeen=v;mask|=1U<<8;}
void CSoundPlayParameters::SetVolume(float v){volumeSeen=v;mask|=1U<<9;}
void CSoundPlayParameters::SetFadeIn(unsigned short v){mask|=1U<<10;data[0]=v;}
void CSoundPlayParameters::SetGroup(unsigned short v){mask|=1U<<11;data[0]=v;}
void CSoundPlayParameters::SetStreamed(bool v){mask|=1U<<12;data[0]=(unsigned)v;}
void CSoundPlayParameters::SetPositional(bool v){mask|=1U<<13;data[0]=(unsigned)v;}
CString& CLoadLog::AddMessage(){++errors;return logText;}
CString& __fastcall CString::operator<<(const char* s){assert(s && *s);return *this;}
CString& __fastcall CString::operator<<(unsigned long n){assert(n==17);return *this;}
void* CSampleMap::Get(unsigned){return found?this:0;}
void* CScriptHost::CreateDriver(void* p,const CSoundPlayParameters&){++creates;return p;}
void CDriverMap::Add(CLoadLog*,void* p,unsigned id){assert(p && id==9);++adds;}
static bool Near(float a,float b){float d=a-b;return d<0.0001f && d>-0.0001f;}
int main(){
 CBankProcessor self;memset(&self,0,sizeof(self));CScriptHost manager;CLoadLog log;self.manager=&manager;self.log=&log;
 SDriverRecord r;memset(&r,0,sizeof(r));r.sample=17;r.index=9;r.frequencyDivisor=-1;
 self.AddDriver(&r);if(creates!=0){puts("00c2e2b0_TEST FAIL");return 1;}
 found=false;self.AddDriver(&r);if(errors!=1){puts("00c2e2b0_TEST FAIL");return 1;}
 found=true;r.pitch=100;r.pan=200;r.volume=127;r.flags=63;r.priority=1;r.frequencyDivisor=2;
 self.applyFadeIn=self.applyGroup=self.allowStreamed=self.allowPositional=true;self.AddDriver(&r);
 if(!(creates==1 && adds==1 && mask==16383 && Near(pitchSeen,1.0f) && Near(panSeen,2.0f) && Near(freqSeen,0.5f) && Near(volumeSeen,1.0f))){puts("00c2e2b0_TEST FAIL");return 1;}
 puts("00c2e2b0_TEST PASS");return 0;}
