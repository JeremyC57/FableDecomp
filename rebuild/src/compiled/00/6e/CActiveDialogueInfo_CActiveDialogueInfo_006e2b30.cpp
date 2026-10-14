// CActiveDialogueInfo::CActiveDialogueInfo(CThing*, unsigned long soundID, EDialogueLayer layer, bool isLayerHead) -- constructor: ThingSpeaking from the pointer, SoundID, Layer, IsLayerHead from the arguments, OutputGain 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2B30, length 64 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Layout from the PDB: ThingSpeaking +0 (CIntelligentPointer<CThing>), ulong SoundID +8,
// EDialogueLayer Layer +0xc, bool IsLayerHead +0x10, float OutputGain +0x14. The vtable stored
// at +0 (0x01238C6C) is CIntelligentPointer's own (a member; the class is not polymorphic).
// CIntelligentPointer(p) is the family shape: base ctor, vtable, SetPItem(p); then the three
// argument stores and the folded float-zero store (ret 0x10).
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PItem;                                          // +0x04 (the pointer is 8 bytes)
    CBaseIntelligentPointer();                                   // 0x00A01B10
    void SetPItem(CBaseObject* item);                            // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {           // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

enum EDialogueLayer { DIALOGUE_LAYER_NONE = 3 };

struct CActiveDialogueInfo {
    CIntelligentPointer ThingSpeaking;                           // +0x00 CIntelligentPointer<CThing>
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