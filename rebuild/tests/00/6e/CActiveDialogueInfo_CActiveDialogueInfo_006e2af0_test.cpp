#include <cstdio>

struct CBaseObject { int x; };
static int g_ctor = 0;
struct CBaseIntelligentPointer { virtual void v00() { } CBaseObject* PItem; CBaseIntelligentPointer() : PItem((CBaseObject*)1) { ++g_ctor; }
    void SetPItem(CBaseObject* item) { PItem = item; } };

struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { SetPItem(0); } };
enum EDialogueLayer { DIALOGUE_LAYER_NONE = 3 };
struct CActiveDialogueInfo { CIntelligentPointer ThingSpeaking; unsigned long SoundID; EDialogueLayer Layer; bool IsLayerHead; float OutputGain;
    CActiveDialogueInfo() : ThingSpeaking(), SoundID(0), Layer(DIALOGUE_LAYER_NONE), IsLayerHead(false), OutputGain(0.0f) { } };
int main()
{
    CActiveDialogueInfo d;
    if (g_ctor != 1 || d.ThingSpeaking.PItem != 0 || d.SoundID != 0 || d.Layer != 3 || d.IsLayerHead || d.OutputGain != 0.0f || sizeof(CActiveDialogueInfo) != 0x18) { std::printf("006e2af0_TEST FAIL\n"); return 1; }
    std::printf("006e2af0_TEST PASS\n"); return 0;
}