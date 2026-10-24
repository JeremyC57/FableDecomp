// std::_Construct(NSpeechGainManager::CActiveDialogueInfo*, const NSpeechGainManager::CActiveDialogueInfo&) -- placement copy of an active-dialogue record: speaking-thing intelligent pointer, sound id, layer, layer-head flag, output gain
// Retail: Fable.exe @ 0x006E4BD0, 63 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N006e4bd0 {
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                        // +0x04
    CBaseIntelligentPointer();                   // 0x00A01B10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // inline copy ctor: base ctor, own vtable, re-link the item
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CThing;
enum EDialogueLayer { DIALOGUE_LAYER_NONE };
struct CActiveDialogueInfo {                     // NSpeechGainManager::CActiveDialogueInfo (PDB, sizeof 0x18)
    CIntelligentPointer<CThing> ThingSpeaking;   // +0x00 (vtable 0x01238C6C)
    unsigned long               SoundID;         // +0x08
    EDialogueLayer              Layer;           // +0x0C
    bool                        IsLayerHead;     // +0x10
    float                       OutputGain;      // +0x14
    __forceinline CActiveDialogueInfo(const CActiveDialogueInfo& o)   // the implicit memberwise copy, spelled out so VC7.1 inlines it
        : ThingSpeaking(o.ThingSpeaking), SoundID(o.SoundID), Layer(o.Layer), IsLayerHead(o.IsLayerHead), OutputGain(o.OutputGain) { }
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CActiveDialogueInfo* p, const CActiveDialogueInfo& value)
{
    if (p != 0)
        new (p) CActiveDialogueInfo(value);
}
}