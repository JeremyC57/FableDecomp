X='    renderPos.X += (GDummyInt1 + region.GetMiniMapOffset().X) / (256.0f / local_render_box.GetWidth());\n'
Y='    renderPos.Y += (GDummyInt2 + region.GetMiniMapOffset().Y) / (256.0f / local_render_box.GetHeight());\n'
R='    return renderPos;\n'
def rep(s, a, b): assert a in s; return s.replace(a, b, 1)
VARIANTS = [
 ("ret_paren", lambda s: rep(s, R, '    return (renderPos);\n')),
 ("y_rhs_paren", lambda s: rep(s, Y, '    renderPos.Y += ((GDummyInt2 + region.GetMiniMapOffset().Y) / (256.0f / local_render_box.GetHeight()));\n')),
 ("x_rhs_paren", lambda s: rep(s, X, '    renderPos.X += ((GDummyInt1 + region.GetMiniMapOffset().X) / (256.0f / local_render_box.GetWidth()));\n')),
 ("both_rhs_paren", lambda s: rep(rep(s, X, '    renderPos.X += ((GDummyInt1 + region.GetMiniMapOffset().X) / (256.0f / local_render_box.GetWidth()));\n'), Y, '    renderPos.Y += ((GDummyInt2 + region.GetMiniMapOffset().Y) / (256.0f / local_render_box.GetHeight()));\n')),
 ("floor_y_paren", lambda s: rep(s, '(float)GFFloatToLongFloor(local_render_box.TLY', '((float)GFFloatToLongFloor(local_render_box.TLY').replace('GetExtents().Y));\n','GetExtents().Y)));\n',1)),
 ("floor_x_paren", lambda s: rep(s, 'C2DVector renderPos((float)GFFloatToLongFloor(mapSize.X * thingPos.X * local_render_box.GetExtents().X + local_render_box.TLX),', 'C2DVector renderPos(((float)GFFloatToLongFloor(mapSize.X * thingPos.X * local_render_box.GetExtents().X + local_render_box.TLX)),')),
]
