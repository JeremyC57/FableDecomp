// CActiveDialogueInfo::CActiveDialogueInfo() -- default constructor: ThingSpeaking (CIntelligentPointer<CThing> at +0) set to null, SoundID 0, Layer 3, IsLayerHead false, OutputGain 0
// Leaf: ??0CActiveDialogueInfo@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2AF0, length 64 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Layout from the PDB (struct_layouts_egor.tsv): ThingSpeaking +0, ulong SoundID +8, EDialogueLayer
// Layer +0xc, bool IsLayerHead +0x10, float OutputGain +0x14 (0x18 bytes). The vtable stored at +0
// (0x01238C6C) is CIntelligentPointer's own: the pointer is a member and the class is not
// polymorphic. CIntelligentPointer() is the family shape: base ctor, vtable, SetPItem(0). The
// float literal 0 is folded to an integer immediate store.
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PItem;                                          // +0x04 (the pointer is 8 bytes)
    CBaseIntelligentPointer();                                   // 0x00A01B10
    void SetPItem(CBaseObject* item);                            // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {           // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { SetPItem(0); }
};

enum EDialogueLayer { DIALOGUE_LAYER_NONE = 3 };

struct CActiveDialogueInfo {
    CIntelligentPointer ThingSpeaking;                           // +0x00 CIntelligentPointer<CThing>
    unsigned long       SoundID;                                 // +0x08
    EDialogueLayer      Layer;                                   // +0x0c
    bool                IsLayerHead;                             // +0x10
    float               OutputGain;                              // +0x14
    CActiveDialogueInfo();
};

CActiveDialogueInfo::CActiveDialogueInfo()
    : ThingSpeaking()
    , SoundID(0)
    , Layer(DIALOGUE_LAYER_NONE)
    , IsLayerHead(false)
    , OutputGain(0.0f)
{
}