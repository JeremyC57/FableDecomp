// NThingShot::CTCShotDefinitionBase::OnRemove() -- when the shot component is removed: stop the owning thing's augmentation trail effect and kill the shot's effect thing if it is still alive
// Leaf: ?OnRemove@CTCShotDefinitionBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BF350, 113 bytes (+15 int3 padding).
// Identity: CTCBase vtable slot 2 (+0x08) is OnRemove (CThing::RemoveTC @ 0x004C9840 calls it); this body sits in slot 2
//          of EVERY shot vtable read from the exe -- CTCShotDefinitionBase 0x0126E1A4, Throw 0x0123CDC4, ThrowPredictive
//          0x0123CE9C, TearInGround 0x0123CF9C, DragonFireball 0x0123D0CC, Projectile 0x01240CCC, MultiArrow 0x0126DF34 --
//          so it is the base class's override (FableWin.pdb has ?OnRemove@CTCShotDefinitionBase@NThingShot@@UAEXXZ and no
//          OnRemove for CTCShotDefinitionThrow); the manifest's "CTCShotDefinitionThrow::vfunc_2" is the first vtable hit.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0xBF = CTCObjectAugmentations: the sign bit
//          of the bitset dword +0x34), CTCObjectAugmentations::StopTrailEffect @ 0x007664E0 (manifest "CTCKraken::
//          EndHeadEffects" is a byte-twin propagation; the body kills ProjectileTrailEffect at +0x65, PDB
//          ?StopTrailEffect@CTCObjectAugmentations@@QAEXXZ), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls),
//          CThing::Kill(bool) @ 0x004C9B80.
// Layout: CTCBase {vptr, PThing +4}; CTCShotDefinitionBase PEffect (CIntelligentPointer<CThing>) +0x0C (Ego_r.pdb);
//          CThing UsingComponents +0x20, Components +0x44, the dead bit +0x91 bit 0.
// Shape: the landed IS_THING_ALIVE(pointer) inline (GetPItem() null test, then the dead bit through a second GetPItem()),
//          then a third GetPItem() for the Kill(true) call.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    void*         __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];              // +0x20
    CInterfaceMap Components;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                         // +0x91 bit 0: dead
    void Kill(bool immediately);                   // 0x004C9B80
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCObjectAugmentations { public:                    // interface 0xBF
    void StopTrailEffect();                        // 0x007664E0
};
class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                               // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->Flags91 & 1))
            return true;
    }
    return false;
}

class CTCBase {
public:
    virtual ~CTCBase();
    virtual void v01();
    virtual void OnRemove();                       // slot 2 (+0x08)
    CThing* PThing;                                // +0x04
    bool    PendingRemoval;                        // +0x08
};

namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase {
public:
    virtual void OnRemove();
    CBaseIntelligentPointer PEffect;               // +0x0C
};

void CTCShotDefinitionBase::OnRemove()
{
    CTCObjectAugmentations* augmentations;
    if (PThing->GetInterface(&augmentations, 0xbf))
        augmentations->StopTrailEffect();
    if (IS_THING_ALIVE(PEffect))
        ((CThing*)PEffect.GetPItem())->Kill(true);
}
}