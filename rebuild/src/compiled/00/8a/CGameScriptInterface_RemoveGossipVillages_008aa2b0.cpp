
struct Str {
    char* p;
    Str(const Str& o);
    ~Str();
};

struct GObj {
    void RemoveGV(Str* s);
};

void __fastcall CGameScriptInterface_RemoveGossipVillages(void* self, Str category)
{
    Str tmp(category);
    ((GObj*)0x13bae50)->RemoveGV(&tmp);
}