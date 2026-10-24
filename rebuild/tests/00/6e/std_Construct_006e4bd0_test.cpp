#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
static int g_extCopies = 0;   // out-of-line copy constructors the retail body calls
static int g_links = 0;       // CBaseIntelligentPointer::SetPItem calls
namespace N006e4bd0 {
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CBaseObject* m_pItem;                        // +0x04
    CBaseIntelligentPointer() : m_pItem(0) { }                   // 0x00A01B10
    CBaseObject* GetPItem() const { return m_pItem; }               // 0x00A01B50
    void SetPItem(CBaseObject* item) { m_pItem = item; ++g_links; }            // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // inline copy ctor: base ctor, own vtable, re-link the item
    virtual void v00() { }
    CIntelligentPointer() { }
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
    CActiveDialogueInfo() { }
    __forceinline CActiveDialogueInfo(const CActiveDialogueInfo& o)   // the implicit memberwise copy, spelled out so VC7.1 inlines it
        : ThingSpeaking(o.ThingSpeaking), SoundID(o.SoundID), Layer(o.Layer), IsLayerHead(o.IsLayerHead), OutputGain(o.OutputGain) { }
};

void __fastcall _Construct(CActiveDialogueInfo* p, const CActiveDialogueInfo& value)
{
    if (p != 0)
        new (p) CActiveDialogueInfo(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N006e4bd0;
    CActiveDialogueInfo src;
    src.ThingSpeaking.m_pItem = reinterpret_cast<CBaseObject*>(0x44);
    src.SoundID = 1234u;
    src.Layer = (EDialogueLayer)2;
    src.IsLayerHead = true;
    src.OutputGain = 0.8f;
    CActiveDialogueInfo* slot = reinterpret_cast<CActiveDialogueInfo*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 0 && g_links == 1;
    if (!(slot->ThingSpeaking.m_pItem == src.ThingSpeaking.m_pItem)) ok = false;
    if (!(slot->SoundID == src.SoundID)) ok = false;
    if (!(slot->Layer == src.Layer)) ok = false;
    if (!(slot->IsLayerHead == src.IsLayerHead)) ok = false;
    if (!(slot->OutputGain == src.OutputGain)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 0 || g_links != 1) ok = false;
    if (!ok) { std::printf("006e4bd0_TEST FAIL\n"); return 1; }
    std::printf("006e4bd0_TEST PASS\n");
    return 0;
}