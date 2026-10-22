// CEngineVSConstantBlockSet::~CEngineVSConstantBlockSet() -- compiler-generated member teardown (EH-tracked) of a vertex-shader constant aggregate: the parameter blocks, data-bank arrays and resource handles in reverse declaration order
// Leaf: ??1CEngineVSConstantBlockSet
// Flags: /EHsc
// Layout: the row label (CEngineVSConstantLayoutLights ctor) is wrong -- that class is 0x150 bytes and this is a destructor of an
// unidentified >=0x338-byte non-polymorphic aggregate (no own vptr store): 0x30-byte parameter blocks (vtables 0x012B3xxx over base
// 0x012B1AB0, a 0x28-byte CParam at +8, dtor 0x00C19250), 0x18-byte bank arrays (dtor 0x00C22A00) and resource handles
// (0x00C23BA0 / 0x00C23FE0 / 0x00C24240 / 0x00C287E0); member names are placeholders.
// Shape: retail writes no EH state before the bank arrays' destructor calls: that dtor is declared throw() (VC7.1 skips the
// state store for a no-throw member destructor); every other member updates the state.
struct CParamBlock { unsigned char _b[0x28]; ~CParamBlock(); };   // 0x00C19250
struct CBankArray { unsigned char _b[0x18]; ~CBankArray() throw(); };     // 0x00C22A00
struct CResourceA { unsigned char _b[0x18]; ~CResourceA(); };     // 0x00C23BA0
struct CResourceC { unsigned char _b[0x18]; ~CResourceC(); };     // 0x00C23FE0
struct CResourceD { unsigned char _b[0x1c]; ~CResourceD(); };     // 0x00C24240
struct CResourceE { unsigned char _b[0x4]; ~CResourceE(); };      // 0x00C287E0
struct CVSConstantBase { virtual ~CVSConstantBase() { } long Register; };   // vtable 0x012B1AB0
struct CVSConstantMain : CVSConstantBase { virtual ~CVSConstantMain() { } CParamBlock Param; };
struct CVSConstantAmbient : CVSConstantBase { virtual ~CVSConstantAmbient() { } CParamBlock Param; };
struct CVSConstantDirectional : CVSConstantBase { virtual ~CVSConstantDirectional() { } CParamBlock Param; };
struct CVSConstantPointA : CVSConstantBase { virtual ~CVSConstantPointA() { } CParamBlock Param; };
struct CVSConstantPointB : CVSConstantBase { virtual ~CVSConstantPointB() { } CParamBlock Param; };
struct CVSConstantPointC : CVSConstantBase { virtual ~CVSConstantPointC() { } CParamBlock Param; };
struct CVSConstantSpot : CVSConstantBase { virtual ~CVSConstantSpot() { } CParamBlock Param; };
struct CVSConstantShadow : CVSConstantBase { virtual ~CVSConstantShadow() { } CParamBlock Param; };
struct CVSConstantFog : CVSConstantBase { virtual ~CVSConstantFog() { } CParamBlock Param; };
struct CVSConstantColour : CVSConstantBase { virtual ~CVSConstantColour() { } CParamBlock Param; };
struct CVSConstantBump : CVSConstantBase { virtual ~CVSConstantBump() { } CParamBlock Param; };
struct CVSConstantEnv : CVSConstantBase { virtual ~CVSConstantEnv() { } CParamBlock Param; };
struct CVSConstantSpecular : CVSConstantBase { virtual ~CVSConstantSpecular() { } CParamBlock Param; };
struct CEngineVSConstantBlockSet {
    CVSConstantMain Main;
    CResourceA ResA;
    CBankArray Bank0;
    CBankArray Bank1;
    CBankArray Bank2;
    CBankArray Bank3;
    CBankArray Bank4;
    CBankArray Bank5;
    CVSConstantAmbient Ambient;
    CVSConstantDirectional Directional;
    CVSConstantPointA Point0;
    CVSConstantPointA Point1;
    CVSConstantPointA Point2;
    CVSConstantPointB PointB;
    CResourceC ResC;
    CBankArray Bank6;
    CVSConstantSpot Spot;
    CVSConstantShadow Shadow;
    CVSConstantFog Fog;
    CVSConstantColour Colour;
    CVSConstantBump Bump;
    CResourceD ResD;
    CResourceE ResE;
    ~CEngineVSConstantBlockSet();
};
CEngineVSConstantBlockSet::~CEngineVSConstantBlockSet()
{
}