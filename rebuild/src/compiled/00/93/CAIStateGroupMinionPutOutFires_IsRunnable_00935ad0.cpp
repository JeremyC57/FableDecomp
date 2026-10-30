// CAIStateGroup_MinionPutOutFires::IsRunnable() -- with perception: clear the fire target, find the nearest burning thing within the creature's extended sight radius (2D) and latch it; runnable when that fire is alive
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the +0x91 dead test through CThing::IsDead() const (Ego_r.pdb
//   ?IsDead@CThing@@QBE_NXZ) keeps retail's branchy test/jne exit instead of a folded flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00935AD0, length 368 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x34 = CTCPerceiveThing, bit 20 of +0x24),
//          CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (temporary CIntelligentPointer<CThing>, vtable 0x01244344),
//          CTCPerceiveThing::GetExtendedSightRadius @ 0x007448F0, CThing::GetPosition @ 0x004C73D0,
//          CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPBestLeafThing<CFilter_Bind2<
//          burning, CThingFilter_In2DRange>, CThingCompare_Nearest, CThing> @ 0x00936070 (the manifest instance name is
//          propagated from an identical body).
// Shape: the radius is a named float read before the search; arguments right to left: the nearest comparator copies
//          the position first, then the 2D range filter copies it again with the radius; the bind holds both filters
//          by reference; the target pointer is assigned through frame temporaries.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04) with the fire
//          intelligent pointer at +0x14.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    const C3DVector& GetPosition();             // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CTCPerceiveThing { public:                       // interface 0x34
    float GetExtendedSightRadius();             // 0x007448F0
};

struct CThingFilter_IsBurning {                 // descriptive: the empty first filter
    CThingFilter_IsBurning() { }
};
class CThingFilter_In2DRange { public:
    C3DVector Centre;
    float Range;
    CThingFilter_In2DRange(const C3DVector& centre, float range) : Centre(centre), Range(range) { }
};
template <class A, class B> class CFilter_Bind2 { public:
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& a, const B& b) : First(a), Second(b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CThingCompare_Nearest { public:
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
class CThingSearchTools { public:
    bool GetPBestLeafThing(const CFilter_Bind2<CThingFilter_IsBurning, CThingFilter_In2DRange>& filter,
                           const CThingCompare_Nearest& compare, CThing** out);   // 0x00936070 (stand-in below)
};
long NearSink_PutOutFires(const void* self, long seen);   // stand-in: the search the real body runs
#pragma auto_inline(off)
// Same-TU stand-in for GetPBestLeafThing (0x00936070, an out-of-line COMDAT template instance): it reads its inputs and
// writes the out pointer only, so VC7.1 sees the filter/comparator/out addresses do not escape (landed 0x00936A60 idiom).
__declspec(noinline) bool CThingSearchTools::GetPBestLeafThing(const CFilter_Bind2<CThingFilter_IsBurning, CThingFilter_In2DRange>& filter,
                                                              const CThingCompare_Nearest& compare, CThing** out)
{
    *out = 0;
    long best = NearSink_PutOutFires(this, *(const long*)&compare + *(const long*)&filter.Second.Range);
    if (best)
        *(long*)out = best;
    return *out != 0;
}
#pragma auto_inline(on)

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};

struct CAIStateGroup_MinionPutOutFires : CAIStateGroupBase {
    CIntelligentPointerThing PFire;             // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionPutOutFires::IsRunnable()
{
    CTCPerceiveThing* perceive;
    if (GetCreature()->GetInterface(&perceive, 0x34)) {
        CThing* fire = 0;
        PFire = CIntelligentPointerThing(0);
        float radius = perceive->GetExtendedSightRadius();
        if (PeekThingSearchTools()->GetPBestLeafThing(
                CThingFilter_IsBurning() && CThingFilter_In2DRange(GetCreature()->GetPosition(), radius),
                CThingCompare_Nearest(GetCreature()->GetPosition()), &fire)) {
            PFire = CIntelligentPointerThing(fire);
            if (IS_THING_ALIVE(PFire))
                return true;
        }
    }
    return false;
}