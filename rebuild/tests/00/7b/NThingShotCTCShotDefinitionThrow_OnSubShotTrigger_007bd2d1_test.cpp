// Behaviour test for 0x007BD2D1 NThingShot::CTCShotDefinitionThrow::OnSubShotTrigger (links the REAL row object: the row body plus the
// TU COMDATs it carries: CDefinitionManager::GetDef<CThingShotDef> with its expanded SetDynamicCast / Release, and CRightHandedSet's
// compiler-generated copy constructor, so the copy handed to Create is the real one).
// Fixtures (native layout): the shot TC (0x9C: vptr, ParentThing +4, PShooter +0x28 / PWeapon +0x30 intelligent pointers, PShotDef +0x38,
// PShotAbilityDef +0x3C, AccuracyPercentage +0x40, DamageMultiplier +0x44, ShotBuildupTime +0x48); CThingShotDef (pointee vptr +0 with
// Delete in slot 1, IVRefCount +4, ShotDefinitionClassName +0x60, ObjectDef +0x7C, NumSubShots +0x98, SubShotType +0x9C,
// SubShotMaxAngle +0xA4); the parent thing (PhysicsTC +0x60 whose vtable slot 72 is GetRHSet); CPlayer (Number +0x28); CCharString
// {PStringData}. Every out-of-line callee is a recording __fastcall stub bound by /alternatename (thiscall = ecx + unused edx).
#include <stdio.h>
#include <string.h>
#include <math.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }

#pragma comment(linker, "/alternatename:@CallRow@8=?OnSubShotTrigger@CTCShotDefinitionThrow@NThingShot@@MAEXXZ")
#pragma comment(linker, "/alternatename:??8?$CBasicString@D@@QBE_NPBD@Z=@StubEq@12")
#pragma comment(linker, "/alternatename:?PMainGameComponent@CMainGameComponent@@1PAV1@A=_g_mgc")
#pragma comment(linker, "/alternatename:?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ=@StubGetPItem@8")
#pragma comment(linker, "/alternatename:?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z=@StubGetPDef@16")
#pragma comment(linker, "/alternatename:?Init@CTCShotDefinitionBase@NThingShot@@QAEXV?$CDefPointer@$$CBVCThingShotDef@@@@PAVCThing@@1ABVC3DVector@@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@@Z=@StubInit@40")
#pragma comment(linker, "/alternatename:?GetPWeapon@CTCShotDefinitionBase@NThingShot@@QBEPAVCThing@@XZ=@StubGetPWeapon@8")
#pragma comment(linker, "/alternatename:?RotateXY@C3DVector@@QAEXM@Z=@StubRotateXY@12")
#pragma comment(linker, "/alternatename:?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z=@StubAddTC@20")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StubStrDtor@8")
#pragma comment(linker, "/alternatename:?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z=@StubCreate@24")
#pragma comment(linker, "/alternatename:?GetPlayer@CThing@@QAEAAVCPlayer@@XZ=@StubGetPlayer@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StubStrCtor@16")
#pragma comment(linker, "/alternatename:?GetPos@CThing@@QBEABVC3DVector@@XZ=@StubGetPos@8")
#pragma comment(linker, "/alternatename:?GetShootAtPos@CTCShotDefinitionBase@NThingShot@@IBE?AVC3DVector@@XZ=@StubShootAt@12")
#pragma comment(linker, "/alternatename:??BCDefString@@QBE?AVCCharString@@XZ=@StubConv@12")
#pragma comment(linker, "/alternatename:?GetDefClassTypeNameFromGlobalIndex@CDefinitionManager@@QBE?AVCDefString@@J@Z=@StubClassName@16")
#pragma comment(linker, "/alternatename:?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ=@StubPeekDM@8")

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }

struct V3 { float x, y, z; };
struct Pointee { void** vt; long ref; };
struct ShotDef { void** vt; long ref; unsigned char p08[0x60 - 8]; long className; unsigned char p64[0x7c - 0x64]; long objectDef;
                 unsigned char p80[0x98 - 0x80]; long numSubShots; long subShotType; float creation; float maxAngle; unsigned char pa8[4]; };
struct IP { void* vt; void* item; };
struct Shot { void* vt; void* parent; unsigned char p08[0x28 - 8]; IP shooter; IP weapon; ShotDef* def; Pointee* ability;
              float accuracy, damage, buildup; unsigned char p4c[0x9c - 0x4c]; };
struct Physics { void** vt; };
struct Thing { unsigned char p00[0x60]; Physics* phys; unsigned char p64[0x100]; };
struct Player { unsigned char p00[0x28]; long number; };
struct RHS { V3 up, fwd; };
struct Str { const char* data; };
typedef char SizeShot[sizeof(Shot) == 0x9c ? 1 : -1];
typedef char OffDef[(size_t)&((ShotDef*)0)->maxAngle == 0xa4 ? 1 : -1];

extern "C" void* g_mgc;
void* g_mgc = (void*)0x1234;
static int g_dm;                       // the definition manager object PeekDefinitionManager returns
static int g_deletes, g_creates, g_addtc, g_inits, g_rot, g_shoot, g_getdef, g_strings;
static float g_angles[16]; static V3 g_targets[16]; static long g_initDef[16];
static Shot g_shot; static ShotDef g_main, g_sub; static Thing g_parent, g_shooterThing, g_weaponThing; static Player g_player;
static V3 g_pos, g_aim; static RHS g_rhs; static Thing g_objects[4]; static int g_createNull[16];
static const char* g_nameText; static long g_objectClassIndex;
static int g_eqTag;                    // the CBasicString the converted class name points at
static int g_otherTag;
static long g_baseRef;

extern "C" bool __fastcall StubEq(void* self, void*, const char* text) { return self == &g_eqTag && strcmp(text, "OBJECT") == 0; }
extern "C" void* __fastcall StubGetPItem(IP* self, void*) { return self->item; }
extern "C" void* __fastcall StubGetPDef(void* self, void*, Pointee** ret, long index)
{
    CHECK(self == &g_dm); g_getdef++;
    *ret = index == 7 ? (Pointee*)&g_sub : 0;
    if (*ret) (*ret)->ref++;
    return ret;
}
extern "C" void __fastcall StubInit(void* self, void*, Pointee* def, void* shooter, void* weapon, const V3* target, float acc, float dmg, float bt, Pointee** ability)
{
    CHECK(def == (Pointee*)&g_sub && def->ref == g_baseRef + 2);   // the row's pointer and the by-value copy each hold a reference
    CHECK(shooter == &g_shooterThing && weapon == &g_weaponThing);
    CHECK(Near(acc, 37.0f) && Near(dmg, 1.5f) && Near(bt, 0.25f) && ability == &g_shot.ability);
    if (g_inits < 16) { g_targets[g_inits] = *target; g_initDef[g_inits] = (long)self; }
    g_inits++;
    def->ref--;                                         // callee destroys its by-value CDefPointer
}
extern "C" void* __fastcall StubGetPWeapon(Shot* self, void*) { CHECK(self == &g_shot); return self->weapon.item; }
extern "C" void __fastcall StubRotateXY(V3* self, void*, float angle) { if (g_rot < 16) g_angles[g_rot] = angle; g_rot++; self->x += angle; self->y -= 2 * angle; }
extern "C" void* __fastcall StubAddTC(Thing* self, void*, const Str* name, bool flag, const void* params)
{
    CHECK(name->data == (const char*)&g_sub.className && flag && params == 0);
    g_addtc++; return (unsigned char*)self + 1;
}
extern "C" void __fastcall StubStrDtor(Str* self, void*) { g_strings--; }
extern "C" void* __fastcall StubStrCtor(Str* self, void*, const char* text, long length) { CHECK(strcmp(text, "") == 0 && length == -1); self->data = text; g_strings++; return self; }
extern "C" void* __fastcall StubCreate(long defIndex, const V3* position, long player, const RHS* rhs, unsigned long flags, const Str* name)
{
    CHECK(defIndex == g_sub.objectDef && position == &g_pos && player == 42 && flags == 0 && strcmp(name->data, "") == 0);
    CHECK(memcmp(rhs, &g_rhs, sizeof(RHS)) == 0 && rhs != &g_rhs);
    int k = g_creates++;
    if (k < 16 && g_createNull[k]) return 0;
    return &g_objects[k & 3];
}
extern "C" void* __fastcall StubGetPlayer(Thing* self, void*) { CHECK(self == &g_shooterThing); return &g_player; }
extern "C" const V3* __fastcall StubGetPos(Thing* self, void*) { CHECK(self == &g_parent); return &g_pos; }
extern "C" V3* __fastcall StubShootAt(Shot* self, void*, V3* ret) { CHECK(self == &g_shot); g_shoot++; *ret = g_aim; return ret; }
extern "C" Str* __fastcall StubConv(long* self, void*, Str* ret)
{
    if (self == &g_sub.className) ret->data = (const char*)&g_sub.className;
    else { CHECK(*self == 555); ret->data = (const char*)g_nameText; }
    g_strings++; return ret;
}
extern "C" long* __fastcall StubClassName(void* self, void*, long* ret, long index) { CHECK(self == &g_dm && index == g_sub.objectDef); *ret = 555; return ret; }
extern "C" void* __fastcall StubPeekDM(void* self, void*) { CHECK(self == g_mgc); return &g_dm; }
extern "C" const RHS* __fastcall StubGetRHSet(Physics* self, void*) { CHECK(self == g_parent.phys); return &g_rhs; }
extern "C" void __fastcall StubDelete(Pointee* self, void*) { g_deletes++; }
extern "C" void __fastcall CallRow(Shot* self, void*);

static void* g_physVt[80]; static void* g_defVt[4]; static Physics g_phys;

static void Setup(long subType, long n, float maxAngle, const char* className)
{
    memset(&g_shot, 0, sizeof g_shot); memset(&g_main, 0, sizeof g_main); memset(&g_sub, 0, sizeof g_sub);
    g_deletes = g_creates = g_addtc = g_inits = g_rot = g_shoot = g_getdef = g_strings = 0; memset(g_createNull, 0, sizeof g_createNull);
    g_defVt[1] = (void*)StubDelete; g_main.vt = g_defVt; g_sub.vt = g_defVt; g_main.ref = 1; g_sub.ref = 1; g_baseRef = 1;
    g_main.subShotType = subType; g_main.numSubShots = n; g_main.maxAngle = maxAngle;
    g_sub.objectDef = 99; g_sub.className = 1234;
    g_physVt[72] = (void*)StubGetRHSet; g_phys.vt = g_physVt; g_parent.phys = &g_phys;
    g_shot.parent = &g_parent; g_shot.def = &g_main; g_shot.shooter.item = &g_shooterThing; g_shot.weapon.item = &g_weaponThing;
    g_shot.accuracy = 37.6f; g_shot.damage = 1.5f; g_shot.buildup = 0.25f;
    g_player.number = 42; g_pos.x = 1; g_pos.y = 2; g_pos.z = 3; g_aim.x = 11; g_aim.y = 22; g_aim.z = 33;
    g_rhs.up.x = 0; g_rhs.up.y = 0; g_rhs.up.z = 1; g_rhs.fwd.x = 0; g_rhs.fwd.y = 1; g_rhs.fwd.z = 0;
    g_nameText = className == 0 ? 0 : (strcmp(className, "OBJECT") == 0 ? (const char*)&g_eqTag : (const char*)&g_otherTag);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    // 1: no sub-shot def -> nothing at all
    g_run = 1; Setup(-1, 3, 30.0f, "OBJECT"); CallRow(&g_shot, 0);
    CHECK(g_getdef == 0 && g_shoot == 0 && g_creates == 0 && g_strings == 0 && g_sub.ref == 1);
    // 2: three OBJECT sub-shots, spread 30: angles -30, 0, 30; the second Create fails
    g_run = 2; Setup(7, 3, 30.0f, "OBJECT"); g_createNull[1] = 1; CallRow(&g_shot, 0);
    CHECK(g_getdef == 1 && g_shoot == 2 && g_creates == 3 && g_addtc == 2 && g_inits == 2 && g_rot == 2);
    CHECK(Near(g_angles[0], -30.0f) && Near(g_angles[1], 30.0f));
    // direction (10,20,30) rotated by the stub then + position
    CHECK(Near(g_targets[0].x, 10 - 30 + 1) && Near(g_targets[0].y, 20 + 60 + 2) && Near(g_targets[0].z, 33));
    CHECK(Near(g_targets[1].x, 10 + 30 + 1) && Near(g_targets[1].y, 20 - 60 + 2) && Near(g_targets[1].z, 33));
    CHECK(g_initDef[0] == (long)&g_objects[0] + 1 && g_initDef[1] == (long)&g_objects[2] + 1);
    CHECK(g_sub.ref == 1 && g_deletes == 0 && g_strings == 0);
    // 3: the class is not OBJECT -> the loop runs but creates nothing
    g_run = 3; Setup(7, 4, 10.0f, "THING"); CallRow(&g_shot, 0);
    CHECK(g_getdef == 1 && g_creates == 0 && g_inits == 0 && g_sub.ref == 1 && g_strings == 0);
    // 4: an empty class name (null string data) is compared with "" -> not OBJECT
    g_run = 4; Setup(7, 2, 10.0f, 0); CallRow(&g_shot, 0);
    CHECK(g_creates == 0 && g_sub.ref == 1 && g_strings == 0);
    // 5: no sub-shots
    g_run = 5; Setup(7, 0, 10.0f, "OBJECT"); CallRow(&g_shot, 0);
    CHECK(g_getdef == 1 && g_shoot == 2 && g_creates == 0 && g_sub.ref == 1 && g_strings == 0);
    // 6: the last reference is the row's -> released through Delete
    g_run = 6; Setup(7, 1, 20.0f, "OBJECT"); g_sub.ref = 0; g_baseRef = 0; CallRow(&g_shot, 0);
    CHECK(g_creates == 1 && g_inits == 1 && g_deletes == 1);
    // 7: five sub-shots: angles -20, -10, 0, 10, 20
    g_run = 7; Setup(7, 5, 20.0f, "OBJECT"); CallRow(&g_shot, 0);
    CHECK(g_creates == 5 && g_inits == 5 && Near(g_angles[0], -20) && Near(g_angles[2], 0) && Near(g_angles[4], 20) && g_sub.ref == 1);
    // 8: sub-shot def index 0 passes the row's >= 0 test, but GetDef only looks up index > 0 and leaves the pointer null, so the
    //    row reads ObjectDef through null (retail behaviour; with `> 0` the row would return quietly)
    g_run = 8; Setup(0, 1, 20.0f, "OBJECT"); int faulted = 0;
    __try { CallRow(&g_shot, 0); } __except (1) { faulted = 1; }
    CHECK(faulted && g_getdef == 0 && g_creates == 0);
    if (!g_fail) printf("007bd2d1_TEST PASS\n");
    return g_fail;
}