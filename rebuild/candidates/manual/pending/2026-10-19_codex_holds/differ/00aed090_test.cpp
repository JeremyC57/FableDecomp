#include <cstdio>

struct Vector3 { float x, y, z; };
struct PositionPair { Vector3 start, end; };
struct TransformPosition { float x, y, z; };
struct EmitterModel {
    bool enabled;
    unsigned short layerWord;
    PositionPair position;
    TransformPosition transform, endTransform;
    EmitterModel() : enabled(false), layerWord(0) {}
    void Set2DPosition(const Vector3& v, long layer) {
        if (!enabled) return;
        position.start.x = v.x; position.start.y = v.y; position.start.z = 0.0f;
        position.end.x = v.x; position.end.y = v.y; position.end.z = 0.0f;
        if (layer != -1)
            layerWord ^= ((*(unsigned char*)&layerWord ^ layer) & 0x7f);
        transform.x = position.start.x; transform.y = position.start.y; transform.z = position.start.z;
        endTransform.x = position.end.x; endTransform.y = position.end.y; endTransform.z = position.end.z;
    }
};

int main() {
    EmitterModel e; Vector3 p;
    p.x=1.25f; p.y=-2.5f; p.z=9.0f;
    e.enabled=true; e.layerWord=0xd59a;
    e.Set2DPosition(p,0x26);
    if (e.layerWord != ((0xd59a & 0xff80) | 0x26) ||
        e.position.start.x != p.x || e.position.start.y != p.y || e.position.start.z != 0.0f ||
        e.position.end.x != p.x || e.position.end.y != p.y || e.position.end.z != 0.0f ||
        e.transform.x != p.x || e.transform.y != p.y || e.transform.z != 0.0f ||
        e.endTransform.x != p.x || e.endTransform.y != p.y || e.endTransform.z != 0.0f) {
        std::printf("00aed090_TEST FAIL\n"); return 1;
    }
    e.layerWord=0xa5f3; e.Set2DPosition(p,-1);
    if (e.layerWord != 0xa5f3) { std::printf("00aed090_TEST FAIL\n"); return 1; }
    e.enabled=false; e.layerWord=0x35aa; e.position.start.x=7.0f; e.transform.x=8.0f;
    e.Set2DPosition(p,0x11);
    if (e.layerWord != 0x35aa || e.position.start.x != 7.0f || e.transform.x != 8.0f) {
        std::printf("00aed090_TEST FAIL\n"); return 1;
    }
    std::printf("00aed090_TEST PASS\n"); return 0;
}