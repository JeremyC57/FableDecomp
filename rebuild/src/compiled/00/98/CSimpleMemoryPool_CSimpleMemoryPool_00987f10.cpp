// CSimpleMemoryPool::~CSimpleMemoryPool() -- re-install the pool's vtable, free its memory blocks, then the base destructor
// Leaf: ??1CSimpleMemoryPool@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00987F10, 22 bytes, thiscall: `mov [esi],0x0129A064 ; call 0x00A5C600 ; mov ecx,esi ; jmp 0x0099A300`.
// Identity (naming wave 25, c692 queue): the vtable 0x0129A064 is CSimpleMemoryPool's (RTTI .?AVCSimpleMemoryPool@@, bases
//   CBaseClassNonCopyable : CBaseClass; slot 0 the landed ??_G 0x00987F30, which follows this function and inlines the same body);
//   FableWin.pdb names the out-of-line ??1CSimpleMemoryPool@@UAE@XZ. Callees: ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ 0x00A5C600
//   (Ego_r.pdb private), ??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300. Was a free DtorInputProcessManager2 on a CInputProcessManager
//   stand-in calling sub_A5C600 / base_dtor_99A300 (the catalog label Dtor_CBaseBase_00987f10 is descriptive).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CSimpleMemoryPool : public CBaseClassNonCopyable {   // RTTI: CBaseClassNonCopyable : CBaseClass; vtable 0x0129A064
public:
    virtual ~CSimpleMemoryPool();
private:
    void ClearMemoryBlocks();                            // 0x00A5C600 (Ego_r.pdb private)
};
CSimpleMemoryPool::~CSimpleMemoryPool()
{
    ClearMemoryBlocks();
}
