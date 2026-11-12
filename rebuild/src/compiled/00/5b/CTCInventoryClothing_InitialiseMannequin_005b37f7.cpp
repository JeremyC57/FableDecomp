// CTCInventoryClothing::InitialiseMannequin() -- create the inventory mannequin from the inventory def at the owner, copy the owner's appearance components onto it and start its DEFAULT animation
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x23] removed re-padded by-value maps +8.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
class CAnimationEntry { public:
    long AnimIndex;                                            // +0x00
    unsigned long GetAnimateThingFlags() const;                // 0x005DC390
    long GetTransitionInTime() const;                          // 0x005DC1D0
};
class CAnimFilterBase { public:                                       // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {                  // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;
    virtual CAnimFilterBase* Clone() const;
};
class CAnimationSet { public:
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                                    // slot 1
    long RefCount;                                             // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* p) : Object(p) { if (Object) ++Object->RefCount; }
    FABLE_FORCEINLINE ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                                   // +0x34
};
class CThingBaseDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
void* FindDef_CThingBaseDef_GetSubDef_0();
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CAnimFrameInfo { public:
    float Current;
    float Last;
    CAnimFrameInfo() : Current(0), Last(0) {}
    CAnimFrameInfo(const CAnimFrameInfo& o) : Current(o.Current), Last(o.Last) {}
};
#include "engine/EAnimUpdateMode.h"
class CAnimInit { public:                                             // 0x2c bytes
    long          UpdateMode;
    unsigned char Type;
    bool          Looping;
    long          Anim;
    float         Speed;
    float         Alpha;
    float         LastAlpha;
    CAnimFrameInfo Frame;                                      // +0x18
    unsigned long Flags;
    unsigned long TypeFlags;
    long          Priority;
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, long updateMode, bool looping, unsigned long flags,
              long priority)
    {
        Init(type, typeFlags, anim, (EAnimUpdateMode)(updateMode), looping, flags, priority);
    }
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode updateMode, bool looping, unsigned long flags, long priority);   // 0x0070C050 Ego_r/FableWin ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z (agentNF16: was Set)
};
struct CAnimFrameUpdate {
    float Time;
    float Reserved0;
    float Reserved1;
    CAnimFrameUpdate(float time) { Time = time; Reserved0 = 0; Reserved1 = 0; }
};
struct CAnimRef { short Index; };
class CAnimSet { public:
    long          UpdateMode;                                  // +0x00
    CAnimRef*     Anims;                                       // +0x04 vector _M_start
    unsigned char _pad08[0x1c - 0x08];
    CAnimFrameInfo Frame;                                      // +0x1c
};
class CAnimateThing { public:
    CAnimSet MainAnimSet;
    CAnimFrameInfo GetFrameInfo() const { return MainAnimSet.Frame; }
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
    void UpdateFrameLogic(const CAnimFrameUpdate& update);     // 0x0070E8B0
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                             // 0x0070B460
};
class CTCBase;
class CTCHeroAttachableAppearanceModifiers { public:                  // interface 0x5E
    void UploadChangesToGraphics();                            // 0x00707930 (Ego_r.pdb QAEXXZ)
};
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#include "engine/C3DVector.h"
class CThing { public:
    unsigned char  _pad00[0x20];
    std::bitset<274>  m_present;                                  // +0x20
    CInterfaceMap  m_interfaces;                               // +0x44
    unsigned char  _pad54[0x10];
    CTCBase*       PTCBase64;                                  // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CThingBaseDef* PBaseDef;                                   // +0x70
    const C3DVector& GetPos() const;                      // 0x004C73D0
    void AddAndCopyTC(const CTCBase& tc, bool activate);       // 0x004C9F60 (Ego_r.pdb, by reference)
    CTCBase* AddTC(const CCharString& name, bool activate, const class CTCParamsBase* params);   // 0x004C9D60 (Ego_r.pdb; name fix NF21)
    bool HasTC(long type) { return m_present.test(type); }
    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *out = (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterfaceFound(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *out = (T*)((it != map->end() && !(wanted < it->first)) ? it : map->end())->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTCFound(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it != map->end() && !(wanted < it->first)) ? it : map->end())->second;
    }
};
class CTCDNone { public:                                    // RTTI .?AVCTCDNone@@ : CTCDriverBase
    static CThing* __fastcall Create(long defIndex, const C3DVector& position, bool flag);   // 0x00835A20 (Ego_r.pdb / FableWin.pdb)
};

class CInventoryDef : public CDefPointeeBase { public:
    unsigned char _pad08[0xd4 - 0x08];
    long MannequinDef;                                         // +0xd4
};
template <class T> struct CDefPointerCopy {
    T* Object;
    FABLE_FORCEINLINE CDefPointerCopy(T* p) : Object(p) { if (Object) ++Object->RefCount; }
    FABLE_FORCEINLINE ~CDefPointerCopy() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
// Layout: retail CTCInventoryClothing (12-byte containers): ParentThing +0x04, the inventory def +0x68, PMannequin +0x158.
class CTCInventoryClothing { public:
    void*   __vftable;
    CThing* ParentThing;                                       // +0x04
    unsigned char _pad08[0x68 - 0x08];
    CInventoryDef* PInventoryDef;                              // +0x68 (CDefPointer)
    unsigned char _pad6c[0x158 - 0x6c];
    CThing* PMannequin;                                        // +0x158
    FABLE_FORCEINLINE CDefPointerCopy<CInventoryDef> GetInventoryDef() const { return CDefPointerCopy<CInventoryDef>(PInventoryDef); }
    FABLE_FORCEINLINE CThing* GetParentThing() const { return ParentThing; }
    void InitialiseMannequin();
};
void CTCInventoryClothing::InitialiseMannequin()
{
    if (GetInventoryDef()->MannequinDef == 0) {
        PMannequin = 0;
        return;
    }
    PMannequin = CTCDNone::Create(GetInventoryDef()->MannequinDef, GetParentThing()->GetPos(), true);
    if (PMannequin) {
        PMannequin->AddAndCopyTC(*ParentThing->PTCBase64, false);
        CTCBase* tc;
        if (ParentThing->GetInterface(&tc, 3))
            PMannequin->AddAndCopyTC(*tc, false);
        if (ParentThing->GetInterface(&tc, 0x5e))
            PMannequin->AddAndCopyTC(*tc, false);
        if (ParentThing->GetInterfaceFound(&tc, 0x78))
            PMannequin->AddAndCopyTC(*tc, false);
        CTCBase* tc5c = 0;
        ParentThing->GetInterface(&tc5c, 0x5c);
        PMannequin->AddAndCopyTC(*tc5c, false);
        PMannequin->AddTC("CTCAnimationComplex", false, 0);
        CTCAnimationComplex* complex = 0;
        PMannequin->GetInterface(&complex, 0x5a);
        CDefPointer<CAppearanceDef> appearance;
        CDefPointer<CThingBaseDef>(PMannequin->PBaseDef)->GetSubDef(&appearance);
        const CAnimationEntry* entry = appearance->Animation.GetAnimation("DEFAULT", 0, CAnimFilterNoGroup());
        CAnimInit init(6, 0, entry->AnimIndex, 1, true, entry->GetAnimateThingFlags(), 100);
        complex->GetAnimation().SetMainAnim(init, 1, 0);
        CTCHeroAttachableAppearanceModifiers* modifiers;
        if (PMannequin->GetInterface(&modifiers, 0x5e))
            modifiers->UploadChangesToGraphics();
    }
}