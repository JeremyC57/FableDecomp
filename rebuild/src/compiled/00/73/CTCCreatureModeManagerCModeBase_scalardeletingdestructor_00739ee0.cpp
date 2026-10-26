// CTCCreatureModeManager::CModeBase::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x012673CC), the CCharString at +0xc destroyed (0x0099EAE0), the CAnimFilterBase member at +8 destroyed inline (its vtable 0x0122FFCC), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00739EE0, length 46 bytes.
// Leaf: ??_GCModeBase@CTCCreatureModeManager@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x012673CC) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
class CAnimFilterBase { public:                           // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }                         // inline: the vtable store
};
struct CModeBaseRoot_00739ee0 {
    virtual void v00();
};
namespace CTCCreatureModeManager {
class CModeBase : public CModeBaseRoot_00739ee0 { public:              // vtable 0x012673CC
    long            ParentThing;                           // +0x04
    CAnimFilterBase m_filter;                      // +0x08
    CCharString     m_name;                        // +0x0c
    __forceinline virtual ~CModeBase() { }
};

}
void* MakeOne_00739ee0()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CTCCreatureModeManager::CModeBase;
}