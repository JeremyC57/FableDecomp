#pragma optimize("s",on)
// Leaf: UpdateShadowScene
struct SVec2 { int a; int b; };

class CEngineLightingManager { public:
    char pad[0x18];
    int  mode;      // +0x18
    char pad2[0x24 - 0x1c];
    void* objA;     // +0x24
    void* objB;     // +0x28
};

#pragma optimize("y", off)

extern void __stdcall Marker(const char* s);

struct ObjA { int* Fetch(SVec2* out); };
struct ObjB { void Apply(SVec2* p); };

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00993F90 ?Write2DVector@CDataOutputStream@@QAEXABVC2DVector@@@Z (was ?Apply@ObjB@@QAEXPAUSVec2@@@Z)
class C2DVector;
class CDataOutputStream { public: void Write2DVector(C2DVector const&); };
void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* edx, SVec2* out)
{
    SVec2 tmp;
    Marker("");
    switch (self->mode) {
    case 2: {
        int* r = ((ObjA*)self->objA)->Fetch(&tmp);
        out->a = r[0];
        out->b = r[1];
        break;
    }
    case 3:
        reinterpret_cast<CDataOutputStream*>(((ObjB*)self->objB))->Write2DVector(*reinterpret_cast<C2DVector const*>(out));
        break;
    }
}