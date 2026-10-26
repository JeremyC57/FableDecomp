
// Leaf: CSymbolInfo>_>
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
struct Sub { char pad[32]; };
struct Owner { char pad[0x50]; };

extern "C" void __fastcall Sub_dtor_00414ea0(Sub* s);

void __fastcall _Dest_val(Owner* self)
{
    Sub_dtor_00414ea0((Sub*)((char*)self + 0x48));
    Sub_dtor_00414ea0((Sub*)((char*)self + 0x3c));
    ((CParentDefClassBase*)(self))->CParentDefClassBase::~CParentDefClassBase();
}