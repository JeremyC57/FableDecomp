// CEngineSubPrimitiveSprite::`vector deleting destructor'  @ 0x00bb1740
struct CEngineSubPrimitiveSprite {
    void Destroy();   // scalar destructor -> 0x0099a300 (this in ecx)
    void* VDD(unsigned int flags);
};

// free-the-object helper (this in ecx, cleans own stack) -> 0x00beb550
void __fastcall FreeMesh(CEngineSubPrimitiveSprite* sprite);

void* CEngineSubPrimitiveSprite::VDD(unsigned int flags)
{
    this->Destroy();          // call 0x0099a300
    if (flags & 1)
        FreeMesh(this);       // mov ecx,esi; call 0x00beb550
    return this;              // mov eax,esi
}