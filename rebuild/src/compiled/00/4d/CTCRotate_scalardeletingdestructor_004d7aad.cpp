#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
void* __fastcall CTCRotate_CompositeScalarDeletingDestructor(void* rotate, int, unsigned int flags)
{
    ((CCharString*)(static_cast<unsigned char*>(rotate) + 44))->~CCharString();
    ((CTCBase*)(rotate))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(rotate);
    return rotate;
}