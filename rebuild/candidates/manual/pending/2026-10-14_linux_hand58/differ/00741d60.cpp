// Leaf: ??_GCModeExplosiveForcePushed@
// NCreatureMode::CModeExplosiveForcePushed::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor of the mode: its target pointer (+0x28, ~CBaseIntelligentPointer @ 0x00A01C10), its CAnimFilterOfGroup (+0x1c: the name string at +0x20 @ 0x0099EAE0, then the CAnimFilterBase vtable 0x0122FFCC), the inline CModeBase destructor (vtable 0x012673CC, the name string at +0xc, the CAnimFilterBase member at +8), then operator delete (0x00BFE9BC) for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00741D60, length 67 bytes.
// Callees: ~CBaseIntelligentPointer @ 0x00A01C10, CCharString::~CCharString @ 0x0099EAE0 (twice), operator delete @ 0x00BFE9BC. Neither the mode nor its CAnimFilterOfGroup / CIntelligentPointer members store their own vtables: all three destructors are compiler-generated (only a user-written destructor resets the vptr), while CModeBase (`__forceinline virtual ~CModeBase() {}`, the landed 0x00739EE0 model) and CAnimFilterBase do. The `new T` factory pulls the vtable, and so this `??_G`, into the object. Shape: `this` in esi, `test byte ptr [esp+8],1`, `ret 4`.
// Grade: DIFFER(ours v retail) -- the lane calls the implicit ~CAnimFilterOfGroup (+0x1c) and ~CModeBase out of line where retail inlines both; an explicit __forceinline ~CAnimFilterOfGroup adds the derived vtable store retail lacks (only compiler-generated destructors skip it: the lane's out-of-line ??1CAnimFilterOfGroup stores just the base vtable), `__declspec(novtable)` drops the CModeBase stores too, and inline_depth/inline_recursion pragmas change nothing.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct CAnimFilterBase {                           // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }                         // inline: the vtable store
};
struct CAnimFilterOfGroup : CAnimFilterBase {      // vtable 0x01267568; implicit destructor
    CCharString   m_name;                          // +0x04
    unsigned long m_hash;                          // +0x08
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // implicit destructor
};
struct CThing;

namespace NCreatureMode {
struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase() { }
    CThing*         m_pThing;                      // +0x04
    CAnimFilterBase m_filter;                      // +0x08
    CCharString     m_name;                        // +0x0c
};

struct CModeExplosiveForcePushed : CModeBase {     // implicit destructor
    unsigned char               _pad10[0xc];       // +0x10 .. +0x1c
    CAnimFilterOfGroup          m_groupFilter;     // +0x1c
    CIntelligentPointer<CThing> PTarget;           // +0x28
};
}

void* MakeOne_00741d60()                           // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new NCreatureMode::CModeExplosiveForcePushed;
}
