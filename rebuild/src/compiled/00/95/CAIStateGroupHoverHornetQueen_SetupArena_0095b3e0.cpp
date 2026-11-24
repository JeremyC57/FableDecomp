// Leaf: ?SetupArena@CAIStateGroup_HoverHornetQueen@@IAEXXZ
// CAIStateGroup_HoverHornetQueen::SetupArena() -- set up the circle points, make the queen not solid to everything around her (60 units), and take the arena centre from the "WaspQueenArenaCentre" track thing near her, else from her initial position
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095B3E0, length 437 bytes (+ int3 padding).
// Callees: CAIStateGroup_HoverHornetQueen::SetUpCirclePoints @ 0x0095AB50, CThing::GetPos @ 0x004C73D0, __ftol2 @ 0x00BFEA70,
// CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, the rough-area collector @ 0x0095C7C0 (box, an empty filter, the output
// vector; the manifest "GetPBestThingInRoughArea<HasTrackName, Nearest, CThingTrackNode>" is a BSim guess with five arguments),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetTC 0x4d CTCSolidToThing = bit 13 of +0x28), CTCSolidToThing::
// AddThingNotSolidTo @ 0x00770FA0, CCharString(const char*, long) @ 0x0099EBF0 / copy ctor @ 0x0099EC30 / ~CCharString
// @ 0x0099EAE0 ("WaspQueenArenaCentre" @ 0x012952E8), CThingSearchTools::GetPFirstThingInRoughArea @ 0x007FD110, the CRT free
// 0x007FD110 is the <CThingFilter_HasScriptName, CThing> instance (filter operator 0x006A7E30 compares the ScriptName; naming wave 19)
// @ 0x00BFEA14 (the local vector's storage).
// Shape: the creature named before the local vector (its load precedes the vector's zero stores); the empty filter needs a user
// ctor (else a dead byte store); the centre out-pointer lives in its own block after the solid-to loop so the 0x4d lookup key
// takes its frame slot (an address-taken local's slot is reserved for its whole scope).
// Layout: HoverHornetQueen from Ego_r.pdb (ArenaCentrePos +0x28); CThingCreatureBase InitialPos +0xe8 (landed creature models).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_ORIGINAL_FOUR_COORD_CTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPos() const;             // 0x004C73D0
    FABLE_FORCEINLINE unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad4c[0xe8 - 0x4c];
    C3DVector InitialPos;                       // +0xe8
};
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_None.h"
#include "engine/CThingFilter_HasScriptName.h"
#include "engine/CThingSearchTools.h"
class CThingObject;   // the search type of the GetAllThingsInRoughArea instance (FableWin.pdb)
class CTCSolidToThing { public:                        // interface 0x4d
    void AddThingNotSolidTo(const CThing* thing);     // 0x00770FA0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (Ego_r.pdb ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ, protected)  // 0x008FD000
public:
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    unsigned char _pad14[0x1c - 0x14];
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    void SetUpCirclePoints();                   // 0x0095AB50
    protected:
    void SetupArena();
    public:
};

void CAIStateGroup_HoverHornetQueen::SetupArena()
{
    SetUpCirclePoints();
    CThing* self = GetCreature();
    std::vector<CThing*> things;
    {
        const C3DVector& pos = self->GetPos();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        PeekThingSearchTools().GetAllThingsInRoughArea<CThingObject>(C2DBoxI(x - 60, y - 60, x + 60, y + 60), CThingFilter_None(), things);
    }
    CThingCreatureBase* me = GetCreature();
    if (me->HasTC(0x4d)) {
        CTCSolidToThing* solid = me->GetTC<CTCSolidToThing>(0x4d);
        unsigned int count = things.size();
        for (unsigned int i = 0; i < count; ++i)
            solid->AddThingNotSolidTo(things[i]);
    }
    {
        CThing* centre = 0;
        {
            CCharString name("WaspQueenArenaCentre");
            CThingFilter_HasScriptName filter(name);
            const C3DVector& pos = GetCreature()->GetPos();
            long x = (long)pos.X;
            long y = (long)pos.Y;
            PeekThingSearchTools().GetPFirstThingInRoughArea(C2DBoxI(x - 60, y - 60, x + 60, y + 60), filter, &centre);
        }
        if (centre)
            ArenaCentrePos = centre->GetPos();
        else
            ArenaCentrePos = GetCreature()->InitialPos;
    }
}