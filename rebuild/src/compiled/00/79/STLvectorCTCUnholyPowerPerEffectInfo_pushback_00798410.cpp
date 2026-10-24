// _STL::vector<CTCUnholyPower::PerEffectInfo>::push_back(const CTCUnholyPower::PerEffectInfo& x) -- append a per-effect record (position + intelligent effect pointer), growing through the out-of-line overflow path when full
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798410, 105 bytes, __fastcall `ret 4`. Byte twin of 0x007935D0 (the other
//   spell's TU).
// Identity: Ego_r.pdb names vector<CTCUnholyPower::PerEffectInfo> (HitEffects; reserve/_Tidy instantiations) and the 20-byte record
//   {C3DVector Position, CIntelligentPointer<CThing> Effect}; two callers in CTCUnholyPower's TU.
// Callees: CBaseIntelligentPointer() 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90 (the Effect pointer's copy
//   constructor; CIntelligentPointer<CThing> vtable 0x01238C6C), STLport `_M_insert_overflow(pos, x, __false_type(), 1,
//   true)` 0x007981B0.
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1, true);` -- the placement copy keeps VC's null test, Position is
//   copied as three dwords (memberwise C3DVector copy constructor), `++_M_finish` re-reads the member (`add dword ptr
//   [ebx+4],14h`), and the empty tag temporary is homed in the dead `x` argument slot after `x` is loaded. The record's
//   copy constructor and the pointer's are `__forceinline` (otherwise VC7.1 calls an out-of-line implicit copy).
// Label correction: catalogued as `StdVector_PushBack` (generic); the body is vector<CTCUnholyPower::PerEffectInfo>::push_back.
class CBaseObject;
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
    virtual void v00();
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer()
    {
        SetPItem(other.GetPItem());
    }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CTCUnholyPower {
public:
    struct PerEffectInfo {                              // 20 bytes
        C3DVector                   Position;            // +0x00
        CIntelligentPointer<CThing> Effect;              // +0x0c
        __forceinline PerEffectInfo(const PerEffectInfo& o) : Position(o.Position), Effect(o.Effect) { }
    };
};

struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct vector {                      // retail: the 12-byte STLport vector
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
    void push_back(const T& x);
};

template <> void vector<CTCUnholyPower::PerEffectInfo>::_M_insert_overflow(CTCUnholyPower::PerEffectInfo* position,
                                                                     const CTCUnholyPower::PerEffectInfo& x,
                                                                     const __false_type&, unsigned int fill_len,
                                                                     bool atend);   // 0x007981B0

template <class T> void vector<T>::push_back(const T& x)
{
    if (_M_finish != _M_end_of_storage) {
        new (_M_finish) T(x);
        ++_M_finish;
    }
    else
        _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
}

template void vector<CTCUnholyPower::PerEffectInfo>::push_back(const CTCUnholyPower::PerEffectInfo& x);