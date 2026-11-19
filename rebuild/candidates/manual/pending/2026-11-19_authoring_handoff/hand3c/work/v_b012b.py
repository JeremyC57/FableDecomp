I='#include "engine/GFPackFloat.h"'
VARIANTS=[
 ('gprod', lambda s: s.replace(I, '#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT\n'+I)),
 ('gquot', lambda s: s.replace(I, '#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_QUOTIENT\n'+I)),
]
