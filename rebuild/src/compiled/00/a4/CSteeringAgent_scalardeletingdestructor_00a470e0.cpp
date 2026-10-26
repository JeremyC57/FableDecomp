class CSteeringAgent { public:
    void* vftable;
};

void __fastcall CSteeringAgent_dtor(CSteeringAgent* agent);
void operator delete(void*);

void* __fastcall CSteeringAgent_scalar_deleting_destructor(CSteeringAgent* agent, int edx_unused, unsigned int flags)
{
    CSteeringAgent_dtor(agent);
    if (flags & 1)
        operator delete(agent);
    return agent;
}