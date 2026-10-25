// NCameraModes::CTools::IsLineOfSight(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, float& distance, const CThing** hitThing, ELOSTest test, bool flag) const -- collect the live things of the camera scan list and run the thing-vector IsLineOfSight overload with them
// Leaf: ?IsLineOfSight@CTools@NCameraModes@@QBE_NABVC3DVector@@0AAV3@1AAMPAPBVCThing@@W4ELOSTest@12@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00877C40, length 215 bytes (`ret 20h`; 224 with padding).
// Naming: Ego_r.pdb has two NCameraModes::CTools::IsLineOfSight overloads, in this TU order: the thing-vector one
//   ?IsLineOfSight@CTools@NCameraModes@@QBE_NABV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@ABVC3DVector@@1AAV5@2AAMPAPBVCThing@@W4ELOSTest@12@_N@Z
//   (0x85E280 = retail 0x00877840, called here) and this one without the vector (0x85E770; eight arguments = `ret 20h`).
//   "IsLineOfSightFiltered" is the forge autoname, not a PDB name; the landed AddIncidentalAttractorLookAt 0x0084E2C0 already
//   calls this address as the plain overload. FINDINGS chunk 401.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3 per element), the vector's _M_insert_overflow(pos, x,
// __true_type, 1, true) @ 0x0049C370 (manifest "Std_Vector_Insert_Range"), CTools::IsLineOfSight(const
// vector<const CThing*>&, ...) const @ 0x00877840, free @ 0x00BFEA14 (the vector storage).
// Layout (Ego_r.pdb, as the landed CTools::FrameUpdate 0x00877D20): CTools ScanThings (vector<CIntelligentPointer<
// CThing const> >, 8-byte elements) +0x04; retail CThing state bits +0x91 (Dead = bit 0).
// Shape: the STLport `_Vector_base` + alloc-proxy local (one zero register), the landed inline push_back (placement
// new under its null test; the empty __true_type temporary homed in a dead argument slot); the end pointer is
// re-read every step; the eight arguments are passed straight through. Retail keeps the vector's finish in edi across the
// GetPItem calls: the local does not escape, because _M_insert_overflow and IsLineOfSight are in retail's TU (VC7.1 escape
// is flow-insensitive) -> same-TU stand-ins for both that pass the bounds by value.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
class C3DVector { public: float x, y, z; };
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                                 // +0x91 bit 0
};
struct CBaseIntelligentPointer {
    void*  m_vtable;
    void*  m_pItem;
    const CThing* GetPItem() const;                         // 0x00A01B50
};
struct __true_type { __true_type() {} };

template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
struct CThingConstVector : _Vector_base<const CThing*> {   // STLport vector<const CThing*>, 12 bytes
    CThingConstVector() : _Vector_base<const CThing*>(allocator<const CThing*>()) { }
    void _M_insert_overflow(const CThing** position, const CThing* const& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x0049C370
    void push_back(const CThing* const& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) const CThing*(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CScanVector {                                        // STLport vector<CIntelligentPointer<CThing const> >
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
    CBaseIntelligentPointer* _M_end_of_storage;
};

namespace NCameraModes {
class CTools {
public:
    enum ELOSTest { };
    void*       CameraManager;                              // +0x00
    CScanVector ScanThings;                                 // +0x04
    bool IsLineOfSight(const CThingConstVector& things, const C3DVector& from, const C3DVector& to, C3DVector& hitPos,
                       C3DVector& hitNormal, float& distance, const CThing** hitThing, ELOSTest test, bool flag) const;   // 0x00877840
    bool IsLineOfSight(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal,
                       float& distance, const CThing** hitThing, ELOSTest test, bool flag) const;
};
}
using NCameraModes::CTools;

void InsertOverflow_0049c370(const CThing*** start, const CThing*** finish, const CThing*** endOfStorage, const CThing** position,
                             const CThing* const& x, unsigned long fillLength, bool atEnd);
bool IsLineOfSight_00877840(const CTools* tools, const CThing* const* first, const CThing* const* last, const C3DVector& from, const C3DVector& to,
                            C3DVector& hitPos, C3DVector& hitNormal, float& distance, const CThing** hitThing, CTools::ELOSTest test, bool flag);
// Stand-ins for the vector<const CThing*> _M_insert_overflow instantiation (0x0049C370) and CTools::IsLineOfSight (0x00877840),
// both in retail's TU: the bounds pass by value, so the local vector does not escape.
__declspec(noinline) void CThingConstVector::_M_insert_overflow(const CThing** position, const CThing* const& x, const __true_type&, unsigned long fill_len, bool atend)
{
    const CThing** start = _M_start;
    const CThing** finish = _M_finish;
    const CThing** endOfStorage = _M_end_of_storage._M_data;
    InsertOverflow_0049c370(&start, &finish, &endOfStorage, position, x, fill_len, atend);
    _M_start = start;
    _M_finish = finish;
    _M_end_of_storage._M_data = endOfStorage;
}
__declspec(noinline) bool CTools::IsLineOfSight(const CThingConstVector& things, const C3DVector& from, const C3DVector& to, C3DVector& hitPos,
                                                C3DVector& hitNormal, float& distance, const CThing** hitThing, ELOSTest test, bool flag) const
{
    return IsLineOfSight_00877840(this, things._M_start, things._M_finish, from, to, hitPos, hitNormal, distance, hitThing, test, flag);
}

bool CTools::IsLineOfSight(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal,
                           float& distance, const CThing** hitThing, ELOSTest test, bool flag) const
{
    CThingConstVector things;
    for (CBaseIntelligentPointer* it = ScanThings._M_start; it != ScanThings._M_finish; ++it) {
        if (it->GetPItem() && !it->GetPItem()->Dead)
            things.push_back(it->GetPItem());
    }
    return IsLineOfSight(things, from, to, hitPos, hitNormal, distance, hitThing, test, flag);
}