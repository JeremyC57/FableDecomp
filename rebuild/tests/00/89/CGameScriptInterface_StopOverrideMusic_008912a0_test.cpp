#include <cstdio>
#if !defined(_MSC_VER)
#  define __fastcall
#  define __cdecl
#endif
#include <cstring>

struct CGSIStopMusicCodeSection
{
    unsigned char pad00[0x0c];
    unsigned char useAlternate;
    unsigned char pad0d[0x1b];
    float primaryValue;
    unsigned char pad2c[0xbc];
    float alternateValue;
};

struct CGSIStopMusicEvent;

struct CGSIStopMusicInner
{
    unsigned char pad00[0x6c];
    CGSIStopMusicEvent* event;
};

struct CGSIStopMusicEmitter
{
    virtual void Slot00();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0c();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1c();
    virtual void Slot20();
    virtual void Slot24();
    virtual void Slot28();
    virtual void Slot2c();
    virtual void Slot30();
    virtual void Slot34();
    virtual void Slot38();
    virtual void Slot3c();
    virtual void Slot40();
    virtual void Slot44();
    virtual void Slot48();
    virtual void Slot4c();
    virtual void Slot50();
    virtual void Slot54();
    virtual void Slot58();
    virtual void Slot5c();
    virtual void Slot60();
    virtual void Slot64();
    virtual void Slot68();
    virtual void Slot6c();
    virtual void Slot70();
    virtual void Slot74();
    virtual void Slot78();
    virtual void Slot7c();
    virtual void Slot80();
    virtual void Slot84();
    virtual void Slot88();
    virtual void Slot8c();
    virtual void Slot90();
    virtual void Slot94();
    virtual void Slot98();
    virtual void Slot9c();
    virtual void Slota0();
    virtual void Slota4();
    virtual void Slota8();
    virtual void Slotac();
    virtual void Slotb0();
    virtual void Slotb4();
    virtual void Stop(unsigned long command, float value, float fade);
};

struct CGameScriptInterface
{
    unsigned char pad00[4];
    CGSIStopMusicInner* inner;

    void StopOverrideMusic(bool enabled) const;
};

extern CGSIStopMusicEmitter* CGSIStopMusic_Emitter;
extern CGSIStopMusicCodeSection* __cdecl CGSIStopMusic_GetCodeSection();
extern void __fastcall CGSIStopMusic_StopEvent(CGSIStopMusicEvent* event);
extern void __fastcall CGSIStopMusic_ResumeEvent(CGSIStopMusicEvent* event);

void CGameScriptInterface::StopOverrideMusic(bool enabled) const
{
    if (enabled)
    {
        if (CGSIStopMusic_Emitter != 0)
        {
            CGSIStopMusicCodeSection* section =
                CGSIStopMusic_GetCodeSection();
            float value;
            if (section->useAlternate != 0)
                value = section->alternateValue;
            else
                value = section->primaryValue;
            CGSIStopMusic_Emitter->Stop(0x100, value, 500.0f);
        }
        CGSIStopMusic_StopEvent(inner->event);
        return;
    }
    CGSIStopMusic_ResumeEvent(inner->event);
}

CGSIStopMusicEmitter* CGSIStopMusic_Emitter;
static CGSIStopMusicCodeSection g_section;
static int g_stopEvents;
static int g_resumeEvents;
static unsigned long g_command;
static float g_value;
static float g_fade;

CGSIStopMusicCodeSection* __cdecl CGSIStopMusic_GetCodeSection()
{
    return &g_section;
}

void __fastcall CGSIStopMusic_StopEvent(CGSIStopMusicEvent*)
{
    ++g_stopEvents;
}

void __fastcall CGSIStopMusic_ResumeEvent(CGSIStopMusicEvent*)
{
    ++g_resumeEvents;
}

void CGSIStopMusicEmitter::Slot00() { }
void CGSIStopMusicEmitter::Slot04() { }
void CGSIStopMusicEmitter::Slot08() { }
void CGSIStopMusicEmitter::Slot0c() { }
void CGSIStopMusicEmitter::Slot10() { }
void CGSIStopMusicEmitter::Slot14() { }
void CGSIStopMusicEmitter::Slot18() { }
void CGSIStopMusicEmitter::Slot1c() { }
void CGSIStopMusicEmitter::Slot20() { }
void CGSIStopMusicEmitter::Slot24() { }
void CGSIStopMusicEmitter::Slot28() { }
void CGSIStopMusicEmitter::Slot2c() { }
void CGSIStopMusicEmitter::Slot30() { }
void CGSIStopMusicEmitter::Slot34() { }
void CGSIStopMusicEmitter::Slot38() { }
void CGSIStopMusicEmitter::Slot3c() { }
void CGSIStopMusicEmitter::Slot40() { }
void CGSIStopMusicEmitter::Slot44() { }
void CGSIStopMusicEmitter::Slot48() { }
void CGSIStopMusicEmitter::Slot4c() { }
void CGSIStopMusicEmitter::Slot50() { }
void CGSIStopMusicEmitter::Slot54() { }
void CGSIStopMusicEmitter::Slot58() { }
void CGSIStopMusicEmitter::Slot5c() { }
void CGSIStopMusicEmitter::Slot60() { }
void CGSIStopMusicEmitter::Slot64() { }
void CGSIStopMusicEmitter::Slot68() { }
void CGSIStopMusicEmitter::Slot6c() { }
void CGSIStopMusicEmitter::Slot70() { }
void CGSIStopMusicEmitter::Slot74() { }
void CGSIStopMusicEmitter::Slot78() { }
void CGSIStopMusicEmitter::Slot7c() { }
void CGSIStopMusicEmitter::Slot80() { }
void CGSIStopMusicEmitter::Slot84() { }
void CGSIStopMusicEmitter::Slot88() { }
void CGSIStopMusicEmitter::Slot8c() { }
void CGSIStopMusicEmitter::Slot90() { }
void CGSIStopMusicEmitter::Slot94() { }
void CGSIStopMusicEmitter::Slot98() { }
void CGSIStopMusicEmitter::Slot9c() { }
void CGSIStopMusicEmitter::Slota0() { }
void CGSIStopMusicEmitter::Slota4() { }
void CGSIStopMusicEmitter::Slota8() { }
void CGSIStopMusicEmitter::Slotac() { }
void CGSIStopMusicEmitter::Slotb0() { }
void CGSIStopMusicEmitter::Slotb4() { }
void CGSIStopMusicEmitter::Stop(unsigned long, float, float) { }
struct CRecordingEmitter : CGSIStopMusicEmitter
{
    void Stop(unsigned long command, float value, float fade)
    {
        g_command = command;
        g_value = value;
        g_fade = fade;
    }
};

int main()
{
    CRecordingEmitter emitterObject;
    CGSIStopMusicInner inner;
    CGameScriptInterface self;
    std::memset(&inner, 0, sizeof(inner));
    std::memset(&self, 0, sizeof(self));
    self.inner = &inner;

    CGSIStopMusic_Emitter = 0;
    self.StopOverrideMusic(true);
    if (g_stopEvents != 1 || g_resumeEvents != 0)
        return 1;

    CGSIStopMusic_Emitter = &emitterObject;
    std::memset(&g_section, 0, sizeof(g_section));
    g_section.primaryValue = 0.75f;
    self.StopOverrideMusic(true);
    if (g_command != 0x100 || g_value != 0.75f || g_fade != 500.0f)
        return 2;

    g_section.useAlternate = 1;
    g_section.alternateValue = 0.25f;
    self.StopOverrideMusic(true);
    if (g_value != 0.25f)
        return 3;

    self.StopOverrideMusic(false);
    if (g_stopEvents != 3 || g_resumeEvents != 1)
        return 4;

    std::printf("FSE2_008912a0_TEST PASS\n");
    return 0;
}
