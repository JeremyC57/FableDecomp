def rep(s, a, b): assert a in s, a; return s.replace(a, b, 1)
def nocast(s): return s.replace('(float)GFFloatToLongFloor','GFFloatToLongFloor')
def unused_gui(s): return rep(s,'    const CPlayerGuiDef& player_gui_def = CPlayerGui::PeekGuiDef();\n','    CPlayerGui::PeekGuiDef();\n')
def explicit_sum(s):
    s=rep(s,'    renderPos.X += (','    renderPos.X = renderPos.X + (')
    return rep(s,'    renderPos.Y += (','    renderPos.Y = renderPos.Y + (')
def world_ptr(s):
    return rep(s,'    const CWorldMap& world_map = CMainGameComponent::Get()->GetWorld().PeekWorldMap();\n','    const CWorldMap& world_map = CMainGameComponent::Get()->GetWorld().PeekWorldMap();\n')
VARIANTS = [("nocast", nocast), ("unused_gui", unused_gui), ("explicit_sum", explicit_sum)]
