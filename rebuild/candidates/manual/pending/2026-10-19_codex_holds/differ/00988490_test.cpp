#include <cstdio>
struct M4 { unsigned int x,y,z,w; };
struct SetTail { unsigned int* p; ~SetTail() { *p |= 0xc990; } };
struct CShaderRenderManager {
    volatile unsigned int m[16];
    unsigned int tail;
    void SetProjectionTransform(M4,M4,M4,M4);
};
void CShaderRenderManager::SetProjectionTransform(M4 a,M4 b,M4 c,M4 d) {
    SetTail finish; finish.p = &tail;
    m[0]=a.x; m[1]=b.x; m[2]=c.x; m[3]=d.x;
    m[4]=a.y; m[5]=b.y; m[6]=c.y; m[7]=d.y;
    m[8]=a.z; m[9]=b.z; m[10]=c.z; m[11]=d.z;
    m[12]=a.w; m[13]=b.w; m[14]=c.w; m[15]=d.w;
}
int main() {
    CShaderRenderManager obj;
    obj.tail = 0x4000;
    M4 a={1,2,3,4}, b={5,6,7,8}, c={9,10,11,12}, d={13,14,15,16};
    obj.SetProjectionTransform(a,b,c,d);
    const unsigned int expected[16] = {1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};
    for (int i=0; i<16; ++i) if (obj.m[i] != expected[i]) {
        std::printf("00988490_TEST FAIL\n"); return 1;
    }
    if (obj.tail != (0x4000u | 0xc990u)) {
        std::printf("00988490_TEST FAIL\n"); return 1;
    }
    std::printf("00988490_TEST PASS\n");
    return 0;
}