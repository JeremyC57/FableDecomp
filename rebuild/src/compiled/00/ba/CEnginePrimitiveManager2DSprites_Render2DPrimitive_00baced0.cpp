
struct Prim  { char pad[8]; int f8; };  // prim->f8 at +0x08
struct Chain;
class CEnginePrimitiveManager2DSprites { public:
    virtual void v0();  virtual void v1();  virtual void v2();  virtual void v3();
    virtual void v4();  virtual void v5();  virtual void v6();  virtual void v7();
    virtual void v8();  virtual void v9();  virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void RenderTypeA(Prim* primitive, Chain* c, int t);  // vtbl+0x34 (slot 13)
    virtual void RenderTypeB(Prim* primitive, Chain* c, int t);  // vtbl+0x38 (slot 14)
    char pad4[8 - 4];
    unsigned char active;    // +0x08
    void Render2DPrimitive(Prim* primitive, Chain* c, int t);
};
void CEnginePrimitiveManager2DSprites::Render2DPrimitive(Prim* primitive, Chain* c, int t)
{
    if (this->active) {
        if (primitive->f8 == 0x23)
            this->RenderTypeA(primitive, c, t);
        else
            this->RenderTypeB(primitive, c, t);
    }
}