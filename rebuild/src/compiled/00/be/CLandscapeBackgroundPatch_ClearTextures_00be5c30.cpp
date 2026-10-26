struct CTexMgr {
  void ReleaseTexture(void* t);
};
struct CGlobal { char pad[0x6b0]; CTexMgr* mgr; };
class CLandscapeBackgroundPatch { public: char pad[0xb8]; void* tex; };

extern CGlobal* g_obj_01436ea8;

void __fastcall CLandscapeBackgroundPatch_ClearTextures(CLandscapeBackgroundPatch* self)
{
  void* tex = self->tex;
  if (tex) {
    g_obj_01436ea8->mgr->ReleaseTexture(tex);
    self->tex = 0;
  }
}