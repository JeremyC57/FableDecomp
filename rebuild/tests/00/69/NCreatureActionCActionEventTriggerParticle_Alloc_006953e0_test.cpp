// behaviour test for 0x006953E0: Alloc builds a fresh event carrying the frame, thing and camera
#include <cstdio>
struct Event { long frame; void* thing; void* camera; Event(long f, void* t, void* c) : frame(f), thing(t), camera(c) { }
    Event* Alloc(long f, void* t, void* c) const { return new Event(f, t, c); } };
int main()
{
    int thing, camera; Event proto(0, 0, 0);
    Event* e = proto.Alloc(12, &thing, &camera);
    if (!e || e == &proto || e->frame != 12 || e->thing != &thing || e->camera != &camera) { std::printf("006953e0_TEST FAIL\n"); return 1; }
    delete e;
    std::printf("006953e0_TEST PASS\n");
    return 0;
}