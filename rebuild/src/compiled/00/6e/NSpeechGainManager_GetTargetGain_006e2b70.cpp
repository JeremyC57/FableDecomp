// NSpeechGainManager::GetTargetGain(const CActiveDialogueInfo& info, const vector<CDialogueLayerDef>& layerDefs) -- the gain a speaking line should fade to: 1 when speech-gain management is off, otherwise the layer def's high/mid/low level picked from the line's layer, whether it heads its layer and how many lines speak on the layers above it
// Leaf: ?GetTargetGain@NSpeechGainManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2B70, 137 bytes (+ int3 padding), `__fastcall` (ecx = info, edx = layerDefs),
//   float in st(0), plain `ret`.
// Identity: Ego_r.pdb name set `?GetTargetGain@NSpeechGainManager@@YIMABVCActiveDialogueInfo@1@ABV?$vector@VCDialogueLayerDef@
//   NSpeechGainManager@@...@std@@@Z`; callers 0x006E340D / 0x006E34C1 (the speech gain manager's add/update code).
// Globals: the speech-gain enable flag @ 0x01375730 (bool, initialised true), the per-layer speaker counts @ 0x013BAB94
//   (a static STLport vector<short>: its first word is freed by the atexit thunk 0x012294F0 and incremented per added
//   line at 0x006E3406), the float pool 1.0f @ 0x0122DED8 and 0.0f @ 0x0122DEDC.
// Layout: PDB NSpeechGainManager::CActiveDialogueInfo (0x18): Layer +0x0C, IsLayerHead +0x10. CDialogueLayerDef (0x40):
//   GainLevelHigh +0x30, GainLevelMid +0x34, GainLevelLow +0x38 (layer n's def at n * 0x40: +0x30, +0x70/+0x78,
//   +0xB0/+0xB4/+0xB8).
// Shape: `float gain = 0.0f` loaded before the switch (each case drops it with `fstp st(0)`); a three-case switch
//   (`sub eax,0 ; je ; dec ; je ; dec ; jne`); vector elements through STLport `*(begin() + n)`; the layer-2 fallback
//   test `!(gain > 0.0f) || counts[1] != 0` keeps the value on the stack (`fcom ; test ah,41h ; jne`; `gain <= 0.0f`
//   gives `jnp`).
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
}

class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* PItem;
};

namespace NSpeechGainManager {

enum EDialogueLayer { DIALOGUE_LAYER_0 = 0, DIALOGUE_LAYER_1 = 1, DIALOGUE_LAYER_2 = 2, DIALOGUE_LAYER_NONE = 3 };

class CActiveDialogueInfo {                                 // 0x18 bytes
public:
    CBaseIntelligentPointer ThingSpeaking;                  // +0x00 CIntelligentPointer<CThing>
    unsigned long SoundID;                                  // +0x08
    EDialogueLayer Layer;                                   // +0x0C
    bool IsLayerHead;                                       // +0x10
    float OutputGain;                                       // +0x14
};

class CDialogueLayerDef {                                   // 0x40 bytes
public:
    unsigned char _base[0x28];                              // CDefClassBase
    float ResponseTimeSecsAttack;                           // +0x28
    float ResponseTimeSecsRelease;                          // +0x2C
    float GainLevelHigh;                                    // +0x30
    float GainLevelMid;                                     // +0x34
    float GainLevelLow;                                     // +0x38
    float FocusShiftDistanceThreshold;                      // +0x3C
};

extern bool g_bSpeechGainManagementEnabled;                 // 0x01375730
extern std::vector<short> g_LayerSpeakerCounts;             // 0x013BAB94

float __fastcall GetTargetGain(const CActiveDialogueInfo& info, const std::vector<CDialogueLayerDef>& layerDefs)
{
    if (!g_bSpeechGainManagementEnabled)
        return 1.0f;
    float gain = 0.0f;
    switch (info.Layer) {
    case DIALOGUE_LAYER_0:
        gain = layerDefs[0].GainLevelHigh;
        break;
    case DIALOGUE_LAYER_1:
        if (g_LayerSpeakerCounts[0] == 0 && info.IsLayerHead)
            gain = layerDefs[1].GainLevelHigh;
        else
            gain = layerDefs[1].GainLevelLow;
        break;
    case DIALOGUE_LAYER_2:
        if (g_LayerSpeakerCounts[0] == 0)
            gain = layerDefs[2].GainLevelHigh;
        else
            gain = layerDefs[2].GainLevelLow;
        if (!(gain > 0.0f) || g_LayerSpeakerCounts[1] != 0)
            gain = layerDefs[2].GainLevelMid;
        break;
    }
    return gain;
}

}