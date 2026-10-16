#include <cstdio>
// C3DMeshInfo copy: every field copied, the shared mesh data gains one reference.
struct Data { unsigned int refs; };
struct Info { Data* MeshData; float box[6]; float sphere[4]; float safe; long physics; float lodErrors[4]; unsigned int lodSizes[4];
    Info() {} Info(const Info& o) : MeshData(o.MeshData) { if (MeshData) ++MeshData->refs; for (int i = 0; i < 6; ++i) box[i] = o.box[i]; for (int i = 0; i < 4; ++i) { sphere[i] = o.sphere[i]; lodErrors[i] = o.lodErrors[i]; lodSizes[i] = o.lodSizes[i]; } safe = o.safe; physics = o.physics; } };
int main() {
    Data d = { 1 }; Info a; a.MeshData = &d; for (int i = 0; i < 6; ++i) a.box[i] = (float)i; for (int i = 0; i < 4; ++i) { a.sphere[i] = 1; a.lodErrors[i] = 0.5f * i; a.lodSizes[i] = 10u * i; } a.safe = 3; a.physics = -1;
    Info b(a);
    if (d.refs != 2 || b.box[5] != 5.0f || b.lodSizes[3] != 30u || b.physics != -1 || b.lodErrors[2] != 1.0f) { std::printf("00a2a400_TEST FAIL\n"); return 1; }
    std::printf("00a2a400_TEST PASS\n"); return 0; }
