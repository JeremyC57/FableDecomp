B='component_cost.RenderCost += poly_count * poly_cost + pixel_count;'
VARIANTS=[
 ('pix_poly_nocost', lambda s: s.replace(B,'component_cost.RenderCost += pixel_count + poly_count;')),
 ('poly_pix_nocost', lambda s: s.replace(B,'component_cost.RenderCost += poly_count + pixel_count;')),
 ('noinit0', lambda s: s.replace('float poly_count = 0.0f;\n','float poly_count;\n')),
 ('costfirst_decl', lambda s: s.replace('        float poly_count = 0.0f;\n        float poly_cost = 1.0f;\n','        float poly_cost = 1.0f;\n        float poly_count = 0.0f;\n')),
]
