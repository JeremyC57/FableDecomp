#pragma optimize("y",off)
#pragma optimize("s",on)
class C3DVector { public: float x, y, z; };
class CRightHandedSet { public: float m[9]; };

void __stdcall GetLookDirHelper(C3DVector* out, CRightHandedSet* rhs, unsigned char* pb);

class CTCCameraPointScripted { public:
    CTCCameraPointScripted* GetLookDirection(C3DVector* out, CRightHandedSet* rhs);
};

CTCCameraPointScripted* CTCCameraPointScripted::GetLookDirection(C3DVector* out, CRightHandedSet* rhs)
{
    unsigned char local;
    GetLookDirHelper(out, rhs, &local);
    return this;
}