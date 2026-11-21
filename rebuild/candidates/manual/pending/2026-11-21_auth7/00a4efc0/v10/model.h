// VC10 --stl standalone model of the Ego_d types the row uses (evidence only; layouts and signatures from Ego_d.pdb TPI, ecls.py)
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
extern const float DELTA_NOTIONAL_ZERO;
class CCharStringData;
class CCharString { CCharStringData* PStringData; const char* CharStringData;
public: CCharString(); CCharString(const CCharString&); ~CCharString(); long GetLength() const; const char* GetAsCharArray() const; };
template<class T> class CArray : public std::vector<T> {
public:
 CArray(); explicit CArray(unsigned int n); CArray(unsigned int n, const T& v); CArray(const CArray& o); ~CArray();
 const T& operator[](unsigned int i) const; T& operator[](unsigned int i);
 void push_back(); void push_back(const T& v);
 unsigned int GetByteSize() const;
};
class C3DVector { public: float X, Y, Z;
 C3DVector(const C3DVector&); C3DVector(float x, float y, float z); C3DVector();
 const C3DVector operator-(const C3DVector&) const; const C3DVector operator*(float) const;
 float GetMagnitude() const; bool IsNonZero() const; };
class C4DVector { public: float X, Y, Z, W; C4DVector(); };
class CQuaternion { public: float X, Y, Z, W; CQuaternion(); CQuaternion(float, float, float, float);
 bool Equals(const CQuaternion&) const; bool IsIdentity() const; };
class CMatrix3x4 { public: float M[12]; };
class C3DKeyframe { public:
 CQuaternion GetRotation() const; C3DVector GetPosition() const; C3DVector GetScaling() const;
 void SetScaling(const C3DVector&); bool InitialiseFromMatrix(const CMatrix3x4&);
 CQuaternion Rotation; C4DVector Position; C4DVector Scaling; };
enum EBoneType { BONE_TYPE_DUMMY = 0 };
class C3DAnimFileSequenceChunk { char pad[104];
public: const CCharString GetBoneName() const; float GetSamplesPerSecond() const; unsigned long GetMatrixCount() const;
 const CMatrix3x4* GetMatrix(unsigned long) const; CMatrix3x4* GetMatrix(unsigned long); EBoneType GetBoneType() const; };
class CSimpleMemoryPool { char pad[28];
public: void* Allocate(unsigned long); template<class T> T* AllocateType(unsigned long count); };
class C3DAnimationSequenceData {
public:
 enum ETrackMode { TRACKMODE_IDENTITY=0, TRACKMODE_CONSTANT=1, TRACKMODE_NORMAL=2, TRACKMODE_PALETTED=3, TRACKMODE_MOTION_BIT=2 };
 class C3DVectorWord { public: short X, Y, Z; C3DVectorWord(const C3DVector&); C3DVectorWord();
  bool operator==(const C3DVectorWord&) const; C3DVector operator*(float) const; };
 unsigned long GetFrameCount() const; void LoopProbe(int);
 bool InitialiseFromFileData(const C3DAnimFileSequenceChunk& file_data, CSimpleMemoryPool& alloc);
private:
 void SetupIdentityFlag(); float GetMaximumAbsoluteComponent(const C3DVector& vect);
 char* BoneName; float SamplesPerSecond; unsigned short FrameCount;
 unsigned char BoneType:5; unsigned char EnabledFlag:1; unsigned char AnimatedFlag:1; unsigned char IdentityFlag:1;
 unsigned char RotationTrackMode:2; unsigned char PositionTrackMode:2; unsigned char ScalingTrackMode:2;
 float PositionFactor; float ScalingFactor; CQuaternion* RotationTrack;
 unsigned short RotationTrackSize, PositionTrackSize, PalettedRotationTrackSize, PalettedPositionTrackSize;
 C3DVectorWord* PositionTrack; unsigned char* PalettedRotationTrack; unsigned char* PalettedPositionTrack;
};
typedef C3DAnimationSequenceData::C3DVectorWord C3DVectorWord;
