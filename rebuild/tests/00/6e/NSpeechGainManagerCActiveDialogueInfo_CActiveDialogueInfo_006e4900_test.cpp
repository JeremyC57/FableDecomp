// Behaviour test for 0x006E4900 NSpeechGainManager::CActiveDialogueInfo copy constructor.
#include <cstdio>
#include <cstring>

class CBaseObject {
public:
    int Id;
};
class CBaseIntelligentPointer {
public:
    CBaseObject* PData;
    static int s_sets;
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; ++s_sets; }
};
int CBaseIntelligentPointer::s_sets = 0;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() {}
    explicit CIntelligentPointer(CBaseObject* p) { SetPItem(p); }
    CIntelligentPointer(const CIntelligentPointer<T>& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
};
class CThing;
class CThingAICreature;
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
namespace NSpeechGainManager {
enum EDialogueLayer { DIALOGUE_LAYER_NONE, DIALOGUE_LAYER_AMBIENT, DIALOGUE_LAYER_SCRIPT };
class CActiveDialogueInfo {
public:
    CIntelligentPointer<CThing> ThingSpeaking;
    unsigned long               SoundID;
    EDialogueLayer              Layer;
    bool                        IsLayerHead;
    float                       OutputGain;
    CActiveDialogueInfo(CBaseObject* p) : ThingSpeaking(p), SoundID(0x1234), Layer(DIALOGUE_LAYER_SCRIPT), IsLayerHead(true), OutputGain(0.8f) {}
    CActiveDialogueInfo(const CActiveDialogueInfo& other)
        : ThingSpeaking(other.ThingSpeaking), SoundID(other.SoundID), Layer(other.Layer), IsLayerHead(other.IsLayerHead),
          OutputGain(other.OutputGain) {}
};
}

int main()
{
    CBaseObject obj; obj.Id = 4;
    NSpeechGainManager::CActiveDialogueInfo a(&obj);
    CBaseIntelligentPointer::s_sets = 0;
    NSpeechGainManager::CActiveDialogueInfo b(a);
    if (!(CBaseIntelligentPointer::s_sets == 1 && b.ThingSpeaking.GetPItem() == &obj && b.SoundID == 0x1234 && b.Layer == NSpeechGainManager::DIALOGUE_LAYER_SCRIPT && b.IsLayerHead && Near(b.OutputGain, 0.8f))) {
        std::printf("006e4900_TEST FAIL\n");
        return 1;
    }
    std::printf("006e4900_TEST PASS\n");
    return 0;
}