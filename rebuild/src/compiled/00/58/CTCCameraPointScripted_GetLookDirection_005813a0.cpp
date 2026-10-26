#pragma optimize("s",on)
class C3DVector { public: float x, y, z; };
class CRightHandedSet { public: float m[9]; };

// callee at 0x581266 (cdecl): takes (out, rhs, &flag)
void __stdcall GetLookDir_helper(C3DVector* out, CRightHandedSet* rhs, char* flag);

class CTCCameraPointScripted { public:
    CTCCameraPointScripted* GetLookDirection(C3DVector* out, CRightHandedSet* rhs);
};

CTCCameraPointScripted* CTCCameraPointScripted::GetLookDirection(C3DVector* out, CRightHandedSet* rhs)
{
    char flag;
    GetLookDir_helper(out, rhs, &flag);
    return this;
}