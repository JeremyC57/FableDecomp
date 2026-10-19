#include <cstdio>
struct Info { long refs; };
struct Model {
    float d0,d1,d2,s0,s1,s2,p0,p1,p2;
    unsigned char use; long pad;
    void* points0; void* points1; void* points2;
    void* data; Info* info;
};
static Model copy_model(const Model& o) {
    Model n = o;
    if (n.info) ++n.info->refs;
    return n;
}
int main() {
    Info i = { 2 };
    Model a = {1,2,3,4,5,6,7,8,9,1,0,0,0,0,(void*)0x1234,&i};
    Model b = copy_model(a);
    if (b.d0!=1 || b.p2!=9 || b.use!=1 || b.data!=(void*)0x1234 || b.info!=&i || i.refs!=3) {
        std::printf("00953bc0_TEST FAIL\n"); return 1;
    }
    std::printf("00953bc0_TEST PASS\n"); return 0;
}