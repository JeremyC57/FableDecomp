// CEnvironmentGameTickState::operator= -- copies six blended themes and wind/light state.
// Retail006b63c0 length317; manifest CEnvironmentThemeDef is wrong.
// PDB CEnvironmentGameTickState layout establishes all field names; donor blended theme524
// shrinks to500 natively because its six vector descriptors use12 vs16bytes.
// PDB explicitly puts OldWindDirection at unaligned+0xc9d (native+0xc0d), so vector
// alignment1 is real layout evidence, not a hidden offset adjustment.
#pragma pack(push,1)
struct CThemeVector {float X,Y,Z;};
#pragma pack(pop)
struct CThemeColour {float R,G,B,A;};
template<class T> struct CThemeArray {T* Begin;T* End;T* Capacity;};
struct CThemeTextureSet {CThemeArray<int> Textures;CThemeArray<float> Blends;};
struct CBlendedEnvironmentTheme {
 float TimeOfDay;CThemeColour Colours[19];float Scalers[30];CThemeTextureSet TextureSets[3];
 CBlendedEnvironmentTheme& operator=(const CBlendedEnvironmentTheme&); //006b5df0
};
struct CEnvironmentGameTickState {
 CBlendedEnvironmentTheme BlendedTheme[6];float Time,WindSpeed;CThemeVector WindDirection;
 float ThemeWindSpeed;CThemeVector ThemeWindDirection,SunVector,MoonVector,DiffuseVector,ShadowVector;
 bool OverrideWindDirection;CThemeVector OldWindDirection;float OldWindSpeed;
};
void AssignEnvironmentState(CEnvironmentGameTickState& a,const CEnvironmentGameTickState& b){a=b;}