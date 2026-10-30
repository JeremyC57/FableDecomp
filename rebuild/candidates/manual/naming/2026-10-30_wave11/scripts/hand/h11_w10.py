# sub-wave w10 (hand): wave-10 DEFERRED items, cheapest first.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
# ---- 0x00BEBAA0 ?CalcObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@ABVCRightHandedSet@@M@Z (Ego_r.pdb +
#      FableWin.pdb; static __fastcall: matrix in ecx, position in edx, orientation and scale on the stack); wave 10 respelled 0x00BCEF80,
#      its DEFERRED list kept the other four callers. Scale is CEngineInternalPrimitiveMeshBase::Scale +0x58 (a float in Ego_r.pdb).
MB = '''class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00BEBAA0 (Ego_r / FableWin)
};'''
edit('00b9d8e0', 'src', 'void __fastcall CalcObjectMatrix(CMatrix3x4* matrix, const C3DVector* position, const CRightHandedSet* set, float scale);   // 0x00BEBAA0', MB)
edit('00b9d8e0', 'src', '    CalcObjectMatrix(&ObjectTransform, &mesh.WorldPos, &mesh.RightHandedSet, Scale);',
     '    CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(ObjectTransform, mesh.WorldPos, mesh.RightHandedSet, Scale);')
FWD = 'class CMatrix3x4;\nclass C3DVector;\nclass CRightHandedSet;\n'
# 0x00B9CFA0 SetPosition: matrix = this+0x70 (ObjectTransform), position = the CEnginePrimitivePosition, orientation at its +0xc
edit('00b9cfa0', 'src', '''// callee at 0xbebaa0 is a __fastcall-shaped this-call:
//   this  -> ecx (self->sub, i.e. this+0x70)
//   rawPos-> edx (the CEnginePrimitivePosition pointer)
//   stack -> (payloadPtr = &pos.payload, field58 = self->[+0x58])
// It cleans its own 2 stack args (8 bytes); this function cleans its 1 ref arg (ret 4).
void __fastcall CEngineInternalPrimitiveStaticMesh_Sub_Apply(
        void* self, const CEnginePrimitivePosition* rawPos,
        const void* payloadPtr, int field58);
''', '''// callee at 0xbebaa0 is CEngineInternalPrimitiveMeshBase::CalcObjectMatrix (static __fastcall, Ego_r / FableWin; naming wave 11):
//   matrix -> ecx (ObjectTransform, this+0x70)
//   position -> edx (the CEnginePrimitivePosition's position, +0)
//   stack -> (orientation = the position's right-handed set +0xc, scale = this+0x58)
// It cleans its own 2 stack args (8 bytes); this function cleans its 1 ref arg (ret 4).
''' + FWD + MB + '\n')
edit('00b9cfa0', 'src', '    int field58;                 // +0x58', '    float field58;               // +0x58 (CEngineInternalPrimitiveMeshBase::Scale)')
edit('00b9cfa0', 'src', '''    CEngineInternalPrimitiveStaticMesh_Sub_Apply(
        &self->sub, &pos, &pos.payload, self->field58);''', '''    CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(
        *(CMatrix3x4*)&self->sub, *(const C3DVector*)&pos, *(const CRightHandedSet*)&pos.payload, self->field58);''')
# 0x00BCE9F0 GetTransformFrameStart: StartPosition +0x88, StartOrientation +0xa4, Scale +0x58
edit('00bce9f0', 'src', '''// callee at 0xbebaa0: ecx = matrix arg, edx = self+0x88, stack: self+0xa4, m58
extern "C" int __fastcall Callee_bebaa0(CMatrix3x4* mtx, void* edxArg, void* pA4, int val58);''', '''// callee at 0xbebaa0: CEngineInternalPrimitiveMeshBase::CalcObjectMatrix (static __fastcall, Ego_r / FableWin; naming wave 11):
// ecx = matrix arg, edx = StartPosition self+0x88, stack: StartOrientation self+0xa4, Scale +0x58
class C3DVector;
class CRightHandedSet;
''' + MB)
edit('00bce9f0', 'src', '    int  m58;        // +0x58', '    float m58;       // +0x58 (Scale)')
edit('00bce9f0', 'src', '    Callee_bebaa0(&mtx, self->m88, self->mA4, self->m58);',
     '    CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(mtx, *(const C3DVector*)self->m88, *(const CRightHandedSet*)self->mA4, self->m58);')
# 0x00BCEA10 GetTransformFrameEnd: EndPosition +0x94, EndOrientation +0xbc, Scale +0x58
edit('00bcea10', 'src', '''class CMatrix3x4;

// callee at 0xbebaa0: __fastcall(ecx=matrix, edx=self+0x94, stack: self+0xbc, field58)
extern "C" void __fastcall FrameEndCallee(CMatrix3x4* matrix, void* sub, void* p, int v);''', '''// callee at 0xbebaa0: CEngineInternalPrimitiveMeshBase::CalcObjectMatrix (static __fastcall, Ego_r / FableWin; naming wave 11):
// ecx = matrix, edx = EndPosition self+0x94, stack: EndOrientation self+0xbc, Scale +0x58
''' + FWD + MB)
edit('00bcea10', 'src', '    int field58;        // +0x58', '    float field58;      // +0x58 (Scale)')
edit('00bcea10', 'src', '    FrameEndCallee(&matrix, base + 0x94, base + 0xbc, self->field58);',
     '    CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(matrix, *(const C3DVector*)(base + 0x94), *(const CRightHandedSet*)(base + 0xbc), self->field58);')
# the 0x00B9CFA0 test links the row: it defines the callee under the PDB spelling
from he import _rw, _w
def put(a, k, text):
    p, rel, t = _rw(a, k)
    if t == text: print('already', a, k); return
    _w(p, text); print('ok', a, k)
put('00b9cfa0', 'tst', '''#include <cstdio>

class CEnginePrimitivePosition { public:
    char pad[0xc];
    int payload;
};
class CMatrix3x4;
class C3DVector;
class CRightHandedSet;

static const void* g_self;
static const void* g_rawPos;
static const void* g_payloadPtr;
static float g_field58;

class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& position, const CRightHandedSet& orientation, float scale);
};
void __fastcall CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& position,
        const CRightHandedSet& orientation, float scale) {
    g_self = &matrix; g_rawPos = &position; g_payloadPtr = &orientation; g_field58 = scale;
}

class CEngineInternalPrimitiveStaticMesh { public:
    char pad0[0x58];
    float field58;
    char pad1[0x70 - 0x58 - 4];
    char sub;
};

void __fastcall CEngineInternalPrimitiveStaticMesh_SetPosition(
        CEngineInternalPrimitiveStaticMesh* self,
        int /*edx*/,
        const CEnginePrimitivePosition& pos);

int main() {
    CEngineInternalPrimitiveStaticMesh obj;
    obj.field58 = 2.5f;
    CEnginePrimitivePosition pos;
    pos.payload = 0x99;

    CEngineInternalPrimitiveStaticMesh_SetPosition(&obj, 0, pos);

    bool ok = (g_self == (void*)&obj.sub)
           && (g_rawPos == &pos)
           && (g_payloadPtr == (const void*)&pos.payload)
           && (g_field58 == 2.5f);
    if (ok) printf("00b9cfa0_TEST PASS\\n");
    else printf("FAIL\\n");
    return 0;
}''')
# ---- 0x0099F830 ?GFIntToCharString@@YI?AVCCharString@@J@Z (Ego_r.pdb 0x00AFB260 + FableWin.pdb) and its concatenation 0x0099F690
#      ??H@YI?AVCCharString@@PBDABV0@@Z (Ego_r 0x00AFAEE0 + FableWin 0x02F63030; already bound by 0x006B9560): the 0x00D3ABE0 row returned
#      them as its row-local CPrisonerSpeechString; they return the CCharString of fable_string.h (wave-10 DEFERRED item).
edit('00d3abe0', 'src', '''CPrisonerSpeechString __fastcall PrisonerNumberString(int); //99F830 resultECX, numberEDX
CPrisonerSpeechString __fastcall operator+(const char*,const CPrisonerSpeechString&); //99F690''',
'''CCharString __fastcall GFIntToCharString(long); //99F830 resultECX, numberEDX; ?GFIntToCharString@@YI?AVCCharString@@J@Z (Ego_r / FableWin)
CCharString __fastcall operator+(const char*,const CCharString&); //99F690 ??H@YI?AVCCharString@@PBDABV0@@Z (Ego_r / FableWin)''')
edit('00d3abe0', 'src', '''static_cast<CCharString&>(out)="TEXT_QST_B01_PROPHET"+PrisonerNumberString(Quest->FreedCount+1);''',
     '''static_cast<CCharString&>(out)="TEXT_QST_B01_PROPHET"+GFIntToCharString(Quest->FreedCount+1);''')
# ---- 0x007E72F0 ?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z (Ego_r +
#      FableWin; virtual, engine/CScriptGameResourceObjectScriptedThingBase.h slot 4): 0x00E0E2E0 pushes 1, 1, 0, 0 (retail
#      0x00E0E4FD..0x00E0E503), so the third argument is move type 0, not a float; the call stays direct (a named local object).
edit('00e0e2e0', 'src', '''    void MoveToPosition(const C3DVector& position, float a2, float a3, bool a4, bool a5);   // 0x007E72F0''',
     '''    virtual void MoveToPosition(const C3DVector& position, float a2, EScriptEntityMoveType moveType, bool a4, bool a5);   // 0x007E72F0 (header slot 4; called directly on the local)''')
edit('00e0e2e0', 'src', '''class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes''',
     '''enum EScriptEntityMoveType { };
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes''')
edit('00e0e2e0', 'src', '''        handle.MoveToPosition(StartPos, 0.0f, 0.0f, true, true);''', '''        handle.MoveToPosition(StartPos, 0.0f, (EScriptEntityMoveType)0, true, true);''')
# ---- the other MoveToPosition stragglers of the baseline index (QAE / long move type spellings, not in the wave-10 list): the
#      header's virtual slot 4 with EScriptEntityMoveType; every call is on a named local (direct), so only the spelling changes.
V = '    virtual void MoveToPosition(const C3DVector& pos, float speed, EScriptEntityMoveType moveType, bool a, bool b);   // 0x007E72F0 (header slot 4; called directly on the local)'
for a in ('00d37530', '00daa260', '00f045e0'):
    edit(a, 'src', '    void MoveToPosition(const C3DVector& pos, float speed, long moveType, bool a, bool b);   // 0x007E72F0', V)
    edit(a, 'src', 'class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes',
         'enum EScriptEntityMoveType { };\nclass CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes')
edit('00d37530', 'src', 'villager.MoveToPosition(ParentClass->VillagerEscapeMarker.GetPos(), 1.0f, 1, false, true);',
     'villager.MoveToPosition(ParentClass->VillagerEscapeMarker.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);')
edit('00daa260', 'src', 'generator.MoveToPosition(windmillPos, 0.0f, 1, false, true);', 'generator.MoveToPosition(windmillPos, 0.0f, (EScriptEntityMoveType)1, false, true);')
edit('00f045e0', 'src', 'handle.MoveToPosition(exitPoint.GetPos(), 1.0f, 1, false, true);', 'handle.MoveToPosition(exitPoint.GetPos(), 1.0f, (EScriptEntityMoveType)1, false, true);')
# 0x00CBF679: the opaque 0x10-byte model gains the vptr (+0, CBaseObject's) so the body shrinks to 0xc; the ctor / dtor stay out of line
edit('00cbf679', 'src', '    void MoveToPosition(const C3DVector& position, float speed, EScriptEntityMoveType type, bool a, bool b);   // 0x007E72F0',
     '    virtual void MoveToPosition(const C3DVector& position, float speed, EScriptEntityMoveType type, bool a, bool b);   // 0x007E72F0 (header slot 4; direct call on the local)')
edit('00cbf679', 'src', '    unsigned char _body[0x10];', '    unsigned char _body[0xc];                           // + the vptr = 0x10 bytes')
# 0x00D50C60: the move type was a typedef of long (J); the PDB parameter is the enum (W4EScriptEntityMoveType@@)
edit('00d50c60', 'src', 'typedef long EScriptEntityMoveType;', 'enum EScriptEntityMoveType { };')
# 0x00D2C810: its invented MoveTo(pos, speed, bool, bool, bool) is the same 0x007E72F0 (retail pushes 1, 0, 1, 2.0f: move type 1)
edit('00d2c810', 'src', '    void MoveTo(const C3DVector& pos, float speed, bool a, bool b, bool c);   // 0x007E72F0', V)
edit('00d2c810', 'src', 'class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes',
     'enum EScriptEntityMoveType { };\nclass CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes')
edit_all('00d2c810', 'src', 'face.MoveTo(Game->GetHero().GetPos(), 2.0f, true, false, true);',
         'face.MoveToPosition(Game->GetHero().GetPos(), 2.0f, (EScriptEntityMoveType)1, false, true);', 4)
edit('00d2c810', 'src', '0x00CD2770 / MoveTo 0x007E72F0 /', '0x00CD2770 / MoveToPosition 0x007E72F0 /')
# ---- 0x00CD23B9 ?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ (Ego_r + FableWin, const): five rows declared it as
#      ScriptResource::IsScripting on their derived handle model (wave-10 DEFERRED); the member moves to the base (no this adjustment).
for a in ('00d2d060', '00d408b0', '00e11bb0', '00e4a060', '00ec46b0'):
    edit(a, 'src', '    void Release();   // 0x00CD2770\n', '    void Release();   // 0x00CD2770\n    bool IsValid() const;   // 0x00CD23B9 (Ego_r / FableWin: QBE)\n')
    edit(a, 'src', ' bool IsScripting(); //00CD23B9\n', '')
    edit_all(a, 'src', 'resource.IsScripting()', 'resource.IsValid()')
# ---- CPlayerManager getters (engine/CPlayerManager.h, Ego_r 0x004450F0 / 0x004450A0 + FableWin): 0x005E3EA0 declared the non-const
#      GetMainPlayer 0x00449970 as returning a pointer (QAEPAV); 0x008FD0A0's invented CPlayerManagerAccess::GetMainPlayer is
#      0x00449700 ?GetMainPlayerNumber@CPlayerManager@@QBEJXZ (`mov eax,[ecx+0x1c]; ret`, the header's const member).
edit('005e3ea0', 'src', '    CPlayer* GetMainPlayer();                           // 0x00449970', '    CPlayer& GetMainPlayer();                           // 0x00449970 ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (Ego_r / FableWin)')
edit('005e3ea0', 'src', 'g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetCurrentMode()', 'g_pMainGameComponent->PlayerManager->GetMainPlayer().GetCurrentMode()')
edit('008fd0a0', 'src', 'struct CPlayerManagerAccess : CPlayerManager { long GetMainPlayer(); };\n', '')
edit('008fd0a0', 'src', 'long playerID = ((CPlayerManagerAccess*)brain->PlayerManager)->GetMainPlayer();',
     'long playerID = brain->PlayerManager->GetMainPlayerNumber();   // 0x00449700 ?GetMainPlayerNumber@CPlayerManager@@QBEJXZ')
edit('008fd0a0', 'src', 'class CPlayerManager { public: long MainPlayer; };', 'class CPlayerManager { public: long MainPlayer; long GetMainPlayerNumber() const; };')
