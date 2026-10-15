// Behaviour test for AddCameraKey (0x00B317A0): eight parameter values (0x1C-byte records, value at +0x18) become position,
// normalised direction, time and speed of one pushed key.
#include <cstdio>
#include <cmath>
struct Vec3 { float X, Y, Z; Vec3() {} Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; } };
struct Param { unsigned char pad[0x18]; float Value; };
struct Params { Param* Data; };
struct Key { Vec3 Position, Direction; float Time, Speed; };
static Key g_keys[4]; static int g_n;
struct Camera { void push_back(const Key& k) { g_keys[g_n++] = k; } };
static void AddCameraKey(Camera* camera, const Params& params)
{
    Key key;
    key.Position = Vec3(params.Data[0].Value, params.Data[1].Value, params.Data[2].Value);
    key.Direction = Vec3(params.Data[3].Value, params.Data[4].Value, params.Data[5].Value);
    key.Time = params.Data[6].Value;
    key.Speed = params.Data[7].Value;
    key.Direction.Normalise();
    camera->push_back(key);
}
int main()
{
    Param p[8]; float v[8] = { 1, 2, 3, 0, 3, 4, 7.5f, 2 };
    for (int i = 0; i < 8; ++i) p[i].Value = v[i];
    Params ps; ps.Data = p; Camera c;
    AddCameraKey(&c, ps);
    const Key& k = g_keys[0];
    if (g_n != 1 || k.Position.X != 1 || k.Position.Z != 3 || std::fabs(k.Direction.Y - 0.6f) > 1e-5f || std::fabs(k.Direction.Z - 0.8f) > 1e-5f || k.Time != 7.5f || k.Speed != 2) { std::printf("00b317a0_TEST FAIL\n"); return 1; }
    std::printf("00b317a0_TEST PASS\n");
    return 0;
}
