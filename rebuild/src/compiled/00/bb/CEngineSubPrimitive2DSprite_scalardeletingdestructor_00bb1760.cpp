#pragma optimize("s",on)
// CEngineSubPrimitive2DSprite::`scalar deleting destructor' @ 0x00bb1760
// Standard MSVC vector deleting destructor.

extern "C" void __fastcall SubPrim_dtor(void* sprite);   // 0x99a300 scalar dtor (this in ecx)
extern "C" void __fastcall SubPrim_opdelete(void* sprite); // 0xbeb550 operator delete path

struct CEngineSubPrimitiveGameMesh
{
    // vector deleting destructor
    void* scalar_deleting_destructor(unsigned int flags)
    {
        SubPrim_dtor(this);
        if (flags & 1)
            SubPrim_opdelete(this);
        return this;
    }
};

void* run_vdd(CEngineSubPrimitiveGameMesh* p, unsigned int flags)
{
    return p->scalar_deleting_destructor(flags);
}