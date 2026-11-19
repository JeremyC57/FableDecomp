X='    renderPos.X += (GDummyInt1 + region.GetMiniMapOffset().X) / (256.0f / local_render_box.GetWidth());\n'
Y='    renderPos.Y += (GDummyInt2 + region.GetMiniMapOffset().Y) / (256.0f / local_render_box.GetHeight());\n'
R='    return renderPos;\n'
FY='(float)GFFloatToLongFloor(local_render_box.TLY + local_render_box.GetExtents().Y - mapSize.Y * thingPos.Y * local_render_box.GetExtents().Y)'
FX='(float)GFFloatToLongFloor(mapSize.X * thingPos.X * local_render_box.GetExtents().X + local_render_box.TLX)'
def rep(s, a, b): assert a in s, a; return s.replace(a, b, 1)
VARIANTS = [
 ("ret_paren", lambda s: rep(s, R, '    return (renderPos);\n')),
 ("floor_y_paren", lambda s: rep(s, FY, '('+FY+')')),
 ("floor_x_paren", lambda s: rep(s, FX, '('+FX+')')),
 ("y_rhs_paren", lambda s: rep(s, Y, Y.replace('+= (','+= ((').replace('GetHeight());','GetHeight()));'))),
 ("x_rhs_paren", lambda s: rep(s, X, X.replace('+= (','+= ((').replace('GetWidth());','GetWidth()));'))),
 ("y_sum_paren", lambda s: rep(s, Y, Y.replace('(GDummyInt2 + region.GetMiniMapOffset().Y)','((GDummyInt2 + region.GetMiniMapOffset().Y))'))),
 ("y_div_paren", lambda s: rep(s, Y, Y.replace('(256.0f / local_render_box.GetHeight())','((256.0f / local_render_box.GetHeight()))'))),
 ("x_div_paren", lambda s: rep(s, X, X.replace('(256.0f / local_render_box.GetWidth())','((256.0f / local_render_box.GetWidth()))'))),
]
