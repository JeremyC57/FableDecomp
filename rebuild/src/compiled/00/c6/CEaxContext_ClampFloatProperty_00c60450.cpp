// CEaxContext::ClampFloatProperty(float* pValue, float minimum, float maximum, int clamp) -- range check of an EAX float property: NaN fails (-3); below the minimum or above the maximum the value is clamped when `clamp` is set or when it is within the 0.99999988 tolerance of the bound, else -3; 0 on success
// Leaf: ClampFloatProperty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C60450, 138 bytes (`__fastcall`, `this` unused, `ret 10h`).
// Callees: MSVCR71 `_isnan` (import thunk 0x00BFEC26). Float pool: 0x012B933C = 0.99999988f (the word after the
//   vtable 0x012B9338).
// Identity: the manifest's "Float_ClampWithFuzzyComparison" is propagated. The five callers are the EAX 4.0 reverb
//   float setters of the same object (0x00C633C0 decay time 0.1..20, 0x00C63690 reflections delay 0..0.3, 0x00C638E0
//   reverb delay 0..0.1, 0x00C63A60 echo time 0.075..0.25, 0x00C63BC0 modulation time 0.04..4: the EAX40 ranges), which
//   pass the address of their own value argument and `this`. The object is the audio wrapper's software EAX 4.0
//   context (vtable 0x012B9340): four FX slots (EAXFXSLOTPROPERTIES at +0x84), four EAXREVERBPROPERTIES records (+0xF4),
//   the context record (+0x2B4), the committed reverb (+0x340), dirty mask +0x3BC and last error +0x3D4. Descriptive name.
// Shape: the lower-bound arm returns from its own epilogue. The tolerance tests are POSITIVE `>=` / `<=` relations
//   (`clamp || value >= minimum * k`, `clamp || value * k <= maximum`): `test ah,41h ; jp` / `jnp`, so an unordered
//   compare fails; the negated `minimum * k > value` form gives `je` / `jne` instead.
extern "C" int __cdecl _isnan(double x);

class CEaxContext {
public:
    long ClampFloatProperty(float* pValue, float minimum, float maximum, int clamp);
};

long CEaxContext::ClampFloatProperty(float* pValue, float minimum, float maximum, int clamp)
{
    if (_isnan(*pValue))
        return -3;
    if (*pValue < minimum)
    {
        if (clamp || *pValue >= minimum * 0.99999988f)
        {
            *pValue = minimum;
            return 0;
        }
        return -3;
    }
    if (*pValue > maximum)
    {
        if (clamp || *pValue * 0.99999988f <= maximum)
            *pValue = maximum;
        else
            return -3;
    }
    return 0;
}