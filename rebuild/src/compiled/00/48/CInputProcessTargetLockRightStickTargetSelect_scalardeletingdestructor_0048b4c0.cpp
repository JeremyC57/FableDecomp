// CInputProcessTargetLockRightStickTargetSelect::`vector_deleting_destructor'
// Retail 0x0048b4c0, __fastcall(this, uint flags) -> void*
// Standard MSVC vector-deleting-destructor: call the scalar destructor,
// then if (flags & 1) operator delete(this), return this.

class CAInputProcess {
public:
    virtual ~CAInputProcess();   // 0x00A0D3D0
};
class CInputProcessTargetLockRightStickTargetSelect;

// The scalar destructor (retail 0x00a0d3d0) — external, produces a real `call`.

// operator delete (retail 0x00bfe9bc) — __cdecl free.

class CInputProcessTargetLockRightStickTargetSelect
{ public:
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CInputProcessTargetLockRightStickTargetSelect::scalar_deleting_destructor(unsigned int flags)
{
    ((CAInputProcess*)(this))->CAInputProcess::~CAInputProcess();
    if (flags & 1)
        operator delete(this);
    return this;
}