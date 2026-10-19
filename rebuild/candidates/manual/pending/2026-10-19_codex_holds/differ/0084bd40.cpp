// Grade: HELD(bytes match retail) -- test exercises a hand-written stand-in, not the candidate | Codex source 00/84/CCreatureActionPerformExpression_CCreatureActionPerformExpression_0084bd40.cpp, flags , pass pattern 0084bd40_TEST PASS
// CCreatureAction_PerformExpression::CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression&) -- copy counted expression data, intelligent pointer and expression parameters
// Retail 0084bd40, length 0xB4; base copy 0x62ebc0, intelligent pointer helpers 0xa01b10/50/90.
struct CBaseObject {};
struct CBaseIntelligentPointer { virtual void v00(); CBaseObject* m_pItem; CBaseIntelligentPointer(); CBaseObject* GetPItem() const; void SetPItem(CBaseObject*); };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00(); CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); } };
struct CDef { void* v; long refs; void AddRef() { ++refs; } };
struct CDefPointer { CDef* p; CDefPointer(const CDefPointer& o) : p(o.p) { if (p) p->AddRef(); } };
struct CCreatureActionBase { virtual void v00(); unsigned char _body[0xa8-4]; CCreatureActionBase(const CCreatureActionBase&); };
struct CCreatureAction_PerformExpression : CCreatureActionBase {
    CDefPointer Expression; CIntelligentPointer PItemRemoved; long ExpressionID; unsigned char Active, Loop, Finished, _pad; long StartFrame, EndFrame, Speed;
    virtual void v00(); CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression&);
};
CCreatureAction_PerformExpression::CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression& o)
 : CCreatureActionBase(o), Expression(o.Expression), PItemRemoved(o.PItemRemoved), ExpressionID(o.ExpressionID), Active(o.Active), Loop(o.Loop), Finished(o.Finished), _pad(o._pad), StartFrame(o.StartFrame), EndFrame(o.EndFrame), Speed(o.Speed) {}