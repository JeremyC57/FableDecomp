# h25_smp.py: naming wave 25, family smp -- the binders of ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ 0x00A5C600 (c692 queue; Ego_r.pdb
#  0x00BCF9C0: a private member).
#  0x00B3B8E0 (~CEngineResourceManager): the local CSimpleMemoryPool declared it public (QAE) -> private, as the PDB does.
#  0x00987F10: landed as a free DtorInputProcessManager2 on a CInputProcessManager stand-in (Leaf ??1CInputProcessManager@); retail stores the
#    CSimpleMemoryPool vtable 0x0129A064, calls ClearMemoryBlocks and tail-jumps to ~CBaseClassNonCopyable 0x0099A300, right before the
#    class's ??_G 0x00987F30 (which inlines the same body): the out-of-line ??1CSimpleMemoryPool@@UAE@XZ (FableWin.pdb names it).
#  0x0122AB10 / 0x0122AB30 / 0x0122AB50: landed as three free ClearCachedDefs on an AutoTinyFieldMethodsTarget stand-in; each is the
#    `dynamic atexit destructor' of one global pool (Ego_r 0x0101E8F0.. / FableWin 0x04012B70..: 'VectorBufferPool', 'GroupVectorBufferPool';
#    MatrixBufferPool the first), registered by the landed initialisers 0x01224450 / 0x01224470 / 0x01224490 (their `push 0122AB10h ; call
#    atexit`). Re-authored as VC7.1 emits them: the TU of the global's definition emits $E1 (initialiser) and $E2 (this atexit destructor);
#    the same source as the initialiser row, graded on $E2.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
he.edit('00b3b8e0', 'src', '''    CArea         Root;                                 // +0x10
    void ClearMemoryBlocks();                           // 0x00A5C600
    virtual ~CSimpleMemoryPool() { ClearMemoryBlocks(); }
};''', '''    CArea         Root;                                 // +0x10
    virtual ~CSimpleMemoryPool() { ClearMemoryBlocks(); }
private:
    void ClearMemoryBlocks();                           // 0x00A5C600 (Ego_r.pdb private: ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ; naming wave 25)
};''')
he.put('00987f10', 'src', '''// CSimpleMemoryPool::~CSimpleMemoryPool() -- re-install the pool's vtable, free its memory blocks, then the base destructor
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
''')
for a, ini, nm in (('0122ab10', '01224450', 'MatrixBufferPool'), ('0122ab30', '01224470', 'VectorBufferPool'),
                   ('0122ab50', '01224490', 'GroupVectorBufferPool')):
    p, rel, t = he._rw(ini, 'src')
    L = t.split('\n')
    assert L[1] == '// Leaf: _$E1', (ini, L[1])
    hdr = ["// `dynamic atexit destructor for '%s'' -- at exit run the global pool's inline destructor: its vtable, ClearMemoryBlocks, then"
           " ~CBaseClassNonCopyable" % nm,
           '// Leaf: _$E2',
           "// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, 30 bytes (`mov ecx,offset %s ; mov [ecx],0x0129A064 ; call 0x00A5C600 ; mov ecx,..."
           " ; jmp 0x0099A300`)." % (a.upper(), nm),
           "// Identity (naming wave 25, c692 queue: the ClearMemoryBlocks binders): the landed initialiser 0x%s ($E1 of the same TU) registers"
           " it with atexit; Ego_r.pdb / FableWin.pdb list `dynamic atexit destructor for '%s''. VC7.1 emits $E1 / $E2 from the one"
           " definition below (the initialiser row's source). Was a free ClearCachedDefs on an AutoTinyFieldMethodsTarget stand-in"
           " (First / Second_0099a300; the catalog label CTCInventoryItem::ClearCachedDefs is propagated)." % (ini.upper(), nm)]
    he.put(a, 'src', '\n'.join(hdr + L[2:]))
