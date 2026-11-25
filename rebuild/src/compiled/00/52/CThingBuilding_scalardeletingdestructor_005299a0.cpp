// CThingBuilding::`vector_deleting_destructor'
// retail 0x005299a0  __fastcall(void* this, unsigned int flags) -> void*
//
//   push esi; mov esi,ecx
//   call 0x838a00              ; scalar destructor helper (this in ecx)
//   test byte ptr [esp+8],1    ; flags & 1
//   je  skip
//   mov ecx,esi; call 0x4c73b0 ; this-in-ecx delete helper
//   skip: mov eax,esi; pop esi; ret 4

class CThingBuilding
{ public:
    // scalar destructor helper (retail 0x838a00) — this-in-ecx member call
    // this-in-ecx delete helper (retail 0x4c73b0)
    void DeleteHelper();

    void* ScalarDeletingDestructor(unsigned int flags);
};

// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x00838A00 ??1CThingGameObject@@UAE@XZ (was ?ScalarDtor@CThingBuilding@@QAEXXZ)
class CThingGameObject { public: virtual ~CThingGameObject(); };
void* CThingBuilding::ScalarDeletingDestructor(unsigned int flags)
{
    reinterpret_cast<CThingGameObject*>(this)->CThingGameObject::~CThingGameObject();
    if (flags & 1)
        this->DeleteHelper();
    return this;
}