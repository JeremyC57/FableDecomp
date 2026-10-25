// std::vector::push_back(const CGenerationPos& x) -- STLport push_back of vector<CTCSmokeGenerator::CGenerationPos>, a smoke generation position (point + emitter thing pointer): copy-construct it at the end when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00723270, length 105 bytes (+ int3 padding), `ret 4` (thiscall, &x).
// Callers: CTCSmokeGenerator::CreatePositions 0x00722590 (landed; 0x007226B2, one per focus_chimney helper point).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90
//   (the inline CIntelligentPointer<CThing> copy constructor, vtable 0x01238C6C), _M_insert_overflow(pos, x,
//   const __false_type&, 1, true) @ 0x00723110.
// Label: catalog "std::vector_push_back_or_grow" is descriptive.
// Element: CTCSmokeGenerator::CGenerationPos {C3DVector Pos; CIntelligentPointer<CThing> PEmitter;} (Ego_r.pdb, 20 bytes,
//   `add dword ptr [ebx+4],14h`), copied memberwise by its implicit copy constructor; C3DVector needs a user copy
//   constructor (with the implicit one VC7.1 copies through two extra register moves, 109 v 105 bytes).
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- as 0x006A3C70 of this chunk; the __false_type temporary
//   sits in the dead `&x` argument slot.
inline void* operator new(unsigned int, void* where) { return where; }

class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* PData;                                     // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for T = CThing
public:
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class C3DVector {                                           // Ego_r.pdb, 12 bytes
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }   // user copy (as the landed 0x00722590 models it)
};

class CTCSmokeGenerator {
public:
    class CGenerationPos {                                  // Ego_r.pdb, 20 bytes
    public:
        C3DVector                   Pos;                    // +0x00
        CIntelligentPointer<CThing> PEmitter;               // +0x0c
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00723110
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template void std::vector<CTCSmokeGenerator::CGenerationPos>::push_back(const CTCSmokeGenerator::CGenerationPos&);