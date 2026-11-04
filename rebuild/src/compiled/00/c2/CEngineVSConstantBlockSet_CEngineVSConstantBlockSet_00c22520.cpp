// CSoundEventSet::~CSoundEventSet() -- compiler-generated member teardown (EH-tracked) of a vertex-shader constant aggregate: the parameter blocks, data-bank arrays and resource handles in reverse declaration order
// Leaf: ??1CSoundEventSet@@QAE@XZ
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C287E0 ??1CStackOwner_00c287e0@@QAE@XZ (was ??1CResourceE@@QAE@XZ); 0x00C24240 ??1CVtable_012b3aa4@@QAE@XZ (was ??1CResourceD@@QAE@XZ); 0x00C19250 ??1PKCTRRedblackTree@@QAE@XZ (was ??1CParamBlock@@QAE@XZ); 0x00C23FE0 ??1CVtable_012b3a9c@@QAE@XZ (was ??1CResourceC@@QAE@XZ); 0x00C23BA0 ??1CVtable_012b3a94@@QAE@XZ (was ??1CResourceA@@QAE@XZ); 0x00C22520 ??1CSoundEventSet@@QAE@XZ (was ??1CEngineVSConstantBlockSet@@QAE@XZ)
// Flags: /EHsc
// Layout: the row label (CEngineVSConstantLayoutLights ctor) is wrong -- that class is 0x150 bytes and this is a destructor of an
// unidentified >=0x338-byte non-polymorphic aggregate (no own vptr store): 0x30-byte parameter blocks (vtables 0x012B3xxx over base
// 0x012B1AB0, a 0x28-byte CParam at +8, dtor 0x00C19250), 0x18-byte bank arrays (dtor 0x00C22A00) and resource handles
// (0x00C23BA0 / 0x00C23FE0 / 0x00C24240 / 0x00C287E0); member names are placeholders.
// Shape: retail writes no EH state before the bank arrays' destructor calls: that dtor is declared throw() (VC7.1 skips the
// state store for a no-throw member destructor); every other member updates the state.
struct PKCTRRedblackTree { unsigned char _b[0x28]; ~PKCTRRedblackTree(); };   // 0x00C19250
struct CBankArray { unsigned char _b[0x18]; ~CBankArray() throw(); };     // 0x00C22A00
struct CVtable_012b3a94 { unsigned char _b[0x18]; ~CVtable_012b3a94(); };     // 0x00C23BA0
struct CVtable_012b3a9c { unsigned char _b[0x18]; ~CVtable_012b3a9c(); };     // 0x00C23FE0
struct CVtable_012b3aa4 { unsigned char _b[0x1c]; ~CVtable_012b3aa4(); };     // 0x00C24240
struct CStackOwner_00c287e0 { unsigned char _b[0x4]; ~CStackOwner_00c287e0(); };      // 0x00C287E0
struct CVSConstantBase { virtual ~CVSConstantBase() { } long Register; };   // vtable 0x012B1AB0
struct CVSConstantMain : CVSConstantBase { virtual ~CVSConstantMain() { } PKCTRRedblackTree Param; };
struct CVSConstantAmbient : CVSConstantBase { virtual ~CVSConstantAmbient() { } PKCTRRedblackTree Param; };
struct CVSConstantDirectional : CVSConstantBase { virtual ~CVSConstantDirectional() { } PKCTRRedblackTree Param; };
struct CVSConstantPointA : CVSConstantBase { virtual ~CVSConstantPointA() { } PKCTRRedblackTree Param; };
struct CVSConstantPointB : CVSConstantBase { virtual ~CVSConstantPointB() { } PKCTRRedblackTree Param; };
struct CVSConstantPointC : CVSConstantBase { virtual ~CVSConstantPointC() { } PKCTRRedblackTree Param; };
struct CVSConstantSpot : CVSConstantBase { virtual ~CVSConstantSpot() { } PKCTRRedblackTree Param; };
struct CVSConstantShadow : CVSConstantBase { virtual ~CVSConstantShadow() { } PKCTRRedblackTree Param; };
struct CVSConstantFog : CVSConstantBase { virtual ~CVSConstantFog() { } PKCTRRedblackTree Param; };
struct CVSConstantColour : CVSConstantBase { virtual ~CVSConstantColour() { } PKCTRRedblackTree Param; };
struct CVSConstantBump : CVSConstantBase { virtual ~CVSConstantBump() { } PKCTRRedblackTree Param; };
struct CVSConstantEnv : CVSConstantBase { virtual ~CVSConstantEnv() { } PKCTRRedblackTree Param; };
struct CVSConstantSpecular : CVSConstantBase { virtual ~CVSConstantSpecular() { } PKCTRRedblackTree Param; };
struct CSoundEventSet {
    CVSConstantMain Main;
    CVtable_012b3a94 ResA;
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
    CVtable_012b3a9c ResC;
    CBankArray Bank6;
    CVSConstantSpot Spot;
    CVSConstantShadow Shadow;
    CVSConstantFog Fog;
    CVSConstantColour Colour;
    CVSConstantBump Bump;
    CVtable_012b3aa4 ResD;
    CStackOwner_00c287e0 ResE;
    ~CSoundEventSet();
};
CSoundEventSet::~CSoundEventSet()
{
}