#include <cstdio>

struct CBaseObject { int x; };
static int g_ctor = 0;
struct CBaseIntelligentPointer { virtual void v00() { } CBaseObject* PItem; CBaseIntelligentPointer() : PItem((CBaseObject*)1) { ++g_ctor; }
    void SetPItem(CBaseObject* item) { PItem = item; } };

struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); } };
enum EDialogueLayer { DIALOGUE_LAYER_NONE = 3, DIALOGUE_LAYER_TWO = 2 };
struct CActiveDialogueInfo { CIntelligentPointer ThingSpeaking; unsigned long SoundID; EDialogueLayer Layer; bool IsLayerHead; float OutputGain;
    CActiveDialogueInfo(CBaseObject* thingSpeaking, unsigned long soundID, EDialogueLayer layer, bool isLayerHead)
        : ThingSpeaking(thingSpeaking), SoundID(soundID), Layer(layer), IsLayerHead(isLayerHead), OutputGain(0.0f) { } };
int main()
{
    CBaseObject o; CActiveDialogueInfo d(&o, 77, DIALOGUE_LAYER_TWO, true);
    if (g_ctor != 1 || d.ThingSpeaking.PItem != &o || d.SoundID != 77 || d.Layer != 2 || !d.IsLayerHead || d.OutputGain != 0.0f) { std::printf("006e2b30_TEST FAIL\n"); return 1; }
    std::printf("006e2b30_TEST PASS\n"); return 0;
}
