// CAnimatingObjectDef::Transfer @ 004ed459
struct SubObject {
    int dummy;
    void Finalise(); // thiscall, no args -> call 0x5dedc0
};

class CPersistContext { public:
    void Persist(SubObject* p); // thiscall, one stack arg -> call 0x47367a
};

class CAnimatingObjectDef { public:
    char pad[0x28];
    SubObject sub; // at +0x28
};

void __fastcall Transfer(CAnimatingObjectDef* self, int /*edx*/, CPersistContext& ctx) {
    SubObject* s = &self->sub; // esi = this+0x28
    ctx.Persist(s);
    s->Finalise();
}