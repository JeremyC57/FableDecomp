# h16_own.py: naming wave 16, family own -- queue own rows (c667-c674) landed under stand-in shapes (free __fastcall /
# descriptive classes / immediate-address views) re-authored on the proven PDB members; each is a whole-file rewrite that
# keeps the body's statements (name-only: the recheck proves the bytes unchanged).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import path, _w
F = {}
F['006925b0'] = '''// CCreatureActionBase::GetTotalTimeWorldFrames() const -- the action's total time in world frames (TotalTime, +0x10)
// Leaf: ?GetTotalTimeWorldFrames@CCreatureActionBase@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006925B0, 4 bytes: mov eax,[ecx+0x10] ; ret.
// Identity (proven, chunk c667): Ego_r.pdb donor run HasPerformedAction 0x00518EF0 / GetTotalTimeWorldFrames 0x00518F00 /
//   GetFramesRunning 0x00518F10 / GetFramesRemaining 0x00518F20 / GetPercentageComplete 0x00518F30 = retail 0x006925A0 /
//   0x006925B0 / 0x006925C0 / 0x006925D0 / 0x006925E0; no GetTotalTime in either PDB.
// Layout: Ego_r.pdb CCreatureActionBase TotalTime +0x10 (engine/CCreatureActionBase.h is the flat layout; the member view is
//   local, as the landed siblings 0x006925D0 / 0x00692620).
// Naming wave 16: landed as the free __fastcall DrawGetEnvironment(const unsigned char*); respelled on the PDB member
//   (name-only: the bytes are unchanged).
class CCreatureActionBase {
public:
    long GetTotalTimeWorldFrames() const;
    unsigned char _pad_0x00[0x10];
    long TotalTime;                                     // +0x10
};

long CCreatureActionBase::GetTotalTimeWorldFrames() const
{
    return TotalTime;
}
'''
F['006925c0'] = '''// CCreatureActionBase::GetFramesRunning() const -- the frames the action has been running (FramesRunning, +0x1C)
// Leaf: ?GetFramesRunning@CCreatureActionBase@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006925C0, 4 bytes: mov eax,[ecx+0x1c] ; ret.
// Identity (proven, chunk c667): Ego_r.pdb donor run (GetFramesRunning 0x00518F10 between GetTotalTimeWorldFrames and
//   GetFramesRemaining) = retail 0x006925C0; already bound so by landed rows.
// Layout: Ego_r.pdb CCreatureActionBase FramesRunning +0x1C (engine/CCreatureActionBase.h is the flat layout; the member view
//   is local, as the landed siblings 0x006925D0 / 0x00692620).
// Naming wave 16: landed as the free __fastcall DrawGetEnvironment(const unsigned char*) (catalog _global::GetMember_dword0x1c);
//   respelled on the PDB member (name-only: the bytes are unchanged).
class CCreatureActionBase {
public:
    long GetFramesRunning() const;
    unsigned char _pad_0x00[0x1c];
    long FramesRunning;                                 // +0x1C
};

long CCreatureActionBase::GetFramesRunning() const
{
    return FramesRunning;
}
'''
F['00692910'] = '''// CCreatureActionBase::PeekDefinitionManager() const -- the acting creature's definition manager (PCreature->PeekDefinitionManager())
// Leaf: ?PeekDefinitionManager@CCreatureActionBase@@IBEABVCGameDefinitionManager@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692910, 15 bytes: add ecx,8 ; call GetPItem ; mov ecx,eax ; jmp 0x004C7990.
// Identity (proven, chunk c667): Ego_r.pdb + FableWin.pdb (protected const); the landed 0x0087CA30 binds it so; no
//   GetDefinitionManager / GetPDefinitionManager member of CCreatureActionBase / CCombatActionBase in either PDB.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (PCreature +0x08, through CIntelligentPointer<T>::operator->),
//   CThing::PeekDefinitionManager() const 0x004C7990 (Ego_r ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, tail).
// Layout: Ego_r.pdb CCreatureActionBase PCreature (CIntelligentPointer<CThingCreatureBase>) +0x08, as the landed 0x0087CA30 model.
// Naming wave 16: landed as the free __fastcall Dest_val(void*) calling dv_h2_004c7990; respelled on the PDB member
//   (name-only: the bytes are unchanged).
class CBaseObject;
class CBaseObjectPointer;
class CGameDefinitionManager;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
class CThing {
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x004C7990
};
class CThingCreatureBase : public CThing {
};

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
protected:
    const CGameDefinitionManager& PeekDefinitionManager() const;
    unsigned char _pad_0x04[0x4];                        // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
};

const CGameDefinitionManager& CCreatureActionBase::PeekDefinitionManager() const
{
    return PCreature->PeekDefinitionManager();
}
'''
F['008fcfc0'] = '''// CAIStateGroupBase::GetWorld() const -- the world the state group's brain lives in (PBrain->ThingManager->World)
// Leaf: ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FCFC0, 10 bytes: mov eax,[ecx+4] ; mov ecx,[eax+0x1c] ; mov eax,[ecx+0x24] ; ret.
// Identity (proven, chunk c667): Ego_r.pdb + FableWin.pdb (protected const, CWorld& result); three landed rows already bind it
//   so; called on a CAIStateGroup `this` (0x0091D8F7) whose result's +0x4C is CWorld::PThingCombatManager.
// Layout: Ego_r.pdb CAIStateGroupBase PBrain +0x04, CAIBrain ThingManager +0x1C; the thing manager's world pointer is the retail
//   +0x24 word (Ego_r CThingManager pads it: descriptive member name).
// Naming wave 16: landed as the descriptive CAIStateGroupBase::GetPWorld() (void* result) over CFirst / CSecond views;
//   respelled on the PDB member (name-only: the bytes are unchanged).
class CWorld;
class CThingManager {
public:
    unsigned char _pad_0x00[0x24];
    CWorld* World;                                      // +0x24 (retail; descriptive)
};
class CAIBrain {
public:
    unsigned char _pad_0x00[0x1c];
    CThingManager* ThingManager;                        // +0x1C
};
class CAIStateGroupBase {
protected:
    CWorld& GetWorld() const;
public:
    unsigned char _pad_0x00[0x4];
    CAIBrain* PBrain;                                   // +0x04
};

CWorld& CAIStateGroupBase::GetWorld() const
{
    return *PBrain->ThingManager->World;
}
'''
F['0099a330'] = '''// CInitBaseClass::IsValid() const -- the initialised flag (Valid, +0x04)
// Leaf: ?IsValid@CInitBaseClass@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A330, 4 bytes: mov al,[ecx+4] ; ret.
// Identity (proven, chunk c671): called on GFGetSystemManager() (CSystemManager : CInitBaseClass, RTTI) in ~CStaticFontBank;
//   CInitBaseClass's only bool getter in Ego_r / FableWin (IsValid QBE_NXZ).
// Layout: Ego_r.pdb CInitBaseClass (vptr, bool Valid +0x04; 8 bytes).
// Naming wave 16: landed as the descriptive C0099a330::M() (unsigned char result; catalog CTCInventoryBase::
//   GetCanItemBeQuickAssigned); respelled on the PDB member (name-only: the bytes are unchanged).
class CInitBaseClass {
public:
    virtual ~CInitBaseClass();                          // 0x0099A410
    bool IsValid() const;
    bool Valid;                                         // +0x04
};

bool CInitBaseClass::IsValid() const
{
    return Valid;
}
'''
F['00ac1ee0'] = '''// CIndexBuffer::Uninitialise() -- release the Direct3D index buffer (its IUnknown::Release, slot 2) and clear the pointer
// Leaf: ?Uninitialise@CIndexBuffer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC1EE0, 25 bytes, thiscall.
// Identity (proven, chunk c672): called right after the CIndexBuffer RTTI vtable 0x0129EF2C store inside the inline
//   ~CIndexBuffer (0x00BABE9A); Ego_r ?Uninitialise@CIndexBuffer@@QAEXXZ.
// Layout: Ego_r.pdb CIndexBuffer PD3DIndexBuffer +0x04 (IDirect3DIndexBuffer9*; its vtable modelled to slot 2, __stdcall).
// Naming wave 16: landed as the descriptive CVolume::ReleaseVolume() (catalog CVolume::ReleaseVolume); respelled on the PDB
//   member (name-only: the bytes are unchanged).
struct IDirect3DIndexBuffer9;
struct IDirect3DIndexBuffer9Vtbl {
    void (__stdcall *QueryInterface)(IDirect3DIndexBuffer9*);
    void (__stdcall *AddRef)(IDirect3DIndexBuffer9*);
    void (__stdcall *Release)(IDirect3DIndexBuffer9*); // slot 2 at offset 0x8
};
struct IDirect3DIndexBuffer9 {
    IDirect3DIndexBuffer9Vtbl* lpVtbl;
};

class CIndexBuffer {
public:
    void Uninitialise();
    void*                  __vftable;                   // +0x00 vptr (flat, as engine/CIndexBuffer.h)
    IDirect3DIndexBuffer9* PD3DIndexBuffer;             // +0x04
};

void CIndexBuffer::Uninitialise()
{
    IDirect3DIndexBuffer9* r = this->PD3DIndexBuffer;
    if (r) {
        r->lpVtbl->Release(r);
        this->PD3DIndexBuffer = 0;
    }
}
'''
F['00a0b6c0'] = '''// CSoundBankLH::Unload_00a0b6c0() -- release the bank object (its slot 0) and clear the pointer (descriptive name)
// Leaf: ?Unload_00a0b6c0@CSoundBankLH@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B6C0, 23 bytes, thiscall.
// Identity (proven, chunk c670): slot 2 of vtable 0x0129C9DC (RTTI CSoundBankLH): `if (BankObject) { BankObject->slot0();
//   BankObject = 0; }` on CSoundBankLH +0x10, the body ~CSoundBankLH 0x00A0B9A0 expands; class proven, member name
//   descriptive (the sound module is in neither PDB; c670 decision: stays descriptive).
// Naming wave 16: landed as the free __fastcall ClearAllChildPrimitives(CEnginePrimitiveRenderer*, ...) (catalog
//   CEnginePrimitiveRenderer::ClearAllChildPrimitives); respelled on the class-proven descriptive member (name-only).
class CSoundBankObject_00a0b6c0 {                       // the bank object's interface (descriptive)
public:
    virtual void Release_00a0b6c0();                    // slot 0
};
class CSoundBankLH {
public:
    virtual void v00();
    virtual void v01();
    virtual void Unload_00a0b6c0();                     // slot 2
    unsigned char _pad_0x04[0xc];
    CSoundBankObject_00a0b6c0* BankObject;              // +0x10 (descriptive)
};

void CSoundBankLH::Unload_00a0b6c0()
{
    CSoundBankObject_00a0b6c0* c = BankObject;
    if (c) {
        c->Release_00a0b6c0();
        BankObject = 0;
    }
}
'''
T = {}
T['00ac1ee0'] = '''#include <stdio.h>

struct IDirect3DIndexBuffer9;
struct IDirect3DIndexBuffer9Vtbl {
    void (__stdcall *QueryInterface)(IDirect3DIndexBuffer9*);
    void (__stdcall *AddRef)(IDirect3DIndexBuffer9*);
    void (__stdcall *Release)(IDirect3DIndexBuffer9*);
};
struct IDirect3DIndexBuffer9 {
    IDirect3DIndexBuffer9Vtbl* lpVtbl;
};

class CIndexBuffer {
public:
    void Uninitialise();
    void*                  __vftable;
    IDirect3DIndexBuffer9* PD3DIndexBuffer;
};

static int g_released = 0;
static IDirect3DIndexBuffer9* g_last = 0;
static void __stdcall MyRelease(IDirect3DIndexBuffer9* r) { g_released++; g_last = r; }

int main() {
    IDirect3DIndexBuffer9Vtbl vt;
    vt.QueryInterface = 0; vt.AddRef = 0; vt.Release = MyRelease;
    IDirect3DIndexBuffer9 res;
    res.lpVtbl = &vt;

    CIndexBuffer v;
    v.__vftable = 0;
    v.PD3DIndexBuffer = &res;

    v.Uninitialise();
    bool ok = (g_released == 1) && (g_last == &res) && (v.PD3DIndexBuffer == 0);

    CIndexBuffer v2;
    v2.__vftable = 0;
    v2.PD3DIndexBuffer = 0;
    v2.Uninitialise();
    ok = ok && (g_released == 1) && (v2.PD3DIndexBuffer == 0);

    if (ok) printf("RELEASEVOLUME_OK\\n");
    else printf("RELEASEVOLUME_FAIL rel=%d\\n", g_released);
    return ok ? 0 : 1;
}'''
T['00a0b6c0'] = '''#include <cstdio>

class CSoundBankObject_00a0b6c0 {
public:
    virtual void Release_00a0b6c0();
};
class CSoundBankLH {
public:
    virtual void v00();
    virtual void v01();
    virtual void Unload_00a0b6c0();
    unsigned char _pad_0x04[0xc];
    CSoundBankObject_00a0b6c0* BankObject;
};
void CSoundBankLH::v00() { }
void CSoundBankLH::v01() { }
void CSoundBankObject_00a0b6c0::Release_00a0b6c0() { }

static int g_called = 0;
static CSoundBankObject_00a0b6c0* g_called_on = 0;
class CTestObject : public CSoundBankObject_00a0b6c0 {
public:
    virtual void Release_00a0b6c0() { g_called++; g_called_on = this; }
};

int main() {
    CTestObject c;
    CSoundBankLH r;
    r.BankObject = &c;

    r.Unload_00a0b6c0();
    bool ok1 = (g_called == 1) && (g_called_on == &c) && (r.BankObject == 0);

    // null path: no call, stays null
    g_called = 0;
    r.BankObject = 0;
    r.Unload_00a0b6c0();
    bool ok2 = (g_called == 0) && (r.BankObject == 0);

    if (ok1 && ok2) printf("00a0b6c0_TEST PASS\\n");
    else printf("FAIL c=%d r=%p\\n", g_called, (void*)r.BankObject);
    return 0;
}'''
for a, t in T.items():
    p, rel = path(a, 'tst')
    if open(p, encoding='utf-8', newline='').read() == t: print('already', a, 'tst')
    else: _w(p, t); print('ok', a, 'tst')
for a, t in F.items():
    p, rel = path(a, 'src')
    if open(p, encoding='utf-8', newline='').read() == t: print('already', a)
    else: _w(p, t); print('ok', a, 'src')
