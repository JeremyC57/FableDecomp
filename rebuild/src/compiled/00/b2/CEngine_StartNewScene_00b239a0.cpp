// Leaf: ?StartNewScene@CEngine@@UAEXKM@Z
class CEngine { public:
  char pad[0x14];
  unsigned char active;   // +0x18
  unsigned long sceneId;  // +0x1c
  float sceneParam;       // +0x20
    virtual void StartNewScene(unsigned long, float);
};

void CEngine::StartNewScene(unsigned long id, float param)
{
    CEngine* engine = this;
  engine->active = 1;
  engine->sceneId = id;
  engine->sceneParam = param;
}
// Flatten route (flatten_pilot v4, NF324): CEngine_StartNewScene -> CEngine::StartNewScene, the RTTI-placed PDB member; class model: the row's local class model.
