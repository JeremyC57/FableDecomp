class CShaderResource {
public:
    void CloseShaderBank();   // 0x00A5D750
};
struct S; 
struct S { char pad[20]; int fo; void M(); };
void S::M(){ ((CShaderResource*)(this))->CloseShaderBank(); this->fo = 0; }