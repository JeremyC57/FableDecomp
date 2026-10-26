#pragma optimize("s", on)
// NPlayerGui::CDrawAbilityButtons::CDrawAbilityButtons(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, three columns, two empty primitive handles, empty special-handle and effect vectors, then one effect slot per ability-button effect in the GUI def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00651A7E, length 134 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65, CBase::CBase @ 0x0099A2D0 (the handles' root, vtable 0x01231710),
// PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments), vector<CIntelligentPointer<CParticleEmitter>>::Resize(long) @ 0x00657037.
// Vtables 0x0125A6C8 (CDrawAbilityButtons), 0x0122F598 (CEnginePrimitiveHandle, rtti-vtables.tsv).
// PDB CDrawAbilityButtons (96 bytes): Columns +0x2c, BackgroundPrimitiveHandle +0x30, InteractBorderPrimitiveHandle +0x38,
// SpecialPrimitiveHandles +0x40, Effects +0x50 -- retail 4 lower up to the special vector, 8 lower after it (12-byte vectors).
// The GUI def holds a vector of 0x5c-byte ability-button effect records at +0x7fc (retail offset).
// Shape: size TU; each handle is built through its address in edi (`__forceinline` constructor -- a plain inline one stays
// out of line in this size TU -- over the out-of-line CBase constructor, then the handle vtable, the
// counted Data pointer -- a member object with its own inline constructor, so the shared `xor ebx,ebx` lands after the
// vtable store -- zeroed from ebx), the vectors' inline constructors store three zeros (Effects through edi, reused
// as `this` of the Resize); the def vector's size() is `(last - first) / 0x5c` (`push 5Ch ; pop ecx ; cdq ; idiv`).
class CPlayerGui;

struct CAbilityButtonEffectDef { unsigned char _body[0x5c]; };

struct CRetailEffectDefVector {                    // VC6 Dinkumware vector<0x5c-byte record>
    CAbilityButtonEffectDef* m_pFirst;
    CAbilityButtonEffectDef* m_pLast;
    CAbilityButtonEffectDef* m_pEnd;
    long size() const { return m_pLast - m_pFirst; }
};

class CPlayerGuiDef { public:
    unsigned char          _pad[0x7fc];
    CRetailEffectDefVector AbilityButtonEffects;   // +0x7fc
};

extern CPlayerGuiDef* __fastcall PeekGuiDef();     // 0x00438570

namespace N_0099a2d0 { struct CBase {                                     // vtable 0x01231710
    virtual void v00();
    CBase();                                       // 0x0099A2D0
}; } using N_0099a2d0::CBase;

class CIVCountedPointeeBase;

struct CIVCountedPointer {                         // embedded member with an inline constructor: its zero store follows the
    CIVCountedPointeeBase* Object;                 // handle's vtable store
    __forceinline CIVCountedPointer() : Object(0) { }
};

class CEnginePrimitiveHandle : public CBase { public:            // vtable 0x0122F598
    CIVCountedPointer Data;                        // +4 (PDB: CIVCountedPointer<CEnginePersistentPrimitiveBase>)
    virtual void v00();
    __forceinline CEnginePrimitiveHandle() { }
};

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CRetailEmitterVector {                      // VC6 Dinkumware vector<CIntelligentPointer<CParticleEmitter>>
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    void Resize(long count);                       // 0x00657037
};

class CDrawBase { public:
    virtual void v00();
    unsigned char _body[0x28 - 4];                 // +0x04 .. +0x28 (GUI pointers, Drawing, PrimitiveHandles)
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

class CDrawAbilityButtons : public CDrawBase { public:           // vtable 0x0125A6C8
    virtual void v00();
    long                   Columns;                        // +0x28
    CEnginePrimitiveHandle BackgroundPrimitiveHandle;      // +0x2c
    CEnginePrimitiveHandle InteractBorderPrimitiveHandle;  // +0x34
    CRetailHandleVector    SpecialPrimitiveHandles;        // +0x3c
    CRetailEmitterVector   Effects;                        // +0x48
    CDrawAbilityButtons(CPlayerGui* gui, void* drawList);
};

CDrawAbilityButtons::CDrawAbilityButtons(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), Columns(3), BackgroundPrimitiveHandle(), InteractBorderPrimitiveHandle(),
      SpecialPrimitiveHandles(), Effects()
{
    Effects.Resize(PeekGuiDef()->AbilityButtonEffects.size());
}

}