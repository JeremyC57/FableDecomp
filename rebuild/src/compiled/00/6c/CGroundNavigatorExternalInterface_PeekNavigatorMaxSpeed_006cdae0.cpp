// CGroundNavigatorExternalInterface::PeekNavigatorMaxSpeed(long mode) const -- the creature's maximum speed for the mode (creature virtual slot 79)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CDAE0, length 32 bytes.
// Callees: the creature's virtual slot 79 (+0x13c, a float on the FPU stack, one argument). Shape: the creature loaded first, the argument through edx, `ret 4`.
// Model: the engine header for the interface (vptr, Creature at +4, Radius at +8); the creature, its physics body (+0x60) and its component map are local models (engine/CThingCreatureBase.h has no names at these offsets).
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the creature's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const long* kind);          // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound)
    STCEntry_Retail* LowerBound(const long* kind) const;    // 0x004365B0 (the second instantiation)
};
class C3DVector { public:
    float x, y, z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
};
class CCreatureNavigationDef;
struct CDefPointer_Nav {                           // CDefPointer<CCreatureNavigationDef>: one counted def pointer (released by hand in the bodies: the slot is reused for the float)
    CCreatureNavigationDef* m_p;
};
struct CDefClassBase_Retail {                      // the component's def-class base (its manager at +0x1c)
    bool GetSubDef(CDefPointer_Nav& out);          // 0x006CD2F0 (CDefClassBase::GetSubDef<CCreatureNavigationDef>)
};
struct CPhysicsBody_Retail {                       // the creature's physics body at +0x60
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacing();                 // slot 70 (+0x118), by value through the hidden pointer
    unsigned char _pad04[8];                       // +0x04
    C3DVector     m_position;                      // +0x0c
    unsigned char _pad18[0x1c];
    float         m_radius;                        // +0x34
};
struct CTCCreatureNavigation_Retail {              // component kind 7
    unsigned char _pad[0x40];
    unsigned char m_obstacles[0x18];               // +0x40 the obstacle record
    float         m_proximityForDirectApproach;    // +0x58
};
class CThingCreatureBase { public:                        // the retail creature as seen from the navigator interface
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual float GetMaxSpeed(long mode);          // slot 79 (+0x13c)
    unsigned char         _pad04[0x1c];
    unsigned long         m_presentBits[9];        // +0x20 bitset of the component kinds present
    CTCMap_Retail         m_tcs;                   // +0x44 the sorted {kind, component} map
    unsigned char         _pad50[0x10];
    CPhysicsBody_Retail*  m_pBody;                 // +0x60 (position at +0xc, facing by slot 70)
    unsigned char         _pad64[0xc];
    CDefClassBase_Retail* PhysicsTC;               // +0x70 (engine/CThing.h: PhysicsTC; the def-class base queried for the navigation def)
    unsigned char         _pad74[0x1d];
    unsigned char         m_dead : 1;              // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    template<class T> __forceinline void GetInterface(T** ppOut, long kind)   // the inlined CThing::GetInterface (out-parameter form)
    {
        if (m_presentBits[kind >> 5] & (1UL << (kind & 31))) {
            long type = kind;
            CTCMap_Retail* map = &m_tcs;
            STCEntry_Retail* it = map->LowerBound(&type);
            if (it == map->m_pLast || kind < it->m_kind)
                it = map->m_pLast;
            *ppOut = (T*)it->m_pTC;
        }
    }
    template<class T> __forceinline void GetInterface(T** ppOut, long kind) const
    {
        if (m_presentBits[kind >> 5] & (1UL << (kind & 31))) {
            long type = kind;
            const CTCMap_Retail* map = &m_tcs;
            STCEntry_Retail* it = map->LowerBound(&type);
            if (it == map->m_pLast || kind < it->m_kind)
                it = map->m_pLast;
            *ppOut = (T*)it->m_pTC;
        }
    }
};
#include "engine/CGroundNavigatorExternalInterface.h"

struct CGroundNavigatorExternalInterface_Methods : CGroundNavigatorExternalInterface {
    float PeekNavigatorMaxSpeed(long mode) const;
};

float CGroundNavigatorExternalInterface_Methods::PeekNavigatorMaxSpeed(long mode) const
{
    float speed = Creature->GetMaxSpeed(mode);   // the named local keeps the call (a bare return tail-jumps)
    return speed;
}