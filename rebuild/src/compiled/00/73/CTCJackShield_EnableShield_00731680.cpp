// CTCJackShield::EnableShield(bool enable) -- show / hide the owner's graphic shield state, switch the owner's switchable-navigation component, and on enable create the "JACK_OF_BLADES_SHIELD_01" particle emitter at the owner (unless a live one exists), on disable kill the live emitter
// Leaf: EnableShield
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00731680, 336 bytes, `ret 4`. Speed TU (esp frame, no pragma).
//   Ego_r.pdb: `?EnableShield@CTCJackShield@@QAEX_N@Z`.
// Callees: the owner's +0x60 object's virtual slots 36 (+0x90, (enable)), 44 (+0xB0) and 43 (+0xAC, (0)); the owner's
//   interface bitset tested inline (0x79 -> word +0x2C bit 25) and CVectorMap LowerBound @ 0x0040F020 (inlined
//   GetInterface), the interface-0x79 component's (bool) method @ 0x00787580 (label "CTCSwitchableNavigation::
//   InitialActivate" is propagated: that is a no-argument slot, this body is `ret 4`), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable
//   0x01238C6C), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::GetPos
//   @ 0x004C73D0 (label "CVertexBufferWin32::DoSizeof" is propagated: `mov eax,[ecx+60h] ; add eax,0Ch`),
//   CTCDParticleEmitter::Create(const CCharString&, const C3DVector&, bool) @ 0x006E0970 (static `__fastcall`),
//   CThing::Kill(bool) @ 0x004C9B80.
// Shape: the emitter name is an implicit const char* -> CCharString conversion (the temporary is built before the
//   bool argument is pushed; an explicit CCharString(...) builds it in push order); the bitset test is inline, so the
//   constant 0x79 folds to `test [thing+2Ch],2000000h`; the intelligent-pointer temporary lives in the 8-byte frame.
// Data: 0x01266F6C "JACK_OF_BLADES_SHIELD_01".
// Layout: Ego_r.pdb CTCJackShield (20 bytes): CTCBase 0x0C, PEmitter +0x0C (CIntelligentPointer<CThing>); CThing
//   interface bitset +0x20, interface map +0x44, graphic object +0x60, dead flag bit 0 of +0x91.
#include "engine/CTCBase.h"

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };

class CTCBaseComponent;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CInterfaceBits {                                     // bitset<> at CThing+0x20
    unsigned long _M_w[9];
    bool test(unsigned long pos) const { return (_M_w[pos / 32] & (1ul << (pos % 32))) != 0; }
};
struct CThingGraphic {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); 
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); 
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); 
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); 
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); 
    virtual void SetShieldShown(bool shown);                // slot 36 (+0x90)
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); 
    virtual void Slot43(long value);                        // slot 43 (+0xAC)
    virtual void Slot44();                                  // slot 44 (+0xB0)
};
class CThing { public:
    unsigned char _pad00[0x20];
    CInterfaceBits present;                                 // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingGraphic* PGraphic;                                // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                                  // +0x91 (bit 0: dead)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((CTCBaseComponent*)((it == map->last || interfaceType < it->first) ? map->last : it)->second);
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;                   // 0x004C73D0
    void Kill(bool silent);                                 // 0x004C9B80
};
class CTCSwitchableNavigation { public:
public:
    void SetAsBlockingNavigation(bool);   // 0x00787580
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(const CCharString& name, const C3DVector& position, bool attached);   // 0x006E0970
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
    bool IsAlive() const { return GetPItem() && !(GetPItem()->Flags91 & 1); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

class CTCJackShield : public CTCBase { public:
    CIntelligentPointer<CThing> PEmitter;                   // +0x0C
    void EnableShield(bool enable);
};

void CTCJackShield::EnableShield(bool enable)
{
    ParentThing->PGraphic->SetShieldShown(enable);
    if (enable)
        ParentThing->PGraphic->Slot44();
    else
        ParentThing->PGraphic->Slot43(0);
    CTCSwitchableNavigation* navigation;
    if (ParentThing->GetInterface(&navigation, 0x79))
        navigation->SetAsBlockingNavigation(enable);
    if (enable) {
        if (!PEmitter.IsAlive()) {
            CThing* emitter = CTCDParticleEmitter::Create("JACK_OF_BLADES_SHIELD_01",
                                                          ParentThing->GetPos(), false);
            if (emitter)
                PEmitter = CIntelligentPointer<CThing>(emitter);
        }
    } else if (PEmitter.IsAlive())
        PEmitter.GetPItem()->Kill(true);
}