// Standalone exact selected function body; external native operations are controlled fixtures.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct PKCCriticalSection {};
class PKCProtectionInstance { public: PKCCriticalSection* m_pSection; unsigned long m_tick; PKCProtectionInstance(PKCCriticalSection*); ~PKCProtectionInstance(); };
PKCCriticalSection LHAudioCriticalSection;
// Native LHAudio types; descriptive spellings, see NAMING_EVIDENCE.md.
#ifndef FABLE_RETAIL_LHAUDIO_EVENT_POSITION_H
#define FABLE_RETAIL_LHAUDIO_EVENT_POSITION_H
class CSoundEventManager;
struct CSoundPositionVector { float X, Y, Z; };
struct CSoundPosition {
    CSoundPositionVector Position;
    bool Relative;
    CSoundPosition(); // 00c20390
};
class CSoundEvent {
public:
    virtual void UnresolvedSlot0(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot1(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot2(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot3(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot4(); // Uncalled signature is not recovered.
    virtual void SetPosition(CSoundEventManager*, const CSoundPosition*); // 00c212f0
    virtual void UnresolvedSlot6(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot7(); // Uncalled signature is not recovered.
    virtual bool IsPositionRelative(CSoundEventManager*); // 00c21330
    virtual void UnresolvedSlot9(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot10(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot11(); // Uncalled signature is not recovered.
private:
    unsigned char UnmodelledMembers[0xb0];
};
class CSoundEventSet {
    unsigned char UnmodelledMembers[0x788];
public:
    CSoundEvent* FindInChildThenSelf(long); // 00c21e20
};
class CSoundEventManager {
public:
    virtual void UnresolvedSlot0(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot1(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot2(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot3(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot4(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot5(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot6(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot7(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot8(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot9(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot10(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot11(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot12(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot13(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot14(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot15(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot16(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot17(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot18(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot19(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot20(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot21(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot22(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot23(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot24(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot25(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot26(); // Uncalled signature is not recovered.
    virtual void SetEventPosition(long key, const CSoundPositionVector& position); // 00c11520
    virtual void UnresolvedSlot28(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot29(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot30(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot31(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot32(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot33(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot34(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot35(); // Uncalled signature is not recovered.
    virtual void UnresolvedSlot36(); // Uncalled signature is not recovered.
private:
    unsigned char UnmodelledPrefix[0x28];
    CSoundEventSet EventSet; // +0x2c, complete native embedded extent
    unsigned char UnmodelledTail[0xdc]; // +0x7b4 through +0x88f
};
#endif

void CSoundEventManager::SetEventPosition(long key, const CSoundPositionVector& position)
{
    PKCProtectionInstance lock(&LHAudioCriticalSection);
    CSoundEvent* entry = EventSet.FindInChildThenSelf(key);
    if (entry) {
        CSoundPosition record;
        record.Relative = entry->IsPositionRelative(this);
        record.Position = position;
        entry->SetPosition(this, &record);
    }
}

static int stage, found, mode, throw_stage, callback_count, count;
static CSoundEventManager* expected_manager;
static CSoundEvent* expected_entry;
static long expected_key;
static CSoundPositionVector expected_position;
static void check(bool b) { if(!b) { fprintf(stderr,"failure case %d stage %d\n",count,stage); exit(1); } }
PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection* p) : m_pSection(p),m_tick(0) { check(stage==0 && p==&LHAudioCriticalSection); stage=1; }
PKCProtectionInstance::~PKCProtectionInstance() { check(stage>=1 && stage<=4); stage=5; }
CSoundPosition::CSoundPosition() { check(stage==2); Position.X=Position.Y=Position.Z=0; Relative=false; stage=3; if(throw_stage==3) throw 3; }
CSoundEvent* CSoundEventSet::FindInChildThenSelf(long key) { check(stage==1 && key==expected_key); stage=2; if(throw_stage==2) throw 2; return found?expected_entry:0; }
bool CSoundEvent::IsPositionRelative(CSoundEventManager* p) { check(stage==3 && p==expected_manager); if(throw_stage==4) throw 4; return mode!=0; }
void CSoundEvent::SetPosition(CSoundEventManager* p,const CSoundPosition* v) { check(stage==3 && p==expected_manager); check(v->Relative==(mode!=0)); check(v->Position.X==expected_position.X && v->Position.Y==expected_position.Y && v->Position.Z==expected_position.Z); ++callback_count; stage=4; if(throw_stage==5) throw 5; }
void CSoundEvent::UnresolvedSlot0() { check(false); }
void CSoundEvent::UnresolvedSlot1() { check(false); }
void CSoundEvent::UnresolvedSlot2() { check(false); }
void CSoundEvent::UnresolvedSlot3() { check(false); }
void CSoundEvent::UnresolvedSlot4() { check(false); }
void CSoundEvent::UnresolvedSlot6() { check(false); }
void CSoundEvent::UnresolvedSlot7() { check(false); }
void CSoundEvent::UnresolvedSlot9() { check(false); }
void CSoundEvent::UnresolvedSlot10() { check(false); }
void CSoundEvent::UnresolvedSlot11() { check(false); }
void CSoundEventManager::UnresolvedSlot0() { check(false); }
void CSoundEventManager::UnresolvedSlot1() { check(false); }
void CSoundEventManager::UnresolvedSlot2() { check(false); }
void CSoundEventManager::UnresolvedSlot3() { check(false); }
void CSoundEventManager::UnresolvedSlot4() { check(false); }
void CSoundEventManager::UnresolvedSlot5() { check(false); }
void CSoundEventManager::UnresolvedSlot6() { check(false); }
void CSoundEventManager::UnresolvedSlot7() { check(false); }
void CSoundEventManager::UnresolvedSlot8() { check(false); }
void CSoundEventManager::UnresolvedSlot9() { check(false); }
void CSoundEventManager::UnresolvedSlot10() { check(false); }
void CSoundEventManager::UnresolvedSlot11() { check(false); }
void CSoundEventManager::UnresolvedSlot12() { check(false); }
void CSoundEventManager::UnresolvedSlot13() { check(false); }
void CSoundEventManager::UnresolvedSlot14() { check(false); }
void CSoundEventManager::UnresolvedSlot15() { check(false); }
void CSoundEventManager::UnresolvedSlot16() { check(false); }
void CSoundEventManager::UnresolvedSlot17() { check(false); }
void CSoundEventManager::UnresolvedSlot18() { check(false); }
void CSoundEventManager::UnresolvedSlot19() { check(false); }
void CSoundEventManager::UnresolvedSlot20() { check(false); }
void CSoundEventManager::UnresolvedSlot21() { check(false); }
void CSoundEventManager::UnresolvedSlot22() { check(false); }
void CSoundEventManager::UnresolvedSlot23() { check(false); }
void CSoundEventManager::UnresolvedSlot24() { check(false); }
void CSoundEventManager::UnresolvedSlot25() { check(false); }
void CSoundEventManager::UnresolvedSlot26() { check(false); }
void CSoundEventManager::UnresolvedSlot28() { check(false); }
void CSoundEventManager::UnresolvedSlot29() { check(false); }
void CSoundEventManager::UnresolvedSlot30() { check(false); }
void CSoundEventManager::UnresolvedSlot31() { check(false); }
void CSoundEventManager::UnresolvedSlot32() { check(false); }
void CSoundEventManager::UnresolvedSlot33() { check(false); }
void CSoundEventManager::UnresolvedSlot34() { check(false); }
void CSoundEventManager::UnresolvedSlot35() { check(false); }
void CSoundEventManager::UnresolvedSlot36() { check(false); }
int main() {
    CSoundEventManager manager;
    CSoundEvent entry;
    expected_manager=&manager; expected_entry=&entry;
    const float values[]={-8.0f,-1.5f,0.0f,0.25f,2.0f,32.0f};
    for(int hit=0;hit<2;++hit) for(int relative=0;relative<2;++relative)
    for(int xyz=0;xyz<6;++xyz) for(int failure=0;failure<
#ifdef FABLE_TEST_EXCEPTIONS
        5
#else
        1
#endif
        ;++failure) {
        stage=0; found=hit; mode=relative; throw_stage=failure?failure+1:0;
        callback_count=0; expected_key=xyz-3;
        expected_position.X=values[xyz]; expected_position.Y=values[(xyz+2)%6]; expected_position.Z=values[(xyz+4)%6];
        int caught=0;
        try { manager.SetEventPosition(expected_key,expected_position); } catch(int x) { caught=x; }
        check(stage==5);
        const int expected_throw=throw_stage==2?2:(hit?throw_stage:0);
        check(caught==expected_throw);
        check(callback_count==(hit&&(throw_stage==0||throw_stage==5)?1:0));
        ++count;
    }
    puts("00c11520_TEST PASS");
    return 0;
}
