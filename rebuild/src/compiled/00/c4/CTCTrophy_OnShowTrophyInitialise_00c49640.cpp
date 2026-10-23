struct Sub; extern void __fastcall sub_00c49ff0(Sub* p);
struct S { char pad[0x70]; int cnt; void M(); };
void S::M(){ sub_00c49ff0((Sub*)((char*)this+0x1c)); this->cnt++; }