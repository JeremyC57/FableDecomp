// Byte-exact reconstruction of CHTG_Executioner::OnPredicateFail @ 0x00d732a0
// Leaf: ?CHTG_Executioner_OnPredicateFail@@YIXPAUCHTG_Executioner@@@Z
// Literal (NF418, 2026-11-25): was an extern char whose address was passed `g_PredMsg` (table ?g_PredMsg@@3DA -> 0x012D29D4); now spelled at the use site:
//   "TEXT_QST_030_FAILURE_EXECUTIONER_KILLED", retail's pooled literal at 0x012D29D4 (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).


// Helper subobject living at obj+0x9c; its method takes a void* stack arg.
// Native member => VC7.1 __fastcall (ecx=this, msg on stack, no edx).
class CCharString;
class CCharString { public:
    CCharString const& operator=(char const*a0);   // 0x0099EFE0 (Ego_r.pdb ??4CCharString@@QAEABV0@PBD@Z)
};
struct SubObj {
};

struct Obj {
    char pad[0x9c];
    // SubObj begins at +0x9c (flag byte at +0x4b is within pad)
};

struct CHTG_Executioner {
    char pad[0x14];
    Obj* obj;   // +0x14
};

void __fastcall CHTG_Executioner_OnPredicateFail(CHTG_Executioner* self)
{
    SubObj* s = (SubObj*)((char*)self->obj + 0x9c);
    ((CCharString*)s)->operator=("TEXT_QST_030_FAILURE_EXECUTIONER_KILLED");
    ((char*)self->obj)[0x4b] = 1;
}