// OrderByHeight(CIntelligentPointer<const CThing> a, CIntelligentPointer<const CThing> b) -- sort predicate: true when both things are live and not dead and a stands lower (position Z) than b
// Leaf: ?OrderByHeight@@YI_NV?$CIntelligentPointer@$$CBVCThing@@@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00926980, 144 bytes, `__fastcall` with both 8-byte pointers on the stack (`ret 10h`),
//   each destroyed by the callee.
// Identity (proven): Ego_r.pdb has exactly one function of type bool (__fastcall*)(CIntelligentPointer<const CThing>,
//   CIntelligentPointer<const CThing>), `?OrderByHeight@@YI_NV?$CIntelligentPointer@$$CBVCThing@@@@0@Z`; this body is the
//   function pointer pushed into the sort at 0x00927083, whose helpers 0x009263F0 _Med3 / 0x00926850 _Adjust_heap are
//   instantiated on that predicate type; the compare is on position +8 (Z).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (no local: every use re-reads), CThing::GetPos() const
//   0x004C73D0, ~CBaseIntelligentPointer 0x00A01C10. Dead flag: bit 0 of CThing byte +0x91 (as the landed
//   CLightningBranch::~CLightningBranch 0x007F2360).
// Shape: one `&&` chain returned directly (bl = 1, a shared `xor bl,bl` and one destructor pair); an if/return pair
//   duplicates the destructor tail (DIFFER 172v144). The compare is `a.Z < b.Z` (`test ah,5 ; jnp` keeps the 1).
class C3DVector {
public:
    float X, Y, Z;
};
class CBaseObject;
class CBaseIntelligentPointer {
public:
    void* PData;
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return GetPItem(); }
};
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    const C3DVector& GetPos() const;                   // 0x004C73D0
};

bool __fastcall OrderByHeight(CIntelligentPointer<const CThing> a, CIntelligentPointer<const CThing> b)
{
    return a.GetPItem() && !a->Dead && b.GetPItem() && !b->Dead && a->GetPos().Z < b->GetPos().Z;
}