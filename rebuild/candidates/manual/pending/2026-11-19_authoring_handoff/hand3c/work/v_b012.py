A='SizeFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2)), UseMidColour(0)'
VARIANTS=[
 ('false', lambda s: s.replace(A, 'SizeFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2)), UseMidColour(false)')),
 ('midfirst', lambda s: s.replace(A, 'UseMidColour(0), SizeFadeMinimum(GFPackFloat(0.0f, 7, 0.0f, 1.0f, 2))')),
 ('allbools_false', lambda s: s.replace('(0)','(false)').replace('(1)','(true)')),
]
