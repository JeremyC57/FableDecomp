// CActiveDialogueInfo::CActiveDialogueInfo(CThing*, unsigned long soundID, EDialogueLayer layer, bool isLayerHead) -- constructor: ThingSpeaking from the pointer, SoundID, Layer, IsLayerHead from the arguments, OutputGain 0
// Leaf: ??0CActiveDialogueInfo@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2B30, length 64 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Layout from the PDB: ThingSpeaking +0 (CIntelligentPointer<CThing>), ulong SoundID +8,
// EDialogueLayer Layer +0xc, bool IsLayerHead +0x10, float OutputGain +0x14. The vtable stored
// at +0 (0x01238C6C) is CIntelligentPointer's own (a member; the class is not polymorphic).
// CIntelligentPointer(p) is the family shape: base ctor, vtable, SetPItem(p); then the three
// argument stores and the folded float-zero store (ret 0x10).
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

class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {           // vtable 0x01238C6C
public:
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

enum EDialogueLayer { DIALOGUE_LAYER_NONE = 3 };

struct CActiveDialogueInfo {
    CIntelligentPointer<CThing> ThingSpeaking;                           // +0x00 CIntelligentPointer<CThing>
    unsigned long       SoundID;                                 // +0x08
    EDialogueLayer      Layer;                                   // +0x0c
    bool                IsLayerHead;                             // +0x10
    float               OutputGain;                              // +0x14
    CActiveDialogueInfo(CBaseObject* thingSpeaking, unsigned long soundID, EDialogueLayer layer, bool isLayerHead);
};

CActiveDialogueInfo::CActiveDialogueInfo(CBaseObject* thingSpeaking, unsigned long soundID, EDialogueLayer layer, bool isLayerHead)
    : ThingSpeaking(thingSpeaking)
    , SoundID(soundID)
    , Layer(layer)
    , IsLayerHead(isLayerHead)
    , OutputGain(0.0f)
{
}