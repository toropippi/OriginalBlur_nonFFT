#include <boost/preprocessor.hpp>

//celr+cerg+cerb code
#define OFFSETADD 2




#define macro2(n,data) float BOOST_PP_CAT(celr, n);float BOOST_PP_CAT(celg, n);float BOOST_PP_CAT(celb, n);
#define INITCEL(z,n,data) macro2(n,data)

#define SHARETOREG(z,n,xpt) \
i = BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[id.x * BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) + n];\
BOOST_PP_CAT(celr, n) = (i % 256);\
BOOST_PP_CAT(celg, n) = (i / 256 % 256);\
BOOST_PP_CAT(celb, n) = (i / 65536 % 256);\

#define REGTOSHARE(z,n,xpt) \
i = ((uint)BOOST_PP_CAT(celr, n)) + 256 * ((uint)BOOST_PP_CAT(celg, n)) + 65536 * ((uint)BOOST_PP_CAT(celb, n));\
BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[id.x * BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) + n] = i;\

#define MAINLOOP(z,n,xpt) \
regr[n & 1] = BOOST_PP_CAT(celr, BOOST_PP_ADD(n, 1)) * WLR;\
regr[(n & 1) ^ 1] = regr[(n & 1) ^ 1] + BOOST_PP_CAT(celr, BOOST_PP_ADD(n, 2)) * WLR;\
BOOST_PP_CAT(celr, BOOST_PP_ADD(n, 1)) = BOOST_PP_CAT(celr, BOOST_PP_ADD(n, 1)) * W + regr[(n & 1) ^ 1];\
regg[n & 1] = BOOST_PP_CAT(celg, BOOST_PP_ADD(n, 1)) * WLR;\
regg[(n & 1) ^ 1] = regg[(n & 1) ^ 1] + BOOST_PP_CAT(celg, BOOST_PP_ADD(n, 2)) * WLR;\
BOOST_PP_CAT(celg, BOOST_PP_ADD(n, 1)) = BOOST_PP_CAT(celg, BOOST_PP_ADD(n, 1)) * W + regg[(n & 1) ^ 1];\
regb[n & 1] = BOOST_PP_CAT(celb, BOOST_PP_ADD(n, 1)) * WLR;\
regb[(n & 1) ^ 1] = regb[(n & 1) ^ 1] + BOOST_PP_CAT(celb, BOOST_PP_ADD(n, 2)) * WLR;\
BOOST_PP_CAT(celb, BOOST_PP_ADD(n, 1)) = BOOST_PP_CAT(celb, BOOST_PP_ADD(n, 1)) * W + regb[(n & 1) ^ 1];\

#define STOT(z_,n,xpt) \
f4.w = 1; \
i = BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[n * TPB + id.x]; \
f4.x = (i % 256); \
f4.y = (i / 256 % 256); \
f4.z = (i / 65536 % 256); \
f4.xyz /= FTOI; \
Tex[float2(id.y, n * TPB + id.x)] = f4; \

#define XTOS(z_,n,xpt) \
LOAD(n,i,BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2)); \
BOOST_PP_CAT(block, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))[n * TPB + id.x] = i; \


//
//
//
//BOOST_PP_MUL(BOOST_PP_ADD(xpt,6),2)
//
//#define LOAD(n,i,NPT) i = Buf[id.y + (n * TPB + id.x) * TPB * NPT];
//#define LOAD(n,i,NPT) f4 = Tex_ro[float2(id.y, n * TPB + id.x)];i = ((uint)(f4.x * FTOI)) + ((uint)(f4.y * FTOI)) * 256 + ((uint)(f4.z * FTOI)) * 65536;
//aaaa

#define ALLCODE(z,xpt,data_) \
\
[numthreads(TPB, 1, 1)]\
void BOOST_PP_CAT(YBlur, BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2))(uint2 id : SV_DispatchThreadID)\
{\
    float4 f4;uint i, j; float regr[2]; float regg[2]; float regb[2];\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), INITCEL, 0);\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), XTOS, xpt);\
    GroupMemoryBarrierWithGroupSync();\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), SHARETOREG, xpt);\
    for (j = 0; j < (uint)loopnum; j++)\
    {\
        GroupMemoryBarrierWithGroupSync();\
        fblock0r[id.x] = celr0; fblock1r[id.x] = BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1));\
        fblock0g[id.x] = celg0; fblock1g[id.x] = BOOST_PP_CAT(celg, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1));\
        fblock0b[id.x] = celb0; fblock1b[id.x] = BOOST_PP_CAT(celb, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1));\
        GroupMemoryBarrierWithGroupSync();\
        regr[1] = celr0 * WLR;regr[0] = celr1 * WLR + fblock1r[(id.x + TPB - 1) % TPB] * WLR;celr0 = celr0 * W + regr[0];\
        regg[1] = celg0 * WLR;regg[0] = celg1 * WLR + fblock1g[(id.x + TPB - 1) % TPB] * WLR;celg0 = celg0 * W + regg[0];\
        regb[1] = celb0 * WLR;regb[0] = celb1 * WLR + fblock1b[(id.x + TPB - 1) % TPB] * WLR;celb0 = celb0 * W + regb[0];\
        BOOST_PP_REPEAT(BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 2), MAINLOOP, xpt);\
        regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] = regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] + fblock0r[(id.x + 1) % TPB] * WLR;\
        BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) = BOOST_PP_CAT(celr, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) * W + regr[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1];\
        regg[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] = regg[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] + fblock0g[(id.x + 1) % TPB] * WLR;\
        BOOST_PP_CAT(celg, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) = BOOST_PP_CAT(celg, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) * W + regg[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1];\
        regb[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] = regb[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1] + fblock0b[(id.x + 1) % TPB] * WLR;\
        BOOST_PP_CAT(celb, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) = BOOST_PP_CAT(celb, BOOST_PP_SUB(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), 1)) * W + regb[(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2) & 1) ^ 1];\
    }\
    GroupMemoryBarrierWithGroupSync();\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), REGTOSHARE, xpt);\
    GroupMemoryBarrierWithGroupSync();\
    BOOST_PP_REPEAT(BOOST_PP_MUL(BOOST_PP_ADD(xpt, OFFSETADD), 2), STOT, xpt);\
}\




BOOST_PP_REPEAT(20,ALLCODE,_)



