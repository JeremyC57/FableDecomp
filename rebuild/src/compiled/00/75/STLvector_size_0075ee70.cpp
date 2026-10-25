// std::vector::size() const -- element count of CTCCameraPointScriptedSpline's 56-byte CKeyCamera vector
// Leaf: ?size@?$vector@VCKeyCamera@CTCCameraPointScriptedSpline@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075EE70, 29 bytes, thiscall, plain `ret`.
// Identity: vector<CTCCameraPointScriptedSpline::CKeyCamera>. All three callers sit in
//   CTCCameraPointScriptedSpline::UpdateCameraParams (0x0075D6F0, 0x0075D723, 0x0075D755: `size() - 1` as the key-camera
//   index range); the divide is by 56 (0x92492493 with the `add edx,esi` correction and `sar edx,5`) = Ego_r.pdb
//   sizeof(CKeyCamera) (DeltaIndex, Pos, LookDirection, FOV, Roll, ShuttleSpeed, Duration, PauseTime, EventName,
//   SpeedFraction; no STL members, so the retail size is the PDB's). Catalog "vector<CScriptThing>::size" is propagated.
// Shape: STLport 4.6 `size() const { return size_type(_M_finish - _M_start); }` (twin of 0x0043CAC0).
class CCharString {
public:
    void* PStringData;
};

class C3DVector {
public:
    float X, Y, Z;
};

class CTCCameraPointScriptedSpline {
public:
    class CKeyCamera {                        // 56 bytes
    public:
        float DeltaIndex;                     // +0x00
        C3DVector Pos;                        // +0x04
        C3DVector LookDirection;              // +0x10
        float FOV;                            // +0x1C
        float Roll;                           // +0x20
        float ShuttleSpeed;                   // +0x24
        float Duration;                       // +0x28
        float PauseTime;                      // +0x2C
        CCharString EventName;                // +0x30
        float SpeedFraction;                  // +0x34
    };
};

namespace std {
template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
};

template vector<CTCCameraPointScriptedSpline::CKeyCamera>::size_type
    vector<CTCCameraPointScriptedSpline::CKeyCamera>::size() const;
}