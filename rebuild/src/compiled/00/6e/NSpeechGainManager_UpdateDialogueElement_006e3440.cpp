// NSpeechGainManager::UpdateDialogueElement(CActiveDialogueInfo& info, long elapsedFrames, const vector<CDialogueLayerDef>& layerDefs) -- move a speaking line's output gain toward its target gain by at most the layer's attack/release delta per elapsed frame and push it to the speaker's sound player; false once the speaker is gone, dead, soundless or the line has stopped playing
// Leaf: ?UpdateDialogueElement@NSpeechGainManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E3440, 316 bytes (+ int3 padding to 320), `__fastcall` (ecx = info, edx =
//   elapsedFrames), `ret 4`, bool in al.
// Identity: Ego_r.pdb / FableWin.pdb name set `?UpdateDialogueElement@NSpeechGainManager@@YI_NAAVCActiveDialogueInfo@1@J
//   ABV?$vector@VCDialogueLayerDef@NSpeechGainManager@@...@std@@@Z`; single caller the landed NSpeechGainManager::Update
//   0x006E37D0. The catalog's "UpdateDialogueElement" is the PDB name.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times), the interface map's LowerBound 0x0040F020 (the
//   speaker's sound player, TC 0x10), CTCSoundPlayer::IsAttachedSoundPlaying 0x0066AE10 (PAttachedEmitter +0xC; the landed
//   row calls it CGameScriptThing::IsSamplePlaying), the landed NSpeechGainManager::GetTargetGain 0x006E2B70,
//   CTCSoundPlayer::SetSpecificSampleVolume 0x0066AFF0 (landed).
// Globals: 1e-4f 0x0129BA3C, 0.0f 0x0122DEDC, the per-layer response times 0x013BABA4 (a static STLport
//   vector<CResponseTimes>; Ego_r.pdb CResponseTimes = {AttackMaxDeltaPerFrame +0, ReleaseMaxDeltaPerFrame +4}).
// Shape: the inline GFFloatAbs (FableWin.pdb ?GFFloatAbs@@YIMM@Z) clears the sign bit of its by-value argument through
//   memory (`and [f],7FFFFFFFh`); the speaker re-fetched after the alive test feeds both HasTC and the interface lookup; the
//   target gain and the named difference stay on the x87 stack; the new gain is ONE ternary stored into OutputGain and
//   passed from it (a named newGain local, or an unnamed difference, spills them to frame slots); step = elapsedFrames *
//   the attack/release delta picked by the sign of the difference.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];                       // +0x20
    CInterfaceMap Components;                               // +0x44
    unsigned char _pad4c[0x91 - 0x4C];
    unsigned char StateBits;                                // +0x91 Dead 0x01
    bool IsDead() const { return (StateBits & 0x01) != 0; }
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCSoundPlayer {
public:
    bool IsAttachedSoundPlaying(unsigned long soundID) const;   // 0x0066AE10
    void SetSpecificSampleVolume(unsigned long sampleID, float volume);   // 0x0066AFF0
};

inline float __fastcall GFFloatAbs(float f)
{
    *(unsigned long*)&f &= 0x7fffffff;
    return f;
}

extern const float g_Epsilon;                               // 0x0129BA3C (1e-4f)

namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

enum { TC_SOUND_PLAYER = 0x10 };

namespace NSpeechGainManager {

enum EDialogueLayer { DIALOGUE_LAYER_0 = 0, DIALOGUE_LAYER_1 = 1, DIALOGUE_LAYER_2 = 2, DIALOGUE_LAYER_NONE = 3 };

class CActiveDialogueInfo {                                 // 0x18 bytes
public:
    CIntelligentPointer<CThing> ThingSpeaking;              // +0x00
    unsigned long SoundID;                                  // +0x08
    EDialogueLayer Layer;                                   // +0x0C
    bool IsLayerHead;                                       // +0x10
    float OutputGain;                                       // +0x14
};

class CDialogueLayerDef;

class CResponseTimes {
public:
    float AttackMaxDeltaPerFrame;                           // +0x00
    float ReleaseMaxDeltaPerFrame;                          // +0x04
};

extern std::vector<CResponseTimes> g_ResponseTimes;         // 0x013BABA4

float __fastcall GetTargetGain(const CActiveDialogueInfo& info, const std::vector<CDialogueLayerDef>& layerDefs);   // 0x006E2B70

bool __fastcall UpdateDialogueElement(CActiveDialogueInfo& info, long elapsedFrames,
                                      const std::vector<CDialogueLayerDef>& layerDefs)
{
    if (info.ThingSpeaking.GetPItem() && !info.ThingSpeaking->IsDead()) {
        CThing* speaker = info.ThingSpeaking.operator->();
        if (speaker->HasTC(TC_SOUND_PLAYER)) {
            CTCSoundPlayer* player = speaker->GetTC<CTCSoundPlayer>(TC_SOUND_PLAYER);
            if (player->IsAttachedSoundPlaying(info.SoundID)) {
                float targetGain = GetTargetGain(info, layerDefs);
                float diff = targetGain - info.OutputGain;
                if (GFFloatAbs(diff) > g_Epsilon) {
                    float step = elapsedFrames * (diff > 0.0f ? g_ResponseTimes[info.Layer].AttackMaxDeltaPerFrame
                                                              : g_ResponseTimes[info.Layer].ReleaseMaxDeltaPerFrame);
                    info.OutputGain = GFFloatAbs(diff) > GFFloatAbs(step) ? step + info.OutputGain : targetGain;
                    player->SetSpecificSampleVolume(info.SoundID, info.OutputGain);
                }
                return true;
            }
        }
    }
    return false;
}

}