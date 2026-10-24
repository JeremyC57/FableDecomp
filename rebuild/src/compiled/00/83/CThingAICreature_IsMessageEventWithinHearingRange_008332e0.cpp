// CThingAICreature::IsMessageEventWithinHearingRange(const CMessageEvent& event) -- true when the creature perceives the event: it has a perception component and the event lies inside the smaller of the event's hearing radius and its own
// Leaf: ?IsMessageEventWithinHearingRange@CThingAICreature
// Retail: Fable.exe (TLC, VC7.1) @ 0x008332E0, 176 bytes (+1 int3), thiscall `ret 4`, bool in al.
// Identity: Ego_r.pdb name set ?IsMessageEventWithinHearingRange@CThingAICreature@@QBE_NABVCMessageEvent@@@Z; the TU neighbours
//   0x008330D0 / 0x00833390 are landed CThingAICreature rows; the sight and hearing twins (0x00833230 / 0x008332E0) are
//   called together by 0x006AF150. Interface 0x34 is CTCPerceiveThing (its radius getters). The catalog label
//   "CVertexBufferWin32_ComputeWaterSurfaceMetrics" is propagated.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the inline GetTC), CTCPerceiveThing::GetSoundRadius @ 0x00744440 (the SoundRadius +0x3c getter, landed as GetMember_float0x3c),
//   CThing::GetPosition @ 0x004C73D0.
// Layout: retail CThing interface bitset +0x20 (interface 0x34 = +0x24 bit 20), interface map +0x44 (as the landed
//   GetSleepTimes 0x00833390); Ego_r.pdb CMessageEvent Pos +0x0c, SightRadius +0x18, HearingRadius +0x1c.
// Shape: `if (HasTC(0x34)) { float radius = min(GetTC(0x34)->GetSoundRadius(), event.HearingRadius); if ((GetPosition() -
//   event.Pos).GetLengthSquared() < radius * radius) return true; } return false;` -- STLport min (`b < a ? b : a`,
//   the component radius in the dead type-key slot, the result homed on the dead `event` slot), the difference vector
//   kept on the x87 stack. Lever: the squared length sums Z*Z, then Y*Y, then X*X (the landed GetNearestMember
//   0x008128F0 form); `X * X + Y * Y + Z * Z` adds X*X before Y*Y (176v176, term order only).
namespace _STL {
template <class _Tp> inline const _Tp& (min)(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }
}

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { float s = Z * Z; s += Y * Y; s += X * X; return s; }
};

class CTCBase;
class CTCPerceiveThing {
public:
    float GetSoundRadius() const;                           // 0x00744440 (SoundRadius +0x3c)
    float GetSightRadius() const;                           // 0x007448F0
};

class CMessageEvent {                                       // Ego_r.pdb (88 bytes)
public:
    virtual void v00();
    unsigned char _pad04[4];
    long          Type;                                     // +0x08 EMessageEventType
    C3DVector     Pos;                                      // +0x0c
    float         SightRadius;                              // +0x18
    float         HearingRadius;                            // +0x1c
};

class CInterfaceMap {                                       // CVectorMap<ETCInterfaceType, CTCBase*>
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

class CThingAICreature : public CThing {
public:
    bool IsMessageEventWithinSightRange(const CMessageEvent& event) const;
    bool IsMessageEventWithinHearingRange(const CMessageEvent& event) const;
};

bool CThingAICreature::IsMessageEventWithinHearingRange(const CMessageEvent& event) const
{
    if (HasTC(0x34)) {
        float radius = _STL::min(GetTC<CTCPerceiveThing>(0x34)->GetSoundRadius(), event.HearingRadius);
        if ((GetPosition() - event.Pos).GetLengthSquared() < radius * radius)
            return true;
    }
    return false;
}