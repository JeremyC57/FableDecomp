# h30_c699.py: naming wave 30, family c699 (scope 1, chunk 699 NAME_FIXES rows): own rows whose catalog name / defined symbol was wrong.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
# 0x0051ED60 (queue row 70): CThingManager::PeekPThingWithUID, the const twin of the landed GetPThingWithUID 0x0051ED80 (Ego_r.pdb donor order
#  ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z 0x00528FD0 directly before ?GetPThingWithUID 0x00529000; FableWin.pdb the same name).
#  Was the free __fastcall stand-in "CStopWatch::GetTicks" (?GetTicks@@YIIPAUCStopWatch@@PAX_J@Z). UIDToThingMap (+0x58) is looked up through
#  the const find of map<unsigned __int64, CThing*> (0x00527D80; its spelling is the row's stand-in, as in the twin).
a = '0051ed60'
he.put(a, 'src', '''// CThingManager::PeekPThingWithUID(unsigned __int64 uid) const @ 0x0051ED60 -- the thing mapped to `uid`, or 0 (the const twin of
//   GetPThingWithUID 0x0051ED80)
// Leaf: ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z
// Naming wave 30 (family c699): the own row is Ego_r.pdb / FableWin.pdb ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z (donor order
//   PeekPThingWithUID 0x00528FD0 -> GetPThingWithUID 0x00529000 = retail 0x0051ED60 -> 0x0051ED80). Was the free __fastcall stand-in
//   "CStopWatch::GetTicks" (?GetTicks@@YIIPAUCStopWatch@@PAX_J@Z).
// Shape: thiscall, the 8-byte by-value uid => ret 8; its stack slot is reused as the find iterator's scratch. UIDToThingMap (+0x58,
//   Ego_r map<unsigned __int64, CThing*>) is searched by its find 0x00527D80 (the row's stand-in spelling, as in the twin row), the
//   returned node compared with the head sentinel, node->second (+0x18) returned, else 0.
class CThing;

struct Node {
    Node* left;         // +0
    char  pad4[0x14];
    CThing* value;      // +0x18
};

struct Map {
    Node* head;         // +0
    Node* Find(void* iterOut) const;   // 0x00527D80
};

class CThingManager { public:
    char pad[0x58];
    Map  themap;        // +0x58 UIDToThingMap
    const CThing* PeekPThingWithUID(unsigned __int64 uid) const;
};

const CThing* CThingManager::PeekPThingWithUID(unsigned __int64 uid) const
{
    const Map* m = &this->themap;
    Node* n = m->Find(&uid);
    if (n != m->head)
        return n->value;
    return 0;
}''')
# The two const CTCHeroStats binders call it as the PDB pair: the protected const ?GetThingManager@CTCBase@@IBEABVCThingManager@@XZ 0x00686AF0
#  (both PDBs; const CThingManager&) then the const PeekPThingWithUID (was CWorld::GetThingFromUID through GetWorld / GetThingManager returning
#  CWorld*). The owner-class model of the row (CTCHeroStats_Retail / the flattened CTCHeroStats) is not this family's.
CM = ('class CThingManager { public:\n'
      '    const CThing* PeekPThingWithUID(unsigned __int64 uid) const;   // 0x0051ED60 (Ego_r.pdb / FableWin.pdb)\n'
      '};\n')
a = '0057abd6'
he.edit(a, 'src', 'class CWorld { public:\n    CThing* GetThingFromUID(unsigned __int64 uid);   // 0x0051ED60\n};\n', CM)
he.edit(a, 'src', '    CWorld* GetWorld() const;                  // 0x00686AF0\n',
        '    const CThingManager& GetThingManager() const; // 0x00686AF0 (Ego_r.pdb / FableWin.pdb: CTCBase, protected, const)\n')
he.edit(a, 'src', '            CThing* home = GetWorld()->GetThingFromUID(node->MaritalHomeUID);',
        '            const CThing* home = GetThingManager().PeekPThingWithUID(node->MaritalHomeUID);')
he.edit(a, 'src', '                return home;', '                return (CThing*)home;')
note(a, 'c699', 'the UID lookup is the const PDB pair CTCBase::GetThingManager() const 0x00686AF0 (const CThingManager&) and\n'
     '//   ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z 0x0051ED60 (both PDBs); was CWorld::GetThingFromUID through GetWorld()')
a = '0057ae53'
he.edit(a, 'src', 'class CWorld { public:\n    CThing* GetThingFromUID(unsigned __int64 uid);   // 0x0051ED60\n};\n', CM)
he.edit(a, 'src', '    CWorld* GetThingManager() const;                 // 0x00686AF0 (CTCBase)\n',
        '    const CThingManager& GetThingManager() const;    // 0x00686AF0 (Ego_r.pdb / FableWin.pdb: CTCBase, protected, const)\n')
he.edit(a, 'src', '            CThing* wife = GetThingManager()->GetThingFromUID(node->WifeUID);',
        '            const CThing* wife = GetThingManager().PeekPThingWithUID(node->WifeUID);')
he.edit(a, 'src', 'wives.push_back(static_cast<CThingCreatureBase*>(wife));', 'wives.push_back((CThingCreatureBase*)wife);')
note(a, 'c699', 'the UID lookup is the const PDB pair CTCBase::GetThingManager() const 0x00686AF0 (const CThingManager&) and\n'
     '//   ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z 0x0051ED60 (both PDBs); was CWorld::GetThingFromUID')
# 0x006868E0 (queue row 72): the empty CTCBase slot 13 is ?OnDie@CTCBase@@UAEXXZ (Ego_r.pdb / FableWin.pdb; CTCBase vtable 0x0125BE0C slot 13 =
#  OnDie in the CTCBase virtual order, chunk 695 OnKill slot 14; labels_naming_review: every trusted function at slot 13 of the hierarchy is
#  OnDie). Was the placeholder struct C006868e0 (?OnDie@C006868e0@@QAEXXZ).
a = '006868e0'
he.put(a, 'src', '''// CTCBase::OnDie() @ 0x006868E0 -- the empty base slot (a bare `ret`)
// Leaf: ?OnDie@CTCBase@@UAEXXZ
// Naming wave 30 (family c699): Ego_r.pdb / FableWin.pdb ?OnDie@CTCBase@@UAEXXZ, CTCBase vtable 0x0125BE0C slot 13 (OnDie in the CTCBase
//   virtual order: OnDie 13, OnKill 14 (chunk 695); every derived slot-13 body of the hierarchy is an OnDie and tails here: CTCInventoryItem
//   0x005D8EC0, CTCScriptedControl 0x00713740, CTCKrakenTentacle 0x007F0890). Was the placeholder struct C006868e0 (?OnDie@C006868e0@@QAEXXZ).
// Layout: CTCBase {vptr, ParentThing +0x04, ToBeRemoved +0x08, IsInGlobalUpdateTCs +0x09} (Ego_r.pdb, 0xc); only the slot is modelled.
class CTCBase { public:
    virtual void OnDie();   // slot 13
};

void CTCBase::OnDie()
{
}''')
# 0x00713740: the own row is ?OnDie@CTCScriptedControl@@UAEXXZ (both PDBs; vtable 0x012658E4 slot 13; was Leaf vfunc_13, catalog OnRemove)
#  and its base call CTCBase::OnDie.
a = '00713740'
he.resub(a, 'src', r'(virtual void |CTCScriptedControl::|CTCBase::)vfunc_13\(\)', r'\1OnDie()', 4)
he.edit(a, 'src', '// CTCScriptedControl::OnRemove() (vtable slot 13) --', '// CTCScriptedControl::OnDie() (vtable slot 13) --')
note(a, 'c699', 'the slot-13 pair is OnDie: own row ?OnDie@CTCScriptedControl@@UAEXXZ and the empty base ?OnDie@CTCBase@@UAEXXZ\n'
     '//   0x006868E0 (Ego_r.pdb / FableWin.pdb; slot 13 = OnDie in the CTCBase order). Was the placeholder slot-13 Leaf / catalog "OnRemove".',
     leaf='?OnDie@CTCScriptedControl@@UAEXXZ')
# 0x007F0890: the own row is ?OnDie@CTCKrakenTentacle@@UAEXXZ (both PDBs; slot 13): private KillRippleFX 0x007F07A0 (both PDBs AAE; the landed
#  0x007F07A0 Leaf), RemoveObstruction 0x007F0810, then the tail into CTCBase::OnDie. Was the free __fastcall Dest_val calling the extern "C"
#  stand-ins Dtor_first / Dtor_node.
a = '007f0890'
he.put(a, 'src', '''// CTCKrakenTentacle::OnDie() @ 0x007F0890 -- kill the ripple effect, remove the tentacle's obstruction, then the empty CTCBase::OnDie
// Leaf: ?OnDie@CTCKrakenTentacle@@UAEXXZ
// Naming wave 30 (family c699): Ego_r.pdb / FableWin.pdb ?OnDie@CTCKrakenTentacle@@UAEXXZ (vtable slot 13, OnDie in the CTCBase order);
//   callees private ?KillRippleFX@CTCKrakenTentacle@@AAEXXZ 0x007F07A0 (both PDBs; the landed 0x007F07A0 Leaf), ?RemoveObstruction@
//   CTCKrakenTentacle@@QAEXXZ 0x007F0810 and the tail ?OnDie@CTCBase@@UAEXXZ 0x006868E0. Was the free __fastcall Dest_val calling the
//   invented extern "C" Dtor_first / Dtor_node.
// Shape: 23 bytes: push esi; mov esi,ecx; call; mov ecx,esi; call; mov ecx,esi; pop esi; jmp (size TU).
#pragma optimize("s",on)

class CTCBase { public:
    virtual void OnDie();       // slot 13, 0x006868E0
};

class CTCKrakenTentacle : public CTCBase {
public:
    virtual void OnDie();
    void RemoveObstruction();   // 0x007F0810
private:
    void KillRippleFX();        // 0x007F07A0
};

void CTCKrakenTentacle::OnDie()
{
    KillRippleFX();
    RemoveObstruction();
    CTCBase::OnDie();
}''')
he.put(a, 'tst', '''#include <stdio.h>
// Behaviour test for 0x007F0890 CTCKrakenTentacle::OnDie: the three callees in order on the same object.
static int g_calls = 0;
static int g_order[3];
static void* g_last[3];

class CTCBase { public:
    virtual void OnDie();
};
class CTCKrakenTentacle : public CTCBase {
public:
    virtual void OnDie();
    void RemoveObstruction();
private:
    void KillRippleFX();
};
void CTCBase::OnDie() { g_order[g_calls] = 3; g_last[g_calls++] = this; }
void CTCKrakenTentacle::KillRippleFX() { g_order[g_calls] = 1; g_last[g_calls++] = this; }
void CTCKrakenTentacle::RemoveObstruction() { g_order[g_calls] = 2; g_last[g_calls++] = this; }

int main(){
    CTCKrakenTentacle t;
    t.CTCKrakenTentacle::OnDie();
    if (g_calls == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3
        && g_last[0] == (void*)&t && g_last[1] == (void*)&t && g_last[2] == (void*)&t)
        printf("OK_007f0890\\n");
    else
        printf("FAIL calls=%d\\n", g_calls);
    return 0;
}
''')
# 0x00A9D3A0 (queue rows 67 / 68): the implicit ??1C3DMeshFilePrimitiveChunk@@UAE@XZ (FableWin.pdb donor 0x033D41A0, directly before
#  ??1C3DMeshFileClothChunk 0x033D4200 = retail 0x00A9D3A0 -> the landed 0x00A9D3C0; sole caller the slot-0 ??_G 0x00ABEEB0 of vtable 0x0129ED74,
#  RTTI .?AVC3DMeshFilePrimitiveChunk@@ : CChunkedFileSuperChunk): frees MaterialIndices (Ego_r CArray<long>, retail +0x2C) then the tail
#  ~CChunkedFileSuperChunk 0x00AA8490 (??1CChunkedFileSuperChunk@@UAE@XZ). Was "CTCInventoryItem::OnDie" over an invented BaseItem
#  (?OnDie@CTCInventoryItem@@QAEXXZ / ?OnDie@BaseItem@@QAEXXZ). Model: the sibling 0x00A9D3C0 source.
import shutil
a = '00a9d3a0'
src = open(os.path.join('/home/user/FableDecomp/rebuild/src/compiled', he.R['00a9d3c0']['Source']), encoding='utf-8').read()
body = src[src.index('extern "C" void __cdecl free(void* block);'):src.index('class C3DMeshFileClothChunk : public CChunkedFileSuperChunk {')]
he.put(a, 'src', '''// C3DMeshFilePrimitiveChunk::~C3DMeshFilePrimitiveChunk() -- the compiler-generated destructor: free the MaterialIndices block, then run
//   ~CChunkedFileSuperChunk
// Leaf: ??1C3DMeshFilePrimitiveChunk@@UAE@XZ
// Naming wave 30 (family c699): was "CTCInventoryItem::OnDie" (?OnDie@CTCInventoryItem@@QAEXXZ over an invented BaseItem whose OnDie
//   named ~CChunkedFileSuperChunk 0x00AA8490; the catalog label is propagated from the 0x00A9D3C0 twin's caller set).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9D3A0, 30 bytes, thiscall, tail `jmp ~CChunkedFileSuperChunk`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00ABEEB0 of vtable 0x0129ED74 (RTTI .?AVC3DMeshFilePrimitiveChunk@@ :
//   CChunkedFileSuperChunk : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); FableWin.pdb ??1C3DMeshFilePrimitiveChunk@@UAE@XZ
//   (donor 0x033D41A0, directly before ??1C3DMeshFileClothChunk 0x033D4200 -- retail has the same pair, 0x00A9D3A0 then the landed
//   0x00A9D3C0). Ego_r.pdb C3DMeshFilePrimitiveChunk: MaterialIndices (CArray<long>) at retail +0x2C (CChunkedFileSuperChunk is 0x2C).
//   No 0x0129ED74 store: the destructor is the implicit one.
// Callees: free 0x00BFEA14, CChunkedFileSuperChunk::~CChunkedFileSuperChunk 0x00AA8490.
// Layout: local, as the sibling 0x00A9D3C0 (engine/C3DMeshFilePrimitiveChunk.h / CChunkedFileSuperChunk.h flatten the vptr).
// Shape: the sibling's model. DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this out-of-line implicit `??1`.
''' + body + '''class C3DMeshFilePrimitiveChunk : public CChunkedFileSuperChunk {   // vtable 0x0129ED74, 0x38 bytes in retail
public:
    C3DMeshFilePrimitiveChunk();
    CArray<long>  MaterialIndices;                          // +0x2c
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(C3DMeshFilePrimitiveChunk* chunk)
{
    chunk->C3DMeshFilePrimitiveChunk::~C3DMeshFilePrimitiveChunk();
}''')
he.put(a, 'tst', '''// Behaviour test for 0x00A9D3A0 C3DMeshFilePrimitiveChunk::~C3DMeshFilePrimitiveChunk (compiler-generated):
// MaterialIndices' block freed, then ~CChunkedFileSuperChunk.
#include <cstdio>
#include <cstdlib>
static int g_log[8];
static int g_n = 0;
static void Note(int e) { if (g_n < 8) g_log[g_n++] = e; }
static void FreeBlock(void* p) { if (p) { Note(1); std::free(p); } }
struct LongArray {
    long* Start; long* Finish; long* EndOfStorage;
    LongArray() : Start(0), Finish(0), EndOfStorage(0) { }
    ~LongArray() { FreeBlock(Start); }
};
struct ChunkedFileSuperChunk { virtual ~ChunkedFileSuperChunk() { Note(9); } unsigned char Body[0x28]; };
struct PrimitiveChunk : ChunkedFileSuperChunk { LongArray MaterialIndices; };
int main()
{
    PrimitiveChunk* chunk = new PrimitiveChunk;
    chunk->MaterialIndices.Start = (long*)std::malloc(16);
    ChunkedFileSuperChunk* base = chunk;
    delete base;
    PrimitiveChunk* empty = new PrimitiveChunk;
    base = empty;
    delete base;
    const int expected[3] = { 1, 9, 9 };
    if (g_n != 3) { std::printf("00a9d3a0_TEST FAIL\\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (g_log[i] != expected[i]) { std::printf("00a9d3a0_TEST FAIL\\n"); return 1; }
    std::printf("00a9d3a0_TEST PASS ONDIE_OK\\n");   // ONDIE_OK: the catalog PassPattern
    return 0;
}
''')
