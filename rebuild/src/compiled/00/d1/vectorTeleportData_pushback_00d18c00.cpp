// vector<TeleportData>::push_back(const TeleportData& x) -- STLport push_back for CQ_BountyHuntScript's TeleportInfo: copy-construct at the finish pointer while capacity remains, else the out-of-line overflow insert
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D18C00, 67 bytes, `__fastcall` (vector in ecx, the element on the stack:
//   `ret 4`). Callers: five sites in the Q_BountyHunt quest script (0x00D181DE, 0x00D190BE, 0x00D19E83, 0x00D1AA0C,
//   0x00D1B4C5).
// Label correction: the catalog "CQ_BountyHuntScript_EnqueueTeleport" is descriptive; the body is STLport's push_back for
//   the 24-byte NScript::CQ_BountyHuntScript::TeleportData (Ego_r.pdb: CCharString defName +0, ulong appearanceSeed +4,
//   float health +8, CCharString scriptName +0xC, teleportTo +0x10, teleportMkr +0x14; vector TeleportInfo).
//   The callee labels are wrong too: 0x00D18C50 ("TeleportData::operator=") is TeleportData's implicit copy constructor
//   (four CCharString copy ctors 0x0099EC30 + the two words), 0x00D18CA0 ("TeleportData_OnReadFinished") is
//   vector<TeleportData>::_M_insert_overflow.
// Callees: TeleportData::TeleportData(const TeleportData&) @ 0x00D18C50 (out of line), _M_insert_overflow(pos, x,
//   __false_type, fill_len, atend) @ 0x00D18CA0.
// Shape: speed TU (no pragma; the 0x00CD3E7B push_back model without its size-TU frame). TeleportData's copy is
//   declared out of line: when VC7.1 sees the implicit memberwise copy it inlines it into _Construct, which then
//   stays out of line (`call ; add esp,8`, 70 v 67). `_Construct(finish, x)` is
//   the placement new with VC's null test on the pointer; finish advances by 0x18 in memory (`add [esi+4],18h`). The
//   __false_type tag is a value-initialised empty POD temporary (one byte store of 0) in the `push ecx` slot.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
namespace NScript {
class CQ_BountyHuntScript {
public:
    struct TeleportData {                               // 24 bytes
        CCharString   defName;                          // +0x00
        unsigned long appearanceSeed;                   // +0x04
        float         health;                           // +0x08
        CCharString   scriptName;                       // +0x0c
        CCharString   teleportTo;                       // +0x10
        CCharString   teleportMkr;                      // +0x14
        TeleportData(const TeleportData& other);        // 0x00D18C50 (the compiler-generated copy, emitted out of line)
    };
};
}
inline void* operator new(unsigned int, void* where) { return where; }
namespace _STL {
struct __false_type { };                                // empty POD tag: value-initialised (byte store of 0)
template <class T> class allocator { };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00D18CA0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
template class vector<NScript::CQ_BountyHuntScript::TeleportData>;
}