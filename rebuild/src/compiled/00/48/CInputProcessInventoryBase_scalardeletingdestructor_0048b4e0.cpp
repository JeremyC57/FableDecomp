// CInputProcessInventoryBase::`vector_deleting_destructor' 0x0048b4e0
// Standard MSVC vector-deleting destructor thunk.

struct CInputProcessInventoryBase
{
};

// operator delete (0x00bfe9bc)

// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x00A0D3D0 ??1CAInputProcess@@UAE@XZ (was ?ScalarDtor@CInputProcessInventoryBase@@QAEXXZ)
class CAInputProcess { public: virtual ~CAInputProcess(); };
void* __fastcall CInputProcessInventoryBase_scalar_deleting_destructor(
    CInputProcessInventoryBase* process, int /*edx*/, unsigned int flags)
{
    reinterpret_cast<CAInputProcess*>(process)->CAInputProcess::~CAInputProcess();
    if (flags & 1)
        operator delete(process);
    return process;
}