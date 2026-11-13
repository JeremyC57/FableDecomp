// Behaviour test for CTCRandomAppearanceMorph::OnAppearanceDraw (0x00727D00): a copy of the draw logic over a model appearance.
#include <cstdio>
#include <vector>
struct Part { unsigned long MeshIndex; };
struct Prim { int type; unsigned long bank; int graphics; int refreshes; };
struct Appearance {
    std::vector<Prim*> owned; int meshes_built; unsigned long last_mesh;
    Prim* AddPersistentPrimitive(int type, unsigned long bank) { Prim* p = new Prim(); p->type = type; p->bank = bank; owned.push_back(p); return p; }
    void Refresh(Prim* p) { ++p->refreshes; }
    void SetPrimitiveGraphics(Prim* p, unsigned long mesh) { ++p->graphics; ++meshes_built; last_mesh = mesh; }
};
static bool Draw(std::vector<Prim*>& handles, const std::vector<Part>& parts, Appearance& app, unsigned long reasons)
{
    bool first_draw = handles.empty();
    unsigned long i = 0;
    for (std::vector<Part>::const_iterator it = parts.begin(); it != parts.end(); ++it) {
        if (first_draw) {
            handles.resize(i + 1);
            handles[i] = app.AddPersistentPrimitive(2, it->MeshIndex);
        } else if (reasons == 2 || reasons == 1 || reasons == 3) {
            app.Refresh(handles[i]);
            ++i;
            continue;
        }
        app.SetPrimitiveGraphics(handles[i], it->MeshIndex);
        ++i;
    }
    return true;
}
int main()
{
    std::vector<Part> parts; Part a = {7}; Part b = {9}; parts.push_back(a); parts.push_back(b);
    std::vector<Prim*> handles; Appearance app; app.meshes_built = 0; app.last_mesh = 0;
    bool ok = Draw(handles, parts, app, 2);
    if (!ok || handles.size() != 2 || handles[0]->type != 2 || handles[0]->bank != 7 || handles[1]->bank != 9 || app.meshes_built != 2
        || handles[1]->refreshes != 0) { std::printf("00727d00_TEST FAIL\n"); return 1; }
    Draw(handles, parts, app, 1);
    if (app.meshes_built != 2 || handles[0]->refreshes != 1 || handles[1]->refreshes != 1) { std::printf("00727d00_TEST FAIL\n"); return 1; }
    Draw(handles, parts, app, 4);
    if (app.meshes_built != 4 || app.last_mesh != 9 || handles[0]->graphics != 2 || handles.size() != 2) { std::printf("00727d00_TEST FAIL\n"); return 1; }
    for (size_t k = 0; k < app.owned.size(); ++k) delete app.owned[k];
    std::printf("00727d00_TEST PASS\n");
    return 0;
}