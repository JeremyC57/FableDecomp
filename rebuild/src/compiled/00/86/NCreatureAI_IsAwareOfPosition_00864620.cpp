// NCreatureAI::IsAwareOfPosition(const CThingCreatureBase*, const C3DVector&) -- the position is within the creature's extended sight radius (2D), in its field of view and in navigation line of sight
// Leaf: ?IsAwareOfPosition@
// Retail 0x00864620, 0xE0 bytes, __fastcall (creature in ecx, position in edx). PDB (Ego_r):
// ?IsAwareOfPosition@NCreatureAI@@YI_NPBVCThingCreatureBase@@ABVC3DVector@@@Z. Callers 0x006C0BBE, 0x006C0C1C.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (twice), CTCPerceiveThing::GetExtendedSightRadius()
// const @ 0x007448F0, CThing::GetPos() const @ 0x004C73D0 (twice), CThingCreatureBase::IsPosInFOV(const C3DVector&, bool)
// const @ 0x00665260, CTCCreatureNavigation::IsPosNavLineOfSightToPos(const C3DVector&, const C3DVector&) const @ 0x006CB610
// (manifest labels "ComputeAngleBetweenPositions"/"CScriptThing::operator==" are propagations; the Ego_r names fit the
// argument shapes).
// Interface types (GetInterfaceType, vtable slot 21): 0x34 = CTCPerceiveThing (bitset word 1 bit 20), 7 = CTCCreatureNavigation
// (word 0 bit 7: `mov al,[edi+20h] ; test al,al ; jns`).
// Shape: null + HasTC(0x34) guard; the radius is spilled to the lookup key's slot across GetPos; the 2D offset stays on
// the x87 stack until after `fcompp` (`lengthSq < radius * radius`); the navigation component comes from the GetInterface
// out-pointer template WITHOUT a check of its result, so the not-present path reads the uninitialised out local from its
// frame slot (`mov esi,[esp+0Ch]`).
// CThing is modelled locally: rebuild/include/engine has no CThing.h and CThingCreatureBase.h pads the CThing bytes.
class CTCBase;

class C3DVector
{
public:
    float X, Y, Z;
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    const SEntry* LowerBound(const ETCInterfaceType& type) const;      // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCPerceiveThing                               // interface type 0x34
{
public:
    float GetExtendedSightRadius() const;            // 0x007448F0
};

class CTCCreatureNavigation                          // interface type 7
{
public:
    bool IsPosNavLineOfSightToPos(const C3DVector& from, const C3DVector& to) const;   // 0x006CB610
};

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 UsingComponents bitset<274>
    CInterfaceMap m_interfaces;                      // +0x44 Components

    const C3DVector& GetPos() const;            // 0x004C73D0

    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CInterfaceMap::SEntry* FindEntry(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        const CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->first <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing
{
public:
    bool IsPosInFOV(const C3DVector& pos, bool flag) const;   // 0x00665260
};

namespace NCreatureAI
{
    bool __fastcall IsAwareOfPosition(const CThingCreatureBase* creature, const C3DVector& pos);
}

bool __fastcall NCreatureAI::IsAwareOfPosition(const CThingCreatureBase* creature, const C3DVector& pos)
{
    if (creature && creature->HasTC(0x34))
    {
        float radius = ((const CTCPerceiveThing*)creature->FindEntry(0x34)->second)->GetExtendedSightRadius();
        const C3DVector& here = creature->GetPos();
        float dx = here.X - pos.X;
        float dy = here.Y - pos.Y;
        if (dx * dx + dy * dy < radius * radius && creature->IsPosInFOV(pos, true))
        {
            const CTCCreatureNavigation* navigation;
            creature->GetInterface(7, &navigation);
            if (navigation->IsPosNavLineOfSightToPos(creature->GetPos(), pos))
                return true;
        }
    }
    return false;
}