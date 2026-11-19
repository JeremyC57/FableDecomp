X='    renderPos.X += (GDummyInt1 + region.GetMiniMapOffset().X) / (256.0f / local_render_box.GetWidth());\n'
Y='    renderPos.Y += (GDummyInt2 + region.GetMiniMapOffset().Y) / (256.0f / local_render_box.GetHeight());\n'
def rep(s, a, b): assert a in s, a; return s.replace(a, b, 1)
def explicit_sum(s):
    s=rep(s,'    renderPos.X += (','    renderPos.X = renderPos.X + (')
    return rep(s,'    renderPos.Y += (','    renderPos.Y = renderPos.Y + (')
def flipped(s):
    s=rep(s,X,'    renderPos.X = (GDummyInt1 + region.GetMiniMapOffset().X) / (256.0f / local_render_box.GetWidth()) + renderPos.X;\n')
    return rep(s,Y,'    renderPos.Y = (GDummyInt2 + region.GetMiniMapOffset().Y) / (256.0f / local_render_box.GetHeight()) + renderPos.Y;\n')
def explicit_y_only(s): return rep(s,'    renderPos.Y += (','    renderPos.Y = renderPos.Y + (')
def explicit_x_only(s): return rep(s,'    renderPos.X += (','    renderPos.X = renderPos.X + (')
def offset_first(s):
    s=rep(s,X,X.replace('(GDummyInt1 + region.GetMiniMapOffset().X)','(region.GetMiniMapOffset().X + GDummyInt1)'))
    return rep(s,Y,Y.replace('(GDummyInt2 + region.GetMiniMapOffset().Y)','(region.GetMiniMapOffset().Y + GDummyInt2)'))
VARIANTS=[("explicit_sum",explicit_sum),("flipped",flipped),("explicit_y_only",explicit_y_only),("explicit_x_only",explicit_x_only),("offset_first",offset_first)]
