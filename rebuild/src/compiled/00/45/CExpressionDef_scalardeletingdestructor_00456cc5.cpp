#pragma optimize("s",on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CExpressionDef { public: int vt; };

void* __fastcall CExpressionDef_scalar_deleting_destructor(CExpressionDef* def, int edx_unused, unsigned int flags) {
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}