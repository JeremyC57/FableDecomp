// CTCInventoryMap::ResetPrimitiveHandles() -- clear every primitive handle of the four map-screen handle vectors (map, quest, teleport, house markers) and empty the vectors
// Leaf: ?ResetPrimitiveHandles@CTCInventoryMap@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FC510, 197 bytes, plain `ret` (thiscall, no arguments). Reached from
//   CTCInventoryMap vtable 0x01257A9C slot 37 (0x005FC610, a `jmp` thunk), CTCInventoryMap::vfunc_31 0x005FCE60,
//   ProcessButtonBReleased 0x005FD0B0 and ProcessButtonAReleased 0x00602F30.
// Identity: FableWin.pdb lists `?ResetPrimitiveHandles@CTCInventoryMap@@QAEXXZ` (public, void()) next to the const helper
//   `?ResetPrimitiveHandles@CTCInventoryMap@@QBEXAAV...` (taking a handle vector by reference). The body is four
//   `for_each(v.begin(), v.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear)); v.clear();` pairs written directly on the
//   members: routing them through an inline copy of that const helper (reference parameter) swaps esi/edi and pushes the
//   functor after the `lea` (197 v 197). Label correction: the catalog `ComboBox_ClearAllLists` is behavioural.
// Callees: SGI for_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle> > 0x0043F360 (/Gr `__fastcall`:
//   hidden result in ecx, first in edx, last and the 4-byte functor on the stack), CEnginePrimitiveHandle::Clear 0x0041BE70
//   (pushed as the functor's pointer-to-member constant), vector<CEnginePrimitiveHandle>::erase(first, last) 0x00440D70
//   (the inline clear()).
// Layout (retail; Ego_r.pdb names, PDB offsets 0x1E4/0x1F4/0x204/0x214 sit high because retail STLport vectors are 12
//   bytes, PDB 16): MapScreenPrimitiveHandles +0x1AC, MapScreenQuestPrimitiveHandles +0x1B8,
//   MapScreenTeleportPrimitiveHandles +0x1C4, MapScreenHousePrimitiveHandles +0x1D0. No engine header exists for the class.
// Shape: speed TU (aligned, no pragma); the dropped functor result of every for_each lives in the one `push ecx` frame word;
//   `this` stays in esi, each member's address (`lea edi,[esi+N]`) is erase's `this`.
class CEnginePrimitiveHandle { public:                    // 8 bytes: vptr + counted primitive pointer
    void* _vtable;
    void* m_pPrimitive;
    void Clear();                                  // 0x0041BE70
};

template <class R, class T> class mem_fun_ref_t {  // SGI functor holding a 4-byte pointer-to-member
public:
    explicit mem_fun_ref_t(R (T::*pfn)()) : m_pfn(pfn) {}
    R operator()(T& object) const { return (object.*m_pfn)(); }
private:
    R (T::*m_pfn)();
};

template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pfn)())
{
    return mem_fun_ref_t<R, T>(pfn);
}

template <class It, class F> F __fastcall for_each(It first, It last, F f);   // 0x0043F360 (SGI, /Gr)

namespace std {
template <class T> class vector {                  // STLport vector: three words
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                   // 0x00440D70
    void clear() { erase(begin(), end()); }
};
}

typedef std::vector<CEnginePrimitiveHandle> CPrimitiveHandleVector;

class CTCInventoryMap { public:
    unsigned char          MembersBeforeHandleVectors[0x1ac];   // +0x000..+0x1AB (not modelled)
    CPrimitiveHandleVector MapScreenPrimitiveHandles;            // +0x1ac
    CPrimitiveHandleVector MapScreenQuestPrimitiveHandles;       // +0x1b8
    CPrimitiveHandleVector MapScreenTeleportPrimitiveHandles;    // +0x1c4
    CPrimitiveHandleVector MapScreenHousePrimitiveHandles;       // +0x1d0
    void ResetPrimitiveHandles();
};

void CTCInventoryMap::ResetPrimitiveHandles()
{
    for_each(MapScreenPrimitiveHandles.begin(), MapScreenPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    MapScreenPrimitiveHandles.clear();
    for_each(MapScreenQuestPrimitiveHandles.begin(), MapScreenQuestPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    MapScreenQuestPrimitiveHandles.clear();
    for_each(MapScreenTeleportPrimitiveHandles.begin(), MapScreenTeleportPrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    MapScreenTeleportPrimitiveHandles.clear();
    for_each(MapScreenHousePrimitiveHandles.begin(), MapScreenHousePrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    MapScreenHousePrimitiveHandles.clear();
}