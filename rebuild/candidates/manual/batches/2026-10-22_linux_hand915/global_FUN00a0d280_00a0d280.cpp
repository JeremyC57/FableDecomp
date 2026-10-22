// _global::FUN_00a0d280() -- constructor zeroing three dwords (+0, +4, +8); returns this
// Leaf: ??0CVector_00a0d280
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0D280, 13 bytes: `mov eax,ecx ; xor ecx,ecx` then three stores of ecx.
//   Unreferenced; the same shape as the STLport vector's empty constructor (_M_start, _M_finish,
//   _M_end_of_storage all null). Modelled as that 12-byte retail vector.
struct CVector_00a0d280 {
    void* _M_start;                              // +0x00
    void* _M_finish;                             // +0x04
    void* _M_end_of_storage;                     // +0x08
    CVector_00a0d280();
};

CVector_00a0d280::CVector_00a0d280()
    : _M_start(0), _M_finish(0), _M_end_of_storage(0)
{
}
