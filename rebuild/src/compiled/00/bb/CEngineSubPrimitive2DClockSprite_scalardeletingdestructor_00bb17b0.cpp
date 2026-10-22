// CEngineSubPrimitive2DClockSprite::`vector deleting destructor'  @ 0x00bb17b0
// push esi; mov esi,ecx; call dtor; test [esp+8],1; je L; mov ecx,esi; call delete; L: mov eax,esi; ret 4

struct CEngineSubPrimitive2DClockSprite {
    void destroy();                       // 0x0099a300  scalar destructor
    void freeMem();                       // 0x00beb550  member operator delete (this in ecx)
    void* vec_deleting_destructor(unsigned int flags);
};

void* CEngineSubPrimitive2DClockSprite::vec_deleting_destructor(unsigned int flags)
{
    this->destroy();
    if (flags & 1)
        this->freeMem();
    return this;
}