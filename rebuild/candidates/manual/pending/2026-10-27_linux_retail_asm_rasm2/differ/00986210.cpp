// CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable() -- whether a child creature is within the extended sight radius and nearer than the thing reacted to
// Leaf: CallingChildrenIsRunnable
// Retail-asm: 0x00986287..0x009862A7 `fld [ebp - 0x18] ; fadd [ebp - 0x14] ; fistp [ebp - 4] ; fild [ebp - 4] ; fsub [ebp - 0x10] ; fstp [ebp - 0x1c] ; mov eax, [ebp - 0x18] ; cmp eax, [ebp - 0x1c] ; jne 0x9862a4 ; dec [ebp - 4] ; mov ecx, [esi + 4] ; mov ecx, [ecx + 0x18]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00986210; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. The extended sight radius is the ceiling block (hence the ebp frame).
#pragma optimize("y", off)
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
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
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
    if (PBrain->PCreature->GetTeam() == 1)
        return false;
    CThingAICreature* found = 0;
    CTCPerceiveThing* perception;
    PBrain->PCreature->GetInterface(&perception, 0x34);
    long radius = CeilToLong(perception->GetExtendedSightRadius());
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
    return found != 0;
}
