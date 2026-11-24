// Leaf: ?WorldFrameUpdate@CEngineResourceManager@@UAEX_N@Z
struct Sub { char pad[4]; };

void __fastcall Sub_Update(Sub* self);

struct Inner {
    char pad[0x1ec];
    Sub sub;
};

class CEngineResourceManager { public:
    char pad[0xc];
    Inner* field10;
    virtual void WorldFrameUpdate(bool);
};

void CEngineResourceManager::WorldFrameUpdate(bool b)
{
    CEngineResourceManager* self = this;
    Inner* p = self->field10;
    if (p) {
        Sub_Update(&p->sub);
    }
}
// Flatten route (flatten_pilot v4, NF324): CEngineResourceManager_WorldFrameUpdate -> CEngineResourceManager::WorldFrameUpdate, the RTTI-placed PDB member; class model: the row's local class model.
