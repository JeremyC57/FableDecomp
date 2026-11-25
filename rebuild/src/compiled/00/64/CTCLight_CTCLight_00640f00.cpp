// CTCLight::CTCLight(CThing* thing) -- CTCBase(thing) + the graphic-appearance component base, everything cleared (flicker factors 1.0), then register with the owner's graphic appearance and fetch the light sub-def
// Leaf: ??0CTCLight@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCLight@@QAE@AAVCThing@@@Z).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00640F00, length 120 bytes (`ret 4`). Vtables: CTCLight primary 0x0125A194, the
// NGraphicAppearance::CAGraphicAppearanceComponent base 0x0125A170 (stored first by its inline ctor) and CTCLight's side 0x0125A208 at +0xC
// (as the landed ~CTCLight 0x00640F80).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (catalog "CEngineSubPrimitive2DClockSprite" is propagated),
// NGraphicAppearance::CTCGraphicAppearance::RegisterComponent @ 0x004BD390 (owner +0x64, the +0xC subobject; catalog
// "DoubleLinkedListInsertNode" is propagated), CDefClassBase::GetSubDef<CLightDef> @ 0x0059346A (owner def +0x70; the catalog's
// <CAbilityDef> instantiation is a byte twin).
// Layout (retail = PDB CTCLight 0x4C with 12-byte STLport vectors, 0x44): PLightInfo +0x10, PLightDef +0x18, Active/Overridden
// +0x1C/+0x1D, AccumilativeFlickerFactor/OldAccumilativeFlickerFactor +0x20/+0x24, Initialised +0x28, FlareSprites +0x2C, Lights +0x38.
// Shape: init-list in declaration order; the vectors as the STLport _Vector_base + _STLP_alloc_proxy model (places `lea edi`); the zeroes share eax, the 1.0f pair edx; the body registers then calls GetSubDef(&PLightDef).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
struct CLightInfo;
#include "engine/CCountedPointer.h"
class CDefClassBase;
class CLightDef;
// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x004BD390 ?RegisterComponent@CTCGraphicAppearance@NGraphicAppearance@@QAEXAAVCAGraphicAppearanceComponent@2@@Z (was ?RegisterComponent@CTCGraphicAppearance@NGraphicAppearance@@QAEXPAVCAGraphicAppearanceComponent@2@@Z)
namespace NGraphicAppearance {
class CPrimitive;
class CAGraphicAppearanceComponent;
class CTCGraphicAppearance { public:
    void RegisterComponent(NGraphicAppearance::CAGraphicAppearanceComponent&);   // 0x004BD390
};
class CAGraphicAppearanceComponent { public:               // vtable 0x0125A170
    virtual void OnAppearanceDraw();
    CAGraphicAppearanceComponent() { }
};
}
struct CThingDefHolder;
class CThing { public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefClassBase* PDef;                            // +0x70
};
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template<class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    CDefPointer() : Object(0) { }
};
class CDefClassBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0059346A GetSubDef<CLightDef> (Ego_r.pdb member template)
};
template <class T> struct CAllocator {              // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {             // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template<class T> struct CPtrVector : CVectorBase<T*> {   // STLport vector<T*> (12 bytes)
    CPtrVector() : CVectorBase<T*>(CAllocator<T*>()) { }
};
class CTCLight : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent { public:   // vtables 0x0125A194 / 0x0125A208
    CCountedPointer<CLightInfo>     PLightInfo;             // +0x10
    CDefPointer<const CLightDef>    PLightDef;              // +0x18
    bool  Active;                                           // +0x1c
    bool  Overridden;                                       // +0x1d
    float AccumilativeFlickerFactor;                        // +0x20
    float OldAccumilativeFlickerFactor;                     // +0x24
    bool  Initialised;                                      // +0x28
    CPtrVector<NGraphicAppearance::CPrimitive> FlareSprites;   // +0x2c
    CPtrVector<NGraphicAppearance::CPrimitive> Lights;         // +0x38
    CTCLight(CThing& thing);
    virtual ~CTCLight();
    virtual void OnAppearanceDraw();
};

CTCLight::CTCLight(CThing& thing)
    : CTCBase(thing)
    , Active(false)
    , Overridden(false)
    , AccumilativeFlickerFactor(1.0f)
    , OldAccumilativeFlickerFactor(1.0f)
    , Initialised(false)
{
    ParentThing->PTCGraphicAppearance->RegisterComponent(*(this));
    ParentThing->PDef->GetSubDef(&PLightDef);
}