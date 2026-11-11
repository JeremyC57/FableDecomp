// CTCInventoryStats::InitialiseMannequin() -- creates the stats-screen mannequin from the hero def's mannequin def at the hero's position, copies the hero's appearance components onto it, adds an animation component and starts its DEFAULT animation, then gives it an editable frame update
// Leaf: InitialiseMannequin@CTCInventoryStats
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x44 - 0x21] removed re-padded by-value maps +8.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CD767, 745 bytes, plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CThing::GetPos 0x004C73D0, the thing-create helper 0x00835A20 (`__fastcall` def index / position, stack bool),
//   CThing::AddAndCopyTC 0x004C9F60, CThing::AddTC(const CCharString&, bool, const CTCParamsBase*) 0x004C9D60, bitset test 0x00410DE0,
//   LowerBound 0x0040F020 (non-const), CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, animation-set lookup 0x005DC340
//   (CAnimFilterNoGroup temporary, vtables 0x0122FFDC / 0x0122FFCC), CAnimationEntry::GetAnimateThingFlags 0x005DC390,
//   CAnimInit::Set 0x0070C050, CTCAnimationComplex::GetAnimation 0x0070B460, CAnimateThing::SetMainAnim 0x0070D580,
//   component 0x5E UploadChangesToGraphics 0x00707930, CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Shape: the hero def at +0x68 is copied twice into def-pointer temporaries whose destructor skips the null test; the parent
//   thing is read into a named local before the create call (retail loads it ahead of the def copy); the 0x5C component is
//   added unconditionally (read uninitialised when absent, as retail); all def-pointer functions are __forceinline.
// Layout: local views -- CTCInventoryStats ParentThing +0x04, hero def +0x68, Mannequin +0x140; CThing presence bits +0x20,
//   interface map +0x44, component +0x64, base def +0x70; hero def mannequin def index +0x2E0; appearance animation set +0x34.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
class C3DVector;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CAnimationEntry { public:
    long AnimIndex;                                     // +0x00
    unsigned long GetAnimateThingFlags() const;         // 0x005DC390
};
class CAnimFilterBase { public:                                // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {           // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
};
class CAnimationSet { public:
    CAnimationEntry* GetAnimation(const CCharString& name, long n, const CAnimFilterBase& filter) const;   // 0x005DC340
};
class CDefPointeeBase { public:
    virtual void v000();
    virtual void DestroyDef();                          // slot 1 (+4)
    long m_refCount;                                    // +4
};
template<class T> class CDefPointer { public:
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    FABLE_FORCEINLINE CDefPointer(T* p) : m_pObject(p) { if (p) ++p->m_refCount; }
    FABLE_FORCEINLINE ~CDefPointer() { if (m_pObject) { if (--m_pObject->m_refCount == 0) m_pObject->DestroyDef(); } }
    T* operator->() const { return m_pObject; }
};
template<class T> struct CDefRef {
    T* m_pObject;
    FABLE_FORCEINLINE CDefRef(T* p) : m_pObject(p) { if (p) ++p->m_refCount; }
    FABLE_FORCEINLINE ~CDefRef() { if (--m_pObject->m_refCount == 0) m_pObject->DestroyDef(); }
    T* operator->() const { return m_pObject; }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x2c];
    CAnimationSet Animation;                            // +0x34
};
class CDefClassBase : public CDefPointeeBase { public:
    CAppearanceDef* FindAppearanceDef();
    bool GetSubDef(CDefPointer<CAppearanceDef>* out);   // 0x0042B0A2
};
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>* out)
{
    CAppearanceDef* def = FindAppearanceDef();
    if (!def)
        return false;
    out->m_pObject = def;
    return true;
}
#pragma auto_inline(on)
struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
#include "engine/EAnimUpdateMode.h"
class CAnimInit { public:                                      // 0x2c bytes
    long          Mode;
    unsigned char AnimType;
    bool          Flag05;
    long          AnimIndex;
    float         Scales[3];
    CBlendPair    Blend;                                // +0x18
    unsigned long Flags;
    long          Arg24;
    long          Percent;
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority);   // 0x0070C050 Ego_r/FableWin ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z (agentNF16: was Set)
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Init((unsigned char)(animType), arg24, animIndex, (EAnimUpdateMode)(mode), flag05, flags, percent);
    }
};
class CAnimateThing { public:
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                      // 0x0070B460
};
class CTCHeroAttachableAppearanceModifiers { public:   // interface 0x5E
    void UploadChangesToGraphics();                     // 0x00707930 (Ego_r.pdb QAEXXZ)
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    std::bitset<274>  m_present;                           // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad54[0x10];
    CTCBase*       Clothing;                            // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefClassBase* PBaseDef;                            // +0x70
    const C3DVector& GetPos() const;               // 0x004C73D0
    void AddAndCopyTC(const CTCBase& tc, bool b);       // 0x004C9F60 (Ego_r.pdb, by reference)
    CTCBase* AddTC(const CCharString& name, bool activate, const class CTCParamsBase* params);   // 0x004C9D60 (Ego_r.pdb; name fix NF21)
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long kind)
    {
        if (m_present.test(kind)) {
            ETCInterfaceType key = (ETCInterfaceType)(kind);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            *out = (T*)((it == map->end() || kind < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CTCDNone { public:                                    // RTTI .?AVCTCDNone@@ : CTCDriverBase
    static CThing* __fastcall Create(long defIndex, const C3DVector& position, bool flag);   // 0x00835A20 (Ego_r.pdb / FableWin.pdb)
};
class CHeroDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x2e0 - 0x08];
    long MannequinDef;                                  // +0x2e0
};
class CTCInventoryStats { public:
    void*     __vftable;
    CThing*   ParentThing;                              // +0x04
    unsigned char _pad08[0x68 - 0x08];
    CHeroDef* HeroDef;                                  // +0x68
    unsigned char _pad6c[0x140 - 0x6c];
    CThing*   Mannequin;                                // +0x140
    void InitialiseMannequin();
};
void CTCInventoryStats::InitialiseMannequin()
{
    bool noMannequin = CDefRef<CHeroDef>(HeroDef)->MannequinDef == 0;
    if (noMannequin) {
        Mannequin = 0;
        return;
    }
    CThing* parent = ParentThing;
    Mannequin = CTCDNone::Create(CDefRef<CHeroDef>(HeroDef)->MannequinDef, parent->GetPos(), true);
    Mannequin->AddAndCopyTC(*ParentThing->Clothing, false);
    CTCBase* tc;
    if (ParentThing->GetInterface(&tc, 0x5e))
        Mannequin->AddAndCopyTC(*tc, false);
    if (ParentThing->GetInterface(&tc, 0x78))
        Mannequin->AddAndCopyTC(*tc, false);
    if (ParentThing->GetInterface(&tc, 3))
        Mannequin->AddAndCopyTC(*tc, false);
    {
        CTCBase* tc5c;
        ParentThing->GetInterface(&tc5c, 0x5c);
        Mannequin->AddAndCopyTC(*tc5c, false);
    }
    Mannequin->AddTC("CTCAnimationComplex", false, 0);
    CTCAnimationComplex* complex;
    Mannequin->GetInterface(&complex, 0x5a);
    CDefPointer<CAppearanceDef> appearance;
    CDefPointer<CDefClassBase>(Mannequin->PBaseDef)->GetSubDef(&appearance);
    CAnimationEntry* entry = appearance->Animation.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
    CAnimInit init(6, 0, entry->AnimIndex, 1, true, entry->GetAnimateThingFlags(), 100);
    complex->GetAnimation().SetMainAnim(init, 1, 0);
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (Mannequin->GetInterface(&modifiers, 0x5e))
        modifiers->UploadChangesToGraphics();
}