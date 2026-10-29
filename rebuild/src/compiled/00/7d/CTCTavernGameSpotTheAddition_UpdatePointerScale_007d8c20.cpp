// CTCTavernGameSpotTheAddition::UpdatePointerScale() -- grow the pointer while a selection is being made (clamped to 0.1 .. 1 by the def's phase speed) and shrink it back to nothing otherwise, then push the scale into the pointer's graphic appearance
// Leaf: UpdatePointerScale
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D8C20, length 384 bytes (plain `ret`, no frame pointer; Ego_r.pdb
//   `?UpdatePointerScale@CTCTavernGameSpotTheAddition@@AAEXXZ`).
// Callees: CTCTavernGame::GetDef(CDefPointer<CTavernGameDef>*) 0x005EA000 (as the landed UpdateValues 0x007D8AB0);
//   the def's virtual slot 1 on the last release; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface
//   0x5B, bit 27 of +0x28); NGraphicAppearance::CTCGraphicAppearance::SetScale(float) 0x004BFA50 (as landed).
// Data: 0.1f 0x0122EE58, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout: retail CTCTavernGame (as the landed UpdateValues 0x007D8AB0: Ego_r.pdb minus 0x1c past the six STLport
//   vectors): TavernGameState +0xc0; CTCTavernGameSpotTheAddition (PDB minus 0x18 up to KnickKnacks, minus 0x1c after):
//   Pointer +0x104, PointerScale +0x108, Selected +0x144; CTavernGameDef PointerPhaseSpeed +0x1a0 (PDB +0x1a4 - 4).
// Shape: two def lookups through CDefPointer locals; the graphic appearance comes through the out-parameter GetInterface.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetScale(float scale);                             // 0x004BFA50
};
}
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
class CTavernGameDef : public CDefVirtual { public:
    long m_count;                                           // +0x04
    unsigned char _pad_0x08[0x1a0 - 0x08];
    float PointerPhaseSpeed;                                // +0x1a0
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};
class CTCTavernGame { public:
    virtual void v00();
    unsigned char _pad_0x04[0xc0 - 0x04];
    long          TavernGameState;                          // +0xc0
    unsigned char _pad_0xc4[0xf0 - 0xc4];
    void GetDef(CDefPointer* out);                          // 0x005EA000
};
struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    unsigned char _pad_0xf0[0x104 - 0xf0];
    CThing*       Pointer;                                  // +0x104 (CThingObject*)
    float         PointerScale;                             // +0x108
    unsigned char _pad_0x10c[0x144 - 0x10c];
    long          Selected;                                 // +0x144
    void UpdatePointerScale();
};

void CTCTavernGameSpotTheAddition::UpdatePointerScale()
{
    if (Pointer == 0)
        return;
    if (TavernGameState == 0x2a && Selected == -1) {
        if (PointerScale != 1.0f) {
            if (PointerScale < 0.1f)
                PointerScale = 0.1f;
            else if (PointerScale < 1.0f) {
                CDefPointer def;
                GetDef(&def);
                PointerScale *= def->PointerPhaseSpeed;
            }
            if (PointerScale > 1.0f)
                PointerScale = 1.0f;
        }
    } else if (PointerScale != 0.0f) {
        if (PointerScale < 0.1f)
            PointerScale = 0.0f;
        else {
            CDefPointer def;
            GetDef(&def);
            PointerScale /= def->PointerPhaseSpeed;
        }
    }
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    if (Pointer->GetInterface(&appearance, 0x5b))
        appearance->SetScale(PointerScale);
}