// CAIStateGroup_CreatureUseCoverBase::GetCoverThingList(vector<CThing*>& list) -- with a live target, gather the things within 15 units of the creature that can be used as cover on the line from the target through the creature
// Grade: DIFFER(307v307) -- frame 0x58 v 0x50: retail lets the CFilter_Bind2 temporary reuse the 12-byte away-vector temporary's slot (every later slot +8); renamed onto the Ego_r symbols; tried by-value ctor params, named filter/cover/box (frame 0x40-0x44), named C2DBoxI, in-call centre (frame 0x50 but centre no longer stored first)
// Leaf: ?GetCoverThingList@CAIStateGroup_CreatureUseCoverBase@@MAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00970DF0, 307 bytes (virtual, `ret 4`). Ego_r.pdb
//   ?GetCoverThingList@CAIStateGroup_CreatureUseCoverBase@@MAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z.
// Callees (Ego_r.pdb spellings): CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CThing::GetPos() const 0x004C73D0,
//   C2DBoxF::ToC2DBoxI() const 0x00A55A30, CAIStateGroupBase::PeekThingSearchTools() const 0x008FD000 (protected),
//   CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>, CFilter_Bind2<CThingFilter_CanBeUsedAsCover,
//   CThingFilter_MakesLineWithPosInDirectionOfLine> >(const C2DBoxI&, const CFilter_Bind2&, vector&) const 0x00971530.
// Globals: 15.0f @ 0x01297498.
// Layout: CAIStateGroupBase through the layout-rename include; CAIStateGroup_EnemyCreatureBase::PTargetThing +0x14
//   (CIntelligentPointer<CThingCreatureBase>, engine/CAIStateGroup_EnemyCreatureBase.h); retail CThing Dead = bit 0 of +0x91;
//   CThingFilter_MakesLineWithPosInDirectionOfLine {Pos, Line}, CFilter_Bind2 {F1*, F2*} (Ego_r.pdb).
#include "engine/CBaseClass.h"
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CAIBrain CAIBrainLayout
#include "engine/CAIBrain.h"
#undef CAIBrain
class CAIBrain;
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
class CBaseObject;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
};
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) { X = v.X; Y = v.Y; }
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DVector& centre, float halfSize)
    {
        TLX = centre.X - halfSize; TLY = centre.Y - halfSize;
        BRX = centre.X + halfSize; BRY = centre.Y + halfSize;
    }
    C2DBoxI ToC2DBoxI() const;                              // 0x00A55A30
};
class CThing { public:
    virtual ~CThing();
    unsigned char _pad04[0x8d];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char _bits91 : 7;
    unsigned char _pad92[0x6];
    const C3DVector& GetPos() const;                        // 0x004C73D0
};
class CThingCreatureBase : public CThing { public: };
class CThingAICreature : public CThingCreatureBase { public: };
#pragma pack(push, 1)
class CBaseIntelligentPointer { public:
    void* __vftable;
    CBaseObject* PItem;                                     // +0x04
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* operator->() const { return (T*)GetPItem(); }
};
#pragma pack(pop)
template <class T> bool __fastcall IS_THING_ALIVE(const T& p)
{
    if (p.GetPItem() != 0) {
        if (!p->Dead)
            return true;
    }
    return false;
}
class CThingFilter_CanBeUsedAsCover { public:
    CThingFilter_CanBeUsedAsCover() { }
    bool operator()(const CThing& thing) const;             // 0x006A7AB0
};
class CThingFilter_MakesLineWithPosInDirectionOfLine { public:
    C3DVector Pos;
    C3DVector Line;
    CThingFilter_MakesLineWithPosInDirectionOfLine(const C3DVector& pos, const C3DVector& line) : Pos(pos), Line(line) { }
    bool operator()(const CThing& thing) const;             // 0x006A7AD0
};
template <class F1, class F2> class CFilter_Bind2 { public:
    F1* F1_;
    F2* F2_;
    CFilter_Bind2(const F1& a, const F2& b) : F1_((F1*)&a), F2_((F2*)&b) { }
};
class CThingSearchTools { public:
    template <class T, class C, class F> void GetAllThingsInRoughArea(const C2DBoxI& area, const F& filter, C& out) const;
};
class CAIBrain : public CAIBrainLayout { public:
    CThingAICreature& GetCreature() const { return *Creature; }
};
#pragma pack(push, 1)
class CAIStateGroupBase : public CBaseClass {               // polymorphic: the generated header models the vptr as a member
protected:
    CAIBrain* PBrain;                                       // +0x04
    unsigned char unobservedGroupState[sizeof(CAIStateGroupBaseLayout) - 8];
    CThingAICreature& GetCreature() const { return ((CAIBrain*)PBrain)->GetCreature(); }
    const CThingSearchTools& PeekThingSearchTools() const;  // 0x008FD000
};
class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase { protected:
    CIntelligentPointer<CThingCreatureBase> PTargetThing;   // +0x14
};
#pragma pack(pop)
class CAIStateGroup_CreatureUseCoverBase : public CAIStateGroup_EnemyCreatureBase {
protected:
    virtual void GetCoverThingList(std::vector<CThing*>& list);
};

void CAIStateGroup_CreatureUseCoverBase::GetCoverThingList(std::vector<CThing*>& list)
{
    if (!IS_THING_ALIVE(PTargetThing))
        return;
    C2DVector centre(GetCreature().GetPos());
    PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxF(centre, 15.0f).ToC2DBoxI(),
        CFilter_Bind2<CThingFilter_CanBeUsedAsCover, CThingFilter_MakesLineWithPosInDirectionOfLine>(
            CThingFilter_CanBeUsedAsCover(),
            CThingFilter_MakesLineWithPosInDirectionOfLine(PTargetThing->GetPos(), GetCreature().GetPos() - PTargetThing->GetPos())),
        list);
}
