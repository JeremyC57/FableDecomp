// NSpeechGainManager::CActiveDialogueInfo::CActiveDialogueInfo(const CActiveDialogueInfo& other) -- copy constructor of an active-dialogue record: speaker pointer, sound id, layer, layer-head flag, output gain
// Leaf: ??0CActiveDialogueInfo@NSpeechGainManager@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E4900, 65 bytes, __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<CIntelligentPointer<CThing>,long><CIntelligentPointer<CThing>,int>" is
//   propagated. A CIntelligentPointer<CThing> (vtable 0x01238C6C) + dword + dword + byte + dword = Ego_r.pdb
//   NSpeechGainManager::CActiveDialogueInfo (24 bytes: ThingSpeaking +0, SoundID +8, Layer +0xC, IsLayerHead +0x10,
//   OutputGain +0x14); FableWin.pdb ??0CActiveDialogueInfo@NSpeechGainManager@@QAE@ABV01@@Z. The same copy appears inlined
//   in the TU's landed _Construct 0x006E4BD0. No direct callers.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90.
// Shape: the memberwise copy; the float gain moves through eax (no FPU) as in the landed _Construct.
// NF95 2026-11-19 (q11285): the NSpeechGainManager::EDialogueLayer enumerators are the PDB names (engine/NSpeechGainManager.h;
//   Ego_d.pdb LF_FIELDLIST 0x14C96, installed-game header_enums.csv): DIALOGUE_LAYER_FOREGROUND 0, _MIDGROUND 1, _BACKGROUND 2, _LAST 3;
//   were the stand-ins DIALOGUE_LAYER_0 / _1 / _2 / _NONE / _COUNT or a bare cast.
class CBaseObject;
class CThing;
class CThingAICreature;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PData;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();                                 // each instantiation has its own vtable
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

namespace NSpeechGainManager {
enum EDialogueLayer { DIALOGUE_LAYER_FOREGROUND };
class CActiveDialogueInfo {
public:
    CIntelligentPointer<CThing> ThingSpeaking;          // +0x00
    unsigned long               SoundID;                // +0x08
    EDialogueLayer              Layer;                  // +0x0C
    bool                        IsLayerHead;            // +0x10
    float                       OutputGain;             // +0x14
    CActiveDialogueInfo(const CActiveDialogueInfo& other);
};

CActiveDialogueInfo::CActiveDialogueInfo(const CActiveDialogueInfo& other)
    : ThingSpeaking(other.ThingSpeaking), SoundID(other.SoundID), Layer(other.Layer), IsLayerHead(other.IsLayerHead),
      OutputGain(other.OutputGain)
{
}
}