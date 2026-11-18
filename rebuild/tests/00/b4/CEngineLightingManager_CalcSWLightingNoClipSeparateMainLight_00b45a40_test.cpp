// Standalone C++03 fixture for the recovered lighting method.
#include <cmath>
#include <cstdio>

struct CRGBFloatColour {
    float R, G, B, A;

    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}

    CRGBFloatColour operator*(float scale) const {
        return CRGBFloatColour(R * scale, G * scale, B * scale, A * scale);
    }

    CRGBFloatColour operator+(const CRGBFloatColour& colour) const {
        return CRGBFloatColour(R + colour.R, G + colour.G, B + colour.B, A + colour.A);
    }
};

struct C3DVector {
    float X, Y, Z;

    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const CRGBFloatColour& colour) : X(colour.R), Y(colour.G), Z(colour.B) {}

    C3DVector operator-(const C3DVector& vector) const {
        return C3DVector(X - vector.X, Y - vector.Y, Z - vector.Z);
    }

    void operator+=(const C3DVector& vector) {
        X += vector.X;
        Y += vector.Y;
        Z += vector.Z;
    }

    float Dot(const C3DVector& vector) const { return X * vector.X + Y * vector.Y + Z * vector.Z; }
    float GetSquaredMagnitude() const { return Dot(*this); }

    void Normalise() {
        const float inverse_length = 1.0f / (float)std::sqrt(GetSquaredMagnitude());
        X *= inverse_length;
        Y *= inverse_length;
        Z *= inverse_length;
    }
};

struct CEngineLocalLightElement {
    C3DVector WorldPos;
    CRGBFloatColour Colour;
    float AttenuationDistSquared, AttenuationConstant;

    CEngineLocalLightElement(const C3DVector& pos, const CRGBFloatColour& colour,
        float distance_factor, float extra)
        : WorldPos(pos), Colour(colour), AttenuationDistSquared(distance_factor), AttenuationConstant(extra) {}
};

struct CEngineLocalLightArray {
    const CEngineLocalLightElement* elements;
    long count;
    long GetCount() const { return count; }
    const CEngineLocalLightElement& PeekLight(long index) const { return elements[index]; }
};

class CEngineLightingManager {
public:
    C3DVector DiffuseVector;
    CRGBFloatColour PrimitiveDiffuseColour;
    CRGBFloatColour PrimitiveAmbientColour;
    CRGBFloatColour PrimitiveBacklightColour;

    CEngineLightingManager() : DiffuseVector(-0.5f, 0.0f, 0.0f),
        PrimitiveDiffuseColour(2.0f, 4.0f, 6.0f, 1.0f),
        PrimitiveAmbientColour(1.0f, 2.0f, 3.0f, 1.0f),
        PrimitiveBacklightColour(4.0f, 6.0f, 8.0f, 1.0f) {}

    void CalcSWLightingNoClipSeparateMainLight(const C3DVector&, const C3DVector&,
        const CEngineLocalLightArray&, CRGBFloatColour&, CRGBFloatColour&);
};

const float DELTA_NOTIONAL_ZERO = 0.0001f;
float GFSqr(float value) { return value * value; }

void GFLimit(float& value, float floor, float ceiling)
{
    if (value < floor)
        value = floor;
    else if (value > ceiling)
        value = ceiling;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Recovered method body, with game dependencies supplied by the fixture.
void CEngineLightingManager::CalcSWLightingNoClipSeparateMainLight(const C3DVector& pos,
    const C3DVector& normal, const CEngineLocalLightArray& light_array,
    CRGBFloatColour& main_light_result, CRGBFloatColour& misc_light_result)
{
    float dp = -normal.Dot(DiffuseVector);
    float frontval = MIN(GFSqr(MAX(dp, 0.0f)), 1.0f);
    float backval = MAX(-dp, 0.0f);
    C3DVector main_col = PrimitiveDiffuseColour * frontval;
    main_light_result.R = main_col.X;
    main_light_result.G = main_col.Y;
    main_light_result.B = main_col.Z;
    C3DVector col = PrimitiveAmbientColour + PrimitiveBacklightColour * backval;
    for (long l = 0; l < light_array.GetCount(); ++l) {
        const CEngineLocalLightElement& light = light_array.PeekLight(l);
        C3DVector light_vec = light.WorldPos - pos;
        light_vec.Normalise();
        float dot = light_vec.Dot(normal);
        GFLimit(dot, 0.0f, 1.0f);
        if (dot > DELTA_NOTIONAL_ZERO) {
            float dist2 = (light.WorldPos - pos).GetSquaredMagnitude();
            float falloff = 1.0f / (dist2 * light.AttenuationDistSquared) + light.AttenuationConstant;
            dot *= falloff;
            GFLimit(dot, 0.0f, 1.0f);
            col += light.Colour * dot;
        }
    }
    misc_light_result.R = col.X;
    misc_light_result.G = col.Y;
    misc_light_result.B = col.Z;
}

static bool Equals(const CRGBFloatColour& colour, float r, float g, float b, float a)
{
    return colour.R == r && colour.G == g && colour.B == b && colour.A == a;
}

int main()
{
    const C3DVector origin(0.0f, 0.0f, 0.0f);
    const C3DVector normal(1.0f, 0.0f, 0.0f);
    const CEngineLocalLightElement elements[] = {
        CEngineLocalLightElement(C3DVector(-1, 0, 0), CRGBFloatColour(20, 20, 20, 0), 1, 0),
        CEngineLocalLightElement(C3DVector(0, 1, 0), CRGBFloatColour(20, 20, 20, 0), 1, 0),
        CEngineLocalLightElement(C3DVector(2, 0, 0), CRGBFloatColour(4, 8, 12, 0), 0.5f, 0.25f),
        CEngineLocalLightElement(C3DVector(1, 0, 0), CRGBFloatColour(2, 3, 5, 0), 1, 0.5f),
        CEngineLocalLightElement(C3DVector(1, 0, 0), CRGBFloatColour(20, 20, 20, 0), 1, -2),
    };
    const CEngineLocalLightArray empty = { 0, 0 };
    const CEngineLocalLightArray lights = { elements, sizeof(elements) / sizeof(elements[0]) };
    CEngineLightingManager manager;
    CRGBFloatColour main(0, 0, 0, 9), misc(0, 0, 0, 8);

    manager.CalcSWLightingNoClipSeparateMainLight(origin, normal, empty, main, misc);
    bool ok = Equals(main, 0.5f, 1.0f, 1.5f, 9) && Equals(misc, 1, 2, 3, 8);
    manager.DiffuseVector = C3DVector(0.5f, 0.0f, 0.0f);
    manager.CalcSWLightingNoClipSeparateMainLight(origin, normal, lights, main, misc);
    ok = ok && Equals(main, 0, 0, 0, 9) && Equals(misc, 8, 14, 21, 8);
    manager.DiffuseVector = C3DVector(-2.0f, 0.0f, 0.0f);
    manager.CalcSWLightingNoClipSeparateMainLight(origin, normal, empty, main, misc);
    ok = ok && Equals(main, 2, 4, 6, 9) && Equals(misc, 1, 2, 3, 8);

    std::puts(ok ? "00b45a40_TEST PASS" : "00b45a40_TEST FAIL");
    return ok ? 0 : 1;
}