# h16_own2.py: naming wave 16, family own -- more queue own rows: IsAcceptButton const (c668), the CTCOpinionSource (CThing&)
# constructor (c669), CEngineLocalDetailGenerator::DeleteMap (c668), CSimpleMemoryPool::ClearMemoryBlocks private (c673),
# SetScriptActiveStatus (c674). Name-only: the recheck proves the bytes unchanged.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, path, _w
# --- 0x00558C10 NUISystem::CNavButton::IsAcceptButton() const (Ego_r + FableWin QBE); CComponent::GetParent / GetType are
#     const in Ego_r (UBE) -- the virtual calls keep their slots
a = '00558c10'
edit(a, 'src', '// Leaf: IsAcceptButton\n', '// Leaf: ?IsAcceptButton@CNavButton@NUISystem@@QBE_NXZ\n'
     '// Naming wave 16 (c668): Ego_r + FableWin both mangle it const (QBE); the landed non-const spelling is respelled, with\n'
     '//   CComponent::GetParent / GetType const as Ego_r spells them (UBE; virtual calls by slot: name-only, bytes unchanged).\n')
edit(a, 'src', '    virtual CComponent* GetParent();               // slot 52 (+0xd0)', '    virtual CComponent* GetParent() const;         // slot 52 (+0xd0)')
edit(a, 'src', '    virtual long GetType();                        // slot 65 (+0x104)', '    virtual long GetType() const;                  // slot 65 (+0x104)')
edit(a, 'src', 'class CNavButton : public CComponent { public:\n    bool IsAcceptButton();\n', 'class CNavButton : public CComponent { public:\n    bool IsAcceptButton() const;\n')
edit(a, 'src', 'bool NUISystem::CNavButton::IsAcceptButton()\n', 'bool NUISystem::CNavButton::IsAcceptButton() const\n')
# --- 0x00771360 CTCOpinionSource::CTCOpinionSource(CThing&) (Ego_r's only constructor); the base is CTCBase(CThing&) 0x00686800
a = '00771360'
edit(a, 'src', '// CTCOpinionSource::CTCOpinionSource(CThing*) --', '// CTCOpinionSource::CTCOpinionSource(CThing& parent) --')
edit(a, 'src', '// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800.\n', '// Leaf: ??0CTCOpinionSource@@QAE@AAVCThing@@@Z\n'
     '// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (Ego_r ??0CTCBase@@QAE@AAVCThing@@@Z).\n'
     '// Naming wave 16 (c669): Ego_r.pdb ??0CTCOpinionSource@@QAE@AAVCThing@@@Z is the only constructor; the landed CThing*\n'
     '//   parameter and the CTCBase_00771360 base stand-in are respelled (one pushed pointer: name-only, bytes unchanged).\n')
edit(a, 'src', 'struct CTCBase_00771360 {\n    virtual void v00();\n    CThing*       ParentThing;                      // +0x04\n    unsigned char _pad08[4];\n    CTCBase_00771360(CThing* owner);                // 0x00686800\n};',
     'class CTCBase {\npublic:\n    virtual void v00();\n    CThing*       ParentThing;                      // +0x04\n    unsigned char _pad08[4];\n    CTCBase(CThing& parent);                        // 0x00686800\n};')
edit(a, 'src', 'class CTCOpinionSource : public CTCBase_00771360 { public:', 'class CTCOpinionSource : public CTCBase { public:')
edit(a, 'src', '    CTCOpinionSource(CThing* owner);\n', '    CTCOpinionSource(CThing& parent);\n')
edit(a, 'src', 'CTCOpinionSource::CTCOpinionSource(CThing* owner)\n    : CTCBase_00771360(owner),', 'CTCOpinionSource::CTCOpinionSource(CThing& parent)\n    : CTCBase(parent),')
# --- whole-file rewrites
F = {}
F['00b59840'] = '''// CEngineLocalDetailGenerator::DeleteMap(CEngineMap* map) -- delete the map's local-detail cache map (its out-of-line destructor, then operator delete) and clear the pointer; `this` is unused
// Leaf: ?DeleteMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B59840, 41 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c668): Ego_r + FableWin ?DeleteMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z; called by
//   ~CEngineMap 0x00B42A90 and CEngineWorldMap::DeleteMap 0x00B42B10 with ecx = [0x01436E88] and the map.
// Callees: CLocalDetailCacheMap::~CLocalDetailCacheMap 0x00BDED60 (Ego_r/FableWin ??1CLocalDetailCacheMap@@QAE@XZ, non-virtual;
//   chunk c671), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CEngineMap LocalDetailCacheMap +0x04.
// Shape: `if (p) { delete p; p = 0; }` keeps the clear inside the branch (before the join); the map stays in edi, the object in esi.
// Naming wave 16: landed as the descriptive CEngineResourceManager::DestroyMapStrip_00b59840(SMapEntry&) over a
//   N_00bded60::CPatchTesselationEdgeStrip stand-in; respelled on the PDB member (name-only: the bytes are unchanged).
void __cdecl operator delete(void* block);         // 0x00BFE9BC

class CLocalDetailCacheMap {
public:
    ~CLocalDetailCacheMap();                       // 0x00BDED60
};
class CEngineLandscapeMap;
class CEngineMap {
public:
    CEngineLandscapeMap*  LandscapeMap;            // +0x00
    CLocalDetailCacheMap* LocalDetailCacheMap;     // +0x04
};

class CEngineLocalDetailGenerator {
public:
    void DeleteMap(CEngineMap* map);
};

void CEngineLocalDetailGenerator::DeleteMap(CEngineMap* map)
{
    if (map->LocalDetailCacheMap) {
        delete map->LocalDetailCacheMap;
        map->LocalDetailCacheMap = 0;
    }
}
'''
F['00a5c600'] = '''// CSimpleMemoryPool::ClearMemoryBlocks() -- deletes the block chain: every block when the first one owns its data, only the first (external buffer) otherwise; then no blocks
// Leaf: ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C600, length 54 bytes.
// Identity (proven, chunk c673): called by the CSimpleMemoryPool scalar deleting dtor 0x00987F30 (vtable 0x0129A064 store, call,
//   ~CBaseClassNonCopyable) and inline in ~CEngine 0x00B261D0; Ego_r + FableWin list only the private
//   ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ.
// Callees: CMemoryBlock's virtual scalar deleting destructor (slot 0, `push 1 ; call [eax]`; landed at 0x00A5C9B0).
// Layout: Ego_r.pdb CSimpleMemoryPool (FirstMemoryBlock +4, Capacity +8, GrowthGranularity +0xc, Root +0x10); CMemoryBlock as the
//   landed 0x00A5C670 / 0x00A5C6E0 bodies and the PDB (BlockData +8, OwnsData +0xc, BlockSize +0x10, NextBlock +0x14),
//   polymorphic, so modelled locally. engine/CSimpleMemoryPool.h is the flat layout and cannot carry the private member
//   (the landed CSimpleMemoryPool_Methods view derived from it named a class retail does not have), so the class is modelled
//   here with the header's members.
// Shape: one `FirstMemoryBlock = 0` written after the if/else; VC7.1 tail-duplicates it into both arms (the loop arm
// stores the null loop pointer from ecx, the else arm an immediate) -- inlined into Reset 0x00A5C790 the same source
// keeps one shared store. The loop is a do/while on a local with the successor read before the delete.
// Naming wave 16: landed as CSimpleMemoryPool_Methods::ClearMemoryBlocks (public, a view class derived from the header);
//   respelled on the PDB private member (name-only: the bytes are unchanged).
struct CMemoryBlock {                           // vtable 0x0129D568 (CSimpleMemoryPool::CMemoryBlock)
    virtual ~CMemoryBlock();
    unsigned long  IVRefCount;                  // +0x04
    unsigned char* BlockData;                   // +0x08
    bool           OwnsData;                    // +0x0c
    unsigned long  BlockSize;                   // +0x10
    CMemoryBlock*  NextBlock;                   // +0x14
};
struct CArea { unsigned long Size; CArea* NextFreeArea; long Status; };

class CSimpleMemoryPool {
public:
    void*         __vftable;                    // +0x00 (the CBaseClassNonCopyable vptr; flat, as engine/CSimpleMemoryPool.h)
    CMemoryBlock* FirstMemoryBlock;             // +0x04
    unsigned long Capacity;                     // +0x08
    unsigned long GrowthGranularity;            // +0x0c
    CArea         Root;                         // +0x10
private:
    void ClearMemoryBlocks();
};

void CSimpleMemoryPool::ClearMemoryBlocks()
{
    CMemoryBlock* block = FirstMemoryBlock;
    if (block) {
        if (block->OwnsData) {
            do {
                CMemoryBlock* next = block->NextBlock;
                delete block;
                block = next;
            } while (block);
        } else {
            delete block;
        }
        FirstMemoryBlock = 0;
    }
}
'''
F['00cbfab8'] = '''// SetScriptActiveStatus(const CCharString& name, EScriptActiveStatus status) -- ScriptActiveStatuses[name] = status
// Leaf: ?SetScriptActiveStatus@@YIXABVCCharString@@W4EScriptActiveStatus@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBFAB8, 18 bytes, __fastcall (name in ecx, status in edx).
// Identity (proven, chunk c674): writes ScriptActiveStatuses[name] = status over the global map 0x0143E910 (calls the STLport
//   map<CCharString, EScriptActiveStatus>::operator[] 0x00CD39B1, kept out of line); Ego_r pairs it with GetScriptActiveStatus
//   0x00CBF9CF in the same TU.
// Callees: std::map<CCharString, EScriptActiveStatus>::operator[] 0x00CD39B1 (`mov ecx, offset ScriptActiveStatuses`).
// Naming wave 16: landed as the free __fastcall fn_00cbfab8(void*, int) calling Glob::h on an immediate address (catalog
//   CRegion::SetMiniMapRegionExitTextOffsetX); respelled on the PDB function and data symbol (name-only: bytes unchanged).
class CCharString;
enum EScriptActiveStatus { };
namespace std {
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { };
template <class _Tp> class allocator { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    _Tp& operator[](const _Key& __k);              // 0x00CD39B1 (out of line)
};
}
extern std::map<CCharString, EScriptActiveStatus> ScriptActiveStatuses;   // 0x0143E910

void __fastcall SetScriptActiveStatus(const CCharString& name, EScriptActiveStatus status)
{
    ScriptActiveStatuses[name] = status;
}
'''
for a, t in F.items():
    p, rel = path(a, 'src')
    if open(p, encoding='utf-8', newline='').read() == t: print('already', a)
    else: _w(p, t); print('ok', a, 'src')
# 0x004175E5 CMainGameComponent::Shutdown: the 0x0099A330 call on InputSystem (+0x13AB8) respelled to the proven
# ?IsValid@CInitBaseClass@@QBE_NXZ (fam_q could not move it: the row's descriptive Uninitialise_004ae990 is private there)
edit('004175e5', 'src', '    bool IsInitialised() const;                      // 0x0099A330\n', '    bool IsValid() const;                            // 0x0099A330\n')
edit('004175e5', 'src', 'CInitBaseClass IsInitialised 0x0099A330', 'CInitBaseClass IsValid 0x0099A330')
edit('004175e5', 'src', '    if (InputSystem.IsInitialised())\n', '    if (InputSystem.IsValid())\n')
# 0x00887A30 CEnvironmentThemePriorityStackEntry::ProcessTransitions: the invented inline helper SetBaseWeight(weight, locked) is
# Ego_r/FableWin ?SetDefaultThemeWeight@CEnvironmentThemePriorityStackEntry@NEnvironmentInternal@@QAEXM_N@Z (clamp, store
# BaseWeight / BaseWeightLocked, RebalanceWeights; inlined in retail), called as SetDefaultThemeWeight(weight, true) (c669 README)
edit_all('00887a30', 'src', 'SetBaseWeight(', 'SetDefaultThemeWeight(', 2)
