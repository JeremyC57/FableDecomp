// CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable() -- whether a child creature is within the extended sight radius and nearer than the thing reacted to
// Grade: DIFFER(454v461) nd=166 (was 483v461 nd=194) -- retail_asm ceil block (0x0098628D fistp) + /Oy- ebp frame + shared fail tail; residue: frame-slot packing (retail 0x5c shares [ebp-0x1c]), radius kept in memory
// Flags: /Oy-
// Leaf: CallingChildrenIsRunnable
// Retail: Fable.exe (TLC, VC7.1) @ 0x00986210; best of the probes graded under VC7.1 RTM (probe/00986210.cpp).
struct CTCBase;
struct C3DVector { float x, y, z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    long GetTeam() const;                       // 0x00662000
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
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct CTCPerceiveThing { float GetExtendedSightRadius() const; };   // 0x007448F0
inline long CeilToLong(float value)            // retail: bare fistp + bit-compare (inline asm in the original)
{
    long n = (long)(value + 0.5f);
    float back = (float)n - 1.0f;
    if (*(long*)&value == *(long*)&back)
        --n;
    return n;
}
struct C2DBoxI { long x0, y0, x1, y1; C2DBoxI(long a, long b, long c, long d) : x0(a), y0(b), x1(c), y1(d) { } };
struct CThingFilter_IsNot { CThing* Thing; };
struct CThingFilter_IsCreatureType { long Type; };
struct CThingFilter_In2DRange { C3DVector Centre; float Range; };
struct CFilter4 { CThingFilter_IsNot* a; CThingFilter_IsCreatureType* b; CThingFilter_In2DRange* c; CThingFilter_In2DRange* d; };
struct CThingAICreature;
struct CThingSearchTools {
    bool GetPFirstThingInRoughArea(const C2DBoxI* box, const CFilter4* filter, CThingAICreature** out);   // 0x009871C0
};
struct CThingPointerView { CThing* GetPItem() const; };   // 0x00A01B50
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot {
    void* __vftable;
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x6b - 0x08];
    unsigned char ThingReactingTo[9];           // +0x6b CIntelligentPointer<CThing>
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    bool CallingChildrenIsRunnable();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable()
{
    long radius;
    CTCPerceiveThing* perception;
    CThingAICreature* found;
    if (PBrain->PCreature->GetTeam() == 1)
        goto fail;
    found = 0;
    PBrain->PCreature->GetInterface(&perception, 0x34);
    float sight = perception->GetExtendedSightRadius();
    float half = 0.5f;
    float one = 1.0f;
    float back;
    // Retail-asm: 0x0098628D fistp dword ptr [ebp-4] (bare fistp ceil block 0x00986287..0x00986296)
    __asm {
        fld sight
        fadd half
        fistp radius
        fild radius
        fsub one
        fstp back
    }
    if (*(long*)&sight == *(long*)&back)
        --radius;
    const C3DVector& pos = PBrain->PCreature->GetPosition();
    long px = (long)pos.x;
    long py = (long)pos.y;
    C2DBoxI box(px - radius, py - radius, px + radius, py + radius);
    CThing* self = PBrain->PCreature;
    CThingPointerView* other = (CThingPointerView*)ThingReactingTo;
    const C3DVector& a = other->GetPItem()->GetPosition();
    const C3DVector& b = self->GetPosition();
    C3DVector d = { a.x - b.x, a.y - b.y, a.z - b.z };
    CThingFilter_In2DRange near1 = { other->GetPItem()->GetPosition(), (float)sqrt(d.x * d.x + d.y * d.y + d.z * d.z) };
    CThingFilter_In2DRange near2 = { PBrain->PCreature->GetPosition(), perception->GetExtendedSightRadius() };
    CThingFilter_IsCreatureType type = { 1 };
    CThingFilter_IsNot notSelf = { PBrain->PCreature };
    CFilter4 filter = { &notSelf, &type, &near2, &near1 };
    PeekThingSearchTools()->GetPFirstThingInRoughArea(&box, &filter, &found);
    if (found != 0)
        return true;
fail:
    return false;
}
