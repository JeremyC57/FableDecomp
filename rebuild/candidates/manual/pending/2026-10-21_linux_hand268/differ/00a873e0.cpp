// CSBAgents::Process(const CSteeringAgent& agent) -- steering behaviour: sum an inverse-square repulsion away from every other relevant agent closer than the cut-off, into the behaviour's output
// Grade: DIFFER(404v404) -- frame and register naming only: retail homes the repulsion 'away' vector BELOW the running force (F+0xC v F+0x14), ours the reverse, and the vcall temporaries use eax/edx where ours use edx/eax|ecx; tried named positions (fixed the eax read-back), force.X = force.Y = 0 (fixed the Y-then-X zero stores), a named cutoff (fixed the early load), away declared at the top, an inline GetOffsetFrom, an inline/forceinline per-agent body
// Leaf: Process
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    C2DVector& operator*=(float s) { X *= s; Y *= s; return *this; }
    C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
    C2DVector& Normalise();                       // 0x00A14540
};
struct CKinematic {
    C2DVector Vector;
    CKinematic() {}
    CKinematic(const C2DVector& v) : Vector(v) {}
};
struct CSteeringAgent;
struct CNavigatorExternalInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05();
    virtual float GetRadius(bool a) const;        // slot 6 (+0x18)
    virtual void v07();
    virtual C2DVector GetPosition() const;        // slot 8 (+0x20)
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual CSteeringAgent* GetIgnoredAgent() const;   // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual bool IsStatic() const;                // slot 20 (+0x50)
    virtual bool IsInactive() const;              // slot 21 (+0x54)
};
struct CSteeringAgent {
    CNavigatorExternalInterface* NavigatorInterface;   // +0x00
    unsigned char _pad04[0x30];
    float         DistCutoff;                          // +0x34
    unsigned char _pad38[0x24];
    bool          Wandering;                           // +0x5c
};
struct CAgentVector { CSteeringAgent** _M_start; CSteeringAgent** _M_finish; CSteeringAgent** _M_end_of_storage; };
extern bool g_SteeringDisabled;                   // 0x013D2D54

struct CSBAgents {
    virtual void v00();
    CKinematic    Output;                         // +0x04
    CAgentVector* Agents;                         // +0x0c
    bool ShouldIgnoreAgent(const CSteeringAgent& agent, const CSteeringAgent* other, const CSteeringAgent* ignored);   // 0x00A87260
    float GetDistanceBetweenAgents(const CSteeringAgent& agent, const CSteeringAgent* other, bool edges);           // 0x00A872F0
    bool Process(const CSteeringAgent& agent);
};

bool CSBAgents::Process(const CSteeringAgent& agent)
{
    C2DVector force;
    force.X = force.Y = 0.0f;
    if (g_SteeringDisabled) {
        Output = CKinematic(force);
        return true;
    }
    CSteeringAgent* ignored = agent.NavigatorInterface->GetIgnoredAgent();
    for (CSteeringAgent** it = Agents->_M_start; it != Agents->_M_finish; ++it) {
        if ((*it)->NavigatorInterface->IsInactive())
            continue;
        if (ShouldIgnoreAgent(agent, *it, ignored))
            continue;
        C2DVector otherPos = (*it)->NavigatorInterface->GetPosition();
        C2DVector agentPos = agent.NavigatorInterface->GetPosition();
        C2DVector away = agentPos - otherPos;
        float dist = GetDistanceBetweenAgents(agent, *it, true);
        if (dist < 0.0f && !(*it)->NavigatorInterface->IsStatic() && !agent.Wandering)
            continue;
        float cutoff = agent.DistCutoff;
        if (dist < cutoff / agent.NavigatorInterface->GetRadius(false)) {
            away.Normalise();
            if (dist < 1.0f)
                dist = 1.0f;
            away *= 1.0f / (dist * dist);
            force += away;
        }
    }
    Output = CKinematic(force);
    return true;
}
